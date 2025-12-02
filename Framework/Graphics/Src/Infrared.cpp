#include "Precompiled.h"
#include "Infrared.h"

#include "RenderObject.h"
#include "Texture.h"
#include "VertexTypes.h"
#include "GraphicsSystem.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;

namespace
{
	const char* gModeNames[] =
	{
		"None",
		"Infrared"
	};
}

void Infrared::Initialize(const std::filesystem::path& filePath)
{
	mVertexShader.Initialize<VertexPX>(filePath);
	mPixelShader.Initialize(filePath);
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mInfraredBuffer.Initialize();
}
void Infrared::Terminate()
{
	mInfraredBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}
void Infrared::Begin(float time)
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i] != nullptr)
		{
			mTextures[i]->BindPS(i);
		}
	}

	InfraredData data{};
	data.mode = static_cast<int>(mMode);
	switch (mMode)
	{
	case Mode::None:
	case Mode::Infrared:
	{
		data.param0 = time;
		data.param1 = mHeatWaveIntensity; // heat intensity
		data.param2 = mBlur; // blur strength
	}
	break;
	default:
		break;
	}

	mInfraredBuffer.Update(data);
	mInfraredBuffer.BindPS(0);
}
void Infrared::End()
{
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		Texture::UnbindPS(i);
	}
}
void Infrared::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}
void Infrared::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTextures.size(), "Infrared: invalid slot index");
	mTextures[slot] = texture;
}
void Infrared::SetMode(Mode mode)
{
	mMode = mode;
}
void Infrared::DebugUI()
{
	if (ImGui::CollapsingHeader("Infrared", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);
		if (ImGui::Combo("Mode", &currentMode, gModeNames, std::size(gModeNames)))
		{
			mMode = static_cast<Mode>(currentMode);
		}
		else if (mMode == Mode::Infrared)
		{
			ImGui::DragFloat("HeatIntensity", &mHeatWaveIntensity, 0.001f, 0.0f, 1.0f); // heat
			ImGui::DragFloat("BlurStrength", &mBlur, 0.1f, 0.0f, 100.0f); // blur
		}
	}
}