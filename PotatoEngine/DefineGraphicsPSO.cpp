#pragma once
#include "DefineGraphicsPSO.h" 

namespace Engine
{ 
	namespace PSO
	{
		ComPtr<ID3D11RasterizerState> solidRS;
		ComPtr<ID3D11RasterizerState> wireRS;
		ComPtr<ID3D11RasterizerState> solidNoneCullRS;
		ComPtr<ID3D11RasterizerState> wireNoneCullRS;
		ComPtr<ID3D11RasterizerState> postProcessRS; 



		ComPtr<ID3D11VertexShader> basicVS;
		ComPtr<ID3D11PixelShader>  basicPS;

		ComPtr<ID3D11VertexShader> normalVS;
		ComPtr<ID3D11GeometryShader> normalGS;
		ComPtr<ID3D11PixelShader> normalPS;

		ComPtr<ID3D11VertexShader> cubeMapVS;
		ComPtr<ID3D11PixelShader> cubeMapPS;

		ComPtr<ID3D11VertexShader> samplingVS;
		ComPtr<ID3D11PixelShader> samplingPS;
		ComPtr<ID3D11PixelShader> bloomDownPS;
		ComPtr<ID3D11PixelShader> bloomUpPS;
		ComPtr<ID3D11PixelShader> combinePS; 

		ComPtr<ID3D11VertexShader> billboardVS; 
		ComPtr<ID3D11GeometryShader> billboardGS; 
		ComPtr<ID3D11PixelShader> billboardPS; 

		ComPtr<ID3D11VertexShader> tessellationQuadVS;
		ComPtr<ID3D11HullShader> tessellationQuadHS;
		ComPtr<ID3D11DomainShader> tessellationQuadDS;
		ComPtr<ID3D11PixelShader> tessellationQuadPS;


		ComPtr<ID3D11InputLayout> basicIL;
		ComPtr<ID3D11InputLayout> pos4IL;



		ComPtr<ID3D11SamplerState> linearWarpSS;
		ComPtr<ID3D11SamplerState> linearClampSS;
		std::vector<ID3D11SamplerState*> samplerStates;



		GraphicsPSO defaultSolidPSO;
		GraphicsPSO defaultWirePSO;
		GraphicsPSO normalPSO;
		GraphicsPSO solidNoneCullPSO;
		GraphicsPSO wireNoneCullPSO;
		GraphicsPSO cubeMapSolidPSO;
		GraphicsPSO cubeMapWirePSO;
		GraphicsPSO postProcessPSO; 
		GraphicsPSO billboardPSO;
		GraphicsPSO tessellationQuadSolidPSO;
		GraphicsPSO tessellationQuadWirePSO;
	}

	void PSO::InitGraphicsPSO(ComPtr<ID3D11Device>& device) {
		InitShader(device);
		InitRasterizerState(device);
		InitSamplerState(device); 

		defaultSolidPSO.vertexShader = basicVS;
		defaultSolidPSO.pixelShader = basicPS;
		defaultSolidPSO.inputLayout = basicIL;
		defaultSolidPSO.rasterizerState = solidRS; 

		defaultWirePSO = defaultSolidPSO;
		defaultWirePSO.rasterizerState = wireRS;

		normalPSO = defaultSolidPSO;
		normalPSO.vertexShader = normalVS;
		normalPSO.geometryShader = normalGS;
		normalPSO.pixelShader = normalPS;
		normalPSO.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; 

		solidNoneCullPSO = defaultSolidPSO;
		solidNoneCullPSO.rasterizerState = solidNoneCullRS; 

		wireNoneCullPSO = defaultWirePSO;
		wireNoneCullPSO.rasterizerState = wireNoneCullRS; 

		cubeMapSolidPSO = defaultSolidPSO;
		cubeMapSolidPSO.vertexShader = cubeMapVS; 
		cubeMapSolidPSO.pixelShader = cubeMapPS; 

		cubeMapWirePSO = cubeMapSolidPSO; 
		cubeMapWirePSO.rasterizerState = wireRS; 

		postProcessPSO = defaultSolidPSO;
		postProcessPSO.vertexShader = samplingVS;
		postProcessPSO.pixelShader = samplingPS;
		postProcessPSO.rasterizerState = postProcessRS; 

		billboardPSO = defaultSolidPSO; 
		billboardPSO.vertexShader = billboardVS;
		billboardPSO.geometryShader = billboardGS;
		billboardPSO.pixelShader = billboardPS; 
		billboardPSO.inputLayout = pos4IL; 
		billboardPSO.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; 

		tessellationQuadSolidPSO = defaultSolidPSO; 
		tessellationQuadSolidPSO.vertexShader = tessellationQuadVS;
		tessellationQuadSolidPSO.hullShader = tessellationQuadHS; 
		tessellationQuadSolidPSO.domainShader = tessellationQuadDS; 
		tessellationQuadSolidPSO.pixelShader = tessellationQuadPS; 
		tessellationQuadSolidPSO.inputLayout = pos4IL; 
		tessellationQuadSolidPSO.primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST; 

		tessellationQuadWirePSO = tessellationQuadSolidPSO; 
		tessellationQuadWirePSO.rasterizerState = wireRS; 
	}

