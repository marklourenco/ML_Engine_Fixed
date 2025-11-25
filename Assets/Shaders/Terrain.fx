// Terrain Effect file that blends 2 textures based on pixel height in the world

cbuffer TransformBuffer : register(b0)
{
    matrix world;
    matrix wvp;
    matrix lwvp;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialEmissive;
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float materialShininess;
}

cbuffer SettingsBuffer : register(b3)
{
    bool useShadowMap;
    float depthBias;
    float lowHeight;
    float blendHeight;
}

Texture2D lowTextureMap : register(t0);
Texture2D highTextureMap : register(t1);
Texture2D shadowMap : register(t2);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 worldNormal : NORMAL;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
    float4 worldPosition : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float4 finalColor = 1.0f;
    
    return finalColor;
}