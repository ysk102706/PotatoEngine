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

		ComPtr<ID3D11VertexShader> basicVS;
		ComPtr<ID3D11PixelShader>  basicPS;
		ComPtr<ID3D11VertexShader> normalVS;
		ComPtr<ID3D11GeometryShader> normalGS;
		ComPtr<ID3D11PixelShader> normalPS;

		ComPtr<ID3D11InputLayout> basicIL;

		ComPtr<ID3D11SamplerState> linearWarpSS;
		ComPtr<ID3D11SamplerState> linearClampSS;
		std::vector<ID3D11SamplerState*> samplerStates;

		GraphicsPSO defaultSolidPSO;
		GraphicsPSO defaultWirePSO;
		GraphicsPSO normalPSO;
		GraphicsPSO solidNoneCullPSO;
		GraphicsPSO wireNoneCullPSO;
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
	}

	void PSO::InitShader(ComPtr<ID3D11Device>& device) {
		std::vector<D3D11_INPUT_ELEMENT_DESC> basicIEs{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"BasicVS.hlsl", basicIEs, basicVS, basicIL);
		D3D11Utils::CreatePixelShader(device, L"BasicPS.hlsl", basicPS); 

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"NormalVS.hlsl", basicIEs, normalVS, basicIL); 
		D3D11Utils::CreateGeometryShader(device, L"NormalGS.hlsl", normalGS);
		D3D11Utils::CreatePixelShader(device, L"NormalPS.hlsl", normalPS);

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