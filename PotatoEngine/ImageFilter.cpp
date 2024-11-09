#include "ImageFilter.h"
 
namespace Engine 
{
	ImageFilter::ImageFilter(ComPtr<ID3D11Device>& device, ComPtr<ID3D11PixelShader>& ps, int width, int height)
	{
		Initialize(device, ps, width, height); 
	}

	void ImageFilter::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11PixelShader>& ps, int width, int height)
	{ 
		pixelShader = ps; 

		ZeroMemory(&viewport, sizeof(viewport));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width;
		viewport.Height = height; 
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1; 

		imageFilterConstantCPU.pixelDelta = Vector2(1.0f / width, 1.0f / height); 

		D3D11Utils::CreateConstantBuffer(device, imageFilterConstantCPU, imageFilterConstantGPU);
	}
	
	void ImageFilter::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context)
	{ 
		D3D11Utils::UpdateConstantBuffer(context, imageFilterConstantCPU, imageFilterConstantGPU); 
	}

	void ImageFilter::Render(ComPtr<ID3D11DeviceContext>& context)
	{
		context->RSSetViewports(1, &viewport);
		context->OMSetRenderTargets(renderTargetViews.size(), renderTargetViews.data(), 0);
		context->PSSetShader(pixelShader.Get(), 0, 0);
		context->PSSetShaderResources(0, shaderResourceViews.size(), shaderResourceViews.data());
		context->PSSetConstantBuffers(2, 1, imageFilterConstantGPU.GetAddressOf());
	}

	void ImageFilter::SetRTV(std::vector<ComPtr<ID3D11RenderTargetView>> rtvs)
	{
		renderTargetViews.clear(); 

		for (auto& a : rtvs) {
			renderTargetViews.push_back(a.Get());
		}
	}

	void ImageFilter::SetSRV(std::vector<ID3D11ShaderResourceView*>& srvs)
	{
		shaderResourceViews = srvs;
	}

	void ImageFilter::SetSRV(std::vector<ComPtr<ID3D11ShaderResourceView>> srvs)
	{ 
		shaderResourceViews.clear();

		for (auto& a : srvs) {
			shaderResourceViews.push_back(a.Get());
		}
	}


	
}