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
		extern ComPtr<ID3D11RasterizerState> postProcessRS;



		extern ComPtr<ID3D11VertexShader> basicVS;
		extern ComPtr<ID3D11PixelShader>  basicPS;

		extern ComPtr<ID3D11VertexShader> normalVS;
		extern ComPtr<ID3D11GeometryShader> normalGS;
		extern ComPtr<ID3D11PixelShader> normalPS;
		
		extern ComPtr<ID3D11VertexShader> cubeMapVS;
		extern ComPtr<ID3D11PixelShader> cubeMapPS; 
		
		extern ComPtr<ID3D11VertexShader> samplingVS;
		extern ComPtr<ID3D11PixelShader> samplingPS; 
		extern ComPtr<ID3D11PixelShader> bloomDownPS;
		extern ComPtr<ID3D11PixelShader> bloomUpPS; 
		extern ComPtr<ID3D11PixelShader> combinePS; 

		extern ComPtr<ID3D11VertexShader> billboardVS;
		extern ComPtr<ID3D11GeometryShader> billboardGS; 
		extern ComPtr<ID3D11PixelShader> billboardPS; 

		extern ComPtr<ID3D11VertexShader> tessellationQuadVS;
		extern ComPtr<ID3D11HullShader> tessellationQuadHS;
		extern ComPtr<ID3D11DomainShader> tessellationQuadDS;
		extern ComPtr<ID3D11PixelShader> tessellationQuadPS;



		extern ComPtr<ID3D11InputLayout> basicIL; 
		extern ComPtr<ID3D11InputLayout> pos4IL; 



		extern ComPtr<ID3D11SamplerState> linearWarpSS;
		extern ComPtr<ID3D11SamplerState> linearClampSS;
		extern std::vector<ID3D11SamplerState*> samplerStates;
		


		extern GraphicsPSO defaultSolidPSO;
		extern GraphicsPSO defaultWirePSO; 
		extern GraphicsPSO normalPSO;
		extern GraphicsPSO solidNoneCullPSO;
		extern GraphicsPSO wireNoneCullPSO;
		extern GraphicsPSO cubeMapSolidPSO;
		extern GraphicsPSO cubeMapWirePSO; 
		extern GraphicsPSO postProcessPSO; 
		extern GraphicsPSO billboardPSO; 
		extern GraphicsPSO tessellationQuadSolidPSO; 
		extern GraphicsPSO tessellationQuadWirePSO; 
		
		void InitGraphicsPSO(ComPtr<ID3D11Device>& device);

		void InitShader(ComPtr<ID3D11Device>& device);
		void InitRasterizerState(ComPtr<ID3D11Device>& device);
		void InitSamplerState(ComPtr<ID3D11Device>& device);
	};
}