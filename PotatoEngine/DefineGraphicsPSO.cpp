#include "DefineGraphicsPSO.h" 

namespace Engine
{ 
	namespace DefineGraphicsPSO
	{
		ComPtr<ID3D11RasterizerState> solidRS;
		ComPtr<ID3D11RasterizerState> wireRS;

		ComPtr<ID3D11VertexShader> basicVS;
		ComPtr<ID3D11PixelShader>  basicPS;

		ComPtr<ID3D11InputLayout> basicIL;

		GraphicsPSO defaultSolidPSO;
		GraphicsPSO defaultWirePSO;
	}

	void DefineGraphicsPSO::InitGraphicsPSO(ComPtr<ID3D11Device>& device) {
		InitShader(device);
		InitRasterizerState(device);

		defaultSolidPSO.vertexShader = basicVS;
		defaultSolidPSO.pixelShader = basicPS;
		defaultSolidPSO.inputLayout = basicIL;
		defaultSolidPSO.rasterizerState = solidRS;

		defaultWirePSO = defaultSolidPSO;
		defaultWirePSO.rasterizerState = wireRS;
	}

	void DefineGraphicsPSO::InitShader(ComPtr<ID3D11Device>& device) {
		std::vector<D3D11_INPUT_ELEMENT_DESC> basicIEs{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(device, L"BasicVS.hlsl", basicIEs, basicVS, basicIL);
		D3D11Utils::CreatePixelShader(device, L"BasicPS.hlsl", basicPS);
	}

	void DefineGraphicsPSO::InitRasterizerState(ComPtr<ID3D11Device>& device) {
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.FrontCounterClockwise = false;
		rd.DepthClipEnable = true;
		rd.MultisampleEnable = true;

		device->CreateRasterizerState(&rd, solidRS.GetAddressOf());

		rd.FillMode = D3D11_FILL_WIREFRAME;

		device->CreateRasterizerState(&rd, wireRS.GetAddressOf());
	}
}