#pragma once
#include "MeshData.h"

namespace Engine 
{
	
	class DefaultObjectGenerator {
	public:
		static MeshData MakeTriangle(float scale);
		static MeshData MakeBox(float scale);
	};
}