#include "EngineBase.h" 
#include <iostream>
#include "DefineGraphicsPSO.h"

namespace Engine { 
	LRESULT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
		
		DefineGraphicsPSO::InitGraphicsPSO(m_device);

		CreateBuffer();

		SetDefaultViewport(); 

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
				Update();
				Render(); 
				m_swapChain->Present(1, 0); 
			}
		}
	}

	void EngineBase::SetGraphicsPSO(const GraphicsPSO &pso)
	{
		m_context->VSSetShader(pso.vertexShader.Get(), 0, 0);
		m_context->PSSetShader(pso.pixelShader.Get(), 0, 0);
		m_context->RSSetState(pso.rasterizerState.Get()); 
		m_context->IASetInputLayout(pso.inputLayout.Get());
		m_context->IASetPrimitiveTopology(pso.primitiveTopology);
	}

}