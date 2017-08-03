#ifndef _CAMERA_H_
#define _CAMERA_H_


// Includes
#include <d3d11.h>
#include <DirectXMath.h>



class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();
	void SetPosition(DirectX::XMVECTOR);
	void SetCameraYaw(float);
	void SetCameraPitch(float);
	void SetMoveLeftRight(float);
	void SetMoveBackForward (float);

	DirectX::XMVECTOR GetPosition();
	DirectX::XMVECTOR GetLookAt();

	void Render();
	DirectX::XMMATRIX GetViewMatrix();
private:

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_CameraForward;
	DirectX::XMFLOAT3 m_CameraRight;
	DirectX::XMFLOAT3 m_CameraUp;
	DirectX::XMFLOAT3 m_CameraTarget;

	float m_MoveLeftRight;
	float m_MoveBackForward;
	float m_CameraYaw ;
	float m_CameraPitch ;

	DirectX::XMFLOAT4X4 m_viewMatrix, m_baseViewMatrix;
};

#endif