#pragma once
#include <d3d11.h>
#include <d3dcompiler.h> 
#include <windows.h>
#include <wrl.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h> 
#include <directxtk/SimpleMath.h>
#include "GraphicsPSO.h"

namespace Engine { 
	using Microsoft::WRL::ComPtr; 
	using DirectX::SimpleMath::Vector3; 
	using DirectX::SimpleMath::Matrix; 

	struct GlobalConstantData {
		Matrix view;
		Matrix proj; 
		Vector3 eyePos;
		float dummy;
		Vector3 lightPos; 
		float diffuse;
		float specular;
		Vector3 dummy1;
	};

	class EngineBase {
	public:
		EngineBase();
		EngineBase(float width, float height);
		virtual bool Initialize();

		bool InitD3D();
		bool InitWindow(); 
		bool InitImGui();

		void CreateBuffer(); 
		void SetDefaultViewport(); 

		void Run();

		virtual void Render() = 0;
		virtual void Update() = 0; 
		virtual void UpdateGUI() = 0; 

		void SetGraphicsPSO(const GraphicsPSO& pso);

		void SetGlobalConstant();
		void UpdateGlobalConstant(); 

		float width;
		float height;
		HWND m_window;
		UINT m_numQualityLevels = 0;

		ComPtr<ID3D11Device> m_device; 
		ComPtr<ID3D11DeviceContext> m_context; 
		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11RenderTargetView> m_backBufferRTV;

		D3D11_VIEWPORT m_viewport; 

		GlobalConstantData globalConstantCPU; 
		ComPtr<ID3D11Buffer> globalConstantGPU;
		
	};
}