#pragma once
#include <d3d11.h> 
#include <wrl.h> 
#include <directxtk/SimpleMath.h> 
#include "MeshData.h"

namespace Engine
{ 
	using Microsoft::WRL::ComPtr; 
	using DirectX::SimpleMath::Vector3; 
	using DirectX::SimpleMath::Vector4; 

	struct TessellationConstantData {
		float distance; 
		Vector3 dummy; 
	};

	class TessellationQuad {
	public: 
		void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, std::vector<Vector4>& vertices); 
		void Render(ComPtr<ID3D11DeviceContext>& context); 

	private: 
		UINT indexCount; 
		ComPtr<ID3D11Buffer> vertexBuffer; 

		TessellationConstantData tessellationConstantCPU; 
		ComPtr<ID3D11Buffer> tessellationConstantGPU; 

	};
}