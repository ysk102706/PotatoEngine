#pragma once
#include <vector>
#include <string>
#include "Vertex.h"

namespace Engine {
	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices; 
		std::string albedoTextureFile; 
		std::string normalMapTextureFile; 
		std::string heightMapTextureFile; 
	};
}