#pragma once

#include "MeshTypes.h"

namespace ML_Engine::Graphics
{
	class MeshBuilder
	{
	public:
		// cube
		static MeshPC CreateCubePC(float size, const Color& color);
		static MeshPC CreateCubePC(float size);

		// pyramid
		static MeshPC CreatePyramidPC(float size);

		// rectangle
		static MeshPC CreateRectanglePC(float width, float height, float depth);

		// plane
		static MeshPC CreatePlanePC(int numRows, int numColums, float spacing, bool horizontal = true);

		// cylinder
		static MeshPC CreateCylinderPC(int slices, int rings);

		// sphere
		static MeshPC CreateSpherePC(int slices, int rings, float radius);

		// cube PX (textures)
		static MeshPX CreateCubePX(float size);

		// sphere PX
		static MeshPX CreateSpherePX(int slices, int rings, float radius);

		// sphere
		static Mesh CreateSphere(int slices, int rings, float radius);

		// plane PX
		static MeshPX CreatePlanePX(int numRows, int numColums, float spacing, bool horizontal = true);

		// plane 
		static Mesh CreatePlane(int numRows, int numColums, float spacing, bool horizontal = true);

		// sky
		static MeshPX CreateSkySpherePX(int slices, int rings, float radius);

		// load obj
		static MeshPX CreateOBJPX(const std::filesystem::path& filePath, float scale);

		// Screen Quad
		static MeshPX CreateScreenQuadPX();
	};
}