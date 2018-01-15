#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine/Sound/CSoundSource.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
	bool Update();
	void Render();
	const D3DXVECTOR3& Getops() 
	{
		return position;
	}
	const D3DXMATRIX& getMatrix() const
	{
		return Mat;
	}
	const D3DVECTOR& GetSpeed()
	{
		return moveSpeed;
	}
	void SetSpeed(D3DXVECTOR3& speed)
	{
		moveSpeed = speed;
	}
	//íeä€Ç™ìGÇ…îÌíeÇµÇƒÇ¢ÇÈÇ©ÇÃîªíË
	void  SetIsHit(bool hit)
	{
		IsHit = hit;
	}
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX& lightProjMatrix);
	void LockOn(D3DXVECTOR3 lock);
private:
	Pad      BPad;
	SkinModel model;
	D3DXMATRIX  Mat;
	static SkinModelData* modelData;
	Animation animation;
	Light light;
	D3DXVECTOR3			position;
	D3DXVECTOR3			moveSpeed;
	D3DXVECTOR3			PlayerFrontPosition;
	D3DXQUATERNION		rot;
	CSoundSource*		m_BulletSe;

	bool IsLifeDown = false;
	int life;		//éıñΩÅB
	bool IsHit = false;
};

