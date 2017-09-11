#include "stdafx.h"
#include "Camera.h"



float dampingK = 35.0f;
//コンストラクタ。
Camera::Camera()
{
	Near = 0.1f;
	Far = 100.0f;
	aspect = (float)FRAME_BUFFER_WIDTH / (float)FRAME_BUFFER_HEIGHT;
	D3DXMatrixIdentity(&viewMatrixInv);
}
//デストラクタ
Camera::~Camera()
{
}
//アスペクト比を設定。
void Camera::SetAspect(float _aspect)
{
	aspect = _aspect;
}
//アスペクト比を取得。
float Camera::GetAspect()
{
	return aspect;
}
//ファーを設定。
void Camera::SetFar(float _far)
{
	Far = _far;
}
//ニアーの取得。
float Camera::GetNear()
{
	return Near;
}
//ファーの取得。
float Camera::GetFar()
{
	return Far;
}
//ニアーを設定。
void Camera::SetNear(float _near)
{
	Near = _near;
}
//視点のセッター
void Camera::SetEyePt(D3DXVECTOR3 pt)
{
	vEyePt = pt;
}
//視点のゲッター
D3DXVECTOR3 Camera::GetEyePt()
{
	return vEyePt;
}
//注視点のセッター
void Camera::SetLookatPt(D3DXVECTOR3 pt)
{
	vLookatPt = pt;
}
//注視点のゲッター。
D3DXVECTOR3 Camera::GetLookatPt()
{
	return vLookatPt;
}
//上方向のセッター
void Camera::SetUpVec(D3DXVECTOR3 up)
{
	vUpVec = up;
}
//上方向のゲッター
D3DXVECTOR3 Camera::GetUpVec()
{
	return vUpVec;
}
//ビュー行列のセッター。
void Camera::SetViewMatrix(D3DXMATRIX mView)
{
	viewMatrix = mView;
}
//ビュー行列のゲッター
D3DXMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}
//プロジェクション行列のセッター
void Camera::SetProjectionMatrix(D3DXMATRIX mProj)
{
	projectionMatrix = mProj;
}
//プロジェクション行列のゲッター
D3DXMATRIX Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
//カメラの更新処理。
void Camera::Update()
{
	D3DXMatrixLookAtLH(&viewMatrix, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4, aspect, Near, Far);
	D3DXMatrixInverse(&viewMatrixInv, NULL, &viewMatrix);
	CameraRot = viewMatrixInv;
	CameraRot.m[3][0] = 0.0f;
	CameraRot.m[3][1] = 0.0f;
	CameraRot.m[3][2] = 0.0f;
	CameraRot.m[3][3] = 1.0f;


}
//カメラの初期化。
void Camera::Init()
{
	vEyePt = D3DXVECTOR3(0.0f, 0.5f, 2.0f);
	vLookatPt = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	Update();
}

D3DXVECTOR3 Camera::CalcSpring(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 Speed,
	                    float Maxspeed, float down)
{
	float deltaTime = min(1.0f / 300.0f, 1.0f / 60.0f);
	D3DXVECTOR3 distans;
	D3DXVec3Subtract(&distans, &target,&pos);
	D3DXVECTOR3 originarDir = distans;
	D3DXVec3Normalize(&originarDir, &originarDir);
	D3DXVECTOR3 springAccle;
	springAccle = distans;

	float t = dampingK / (2.0f * down);
	float springK = t * t;
	D3DXVec3Scale(&springAccle, &springAccle, springK);
	D3DXVECTOR3 vt = Speed;
	D3DXVec3Scale(&vt, &vt, dampingK);
	D3DXVec3Subtract(&springAccle, &springAccle, &vt);

	D3DXVec3Scale(&springAccle, &springAccle, deltaTime);
	D3DXVec3Add(&Speed, &Speed, &springAccle);
	if (D3DXVec3LengthSq(&Speed) > Maxspeed*Maxspeed)
	{
		D3DXVec3Normalize(&Speed, &Speed);
		D3DXVec3Scale(&Speed, &Speed, Maxspeed);

	}
	D3DXVECTOR3 newpos = pos;
	D3DXVECTOR3 addpos = Speed;
	D3DXVec3Scale(&addpos, &addpos, deltaTime);
	D3DXVec3Add(&newpos,&newpos,&addpos);
	D3DXVec3Subtract(&vt, &target, &newpos);
	D3DXVec3Normalize(&vt, &vt);
	if (D3DXVec3Dot(&vt, &originarDir) < 0.0f)
	{
		newpos = target;
		Speed = { 0,0,0 };
	}
	return newpos;

}