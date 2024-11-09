#include "PostProcess.h" 
#include "DefaultObjectGenerator.h" 
#include "D3D11Utils.h" 

namespace Engine 
{
	void PostProcess::Initialize(ComPtr<ID3D11Device>& device, 
								 ComPtr<ID3D11DeviceContext>& context,
								 int width, int height,
								 const std::vector<ComPtr<ID3D11RenderTargetView>>& rtv,
								 const std::vector<ComPtr<ID3D11ShaderResourceView>>& srv, 
								 int bloomLevels)
	{ 
		MeshData meshData = DefaultObjectGenerator::MakeSquareGrid(1.0f, 1, 1); 
		
		postProcessSquare = std::make_shared<Mesh>(); 
		D3D11Utils::CreateVertexBuffer(device, meshData.vertices, postProcessSquare->vertexBuffer);
		postProcessSquare->indexCount = meshData.indices.size();
		D3D11Utils::CreateIndexBuffer(device, meshData.indices, postProcessSquare->indexBuffer); 

		bloomRTVs.resize(bloomLevels); 
		bloomSRVs.resize(bloomLevels); 
		for (int i = 0; i < bloomLevels; i++) { 
			int div = pow(2, i);
			CreateBuffer(device, width / div, height / div, bloomRTVs[i], bloomSRVs[i]);
		}

		samplingFilter.Initialize(device, PSO::samplingPS, width, height);
		samplingFilter.SetSRV({srv[0]}); 
		samplingFilter.SetRTV({bloomRTVs[0]}); 
		samplingFilter.imageFilterConstantCPU.threshold = 0.7f; 
		samplingFilter.UpdateConstantBuffer(context); 
		
		bloomDownFilter.resize(bloomLevels - 1);
		for (int i = 0; i < bloomLevels - 1; i++) {
			int div = pow(2, i + 1);
			bloomDownFilter[i].Initialize(device, PSO::bloomDownPS, width / div, height / div);
			bloomDownFilter[i].SetSRV({ bloomSRVs[i] });
			bloomDownFilter[i].SetRTV({ bloomRTVs[i + 1] });
		}

		bloomUpFilter.resize(bloomLevels - 1);
		for (int i = 0; i < bloomLevels - 1; i++) { 
			int level = bloomLevels - 2 - i;
			int div = pow(2, level);
			bloomUpFilter[i].Initialize(device, PSO::bloomUpPS, width / div, height / div); 
			bloomUpFilter[i].SetSRV({ bloomSRVs[level + 1] });
			bloomUpFilter[i].SetRTV({ bloomRTVs[level] });
		} 

		combineFilter.Initialize(device, PSO::combinePS, width, height); 
		combineFilter.SetSRV({ srv[0], bloomSRVs[0] }); 
		combineFilter.SetRTV(rtv); 
		combineFilter.imageFilterConstantCPU.bloomStrength = 0.0f; 
		combineFilter.UpdateConstantBuffer(context); 
	} 

	void PostProcess::Render(ComPtr<ID3D11DeviceContext>& context)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, postProcessSquare->vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(postProcessSquare->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		if (samplingFilter.useSamplingFilter) {
			RenderImageFilter(context, samplingFilter); 
			bloomDownFilter[0].SetSRV({ bloomSRVs[0] });
		}
		else {
			bloomDownFilter[0].SetSRV(samplingFilter.shaderResourceViews); 
		}

		if (combineFilter.imageFilterConstantCPU.bloomStrength > 0.0f) {
			for (int i = 0; i < bloomDownFilter.size(); i++) {
				RenderImageFilter(context, bloomDownFilter[i]);
			}

			for (int i = 0; i < bloomDownFilter.size(); i++) {
				RenderImageFilter(context, bloomUpFilter[i]);
			}
		}

		RenderImageFilter(context, combineFilter);
	} 

	void PostProcess::RenderImageFilter(ComPtr<ID3D11DeviceContext>& context, ImageFilter& imageFilter)
	{
		imageFilter.Render(context);
		context->DrawIndexed(postProcessSquare->indexCount, 0, 0);
	}

	void PostProcess::CreateBuffer(ComPtr<ID3D11Device>& device, int width, int height, 
								   ComPtr<ID3D11RenderTargetView>& rtv, 
								   ComPtr<ID3D11ShaderResourceView>& srv)
	{
		ComPtr<ID3D11Texture2D> texture;

		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = width;
		td.Height = height;
		td.MipLevels = td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		td.MiscFlags = 0;
		td.CPUAccessFlags = 0;
		
		device->CreateTexture2D(&td, 0, texture.GetAddressOf()); 
		device->CreateRenderTargetView(texture.Get(), 0, rtv.GetAddressOf());
		device->CreateShaderResourceView(texture.Get(), 0, srv.GetAddressOf()); 
	}
}