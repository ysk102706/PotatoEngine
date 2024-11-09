#include "EngineBase.h" 
#include <iostream>
#include "DefineGraphicsPSO.h"
#include "ResourceLoader.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

namespace Engine { 
	LRESULT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	EngineBase::EngineBase() : width(2880), height(1600), m_window(0), m_viewport(D3D11_VIEWPORT())
	{
	}

	EngineBase::EngineBase(float width, float height) : width(width), height(height), m_window(0), m_viewport(D3D11_VIEWPORT())
	{
	}

	bool EngineBase::Initialize()
	{
		if (!InitWindow()) return false;
		if (!InitD3D()) return false;
		if (!InitImGui()) return false;

		return true;
	}

	bool EngineBase::InitD3D()
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd)); 
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height; 
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferCount = 2;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_window;
		sd.Windowed = true;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0; 

		D3D_FEATURE_LEVEL featureLevels[1] = {
			D3D_FEATURE_LEVEL_11_0
		};

		D3D_FEATURE_LEVEL featureLevel;

		HRESULT ret = D3D11CreateDeviceAndSwapChain(
			0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 
			featureLevels, 1, D3D11_SDK_VERSION, &sd, 
			m_swapChain.GetAddressOf(), m_device.GetAddressOf(), 
			&featureLevel, m_context.GetAddressOf());

		if (FAILED(ret)) return false; 
		
		PSO::InitGraphicsPSO(m_device);

		CreateBuffer();

		SetDefaultViewport(); 

		D3D11Utils::CreateConstantBuffer(m_device, globalConstantCPU, globalConstantGPU); 

		return true;
	}

	bool EngineBase::InitWindow()
	{
		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
						 GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
						 L"PotatoEngine", NULL };

		if (!RegisterClassEx(&wc)) {
			return false;
		}

		RECT wr = { 0, 0, width, height };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);
		m_window = CreateWindow(wc.lpszClassName, L"PotatoEngine", WS_OVERLAPPEDWINDOW,
			0, 0, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, wc.hInstance, NULL);

		if (!m_window) return false;

		ShowWindow(m_window, SW_SHOWDEFAULT);
		UpdateWindow(m_window);

		return true;
	}

	bool EngineBase::InitImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.DisplaySize = ImVec2(width, height);
		ImGui::StyleColorsLight();

		if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get())) {
			return false;
		}

		if (!ImGui_ImplWin32_Init(m_window)) {
			return false;
		}

		return true;
	}

	void EngineBase::CreateBuffer()
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		if (backBuffer) {
			m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_backBufferRTV.GetAddressOf());
			backBuffer->Release();
		} 
		else {
			std::cout << "Create RTV failed\n";
		} 
		
		D3D11_TEXTURE2D_DESC td; 
		ZeroMemory(&td, sizeof(td));
		td.Width = width;
		td.Height = height;
		td.MipLevels = td.ArraySize = 1; 
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; 
		td.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; 
		td.Usage = D3D11_USAGE_DEFAULT;
		td.CPUAccessFlags = 0; 
		td.MiscFlags = 0; 
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0; 

		m_device->CreateTexture2D(&td, 0, postProcessBuffer.GetAddressOf()); 
		m_device->CreateRenderTargetView(postProcessBuffer.Get(), 0, postProcessRTV.GetAddressOf()); 
		m_device->CreateShaderResourceView(postProcessBuffer.Get(), 0, postProcessSRV.GetAddressOf());

		CreateDepthBuffer(); 

		postProcess.Initialize(m_device, m_context, width, height, { m_backBufferRTV.Get() }, { postProcessSRV.Get() }, 5); 
 	} 

	void EngineBase::CreateDepthBuffer()
	{
		D3D11_TEXTURE2D_DESC td; 
		ZeroMemory(&td, sizeof(td));
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		ComPtr<ID3D11Texture2D> depthStencilBuffer; 
		m_device->CreateTexture2D(&td, 0, depthStencilBuffer.GetAddressOf());
		m_device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, m_DSV.GetAddressOf());
	}

	void EngineBase::CreateCubeMap(std::wstring path, std::wstring env, std::wstring diffuse, std::wstring specular)
	{ 
		ResourceLoader::CreateDDSTexture(m_device, (path + env).c_str(), true, m_envSRV);
		ResourceLoader::CreateDDSTexture(m_device, (path + diffuse).c_str(), true, m_diffuseSRV);
		ResourceLoader::CreateDDSTexture(m_device, (path + specular).c_str(), true, m_specularSRV);
	}

	void EngineBase::SetDefaultViewport()
	{
		ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = width;
		m_viewport.Height = height;
		m_viewport.MinDepth = 0;
		m_viewport.MaxDepth = 1;

		m_context->RSSetViewports(1, &m_viewport);
	}

	void EngineBase::Run()
	{
		MSG msg = { 0 };
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				ImGui_ImplDX11_NewFrame(); 
				ImGui_ImplWin32_NewFrame();

				ImGui::NewFrame(); 
				ImGui::Begin("Scene Control");

				ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
					1000.0f / ImGui::GetIO().Framerate,
					ImGui::GetIO().Framerate);

				UpdateGUI(); 

				ImGui::End();
				ImGui::Render();

				Update();
				Render();

				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				m_swapChain->Present(1, 0); 
			}
		}
	}

	void EngineBase::SetGraphicsPSO(const GraphicsPSO& pso)
	{
		m_context->VSSetShader(pso.vertexShader.Get(), 0, 0); 
		m_context->GSSetShader(pso.geometryShader.Get(), 0, 0);
		m_context->PSSetShader(pso.pixelShader.Get(), 0, 0);
		m_context->RSSetState(pso.rasterizerState.Get()); 
		m_context->IASetInputLayout(pso.inputLayout.Get());
		m_context->IASetPrimitiveTopology(pso.primitiveTopology);
	}

	void EngineBase::SetGlobalConstant()
	{
		m_context->VSSetConstantBuffers(1, 1, globalConstantGPU.GetAddressOf());
		m_context->GSSetConstantBuffers(1, 1, globalConstantGPU.GetAddressOf());
		m_context->PSSetConstantBuffers(1, 1, globalConstantGPU.GetAddressOf());
	}

	void EngineBase::UpdateGlobalConstant()
	{
		globalConstantCPU.view = DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		globalConstantCPU.view = globalConstantCPU.view.Transpose();
		globalConstantCPU.proj = DirectX::XMMatrixPerspectiveFovLH(70.0f * DirectX::XM_PI / 180.0f, width / height, 0.1f, 1000.0f); 
		globalConstantCPU.proj = globalConstantCPU.proj.Transpose(); 
		globalConstantCPU.eyePos = Vector3(0.0f, 0.0f, -2.0f); 

		D3D11Utils::UpdateConstantBuffer(m_context, globalConstantCPU, globalConstantGPU);
	}
}