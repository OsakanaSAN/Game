#include "stdafx.h"
#include "SpringCamera.h"
#include "myEngine/Time/GameTime.h"


float dampingK = 35.0f;
float CalcSpringScalar(
	float positionNow,
	float positionTarget,
	float& moveSpeed)
	
{
	float deltaTime = min(1.0f / 30.0f, GameTime().GetFrameDeltaTime());

	float dampingRate = 0.2f;
	float distance;
	distance = positionTarget - positionNow;
	if (fabsf(distance) < FLT_EPSILON) {
		moveSpeed = 0.0f;
		return positionTarget;
	}
	float originalDir = distance;
	originalDir /= fabsf(distance);
	float springAccel;
	springAccel = distance;

	float t = dampingK / (2.0f * dampingRate);
	float springK = t * t;
	springAccel *= springK;
	//加速度を決定。
	float vt = moveSpeed;
	vt *= dampingK;
	springAccel -= vt;
	springAccel *= deltaTime;
	moveSpeed += springAccel;

	float newPos = positionNow;
	float addPos = moveSpeed;
	addPos *= deltaTime;
	newPos += addPos;
	vt = positionTarget - newPos;
	if (fabsf(vt) < FLT_EPSILON) {
		//目標座標まで移動完了した。
		newPos = positionTarget;
		moveSpeed = 0.0f;
	}
	else {
		vt /= fabsf(vt);
		if (vt * originalDir < 0.0f) {
			//目標座標を超えた。
			newPos = positionTarget;
			moveSpeed = 0.0f;
		}
	}
	return newPos;
}
/*!
* @brief	バネ減衰を使用して、現在の位置、目標となる位置、速度、加速度から新しい位置を計算する。
*/

D3DXVECTOR3 CalcSpringVector(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 Speed,
	float Maxspeed, float down)
{
	float deltaTime = 1.0f / 30.0f;//min(1.0f / 30.0f, 1.0f / 60.0f);
	D3DXVECTOR3 distans;
	D3DXVec3Subtract(&distans, &target, &pos);
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
	D3DXVec3Add(&newpos, &newpos, &addpos);
	D3DXVec3Subtract(&vt, &target, &newpos);
	D3DXVec3Normalize(&vt, &vt);
	if (D3DXVec3Dot(&vt, &originarDir) < 0.0f)
	{
		newpos = target;
		Speed = { 0,0,0 };
	}
	return newpos;

}

/*!
* @brief	コンストラクタ。
*/
SpringCamera::SpringCamera()
{
}
/*!
* @brief	デストラクタ
*/
SpringCamera::~SpringCamera()
{
}
/*!
* @brief	初期化。
*/
void SpringCamera::Init(const D3DXVECTOR3& target, const D3DXVECTOR3& position, float maxMoveSpeed)
{
	m_camera.SetLookatPt(target);
	m_camera.SetEyePt(position);
	m_target = target;
	m_position = position;
	m_targetMoveSpeed = { 0.0f,0.0f,0.0f };
	m_positionMoveSpeed = { 0.0f,0.0f,0.0f };
	m_maxMoveSpeed = maxMoveSpeed;
}
void SpringCamera::UpdateSpringCamera()
{
	m_dampingRate = CalcSpringScalar(m_targetDampingRate, m_dampingRate, m_dampingRateVel);
	D3DXVECTOR3 target = CalcSpringVector(m_camera.GetLookatPt(), m_target, m_targetMoveSpeed, m_maxMoveSpeed, m_dampingRate);
	D3DXVECTOR3 position = CalcSpringVector(m_camera.GetEyePt(), m_position, m_positionMoveSpeed, m_maxMoveSpeed, m_dampingRate);
	m_camera.SetLookatPt(target);
	m_camera.SetEyePt(position);
}
/*!
* @brief	更新。
*/
void SpringCamera::Update()
{
	UpdateSpringCamera();
	UpdateCamera();
}