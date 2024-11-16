#include "TessellationQuad.h" 
#include "Vertex.h" 
#include "D3D11Utils.h"

namespace Engine
{
	void TessellationQuad::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, std::vector<Vector4>& vertices)
	{ 
		indexCount = vertices.size(); 
		D3D11Utils::CreateVertexBuffer(device, vertices, vertexBuffer); 

		tessellationConstantCPU.distance = 3.0f; 
		D3D11Utils::CreateConstantBuffer(device, tessellationConstantCPU, tessellationConstantGPU); 
		D3D11Utils::UpdateConstantBuffer(context, tessellationConstantCPU, tessellationConstantGPU); 
	} 

	void TessellationQuad::Render(ComPtr<ID3D11DeviceContext>& context) 
	{ 
		UINT stride = sizeof(Vector4); 
		UINT offset = 0; 

		context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		context->HSSetConstantBuffers(0, 1, tessellationConstantGPU.GetAddressOf()); 
		context->Draw(indexCount, 0); 
	}
}
