#pragma once
#include <vector>

#include "Vertex.h"

namespace Engine {
	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};
}