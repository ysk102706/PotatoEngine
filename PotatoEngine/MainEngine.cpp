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

		auto meshData = DefaultObjectGenerator::MakeBox(1.0f); 
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
			a->meshConstantCPU.world = (sr * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.5f))).Transpose();
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

		m_context->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), nullptr);
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_context->ClearRenderTargetView(m_backBufferRTV.Get(), color); 

		SetGraphicsPSO(PSO::defaultSolidPSO); 

		for (auto& a : m_objectList) {
			a->Render(m_context);
		}
	}

	void MainEngine::UpdateGUI() {
		ImGui::SliderFloat3("ambient", &globalConstantCPU.mat.ambient.x, 0.0f, 0.1f);
		ImGui::SliderFloat("diffuse", &diff, 0.0f, 1.0f); 
		ImGui::SliderFloat("specular", &spec, 0.0f, 1.0f); 
		globalConstantCPU.mat.diffuse = Vector3(diff);
		globalConstantCPU.mat.specular = Vector3(spec); 

		ImGui::SliderFloat("shininess", &globalConstantCPU.mat.shininess, 1.0f, 256.0f);
		ImGui::SliderFloat("spotFactor", &globalConstantCPU.light[0].spotFactor, 0.0f, 256.0f); 
		ImGui::SliderFloat("fallEnd", &globalConstantCPU.light[0].fallEnd, 1.0f, 10.0f);

		if (ImGui::RadioButton("Directional Light", globalConstantCPU.light[0].lightType == 0)) {
			globalConstantCPU.light[0].lightType = 0;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Point Light", globalConstantCPU.light[0].lightType == 1)) {
			globalConstantCPU.light[0].lightType = 1;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Spot Light", globalConstantCPU.light[0].lightType == 2)) {
			globalConstantCPU.light[0].lightType = 2;
		}
	}

}
