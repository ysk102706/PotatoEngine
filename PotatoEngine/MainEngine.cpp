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
		//auto meshData = DefaultObjectGenerator::MakeSphere(2.0f, 4, 4); 
		auto meshData = DefaultObjectGenerator::ReadFromFile("../Resources/3D_Model/", "stanford_dragon.stl", false);
		//meshData = DefaultObjectGenerator::SubdivideToSphere(1.5f, meshData);
		//meshData = DefaultObjectGenerator::SubdivideToSphere(1.5f, meshData);
		//meshData.albedoTextureFile = "../Resources/Texture/hanbyeol.png";
		auto model = std::make_shared<Model>(m_device, m_context, meshData); 
		m_objectList.push_back(model); 

		globalConstantCPU.light.pos = Vector3(0.0f, 0.0f, -1.0f);
		globalConstantCPU.light.dir = Vector3(0.0f, 0.0f, 1.0f);
		globalConstantCPU.light.strength = Vector3(1.0f);

		return true;
	}

	void MainEngine::Update()
	{ 
		UpdateGlobalConstant();

		static float rot = 0;
		rot += 0.002f;
		for (auto& a : m_objectList) { 
			Matrix sr = Matrix::CreateScale(Vector3(1.0f, 1.0f, 1.0f)) * Matrix::CreateRotationX(90 * DirectX::XM_PI / 180) * Matrix::CreateRotationY(rot);
			a->modelConstantCPU.world = (sr * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f))).Transpose();
			a->modelConstantCPU.invTranspose = sr.Invert();
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

	namespace GUI {
		float diffuse;
		float specular;
	}

	void MainEngine::UpdateGUI() {
		ImGui::Checkbox("useWire", &useWire);
		ImGui::Checkbox("useNormal", &useNormal);
		ImGui::Checkbox("useBackFaceCull", &useBackFaceCull); 

		ImGui::SliderFloat("diffuse", &GUI::diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("specular", &GUI::specular, 0.0f, 1.0f);
		ImGui::SliderFloat("shininess", &m_objectList[0]->materialConstantCPU.mat.shininess, 0.0f, 256.0f);

		m_objectList[0]->materialConstantCPU.mat.diffuse = Vector3(GUI::diffuse);
		m_objectList[0]->materialConstantCPU.mat.specular = Vector3(GUI::specular);
	}
}
