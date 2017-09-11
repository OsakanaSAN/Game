/*!
 * @brief	�Q�[��
 */
#include "stdafx.h"
#include "game.h"

Enemy enemy3;

/*!
 * @brief	�R���X�g���N�^�B
 */
Game::Game()
{


}
/*!
 * @brief	�f�X�g���N�^�B
 */
Game::~Game()
{
}
/*!
 * @brief	�Q�[�����N�����Ă����x�����Ă΂��֐��B
 */
void Game::Start()
{
	D3DXCreateSprite(g_pd3dDevice, &spt);
	g_physicsWorld = new PhysicsWorld;
	g_physicsWorld->Init();
	camera.Init();
	camera.SetEyePt(D3DXVECTOR3(0.0f, 2.5f, 8.0f));
	camera.SetLookatPt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera.Update();
	map.Init();

	//�v���C���[�̃C���X�^���X�𐶐�
	player.Start();

	enemy.Start();
	this->sprite = new Sprite;
	this->sprite->Loadtex("Assets/sprite/AC2.png");
	this->sprite->Initialize();
	/*enemy2.Start();
	enemy3.Start();*/
	
	D3DXVECTOR3 V = player.GetPos() - camera.GetEyePt();
	length = D3DXVec3Length(&V);

}
/*!
 * @brief	�X�V�B
 */
void Game::Update()
{
	player.Update(); //�v���C���[�̍X�V
	enemy.Update();  //�G�̍X�V
	/*enemy2.Update();
	enemy3.Update();*/

	auto bulletIt = PlayerBullets.begin();
	while (bulletIt != PlayerBullets.end()) {
		if (!(*bulletIt)->Update()) {
			//���S
			bulletIt = PlayerBullets.erase(bulletIt);
		}
		else {
			bulletIt++;
		}


	}
	auto EbulletIt = EnemyBullets.begin();
	while (EbulletIt != EnemyBullets.end()) {
		if (!(*EbulletIt)->Update()) {
			//���S
			EbulletIt = EnemyBullets.erase(EbulletIt);
		}
		else {
			EbulletIt++;
		}


	}


	CameraAngle();
	camera.Update(); //�J�����̍X�V
	map.Update();
	this->sprite->Update();
	
	
}

void Game::CameraAngle()
{
	float rStick_x = pad.GetRStickXF();
	float rStick_y = pad.GetRStickYF();

	toEyePos = camera.GetEyePt() - camera.GetLookatPt();

	//Y������̉�]�s����쐬�B
	D3DXMATRIX mRot;
	//�P�ʍs����쐬�B
	D3DXMatrixIdentity(&mRot);
	if (fabs(rStick_x) > 0.0f) 
	{
		D3DXMatrixRotationY(&mRot, 0.05f*rStick_x);
	}
	//toEyePos���񂷁B
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);

	//�����āA�㉺�ɉ񂷁B
	//�P�ʍs��ɂ��Ƃ��B
	D3DXMatrixIdentity(&mRot);
	//��]�����鎲�����߂�B
	D3DXVECTOR3 rotAxis;
	D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
	D3DXVec3Cross(&rotAxis, &up, &toEyePos);
	//��]���𐳋K������B
	D3DXVec3Normalize(&rotAxis, &rotAxis);

	if (fabs(rStick_y) > 0) {
		D3DXMatrixRotationAxis(&mRot, &rotAxis, 0.05f*rStick_y);
	}
	//toEyePos���񂷁B
	D3DXVECTOR3 toEyePosOld = toEyePos;
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);
	//�㉺�̉�]�͐�����݂���B
	D3DXVECTOR3 toEyePosNormalized;
	D3DXVec3Normalize(&toEyePosNormalized, &toEyePos);
	if (fabsf(toEyePosNormalized.x) < 0.5f && fabsf(toEyePosNormalized.z) < 0.5f) {
		//����𒴂��Ă���B
		toEyePos = toEyePosOld;
	}

	//�J�������L�����ɒǏ]������
	float Delta_time = 1.0f / 60.0f;

	Time += Delta_time;
	D3DXVECTOR3 tlen = player.GetPos() - camera.GetEyePt();
	float length2 = D3DXVec3Length(&tlen);
	float LENGTH = length2 -length;

	if (LENGTH > 1.5f || LENGTH < -1.5f)
	{
		D3DXVECTOR3 V = player.GetPos();

		camera.SetLookatPt(V);

	}
		V.x = V.x + toEyePos.x;
		V.y = V.y + toEyePos.y;
		V.z = V.z + toEyePos.z;

		camera.SetEyePt(V);
	
	
	pad.Update();
	
}
/*!
* @brief	�`��B
*/
void Game::Render()
{
	
	player.Render();
	enemy.Render();
	
	/*enemy2.Render();
	enemy3.Render();*/
	map.Render();
	
	for (auto bullet : PlayerBullets) {
		bullet->Render();
	}
	for (auto bullet : EnemyBullets) {
		bullet->Render();
	}

	this->sprite->Draw(this->spt);
}
