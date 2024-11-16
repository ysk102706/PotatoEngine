#include "MainEngine.h"
#include <vector> 
#include <directXtk/SimpleMath.h>
#include <iostream>
#include "Vertex.h"
#include "D3D11Utils.h"
#include "DefaultObjectGenerator.h"
#include "DefineGraphicsPSO.h"
#include "ResourceLoader.h"
#include <vector> 

namespace Engine {

	MainEngine::MainEngine() : EngineBase()
	{
	}

	bool MainEngine::Initialize()
	{
		if (!EngineBase::Initialize()) return false; 

		//std::vector<Vector4> billboardPoints; 
		//Vector4 p = Vector4(-20.0f, 1.0f, 20.0f, 1.0f);
		//for (int i = 0; i < 20; i++) {
		//	billboardPoints.push_back(p);
		//	p.x += 2.0f; 
		//}
		//billboards.Initialize(m_device, m_context, billboardPoints, 1.8f);

		auto tq = DefaultObjectGenerator::MakeTessellationQuad();
		tessellationQuad.Initialize(m_device, m_context, tq); 

		CreateCubeMap(L"../Resources/Texture/CubeMap/", L"SampleEnvMDR.dds", L"SampleDiffuseMDR.dds", L"SampleSpecularMDR.dds");
		
		MeshData envMapMeshData = DefaultObjectGenerator::MakeBox(40.0f); 
		std::reverse(envMapMeshData.indices.begin(), envMapMeshData.indices.end()); 
		m_envMap = std::make_shared<Model>(m_device, m_context, std::vector{ envMapMeshData }); 

		{
			Vector3 pos = Vector3(0.0f, 0.0f, 3.0f);
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
			model->isVisible = false; 
			model->modelConstantCPU.world = Matrix::CreateTranslation(pos).Transpose(); 
			model->UpdateConstantBuffer(m_context); 

			m_objectList.push_back(model);

			m_objectBS = DirectX::BoundingSphere(pos, 1.5f); 
		}

		{
			auto meshData = DefaultObjectGenerator::MakeSphere(0.03f, 10, 10); 
			m_cursorSphere = std::make_shared<Model>(m_device, m_context, std::vector{ meshData }); 
			m_cursorSphere->isVisible = false; 
			m_cursorSphere->materialConstantCPU.useAmbient = true;
			m_cursorSphere->materialConstantCPU.mat.ambient = Vector3(1.0f, 1.0f, 0.0f); 
			m_cursorSphere->UpdateConstantBuffer(m_context); 

			m_objectList.push_back(m_cursorSphere); 
		}

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

		Vector3 pickPoint; 
		Quaternion q; 
		Vector3 dragTranslation;
		if (MousePicking(m_objectBS, q, dragTranslation, pickPoint)) {  
			Matrix& world = m_objectList[0]->modelConstantCPU.world; 
			world = world.Transpose();
			Vector3 translation = world.Translation();
			world.Translation(Vector3(0.0f)); 
			world = world * Matrix::CreateFromQuaternion(q) * Matrix::CreateTranslation(dragTranslation + translation); 
			world = world.Transpose();
			m_objectBS.Center = dragTranslation + translation; 

			m_cursorSphere->modelConstantCPU.world =  Matrix::CreateTranslation(pickPoint).Transpose(); 
			m_cursorSphere->isVisible = true; 
		}
		else {
			m_cursorSphere->isVisible = false; 
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
		
		//SetGraphicsPSO(PSO::billboardPSO); 
		
		//billboards.Render(m_context); 

		SetGraphicsPSO(useWire ? PSO::tessellationQuadWirePSO : PSO::tessellationQuadSolidPSO); 

		tessellationQuad.Render(m_context); 

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
