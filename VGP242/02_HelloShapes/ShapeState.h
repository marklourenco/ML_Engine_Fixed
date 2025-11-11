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

    struct Vertex
    {
        ML_Engine::Math::Vector3 position;
        ML_Engine::Graphics::Color color;
    };
    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
};

class TriangleShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;
protected:
    void CreateShape() override;
};

class TriforceShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;
protected:
    void CreateShape() override;
};

class DiamondShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;
protected:
    void CreateShape() override;
};

class HeartShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;
protected:
    void CreateShape() override;
};