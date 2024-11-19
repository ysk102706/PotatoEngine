#pragma once
#include <d3d11.h>
#include <wrl.h> 
#include "DefineGraphicsPSO.h" 
#include <directxtk/SimpleMath.h> 

namespace Engine
{
	using Microsoft::WRL::ComPtr; 
	using DirectX::SimpleMath::Vector2;

	struct ImageFilterConstantData {
		Vector2 pixelDelta; 
		float bloomStrength;
		float threshold; 
		int useToneMapping; 
		float exposure; 
		float gamma; 
		float dummy; 
	};

	class ImageFilter { 
	public: 
		ImageFilter() {};
		ImageFilter(ComPtr<ID3D11Device>& device, ComPtr<ID3D11PixelShader>& ps,
					int width, int height);
		void Initialize(ComPtr<ID3D11Device>& device, 
						ComPtr<ID3D11PixelShader>& ps, int width, int height); 
		void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context); 
		void Render(ComPtr<ID3D11DeviceContext>& context); 
		void SetRTV(std::vector<ComPtr<ID3D11RenderTargetView>> rtvs);
		void SetSRV(std::vector<ID3D11ShaderResourceView*>& srvs); 
		void SetSRV(std::vector<ComPtr<ID3D11ShaderResourceView>> srvs); 

		std::vector<ID3D11RenderTargetView*> renderTargetViews;
		std::vector<ID3D11ShaderResourceView*> shaderResourceViews;
		
		ImageFilterConstantData imageFilterConstantCPU; 

		int useSamplingFilter;

	private: 
		D3D11_VIEWPORT viewport; 
		ComPtr<ID3D11PixelShader> pixelShader; 

		ComPtr<ID3D11Buffer> imageFilterConstantGPU; 

	};
}