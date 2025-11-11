#pragma once

#include <Inc/ML_Engine.h>

class GameState : public ML_Engine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	struct Object
	{
		ML_Engine::Math::Matrix4 matWorld = ML_Engine::Math::Matrix4::Identity;
		ML_Engine::Graphics::MeshBuffer meshBuffer;
		ML_Engine::Graphics::TextureId textureId = 0;
	};

	void UpdateCamera(float deltaTime);
	void RenderObject(const Object& object, const ML_Engine::Graphics::Camera& camera);

	ML_Engine::Graphics::Camera mCamera;
	ML_Engine::Graphics::Camera mRenderTargetCamera;

	// GPU communication
	ML_Engine::Graphics::ConstantBuffer mTransformBuffer;
	ML_Engine::Graphics::VertexShader mVertexShader;
	ML_Engine::Graphics::PixelShader mPixelShader;
	ML_Engine::Graphics::Sampler mSampler;

	// render object
	Object mObject0;
	Object mObject1;

	// render target
	ML_Engine::Graphics::RenderTarget mRenderTarget;
};