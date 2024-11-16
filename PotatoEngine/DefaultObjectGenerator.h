#pragma once
#include "MeshData.h"

namespace Engine 
{
	using DirectX::SimpleMath::Vector4;

	class DefaultObjectGenerator {
	public:
		static MeshData MakeTriangle(float scale); 
		static MeshData MakeSquareGrid(float scale, int column, int row);
		static MeshData MakeBox(float scale);
		static MeshData MakeCylinder(float topRadius, float bottomRadius, float height, int column, int row); 
		static MeshData MakeSphere(float radius, int column, int row);
		static MeshData SubdivideToSphere(float radius, MeshData meshData); 
		static std::vector<MeshData> ReadFromFile(std::string path, std::string filename, bool revertNormal); 
		static std::vector<Vector4> MakeTessellationQuad();
	};
}