	void PSO::InitShader(ComPtr<ID3D11Device>& device) {
		std::vector<D3D11_INPUT_ELEMENT_DESC> basicIEs{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
		}; 

		std::vector<D3D11_INPUT_ELEMENT_DESC> pos4IEs{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		}; 

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"BasicVS.hlsl", basicIEs, basicVS, basicIL);
		D3D11Utils::CreatePixelShader(device, L"BasicPS.hlsl", basicPS); 

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"NormalVS.hlsl", basicIEs, normalVS, basicIL); 
		D3D11Utils::CreateGeometryShader(device, L"NormalGS.hlsl", normalGS);
		D3D11Utils::CreatePixelShader(device, L"NormalPS.hlsl", normalPS); 

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"CubeMapVS.hlsl", basicIEs, cubeMapVS, basicIL);
		D3D11Utils::CreatePixelShader(device, L"CubeMapPS.hlsl", cubeMapPS); 

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"SamplingVS.hlsl", basicIEs, samplingVS, basicIL); 
		D3D11Utils::CreatePixelShader(device, L"SamplingPS.hlsl", samplingPS);
		D3D11Utils::CreatePixelShader(device, L"BloomDownPS.hlsl", bloomDownPS);
		D3D11Utils::CreatePixelShader(device, L"BloomUpPS.hlsl", bloomUpPS);
		D3D11Utils::CreatePixelShader(device, L"CombinePS.hlsl", combinePS); 

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"BillboardVS.hlsl", pos4IEs, billboardVS, pos4IL);
		D3D11Utils::CreateGeometryShader(device, L"BillboardGS.hlsl", billboardGS); 
		D3D11Utils::CreatePixelShader(device, L"BillboardPS.hlsl", billboardPS); 

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"TessellationQuadVS.hlsl", pos4IEs, tessellationQuadVS, pos4IL); 
		D3D11Utils::CreateHullShader(device, L"TessellationQuadHS.hlsl", tessellationQuadHS);
		D3D11Utils::CreateDomainShader(device, L"TessellationQuadDS.hlsl", tessellationQuadDS);
		D3D11Utils::CreatePixelShader(device, L"TessellationQuadPS.hlsl", tessellationQuadPS); 
	}

	void PSO::InitRasterizerState(ComPtr<ID3D11Device>& device) {
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK; 
		rd.FrontCounterClockwise = false;
		rd.DepthClipEnable = true;
		rd.MultisampleEnable = true;

		device->CreateRasterizerState(&rd, solidRS.GetAddressOf());

		rd.CullMode = D3D11_CULL_NONE;

		device->CreateRasterizerState(&rd, solidNoneCullRS.GetAddressOf());

		rd.FillMode = D3D11_FILL_WIREFRAME;

		device->CreateRasterizerState(&rd, wireNoneCullRS.GetAddressOf());

		rd.CullMode = D3D11_CULL_BACK;

		device->CreateRasterizerState(&rd, wireRS.GetAddressOf()); 

		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_NONE;
		rd.FrontCounterClockwise = false; 
		rd.DepthClipEnable = false; 

		device->CreateRasterizerState(&rd, postProcessRS.GetAddressOf()); 
	}

	void PSO::InitSamplerState(ComPtr<ID3D11Device>& device)
	{
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;

		device->CreateSamplerState(&sd, linearWarpSS.GetAddressOf()); 
		samplerStates.push_back(linearWarpSS.Get());

		sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; 
		sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		device->CreateSamplerState(&sd, linearClampSS.GetAddressOf());
		samplerStates.push_back(linearClampSS.Get());
	}
}