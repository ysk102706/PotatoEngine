#pragma once 
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include "MeshData.h"

namespace Engine
{
	using Microsoft::WRL::ComPtr;

	class ResourceLoader {
	public: 
		static void CreateTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::string filename, ComPtr<ID3D11Texture2D>& tex, ComPtr<ID3D11ShaderResourceView>& srv); 
		static std::vector<MeshData> LoadModel(std::string path, std::string filename, bool revertNormal);
		static void CreateDDSTexture(ComPtr<ID3D11Device>& device, const wchar_t* filepath, bool isCubeMap, ComPtr<ID3D11ShaderResourceView>& srv);
	};
} 