#include "D3D11Utils.h"
#include <d3dcompiler.h>

namespace Engine {
	void D3D11Utils::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device> &device, const std::wstring &filename, 
													  const std::vector<D3D11_INPUT_ELEMENT_DESC> &inputElements, 
													  ComPtr<ID3D11VertexShader> &vertexShader, ComPtr<ID3D11InputLayout> &inputLayout)
	{
		ComPtr<ID3DBlob> vsBuffer;

		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, vsBuffer.GetAddressOf(), 0);
		device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, vertexShader.GetAddressOf());

		device->CreateInputLayout(inputElements.data(), inputElements.size(), 
								  vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), inputLayout.GetAddressOf());
	}

	void D3D11Utils::CreateGeometryShader(ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11GeometryShader>& geometryShader)
	{
		ComPtr<ID3DBlob> gsBuffer;

		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", 0, 0, gsBuffer.GetAddressOf(), 0);
		device->CreateGeometryShader(gsBuffer->GetBufferPointer(), gsBuffer->GetBufferSize(), 0, geometryShader.GetAddressOf()); 
	}

	void D3D11Utils::CreatePixelShader(ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader)
	{
		ComPtr<ID3DBlob> psBuffer;

		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, psBuffer.GetAddressOf(), 0);
		device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, pixelShader.GetAddressOf());
	}





	void D3D11Utils::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
	{
		D3D11_BUFFER_DESC id;
		ZeroMemory(&id, sizeof(id));
		id.ByteWidth = sizeof(uint16_t) * indices.size();
		id.Usage = D3D11_USAGE_IMMUTABLE;
		id.BindFlags = D3D11_BIND_INDEX_BUFFER;
		id.CPUAccessFlags = 0;
		id.StructureByteStride = sizeof(uint16_t);

		D3D11_SUBRESOURCE_DATA isd = { 0 };
		isd.pSysMem = indices.data();

		device->CreateBuffer(&id, &isd, indexBuffer.GetAddressOf());
	}
}
