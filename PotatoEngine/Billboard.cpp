#include "Billboard.h" 
#include "D3D11Utils.h" 
#include "Vertex.h"

namespace Engine
{
	void Billboard::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, std::vector<Vector4>& points, float width)
	{
		indexCount = points.size(); 
		D3D11Utils::CreateVertexBuffer(device, points, vertexBuffer); 

		billboardConstantCPU.width = width; 
		D3D11Utils::CreateConstantBuffer(device, billboardConstantCPU, billboardConstantGPU); 
		D3D11Utils::UpdateConstantBuffer(context, billboardConstantCPU, billboardConstantGPU);
	} 

	void Billboard::Render(ComPtr<ID3D11DeviceContext>& context)
	{ 
		UINT stride = sizeof(Vector4);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset); 
		context->GSSetConstantBuffers(0, 1, billboardConstantGPU.GetAddressOf());
		context->Draw(indexCount, 0); 
	} 
} 
