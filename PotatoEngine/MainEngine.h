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
		std::vector<std::shared_ptr<Model>> m_objectList; 

		ComPtr<ID3D11Texture2D> texture;
		ComPtr<ID3D11ShaderResourceView> SRV; 
		ComPtr<ID3D11SamplerState> ss;

	};
}