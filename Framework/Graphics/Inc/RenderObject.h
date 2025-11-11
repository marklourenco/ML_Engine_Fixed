#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "ModelManager.h"

namespace ML_Engine::Graphics
{
	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;      // location
		MeshBuffer meshBuffer;    // shape
		Material material;        // light data
		TextureId diffuseMapId;   // diffuse texture for an object
		TextureId specMapId;
		TextureId normalMapId;
		TextureId bumpMapId;
	};

	class RenderGroup
	{
	public:
		void Initialize(const std::filesystem::path& modelFilePath);
		void Terminate();

		ModelId modelId;
		Transform transform;
		std::vector<RenderObject> renderObjects;
	};
}