#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace Engine {
	using Microsoft::WRL::ComPtr;

	struct Mesh {
		ComPtr<ID3D11Buffer> vertexBuffer;
		ComPtr<ID3D11Buffer> indexBuffer; 

		ComPtr<ID3D11Buffer> vertexConstantBuffer;
		ComPtr<ID3D11Buffer> pixelConstantBuffer;

		ComPtr<ID3D11Texture2D> albedoTexture;
		ComPtr<ID3D11Texture2D> normalMapTexture; 
		ComPtr<ID3D11Texture2D> heightMapTexture; 
		ComPtr<ID3D11Texture2D> AOTexture; 

		ComPtr<ID3D11ShaderResourceView> albedoSRV; 
		ComPtr<ID3D11ShaderResourceView> normalMapSRV; 
		ComPtr<ID3D11ShaderResourceView> heightMapSRV; 
		ComPtr<ID3D11ShaderResourceView> AOSRV; 

		UINT indexCount = 0;
		UINT vertexCount = 0;
		UINT stride = 0;
		UINT offset = 0;
	};
}