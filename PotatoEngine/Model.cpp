#include "Model.h"
#include <memory>
#include "D3D11Utils.h"
#include <directxtk/SimpleMath.h>
#include "ResourceLoader.h"

namespace Engine
{
	using DirectX::SimpleMath::Matrix;

Engine::Model::Model()
{
}

Engine::Model::Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::vector<MeshData>& meshData)
{
	Initialize(device, context, meshData);
}

void Engine::Model::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::vector<MeshData>& meshData)
{ 
	meshConstantCPU.world = Matrix();

	D3D11Utils::CreateConstantBuffer(device, meshConstantCPU, meshConstantGPU); 

	for (const auto& a : meshData) {
		auto newMesh = std::make_shared<Mesh>(); 
		newMesh->stride = sizeof(Vertex);
		newMesh->vertexCount = a.vertices.size(); 
		newMesh->indexCount = a.indices.size();

		D3D11Utils::CreateVertexBuffer(device, a.vertices, newMesh->vertexBuffer); 
		D3D11Utils::CreateIndexBuffer(device, a.indices, newMesh->indexBuffer); 

		if (!a.albedoTextureFile.empty()) {
			ResourceLoader::CreateTexture(device, context, a.albedoTextureFile, newMesh->albedoTexture, newMesh->albedoSRV);
		}

		newMesh->vertexConstantBuffer = meshConstantGPU; 

		meshes.push_back(newMesh);
	}
}

void Model::Render(ComPtr<ID3D11DeviceContext>& context)
{
	for (const auto& a : meshes) { 
		context->VSSetConstantBuffers(0, 1, a->vertexConstantBuffer.GetAddressOf());

		std::vector<ID3D11ShaderResourceView*> SRVs = {
			a->albedoSRV.Get()
		};
		context->PSSetShaderResources(0, SRVs.size(), SRVs.data());

		context->IASetVertexBuffers(0, 1, a->vertexBuffer.GetAddressOf(), &a->stride, &a->offset);
		context->IASetIndexBuffer(a->indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->DrawIndexed(a->indexCount, 0, 0);
	}
}

void Model::NormalRender(ComPtr<ID3D11DeviceContext>& context)
{
	for (const auto& a : meshes) { 
		context->GSSetConstantBuffers(0, 1, a->vertexConstantBuffer.GetAddressOf());

		context->IASetVertexBuffers(0, 1, a->vertexBuffer.GetAddressOf(), &a->stride, &a->offset);
		context->Draw(a->vertexCount, 0);
	}
}

void Model::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context)
{
	D3D11Utils::UpdateConstantBuffer(context, meshConstantCPU, meshConstantGPU);
}

}
