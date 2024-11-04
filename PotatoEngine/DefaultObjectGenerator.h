#pragma once
#include "MeshData.h"

namespace Engine 
{
	
	class DefaultObjectGenerator {
	public:
		static MeshData MakeTriangle(float scale); 
		static MeshData MakeSquareGrid(float scale, int column, int row);
		static MeshData MakeBox(float scale);
		static MeshData MakeCylinder(float topRadius, float bottomRadius, float height, int column, int row); 
		static MeshData MakeSphere(float radius, int column, int row);
		static MeshData SubdivideToSphere(float radius, MeshData meshData);
	};
}