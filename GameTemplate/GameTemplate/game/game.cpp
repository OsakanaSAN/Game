/*!
 * @brief	ゲーム
 */
#include "stdafx.h"
#include "game.h"


/*!
 * @brief	コンストラクタ。
 */
Game::Game()
{


}
/*!
 * @brief	デストラクタ。
 */
Game::~Game()
{
}
/*!
 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
 */
void Game::Start()
{
	g_physicsWorld = new PhysicsWorld;
	g_physicsWorld->Init();
	camera.Init();
	camera.SetEyePt(D3DXVECTOR3(0.0f, 2.5f, 8.0f));
	camera.SetLookatPt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera.Update();
	map.Init();

	//プレイヤーのインスタンスを生成
	player.Start();

	enemy.Start();

}
/*!
 * @brief	更新。
 */
void Game::Update()
{
	player.Update(); //プレイヤーの更新
	enemy.Update();  //敵の更新
	auto bulletIt = PlayerBullets.begin();
	while (bulletIt != PlayerBullets.end()) {
		if (!(*bulletIt)->Update()) {
			//死亡
			bulletIt = PlayerBullets.erase(bulletIt);
		}
		else {
			bulletIt++;
		}


	}
	auto EbulletIt = EnemyBullets.begin();
	while (EbulletIt != EnemyBullets.end()) {
		if (!(*EbulletIt)->Update()) {
			//死亡
			EbulletIt = EnemyBullets.erase(EbulletIt);
		}
		else {
			EbulletIt++;
		}


	}


	CameraAngle();
	camera.Update(); //カメラの更新
	map.Update();
}

void Game::CameraAngle()
{
	float rStick_x = pad.GetRStickXF();
	float rStick_y = pad.GetRStickYF();

	toEyePos = camera.GetEyePt() - camera.GetLookatPt();

	//Y軸周りの回転行列を作成。
	D3DXMATRIX mRot;
	//単位行列を作成。
	D3DXMatrixIdentity(&mRot);
	if (fabs(rStick_x) > 0.0f) 
	{
		D3DXMatrixRotationY(&mRot, 0.05f*rStick_x);
	}
	//toEyePosを回す。
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);

	//続いて、上下に回す。
	//単位行列にしとく。
	D3DXMatrixIdentity(&mRot);
	//回転させる軸を求める。
	D3DXVECTOR3 rotAxis;
	D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
	D3DXVec3Cross(&rotAxis, &up, &toEyePos);
	//回転軸を正規化する。
	D3DXVec3Normalize(&rotAxis, &rotAxis);

	if (fabs(rStick_y) > 0) {
		D3DXMatrixRotationAxis(&mRot, &rotAxis, 0.05f*rStick_y);
	}
	//toEyePosを回す。
	D3DXVECTOR3 toEyePosOld = toEyePos;
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);
	//上下の回転は制限を設ける。
	D3DXVECTOR3 toEyePosNormalized;
	D3DXVec3Normalize(&toEyePosNormalized, &toEyePos);
	if (fabsf(toEyePosNormalized.x) < 0.5f && fabsf(toEyePosNormalized.z) < 0.5f) {
		//可動域を超えている。
		toEyePos = toEyePosOld;
	}

	//カメラをキャラに追従させる
	D3DXVECTOR3 V = player.GetPos();
	
	camera.SetLookatPt(V);
	V.x = V.x + toEyePos.x;
	V.y = V.y + toEyePos.y;
	V.z = V.z + toEyePos.z;
	camera.SetEyePt(V);

	pad.Update();
}
/*!
* @brief	描画。
*/
void Game::Render()
{
	player.Render();
	enemy.Render();
	map.Render();
	for (auto bullet : PlayerBullets) {
		bullet->Render();
	}
	for (auto bullet : EnemyBullets) {
		bullet->Render();
	}

}
