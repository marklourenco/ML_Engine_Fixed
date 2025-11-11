#pragma once

#include <Inc/ML_Engine.h>

class ShapeState : public ML_Engine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;

protected:
    virtual void CreateShape();
    
    ML_Engine::Graphics::MeshPC mMesh;
    ML_Engine::Graphics::Camera mCamera;
    ML_Engine::Graphics::ConstantBuffer mTransformBuffer;
    ML_Engine::Graphics::MeshBuffer mMeshBuffer;
    ML_Engine::Graphics::VertexShader mVertexShader;
    ML_Engine::Graphics::PixelShader mPixelShader;
};