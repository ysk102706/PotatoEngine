#define _CRT_SECURE_NO_WARNINGS
#include "ResourceLoader.h"
#include <vector> 
#include <iostream> 

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include <filesystem>
#include <DirectXMesh.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <directxtk/SimpleMath.h> 

#include <directxtk/DDSTextureLoader.h>

namespace Engine {
	using DirectX::SimpleMath::Vector3;
	using DirectX::SimpleMath::Matrix;

	namespace ModelLoad {
		std::string path;
		bool isGLTF;
		bool revertNormal; 
		std::vector<MeshData> meshes;
	}

	void ReadImage(const std::string filename, int& width, int& height, int& channels, std::vector<uint8_t>& image)
	{
		unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		image.resize(width * height * 4);

		if (channels <= 4) {
			for (int i = 0; i < width * height; i++) {
				for (int j = 0; j < 4; j++) image[i * 4 + j] = j < channels ? img[i * channels + j] : 255;
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
	}

	void ResourceLoader::CreateMipMapTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::string filename, ComPtr<ID3D11Texture2D>& tex, ComPtr<ID3D11ShaderResourceView>& srv)
	{
		int width, height, channels;
		std::vector<uint8_t> image;

		ReadImage(filename, width, height, channels, image); 

		ComPtr<ID3D11Texture2D> stagingTexture = CreateStagingTexture(device, context, width, height, image); 

		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = width;
		td.Height = height;
		td.MipLevels = 0; 
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.Usage = D3D11_USAGE_DEFAULT; 
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		td.CPUAccessFlags = 0;

		device->CreateTexture2D(&td, 0, tex.GetAddressOf()); 
		
		context->CopySubresourceRegion(tex.Get(), 0, 0, 0, 0, stagingTexture.Get(), 0, 0); 

		device->CreateShaderResourceView(tex.Get(), 0, srv.GetAddressOf()); 

		context->GenerateMips(srv.Get());
	}

	ComPtr<ID3D11Texture2D> ResourceLoader::CreateStagingTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, int width, int height, std::vector<uint8_t>& image)
	{
		ComPtr<ID3D11Texture2D> stagingTexture;

		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.Usage = D3D11_USAGE_STAGING;
		td.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		device->CreateTexture2D(&td, 0, stagingTexture.GetAddressOf());

		D3D11_MAPPED_SUBRESOURCE ms;
		context->Map(stagingTexture.Get(), 0, D3D11_MAP_WRITE, 0, &ms); 
		uint8_t* pData = (uint8_t*)ms.pData; 
		for (int i = 0; i < height; i++) {
			memcpy(&pData[i * ms.RowPitch], &image[i * width * 4], width * sizeof(uint8_t) * 4);
		}
		context->Unmap(stagingTexture.Get(), 0); 

		return stagingTexture; 
	}

	std::string GetExtension(const std::string filename)
	{
		std::string ext(std::filesystem::path(filename).extension().string());
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		return ext;
	} 

	MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (UINT i = 0; i < mesh->mNumVertices; i++) {
			Vertex v;

			v.position.x = mesh->mVertices[i].x;
			v.position.y = mesh->mVertices[i].y;
			v.position.z = mesh->mVertices[i].z;

			v.normal.x = mesh->mNormals[i].x;
			if (ModelLoad::isGLTF) {
				v.normal.y = mesh->mNormals[i].z;
				v.normal.z = -mesh->mNormals[i].y;
			}
			else {
				v.normal.y = mesh->mNormals[i].y;
				v.normal.z = mesh->mNormals[i].z;
			}

			if (ModelLoad::revertNormal) {
				v.normal *= -1;
			}

			v.normal.Normalize();

			if (mesh->mTextureCoords[0]) {
				v.texcoord.x = float(mesh->mTextureCoords[0][i].x);
				v.texcoord.y = float(mesh->mTextureCoords[0][i].y);
			}

			vertices.push_back(v);
		}

		for (UINT i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		MeshData newMeshData;
		newMeshData.vertices = vertices;
		newMeshData.indices = indices; 

		/*if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 

			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

				std::string fullPath = ModelLoad::path + std::string(std::filesystem::path(path.C_Str()).filename().string());

				newMeshData.albedoTextureFile = fullPath;
			}
		}*/

		return newMeshData;
	}

