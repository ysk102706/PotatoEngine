#pragma once
#include <d3D11.h> 
#include <wrl.h> 
#include <vector> 
#include <directxtk/SimpleMath.h> 

namespace Engine 
{ 
	using Microsoft::WRL::ComPtr; 
	using DirectX::SimpleMath::Vector3; 
	using DirectX::SimpleMath::Vector4; 

	struct BillboardConstantData { 
		float width; 
		Vector3 dummy; 
	};

	class Billboard {
	public: 
		void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, std::vector<Vector4>& points, float width); 
		void Render(ComPtr<ID3D11DeviceContext>& context); 

	private: 
		UINT indexCount; 
		ComPtr<ID3D11Buffer> vertexBuffer; 

		BillboardConstantData billboardConstantCPU;
		ComPtr<ID3D11Buffer> billboardConstantGPU; 

	};
}