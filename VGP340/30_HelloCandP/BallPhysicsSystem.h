#pragma once

#include "Ball.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class BallPhysicsSystem
{
public:
	// general settings for the system
	struct Settings
	{
		Math::Vector3 gravity = Math::Vector3(0.0f, -9.81f, 0.0f);
		float floorY = 0.0f;
		float bounciness = 0.7f;
		float friction = 0.15f;
	};

	// to send data to the calculation thread
	struct CollisionPair
	{
		size_t indexA;
		size_t indexB;
		Math::Vector3 normal;
		float overlap;
		float relSpeed;
		float impulse;
	};

	// the usual suspects
	void Initialize(const Settings& settings, const Graphics::Camera& camera, const Graphics::DirectionalLight& light);
	void Terminate();
	void Update(float deltaTime);
	void Render();
	void DebugUI();

	// helper functions
	void AddBall(const Ball& ball);
	void ClearBalls();
	void SetCamera(const Graphics::Camera& camera);

private:
	// physics functions
	void Integrate(float deltaTime);
	void ResolveFloor();

	// balls & settings
	std::vector<Ball> mBalls;
	Settings mSettings;

	// graphics, ball mesh, camera
	Graphics::StandardEffect mEffect;
	Graphics::RenderObject mSphereObject;
	Graphics::Camera mCamera;

	// collision detection
	void CollisionThreadLoop();
	void DetectCollisions();
	void ApplyCollisionResults();

	// multithreading
	std::thread mCollisionThread; // collision thread
	std::mutex mMutex; // mutex
	std::condition_variable mDetectCV; // condition variables
	std::condition_variable mResultCV;
	std::atomic<bool> mThreadRunning = false; // atomic
	bool mDetectReady = false; // bools for the condition variables
	bool mResultReady = false;

	std::vector<CollisionPair> mCollisionPairs; // store collision pairs for collision calculation
};