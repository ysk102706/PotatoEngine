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

		UINT indexCount = 0;
		UINT vertexCount = 0;
		UINT stride = 0;
		UINT offset = 0;
	};
}