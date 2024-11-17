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
	using DirectX::SimpleMath::Vector3; 
	using DirectX::SimpleMath::Matrix; 

	struct PhongShading {
		Vector3 ambient; 
		float shininess;
		Vector3 diffuse;
		float dummy1;
		Vector3 specular;
		float dummy2; 
	}; 

	struct RimEffect {
		bool useRim;
		Vector3 color;
		float strength;
		float factor;
		bool useSmoothStep;
		float dummy;
	}; 

	struct FresnelEffect {
		bool useFresnel; 
		Vector3 fresnelR0;
	};

	struct ModelConstantData {
		Matrix world; 
		Matrix invTranspose; 
	}; 

	struct MaterialConstantData { 
		PhongShading mat; 
		RimEffect rim;
		FresnelEffect fresnel; 
		bool useAmbient; 
		float mipLevel; 
		bool useTextureLOD; 
		float dummy; 
		//Vector2 dummy; 
	};

	class Model {
	public: 
		Model();
		Model(ComPtr<ID3D11Device> &device, ComPtr<ID3D11DeviceContext> &context, const std::vector<MeshData> &meshData); 
		
		void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::vector<MeshData>& meshData); 

		void Render(ComPtr<ID3D11DeviceContext>& context); 
		void NormalRender(ComPtr<ID3D11DeviceContext>& context);

		void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context);

		ModelConstantData modelConstantCPU;
		MaterialConstantData materialConstantCPU; 

		bool isVisible;

	private:
		std::vector<std::shared_ptr<Mesh>> meshes; 

		ComPtr<ID3D11Buffer> modelConstantGPU;
		ComPtr<ID3D11Buffer> materialConstantGPU;

	};
}