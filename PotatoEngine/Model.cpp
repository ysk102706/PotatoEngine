#include "Model.h"
#include <memory>
#include "D3D11Utils.h"
#include <directxtk/SimpleMath.h>

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
	meshConstantCPU.world = meshConstantCPU.view = meshConstantCPU.proj = Matrix();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(meshConstantCPU);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; 
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = &meshConstantCPU;

	device->CreateBuffer(&bd, &sd, meshConstantGPU.GetAddressOf()); 

	for (const auto& a : meshData) {
		auto newMesh = std::make_shared<Mesh>(); 
		newMesh->stride = sizeof(Vertex);
		newMesh->indexCount = a.indices.size();
		newMesh->vertexCount = a.vertices.size(); 

		D3D11Utils::CreateVertexBuffer(device, a.vertices, newMesh->vertexBuffer); 
		D3D11Utils::CreateVertexBuffer(device, a.indices, newMesh->indexBuffer); 

		newMesh->vertexConstantBuffer = meshConstantGPU; 

		meshes.push_back(newMesh);
	}
}

void Model::Render(ComPtr<ID3D11DeviceContext>& context)
{
	for (const auto& a : meshes) { 
		context->VSSetConstantBuffers(0, 1, a->vertexConstantBuffer.GetAddressOf());
		context->IASetVertexBuffers(0, 1, a->vertexBuffer.GetAddressOf(), &a->stride, &a->offset);
		context->IASetIndexBuffer(a->indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->DrawIndexed(a->indexCount, 0, 0);
	}
}

void Model::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context)
{
	D3D11Utils::UpdateConstantBuffer(context, meshConstantCPU, meshConstantGPU);
}

}