	void ProcessNode(aiNode* node, const aiScene* scene, Matrix tr) {
		Matrix m;
		ai_real* temp = &node->mTransformation.a1;
		float* mTemp = &m._11;
		for (int i = 0; i < 16; i++) {
			mTemp[i] = float(temp[i]);
		}
		m = m.Transpose() * tr;

		for (UINT i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			auto newMesh = ProcessMesh(mesh, scene);

			for (auto& a : newMesh.vertices) {
				a.position = Vector3::Transform(a.position, m);
			}

			ModelLoad::meshes.push_back(newMesh);
		}

		for (UINT i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene, m);
		}
	} 

	void UpdateNormal() 
	{ 
		for (auto& a : ModelLoad::meshes) {
			std::vector<Vector3> normalSum(a.vertices.size(), Vector3(0.0f));
			std::vector<float> normalCount(a.vertices.size(), 0);

			for (int i = 0; i < a.indices.size(); i += 3) {
				int i1 = a.indices[i];
				int i2 = a.indices[i + 1];
				int i3 = a.indices[i + 2];

				Vertex v1 = a.vertices[i1];
				Vertex v2 = a.vertices[i2];
				Vertex v3 = a.vertices[i3];

				Vector3 faceNormal = (v2.position - v1.position).Cross(v3.position - v1.position);

				normalSum[i1] += faceNormal;
				normalSum[i2] += faceNormal;
				normalSum[i3] += faceNormal;

				normalCount[i1]++;
				normalCount[i2]++;
				normalCount[i3]++;
			}

			for (int i = 0; i < a.vertices.size(); i++) {
				if (normalCount[i]) {
					a.vertices[i].normal = normalSum[i] / normalCount[i];
					a.vertices[i].normal.Normalize();
				}
			}
		}
	} 

	void UpdateTangent()
	{
		for (auto& a : ModelLoad::meshes) {
			std::vector<DirectX::XMFLOAT3> pos(a.vertices.size()); 
			std::vector<DirectX::XMFLOAT3> normal(a.vertices.size()); 
			std::vector<DirectX::XMFLOAT2> tex(a.vertices.size()); 
			std::vector<DirectX::XMFLOAT3> tan(a.vertices.size()); 
			std::vector<DirectX::XMFLOAT3> bitan(a.vertices.size()); 

			for (int i = 0; i < a.vertices.size(); i++) {
				auto& v = a.vertices[i]; 
				pos[i] = v.position; 
				normal[i] = v.normal; 
				tex[i] = v.texcoord; 
			} 

			DirectX::ComputeTangentFrame(a.indices.data(), a.indices.size() / 3, pos.data(), normal.data(), tex.data(), a.vertices.size(), tan.data(), bitan.data()); 

			for (int i = 0; i < a.vertices.size(); i++) {
				a.vertices[i].tangent = tan[i]; 
			}
		}
	}

	std::vector<MeshData> ResourceLoader::LoadModel(std::string path, std::string filename, bool revertNormal)
	{
		ModelLoad::isGLTF = false;
		ModelLoad::revertNormal = false;
		ModelLoad::meshes.clear();
		
		if (GetExtension(filename) == ".gltf") {
			ModelLoad::isGLTF = true;
			ModelLoad::revertNormal = revertNormal;
		} 
		
		ModelLoad::path = path;

		Assimp::Importer importer; 

		const aiScene* scene = importer.ReadFile(
			path + filename,
			aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
		);

		if (scene) {
			Matrix tr; 
			ProcessNode(scene->mRootNode, scene, tr); 

			//UpdateNormal(); 
			//UpdateTangent(); 
		}
		else std::cout << "Failed to read file " << path + filename << '\n';

		return ModelLoad::meshes; 
	} 

	void ResourceLoader::CreateDDSTexture(ComPtr<ID3D11Device>& device, const wchar_t* filepath, bool isCubeMap, ComPtr<ID3D11ShaderResourceView>& srv)
	{
		ComPtr<ID3D11Texture2D> texture;

		UINT miscFlags = 0;
		DirectX::CreateDDSTextureFromFileEx(
			device.Get(), filepath, 0, D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE, 0, miscFlags, DirectX::DX11::DDS_LOADER_FLAGS(false), 
			(ID3D11Resource **)texture.GetAddressOf(), srv.GetAddressOf(), 0); 
	}
}