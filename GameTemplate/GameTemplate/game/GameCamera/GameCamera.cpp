#include "stdafx.h"
#include "GameCamera.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}


void GameCamera::Strat()
{

	Springcamera.Init(Eyepos, camera.GetLookatPt(),80.0f); //�o�l�J�����̏�����
	Springcamera.SetPosition(Eyepos);
	Springcamera.SetTarget (D3DXVECTOR3(0.0f, 7.0f, 0.0f));

	CameraCol.Init(0.2f); //���̃J�����̏�����
	
}

void GameCamera::Update()
{
	
	CameraAngle();
	pad.Update();
	
}
void GameCamera::CameraAngle()
{
	float rStick_x = pad.GetRStickXF();
	float rStick_y = pad.GetRStickYF();
	float lStick_x = pad.GetLStickXF();

/////////////////////////////////////////////////////////////////////////////////////////////////
	//�N�C�b�N�^�[���i�u���Ɍ�������)
	if (pad.IsTrigger(Pad::enButtonLB3))
	{
		QuickTurn = true;
	}
	if (QuickTurn)
	{
		D3DXMATRIX mRot;
		D3DXMatrixRotationY(&mRot, 0.314f * 1);
		Quick += 0.1f;
		D3DXVECTOR3 mEyepos = Eyepos;
		Eyepos.x = mEyepos.x * mRot.m[0][0] + mEyepos.y * mRot.m[1][0] + mEyepos.z * mRot.m[2][0] + mRot.m[3][0];
		Eyepos.y = mEyepos.x * mRot.m[0][1] + mEyepos.y * mRot.m[1][1] + mEyepos.z * mRot.m[2][1] + mRot.m[3][1];
		Eyepos.z = mEyepos.x * mRot.m[0][2] + mEyepos.y * mRot.m[1][2] + mEyepos.z * mRot.m[2][2] + mRot.m[3][2];
		if (Quick > 1.0)
		{
			QuickTurn = false;
			Quick = 0;
		}

	}

////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (fabs(rStick_x) > 0.0f)
	{
		D3DXMATRIX mRot;
		D3DXMatrixRotationY(&mRot, 0.05f*rStick_x);
		
		D3DXVECTOR3 mEyepos = Eyepos;
		Eyepos.x = mEyepos.x * mRot.m[0][0] + mEyepos.y * mRot.m[1][0] + mEyepos.z * mRot.m[2][0] + mRot.m[3][0];
		Eyepos.y = mEyepos.x * mRot.m[0][1] + mEyepos.y * mRot.m[1][1] + mEyepos.z * mRot.m[2][1] + mRot.m[3][1];
		Eyepos.z = mEyepos.x * mRot.m[0][2] + mEyepos.y * mRot.m[1][2] + mEyepos.z * mRot.m[2][2] + mRot.m[3][2];

	}

	if (fabs(rStick_y) > 0.0f)
	{ 
		D3DXVECTOR3 rotAxis;
		D3DXVec3Cross(&rotAxis, &Up, &Eyepos);
		D3DXVec3Normalize(&rotAxis, &rotAxis);
		D3DXMATRIX mRot;
		D3DXMatrixRotationAxis(&mRot, &rotAxis, 0.05f*rStick_y);
		D3DXVECTOR3 toPositionOld = Eyepos;
		D3DXVECTOR3 mEyepos = Eyepos;
		Eyepos.x = mEyepos.x * mRot.m[0][0] + mEyepos.y * mRot.m[1][0] + mEyepos.z * mRot.m[2][0] + mRot.m[3][0];
		Eyepos.y = mEyepos.x * mRot.m[0][1] + mEyepos.y * mRot.m[1][1] + mEyepos.z * mRot.m[2][1] + mRot.m[3][1];
		Eyepos.z = mEyepos.x * mRot.m[0][2] + mEyepos.y * mRot.m[1][2] + mEyepos.z * mRot.m[2][2] + mRot.m[3][2];

		D3DXVECTOR3 toPosDir = Eyepos;
		D3DXVec3Normalize(&toPosDir, &toPosDir);
		if (toPosDir.y < -0.5f) {
			//�J����������������B
			Eyepos = toPositionOld;
		}
		else if (toPosDir.y > 0.8f) {
			//�J�����������������B
			Eyepos = toPositionOld;
		}


	}
	
	if (LookOn)
	{
		Vpos = game->GetPlayer()->GetPos();
		Vpos.y += 2.0f;
		Springcamera.SetTarTarget(Vpos);
		D3DXVECTOR3 PlayerBack = { game->GetPlayer()->GetWorldMatrix().m[2][0] ,
									game->GetPlayer()->GetWorldMatrix().m[2][1],
									game->GetPlayer()->GetWorldMatrix().m[2][2]};
		D3DXVec3Add(&Vpos, &Vpos, &(-PlayerBack * 7));
		Springcamera.SetTarPosition(Vpos);

	}
	
	else if (game->GetPlayer() != NULL )
	{
		Vpos = game->GetPlayer()->GetPos();
		Vpos.y += 4.0f;
		Springcamera.SetTarTarget(Vpos);
		D3DXVec3Add(&Vpos, &Vpos, &Eyepos);
		Springcamera.SetTarPosition(Vpos);
	}

	

	//�J�����̂����蔻��
	D3DXVECTOR3 newPos;
	if (CameraCol.Execute(newPos, Springcamera.GetTarPosition(),Springcamera.GetTarTarget()))
	{
		Springcamera.SetTarPosition(newPos);


	}

	Springcamera.Update();
	camera.Update();
	

}


void GameCamera::Render()
{

}