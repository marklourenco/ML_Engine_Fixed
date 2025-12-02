// Simpler shader that applies post processing effects to the scene
cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float param0;
    float param1;
    float param2;
    float param3;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 0.0f;
    if (mode == 0) // none
    {
        finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    }
    else if (mode == 1) // infrared
    {
        // get uv coordinates
        float2 uv = input.texCoord;

        // distort uv with sine wave
        float shimmer = sin((uv.y * 40.0f) + param0 * 10.0f) * param1;
        float2 distortedUV = uv + shimmer * float2(0.02f, 0.02f);
        
        // sample neighbor for blur
        float2 texel = float2(param2 / 1024.0f, param2 / 768.0f); // render size
        float3 sum = 0.0f;
        float weight = 0.0f;
        
        // 3x3 gaussian weights
        float kernel[3] = { 0.27901f, 0.44198f, 0.27901f };

        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                float w = kernel[abs(x)] * kernel[abs(y)];
                sum += textureMap0.Sample(textureSampler, distortedUV + texel * float2(x, y)).rgb * w;
                weight += w;
            }
        }
        
        float3 blurredColor = sum / weight;

        // calculate brightness | RGB -? Luminance
        float brightness = dot(blurredColor, float3(0.299f, 0.587f, 0.114f));

        // push brightness down to keep average areas cooler
        brightness = pow(saturate(brightness), 1.8f);

        // color gradient
        float3 coldColor = float3(0.0f, 0.1f, 0.4f); // deeper blue
        float3 midColor = float3(1.0f, 0.0f, 0.0f); // red
        float3 hotColor = float3(1.0f, 1.0f, 0.0f); // yellow-white

        float3 heatColor;
        if (brightness < 0.4f)
            heatColor = lerp(coldColor, midColor, smoothstep(0.0f, 0.4f, brightness));
        else
            heatColor = lerp(midColor, hotColor, smoothstep(0.4f, 1.0f, brightness));

        // emissive glow to bright spots
        float glow = pow(brightness, 4.0f);
        heatColor += glow * 0.25f;

        // edge detection
        float3 sobelX =
        textureMap0.Sample(textureSampler, uv + texel * float2(1, 0)).rgb -
        textureMap0.Sample(textureSampler, uv + texel * float2(-1, 0)).rgb;
        float3 sobelY =
        textureMap0.Sample(textureSampler, uv + texel * float2(0, 1)).rgb -
        textureMap0.Sample(textureSampler, uv + texel * float2(0, -1)).rgb;
        float edge = length(sobelX + sobelY);
        heatColor += edge * 0.3f;

        finalColor = float4(heatColor, 1.0f);
        
        float4 color0 = finalColor;
        float4 color1 = textureMap1.Sample(textureSampler, input.texCoord);
        color1.a *= 1; // alpha of predatorHUD
        finalColor = (color0 * (1.0f - color1.a)) + (color1 * color1.a);

    }
    return finalColor;
}