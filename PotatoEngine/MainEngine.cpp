#include "MainEngine.h"
#include <vector> 
#include <directXtk/SimpleMath.h>
#include <iostream>
#include "Vertex.h"
#include "D3D11Utils.h"
#include "DefaultObjectGenerator.h"
#include "DefineGraphicsPSO.h"
#include "ResourceLoader.h"

namespace Engine {

	MainEngine::MainEngine() : EngineBase()
	{
	}

	bool MainEngine::Initialize()
	{
		if (!EngineBase::Initialize()) return false;

		//auto meshData = DefaultObjectGenerator::MakeSquareGrid(1.0f, 3, 2); 
		//auto meshData = DefaultObjectGenerator::MakeCylinder(1.0f, 1.5f, 2.0f, 20, 5); 
		auto meshData = DefaultObjectGenerator::MakeSphere(2.0f, 4, 4); 
		meshData = DefaultObjectGenerator::SubdivideToSphere(1.5f, meshData);
		meshData = DefaultObjectGenerator::SubdivideToSphere(1.5f, meshData);
		meshData.albedoTextureFile = "../Resources/Texture/hanbyeol.png";
		auto model = std::make_shared<Model>(m_device, m_context, std::vector<MeshData>{ meshData }); 
		m_objectList.push_back(model); 

		return true;
	}

	void MainEngine::Update()
	{ 
		UpdateGlobalConstant();

		static float rot = 0;
		rot += 0.002f;
		for (auto& a : m_objectList) { 
			Matrix sr = Matrix::CreateScale(Vector3(1.0f, 1.0f, 1.0f)) * Matrix::CreateRotationY(rot);
			a->meshConstantCPU.world = (sr * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 3.0f))).Transpose();
			a->meshConstantCPU.invTranspose = sr.Invert();
		} 

		for (auto& a : m_objectList) {
			a->UpdateConstantBuffer(m_context);
		}

	}

	void MainEngine::Render()
	{
		SetDefaultViewport(); 

		m_context->VSSetSamplers(0, PSO::samplerStates.size(), PSO::samplerStates.data());
		m_context->PSSetSamplers(0, PSO::samplerStates.size(), PSO::samplerStates.data());
		
		SetGlobalConstant();

		m_context->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), m_DSV.Get());
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_context->ClearRenderTargetView(m_backBufferRTV.Get(), color);  
		m_context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);

		if (useBackFaceCull) SetGraphicsPSO(useWire ? PSO::defaultWirePSO : PSO::defaultSolidPSO); 
		else SetGraphicsPSO(useWire ? PSO::wireNoneCullPSO : PSO::solidNoneCullPSO); 

		for (auto& a : m_objectList) {
			a->Render(m_context);
		}

		if (useNormal) {
			SetGraphicsPSO(PSO::normalPSO);

			for (auto& a : m_objectList) {
				a->NormalRender(m_context);
			}
		}
	}

	void MainEngine::UpdateGUI() {
		ImGui::Checkbox("useWire", &useWire);
		ImGui::Checkbox("useNormal", &useNormal);
		ImGui::Checkbox("useBackFaceCull", &useBackFaceCull);
	}

}
