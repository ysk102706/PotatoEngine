#define _CRT_SECURE_NO_WARNINGS
#include "ResourceLoader.h"
#include <vector> 
#include <iostream> 

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

namespace Engine {
	void ReadImage(const std::string filename, int& width, int& height, int& channels, std::vector<uint8_t>& image)
	{
		unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		image.resize(width * height * 4);

		if (channels <= 4) {
			for (int i = 0; i < width * height; i++) {
				for (int j = 0; j < 4; j++) image[i * 4 + j] = j < channels ? img[i * 4 + j] : 255;
			}
		}
		else std::cout << "Can not read " << channels << " channels\n";

		delete[] img;
	}

	void ResourceLoader::CreateTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::string filename, ComPtr<ID3D11Texture2D>& tex, ComPtr<ID3D11ShaderResourceView>& srv)
	{
		int width, height, channels;
		std::vector<uint8_t> image;

		ReadImage(filename, width, height, channels, image);

		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = width;
		td.Height = height; 
		td.MipLevels = 1;
		td.ArraySize = 1; 
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.Usage = D3D11_USAGE_IMMUTABLE;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.MiscFlags = 0;
		td.CPUAccessFlags = 0; 

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = image.data();
		sd.SysMemPitch = width * sizeof(uint8_t) * 4;

		device->CreateTexture2D(&td, &sd, tex.GetAddressOf()); 
		device->CreateShaderResourceView(tex.Get(), 0, srv.GetAddressOf()); 

		//context->GenerateMips(srv.Get()); 
	}
}