#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace Engine
{
	using Microsoft::WRL::ComPtr; 

	class GraphicsPSO {
	public:
		ComPtr<ID3D11VertexShader> vertexShader; 
		ComPtr<ID3D11PixelShader> pixelShader; 
		ComPtr<ID3D11InputLayout> inputLayout; 
		ComPtr<ID3D11RasterizerState> rasterizerState; 
		ComPtr<ID3D11SamplerState> samplerState;

		D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}; 
}