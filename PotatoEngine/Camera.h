#pragma once 
#include <d3d11.h>
#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>

namespace Engine
{
	using DirectX::SimpleMath::Vector3; 
	using DirectX::SimpleMath::Matrix; 
	using DirectX::SimpleMath::Quaternion; 

	class Camera {
	public: 
		Camera(); 

		void MoveForward(float dt); 
		void MoveRight(float dt); 
		void MoveUp(float dt); 

		void Rotate(float _yaw, float _pitch);

		Matrix GetView(); 
		Matrix GetProj(); 

		Vector3 GetPos();

	private: 
		Vector3 pos; 
		Quaternion rot_quat; 
		
		Vector3 forward; 
		Vector3 right; 
		Vector3 up; 
		
		float moveSpeed; 

		float fov; 
		float aspect; 
		float zNear; 
		float zFar; 
		
	};
}