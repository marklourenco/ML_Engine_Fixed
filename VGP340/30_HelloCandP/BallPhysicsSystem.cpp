#include "BallPhysicsSystem.h"

#include <execution>
#include <algorithm>
#include <numeric>

using namespace ML_Engine;
using namespace ML_Engine::Physics;
using namespace ML_Engine::Graphics;

void BallPhysicsSystem::Initialize(const Settings& settings, const Camera& camera, const DirectionalLight& light)
{
	mSettings = settings;

	Graphics::Mesh sphere = Graphics::MeshBuilder::CreateSphere(16, 16, 1.0f);
	mSphereObject.meshBuffer.Initialize(sphere);
	mSphereObject.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mSphereObject.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mSphereObject.material.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mSphereObject.material.emissive = { 0.0f, 0.0f, 0.0f, 1.0f };

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mEffect.Initialize(shaderFile);
	mEffect.SetCamera(camera);
	mEffect.SetDirectionalLight(light);

	mThreadRunning = true;
	mCollisionThread = std::thread(&BallPhysicsSystem::CollisionThreadLoop, this); // start collision thread
}

void BallPhysicsSystem::Terminate()
{
	// stop collision thread
	mThreadRunning = false;
	mDetectReady = true;
	mDetectCV.notify_all();
	mCollisionThread.join();

	mEffect.Terminate();
	mSphereObject.Terminate();
	mBalls.clear();
}

void BallPhysicsSystem::Update(float deltaTime)
{
	// integrate aka physics for each ball
	Integrate(deltaTime);
	// physics considering floor
	ResolveFloor();

	// lock shared flags for collision detection
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mDetectReady = true;
		mResultReady = false;
	}
	// wake up collision thread to check for collisions
	mDetectCV.notify_one();

	// wait for collision results
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mResultCV.wait(lock, [this] { return mResultReady; });
	}

	// apply collision results
	ApplyCollisionResults();
}

void BallPhysicsSystem::ApplyCollisionResults()
{
	for (const CollisionPair& pair : mCollisionPairs)
	{
		Ball& a = mBalls[pair.indexA];
		Ball& b = mBalls[pair.indexB];

		// separate overlapping balls
		a.mPosition -= pair.normal * pair.overlap;
		b.mPosition += pair.normal * pair.overlap;

		// normal impulse
		a.mVelocity -= pair.normal * (pair.impulse / a.mass);
		b.mVelocity += pair.normal * (pair.impulse / b.mass);

		// friction impulse
		Math::Vector3 relVel = b.mVelocity - a.mVelocity;
		Math::Vector3 tangent = relVel - pair.normal * Math::Dot(relVel, pair.normal);
		float tangentSpeed = Math::Magnitude(tangent);
		if (tangentSpeed > 0.0001f)
		{
			tangent = tangent / tangentSpeed;
			float frictionImpulse = -Math::Dot(relVel, tangent) / (1.0f / a.mass + 1.0f / b.mass);
			frictionImpulse = Math::Clamp(frictionImpulse, -mSettings.friction * pair.impulse, mSettings.friction * pair.impulse);
			a.mVelocity -= tangent * (frictionImpulse / a.mass);
			b.mVelocity += tangent * (frictionImpulse / b.mass);
		}
	}
}

void BallPhysicsSystem::Render()
{
	mEffect.Begin();
	for (const Ball& ball : mBalls)
	{
		mSphereObject.transform.position = ball.mPosition;
		mSphereObject.transform.rotation = ball.mRotation;
		mSphereObject.transform.scale = { ball.radius, ball.radius, ball.radius };
		mSphereObject.material.diffuse = ball.mColor;
		mSphereObject.material.ambient = ball.mColor;

		mEffect.Render(mSphereObject);
	}
	mEffect.End();
}

