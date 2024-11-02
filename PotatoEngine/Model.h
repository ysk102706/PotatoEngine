#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>

#include "MeshData.h"
#include "Mesh.h"
#include <directxtk/SimpleMath.h>

namespace Engine
{
	using Microsoft::WRL::ComPtr;
	using DirectX::SimpleMath::Matrix;

	struct ModelConstantData {
		Matrix world; 
		Matrix invTranspose; 
	}; 

	class Model {
	public: 
		Model();
		Model(ComPtr<ID3D11Device> &device, ComPtr<ID3D11DeviceContext> &context, const std::vector<MeshData> &meshData); 
		
		void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::vector<MeshData>& meshData); 

		void Render(ComPtr<ID3D11DeviceContext>& context); 

		void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context);

		ModelConstantData meshConstantCPU;

	private:
		std::vector<std::shared_ptr<Mesh>> meshes; 

		ComPtr<ID3D11Buffer> meshConstantGPU;

	};
}