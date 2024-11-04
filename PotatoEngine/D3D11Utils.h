#pragma once
#include <d3d11.h> 
#include <string>
#include <wrl.h>
#include <vector>

namespace Engine 
{
	using Microsoft::WRL::ComPtr;

	class D3D11Utils {
	public:
		static void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device> &device, const std::wstring &filename, 
													 const std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElement, 
													 ComPtr<ID3D11VertexShader> &vertexShader, ComPtr<ID3D11InputLayout> &inputLayout);
		static void CreateGeometryShader(ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11GeometryShader>& geometryShader); 
		static void CreatePixelShader(ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader); 


		template<typename T_Vertex>
		static void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const std::vector<T_Vertex>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer) {
			D3D11_BUFFER_DESC vd;
			ZeroMemory(&vd, sizeof(vd));
			vd.ByteWidth = sizeof(T_Vertex) * vertices.size();
			vd.Usage = D3D11_USAGE_IMMUTABLE;
			vd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vd.CPUAccessFlags = 0;
			vd.StructureByteStride = sizeof(T_Vertex);

			D3D11_SUBRESOURCE_DATA vsd = { 0 };
			vsd.pSysMem = vertices.data();

			device->CreateBuffer(&vd, &vsd, vertexBuffer.GetAddressOf());
		}
		static void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& pixelBuffer); 
		
		template<typename T_Data>
		static void CreateConstantBuffer(ComPtr<ID3D11Device>& device, const T_Data& ConstantData, ComPtr<ID3D11Buffer>& constantBuffer) {
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.ByteWidth = sizeof(ConstantData);
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd = { 0 };
			sd.pSysMem = &ConstantData; 

			device->CreateBuffer(&bd, &sd, constantBuffer.GetAddressOf());
		}

		template<typename T_Data>
		static void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context, const T_Data& ConstantData, ComPtr<ID3D11Buffer>& buffer) {
			D3D11_MAPPED_SUBRESOURCE sd;
			context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &sd);
			memcpy(sd.pData, &ConstantData, sizeof(ConstantData));
			context->Unmap(buffer.Get(), NULL);
		}
	};
}