#include "Camera.h" 
#include <iostream>

namespace Engine
{
	Camera::Camera() : pos(Vector3(0.0f, 0.0f, -1.0f)), forward(Vector3(0.0f, 0.0f, 1.0f)), 
					   right(Vector3(1.0f, 0.0f, 0.0f)), up(Vector3(0.0f, 1.0f, 0.0f)), moveSpeed(1.0f), 
					   fov(70.0f), aspect(1.6f), zNear(0.1f), zFar(1000.0f) 
	{ 
	}
	
	void Camera::MoveForward(float dt)
	{ 
		pos += forward * dt * moveSpeed; 
	} 

	void Camera::MoveRight(float dt)
	{ 
		pos += right * dt * moveSpeed; 
	} 

	void Camera::MoveUp(float dt) 
	{ 
		pos += up * dt * moveSpeed; 
	}

	void Camera::Rotate(float _yaw, float _pitch)
	{
		Quaternion q = Quaternion::CreateFromAxisAngle(up, _yaw) * Quaternion::CreateFromAxisAngle(right, _pitch);
		forward = Vector3::Transform(forward, Matrix::CreateFromQuaternion(q)); 
		right = up.Cross(forward); 
	} 

	Matrix Camera::GetView()
	{
		return DirectX::XMMatrixLookAtLH(pos, pos + forward, up);
	}

	Matrix Camera::GetProj()
	{
		return DirectX::XMMatrixPerspectiveFovLH(fov * DirectX::XM_PI / 180.0f, aspect, zNear, zFar); 
	} 

	Vector3 Camera::GetPos()
	{
		return pos; 
	}
}
