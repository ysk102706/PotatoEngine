#pragma once
#include "EngineBase.h" 
#include "Mesh.h"
#include "Model.h"

namespace Engine {
	class MainEngine : public EngineBase
	{
	public: 
		MainEngine();

		virtual bool Initialize() override;
		virtual void Render() override;
		virtual void Update() override;

	private: 
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader> pixelShader;
		ComPtr<ID3D11InputLayout> inputLayout; 

		ComPtr<ID3D11RasterizerState> rasterizerState;

		std::vector<std::shared_ptr<Model>> m_objectList;

	};
}