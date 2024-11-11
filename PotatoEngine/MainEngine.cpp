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

		CreateCubeMap(L"../Resources/Texture/CubeMap/", L"SampleEnvMDR.dds", L"SampleDiffuseMDR.dds", L"SampleSpecularMDR.dds");
		
		MeshData envMapMeshData = DefaultObjectGenerator::MakeBox(40.0f); 
		std::reverse(envMapMeshData.indices.begin(), envMapMeshData.indices.end());
		m_envMap = std::make_shared<Model>(m_device, m_context, std::vector{ envMapMeshData });

		//auto meshData = DefaultObjectGenerator::MakeSquareGrid(1.0f, 3, 2); 
		//auto meshData = DefaultObjectGenerator::MakeBox(1.0f);
		//auto meshData = DefaultObjectGenerator::MakeCylinder(1.0f, 1.5f, 2.0f, 20, 5); 
		auto meshData = DefaultObjectGenerator::MakeSphere(2.0f, 50, 50); 
		//auto meshData = DefaultObjectGenerator::ReadFromFile("../Resources/3D_Model/", "stanford_dragon.stl", false);
		meshData = DefaultObjectGenerator::SubdivideToSphere(1.5f, meshData);
		meshData = DefaultObjectGenerator::SubdivideToSphere(1.5f, meshData);
		meshData.albedoTextureFile = "../Resources/Texture/hanbyeol.png";
		auto model = std::make_shared<Model>(m_device, m_context, std::vector{ meshData });
		//auto model = std::make_shared<Model>(m_device, m_context, meshData);
		m_objectList.push_back(model); 

		globalConstantCPU.light.pos = Vector3(0.0f, 0.0f, -1.0f);
		globalConstantCPU.light.dir = Vector3(0.0f, 0.0f, 1.0f);
		globalConstantCPU.light.strength = Vector3(1.0f);

		return true;
	}

	void MainEngine::Update()
	{ 
		InputProcess(); 
		EulerCalc(); 

		Matrix view = camera.GetView();
		Matrix proj = camera.GetProj();
		Vector3 eyePos = camera.GetPos(); 

		UpdateGlobalConstant(view, proj, eyePos); 

		static float rot = 0;
		//rot += 0.002f;
		for (auto& a : m_objectList) { 
			//Matrix sr = Matrix::CreateScale(Vector3(1.0f, 1.0f, 1.0f)) * Matrix::CreateRotationX(90 * DirectX::XM_PI / 180) * Matrix::CreateRotationY(rot);
			Matrix sr = Matrix::CreateScale(Vector3(1.0f, 1.0f, 1.0f)) * Matrix::CreateRotationY(rot);
			a->modelConstantCPU.world = (sr * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 3.0f))).Transpose();
			a->modelConstantCPU.invTranspose = sr.Invert();
		} 

		for (auto& a : m_objectList) {
			a->UpdateConstantBuffer(m_context);
		} 

		postProcess.samplingFilter.UpdateConstantBuffer(m_context);
		postProcess.combineFilter.UpdateConstantBuffer(m_context);
	}

	void MainEngine::Render()
	{
		SetDefaultViewport(); 

		m_context->VSSetSamplers(0, PSO::samplerStates.size(), PSO::samplerStates.data());
		m_context->PSSetSamplers(0, PSO::samplerStates.size(), PSO::samplerStates.data());
		
		std::vector<ID3D11ShaderResourceView*> srvs = {
			m_envSRV.Get(), m_diffuseSRV.Get(), m_specularSRV.Get() 
		}; 
		m_context->PSSetShaderResources(10, srvs.size(), srvs.data());

		SetGlobalConstant(); 

		m_context->OMSetRenderTargets(1, postProcessRTV.GetAddressOf(), m_DSV.Get());
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_context->ClearRenderTargetView(postProcessRTV.Get(), color);  
		m_context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);

		SetGraphicsPSO(useWire ? PSO::cubeMapWirePSO : PSO::cubeMapSolidPSO); 
		
		m_envMap->Render(m_context);

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

		SetGraphicsPSO(PSO::postProcessPSO);
		postProcess.Render(m_context); 
	}

	namespace GUI {
		float diffuse;
		float specular; 
	}

	void MainEngine::UpdateGUI() { 

		if (ImGui::TreeNode("Resterizer")) {
			ImGui::Checkbox("useWire", &useWire);
			ImGui::Checkbox("useNormal", &useNormal);
			ImGui::Checkbox("useBackFaceCull", &useBackFaceCull); 
			ImGui::SliderFloat("envStrength", &globalConstantCPU.envStrength, 0.0f, 5.0f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Phong")) {
			ImGui::SliderFloat("diffuse", &GUI::diffuse, 0.0f, 3.0f);
			ImGui::SliderFloat("specular", &GUI::specular, 0.0f, 3.0f);
			ImGui::SliderFloat("shininess", &m_objectList[0]->materialConstantCPU.mat.shininess, 0.0f, 20.0f);
			
			ImGui::TreePop();
		}

		m_objectList[0]->materialConstantCPU.mat.diffuse = Vector3(GUI::diffuse);
		m_objectList[0]->materialConstantCPU.mat.specular = Vector3(GUI::specular);

		if (ImGui::TreeNode("Rim")) {
			ImGui::Checkbox("useRim", &m_objectList[0]->materialConstantCPU.rim.useRim);
			ImGui::SliderFloat3("color", &m_objectList[0]->materialConstantCPU.rim.color.x, 0.0f, 1.0f);
			ImGui::SliderFloat("strength", &m_objectList[0]->materialConstantCPU.rim.strength, 0.0f, 10.0f);
			ImGui::SliderFloat("factor", &m_objectList[0]->materialConstantCPU.rim.factor, 0.0f, 10.0f);
			ImGui::Checkbox("useSmoothStep", &m_objectList[0]->materialConstantCPU.rim.useSmoothStep);
			
			ImGui::TreePop();
		} 

		if (ImGui::TreeNode("Fresnel")) {
			ImGui::Checkbox("useFresnel", &m_objectList[0]->materialConstantCPU.fresnel.useFresnel);
			ImGui::SliderFloat3("fresnelR0", &m_objectList[0]->materialConstantCPU.fresnel.fresnelR0.x, 0.0f, 1.0f);

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("PostProcess")) { 
			ImGui::Checkbox("useSampling", &postProcess.samplingFilter.useSamplingFilter); 
			ImGui::SliderFloat("strength", &postProcess.combineFilter.imageFilterConstantCPU.bloomStrength, 0.0, 1.0);

			ImGui::TreePop();
		}
	}
}
