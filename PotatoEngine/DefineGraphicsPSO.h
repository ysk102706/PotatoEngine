#pragma once
#include "D3D11Utils.h"
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsPSO.h"
#include <vector>

namespace Engine
{
	using Microsoft::WRL::ComPtr;

	namespace PSO
	{ 
		extern ComPtr<ID3D11RasterizerState> solidRS;
		extern ComPtr<ID3D11RasterizerState> wireRS;
		extern ComPtr<ID3D11RasterizerState> solidNoneCullRS;
		extern ComPtr<ID3D11RasterizerState> wireNoneCullRS;

		extern ComPtr<ID3D11VertexShader> basicVS;
		extern ComPtr<ID3D11PixelShader>  basicPS;
		extern ComPtr<ID3D11VertexShader> normalVS;
		extern ComPtr<ID3D11GeometryShader> normalGS;
		extern ComPtr<ID3D11PixelShader> normalPS;

		extern ComPtr<ID3D11InputLayout> basicIL;

		extern ComPtr<ID3D11SamplerState> linearWarpSS;
		extern ComPtr<ID3D11SamplerState> linearClampSS;
		extern std::vector<ID3D11SamplerState*> samplerStates;

		extern GraphicsPSO defaultSolidPSO;
		extern GraphicsPSO defaultWirePSO; 
		extern GraphicsPSO normalPSO;
		extern GraphicsPSO solidNoneCullPSO;
		extern GraphicsPSO wireNoneCullPSO;
		
		void InitGraphicsPSO(ComPtr<ID3D11Device>& device);

		void InitShader(ComPtr<ID3D11Device>& device);
		void InitRasterizerState(ComPtr<ID3D11Device>& device);
		void InitSamplerState(ComPtr<ID3D11Device>& device);
	};
}