void BallPhysicsSystem::Integrate(float deltaTime)
{
	// parallel, split between cpus
	// unsequential bc math can be reordered
	std::for_each(std::execution::par_unseq,
		mBalls.begin(), mBalls.end(),
		[&](Ball& ball)
		{
			ball.mVelocity += mSettings.gravity * deltaTime;
			ball.mPosition += ball.mVelocity * deltaTime;

			Math::Vector3 angularVelocity = Math::Cross(ball.mVelocity, Math::Vector3::YAxis) / ball.radius;
			float spinSpeed = Math::Magnitude(angularVelocity);
			if (spinSpeed > 0.0001f)
			{
				ball.mSpinAxis = angularVelocity / spinSpeed;
				ball.mAngle += spinSpeed * deltaTime;
			}
			ball.mRotation = Math::Quaternion::CreateFromAxisAngle(ball.mSpinAxis, ball.mAngle);
		});
}

void BallPhysicsSystem::ResolveFloor()
{
	// par unseq like integrate
	std::for_each(std::execution::par_unseq,
		mBalls.begin(), mBalls.end(),
		[&](Ball& ball)
		{
			float floorContact = mSettings.floorY + ball.radius;
			if (ball.mPosition.y < floorContact)
			{
				ball.mPosition.y = floorContact;
				ball.mVelocity.y = -ball.mVelocity.y * mSettings.bounciness;

				// friction
				ball.mVelocity.x = ball.mVelocity.x * (1.0f - mSettings.friction);
				ball.mVelocity.z = ball.mVelocity.z * (1.0f - mSettings.friction);
			}
		});
}

void BallPhysicsSystem::CollisionThreadLoop()
{
    while (mThreadRunning)
    {
        {
            std::unique_lock<std::mutex> lock(mMutex);
			// sleep thread
            mDetectCV.wait(lock, [this] { return mDetectReady || !mThreadRunning; });
            if (!mThreadRunning) break;
            mDetectReady = false;
        }

        DetectCollisions();

        {
            std::lock_guard<std::mutex> lock(mMutex);
			// notify main thread
            mResultReady = true;
        }
        mResultCV.notify_one();
    }
}

void BallPhysicsSystem::DetectCollisions()
{
	// clear for new collisions
	mCollisionPairs.clear();
	for (size_t i = 0; i < mBalls.size(); ++i)
	{
		for (size_t j = i + 1; j < mBalls.size(); ++j)
		{
			const Ball& a = mBalls[i];
			const Ball& b = mBalls[j];

			Math::Vector3 delta = b.mPosition - a.mPosition;
			float distSq = Math::Dot(delta, delta);
			float minDist = a.radius + b.radius;

			// ignore collisions that are too far away
			if (distSq >= minDist * minDist || distSq == 0.0f)
			{
				continue;
			}

			float dist = std::sqrt(distSq);
			Math::Vector3 normal = delta / dist;
			float overlap = (minDist - dist) * 0.5f;

			Math::Vector3 relVel = b.mVelocity - a.mVelocity;
			float relSpeed = Math::Dot(relVel, normal);

			// ignore collisions that are moving away
			if (relSpeed >= 0.0f)
			{
				continue;
			}

			float impulse = -(1.0f + mSettings.bounciness) * relSpeed / (1.0f / a.mass + 1.0f / b.mass);

			// add to collision pairs list
			mCollisionPairs.push_back({ i, j, normal, overlap, relSpeed, impulse });
		}
	}
}

void BallPhysicsSystem::DebugUI()
{
	ImGui::Text("Balls: %zu", mBalls.size());
	ImGui::DragFloat3("Gravity", &mSettings.gravity.x, 0.1f);
	ImGui::DragFloat("Floor Y", &mSettings.floorY, 0.1f);
	ImGui::SliderFloat("Bounciness", &mSettings.bounciness, 0.0f, 1.0f);
	ImGui::SliderFloat("Friction", &mSettings.friction, 0.0f, 1.0f);

	if (ImGui::Button("SpawnBall"))
	{
		Ball b;
		b.mPosition = { rand() % 10 - 5.0f,  5.0f, 0.0f};
		b.radius = 0.5f;
		b.mass = 1.0f;
		b.mColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		AddBall(b);
	}

	ImGui::SameLine();
	
	if (ImGui::Button("ClearBalls"))
	{
		ClearBalls();
	}
}

void BallPhysicsSystem::AddBall(const Ball& ball)
{
	mBalls.push_back(ball);
}

void BallPhysicsSystem::ClearBalls()
{
	mBalls.clear();
}

void BallPhysicsSystem::SetCamera(const Camera& camera)
{
	mEffect.SetCamera(camera);
}