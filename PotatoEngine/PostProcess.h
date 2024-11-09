#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include "Mesh.h"
#include "ImageFilter.h"

namespace Engine 
{
	using Microsoft::WRL::ComPtr;

	class PostProcess {
	public: 

		void Initialize(ComPtr<ID3D11Device>& device, 
						ComPtr<ID3D11DeviceContext>& context, 
						int width, int height,
						const std::vector<ComPtr<ID3D11RenderTargetView>>& rtv,
						const std::vector<ComPtr<ID3D11ShaderResourceView>>& srv,
						int bloomLevels);
		void Render(ComPtr<ID3D11DeviceContext>& context); 
		void RenderImageFilter(ComPtr<ID3D11DeviceContext>& context, ImageFilter& imageFilter); 
		void CreateBuffer(ComPtr<ID3D11Device>& device, int width, int height, 
						  ComPtr<ID3D11RenderTargetView>& rtv, 
						  ComPtr<ID3D11ShaderResourceView>& srv);

		ImageFilter samplingFilter;
		ImageFilter combineFilter;

	private: 
		std::shared_ptr<Mesh> postProcessSquare; 

		std::vector<ImageFilter> bloomDownFilter;
		std::vector<ImageFilter> bloomUpFilter;

		std::vector<ComPtr<ID3D11RenderTargetView>> bloomRTVs;
		std::vector<ComPtr<ID3D11ShaderResourceView>> bloomSRVs;
		
	}; 
}