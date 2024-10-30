#include "MainEngine.h"
#include <vector> 
#include <directXtk/SimpleMath.h>
#include <iostream>
#include "Vertex.h"
#include "D3D11Utils.h"
#include "DefaultObjectGenerator.h"
#include "DefineGraphicsPSO.h"

namespace Engine {

	MainEngine::MainEngine() : EngineBase()
	{
	}

	bool MainEngine::Initialize()
	{
		if (!EngineBase::Initialize()) return false;

		auto meshData = DefaultObjectGenerator::MakeBox(1.0f); 
		auto Triangle = std::make_shared<Model>(m_device, m_context, std::vector<MeshData>{ meshData }); 
		m_objectList.push_back(Triangle);

		

		return true;
	}

	void Engine::MainEngine::Update()
	{ 
		static float rot = 0;
		float aspect = width / height; 
		rot += 0.02f;
		for (auto& a : m_objectList) {
			a->meshConstantCPU.world = (Matrix::CreateRotationY(rot) * Matrix::CreateTranslation(Vector3(0.0f, -0.3f, 1.0f))).Transpose(); 
			a->meshConstantCPU.view = DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -3.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
			a->meshConstantCPU.view = a->meshConstantCPU.view.Transpose();
			a->meshConstantCPU.proj = DirectX::XMMatrixPerspectiveFovLH(70.0f * DirectX::XM_PI / 180.0f, aspect, 0.1f, 1000.0f); 
			a->meshConstantCPU.proj = a->meshConstantCPU.proj.Transpose(); 
		}
		 
		for (auto& a : m_objectList) {
			a->UpdateConstantBuffer(m_context);
		}
	}

	void Engine::MainEngine::Render()
	{
		SetDefaultViewport();
		m_context->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), nullptr);
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_context->ClearRenderTargetView(m_backBufferRTV.Get(), color); 

		SetGraphicsPSO(DefineGraphicsPSO::defaultSolidPSO);

		for (auto& a : m_objectList) {
			a->Render(m_context);
		}
	}

}
