#pragma once
#include "D3D11Utils.h"
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsPSO.h"
#include <vector>

namespace Engine
{
	using Microsoft::WRL::ComPtr;

	namespace DefineGraphicsPSO
	{ 
		extern ComPtr<ID3D11RasterizerState> solidRS;
		extern ComPtr<ID3D11RasterizerState> wireRS;

		extern ComPtr<ID3D11VertexShader> basicVS;
		extern ComPtr<ID3D11PixelShader>  basicPS;

		extern ComPtr<ID3D11InputLayout> basicIL;

		extern GraphicsPSO defaultSolidPSO;
		extern GraphicsPSO defaultWirePSO;
		
		void InitGraphicsPSO(ComPtr<ID3D11Device>& device);

		void InitShader(ComPtr<ID3D11Device>& device);
		void InitRasterizerState(ComPtr<ID3D11Device>& device);

	};
}