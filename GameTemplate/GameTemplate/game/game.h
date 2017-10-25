/*!
 * @brief	�Q�[���N���X�B
 */

#pragma once

#include "myEngine/Physics/Physics.h"
#include "Player.h"
#include "Enemy.h"
#include "myEngine/HID/Pad.h"
#include "Map.h"
#include "Bullet.h"
#include <list>
#include "Primitive.h"
#include "GameCamera.h"
#include "Sprite.h"
#include "Bootht.h"
#include "C2DImage.h"
#include "TITLE.h"
#include "Fade.h"

 /*!
 * @brief	�Q�[���N���X�B
 */
class Game{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	Game();
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~Game();
	/*!
	 * @brief	�Q�[�����N�����Ă����x�����Ă΂��֐��B
	 */
	void Start();
	/*!
	 * @brief	�X�V�B
	 */
	void Update();
	/*!
	 * @brief	�`��B
	 */
	void Render();

	Camera* GetCamera()
	{
		return Gamecamera.Getcamera();
	}
	Pad* GetPad()
	{
		return &pad;
	}
	Enemy* GetEnemy()
	{
		return &enemy;
	}
	//�v���C���[�̒e�̃��X�g
	const std::list<Bullet*>& GetPlayerBullet()
	{
		return PlayerBullets;
	}
	//�G�̒e�̃��X�g
	const std::list<Bullet*>& GetEnemyBullet()
	{
		return EnemyBullets;
	}
	//�G�l�~�[���X�g
	const std::list<Enemy*>& GetEnemys()
	{
		return Enemys;
	}
	/*!
	* @brief	�v���C���[���������e��ǉ��B
	*/
	void AddPlayerBullets(Bullet* bullet)
	{
		PlayerBullets.push_back(bullet);
	}

	void AddEnemyBullets(Bullet* bullet)
	{
		EnemyBullets.push_back(bullet);
	}

	void AddEnemy(Enemy* enemy)
	{
		Enemys.push_back(enemy);
	}

	Player* GetPlayer()
	{
		return &player;
	}
	Fade* GetFade()
	{
		return &g_fade;
	}
	void SetGameScene()
	{
		Scene = Game_Scene;
	}
private:
	enum SceneState {
		Title_Scene,	//�^�C�g�����
		Game_Scene,		//�Q�[�����
	};


	D3DXVECTOR3		toEyePos;		//�����_���王�_�܂ł̃x�N�g���B
	D3DXVECTOR3    oldPos;
	GameCamera Gamecamera;        //�J����
	Player player;                //�v���C���[
	Enemy  enemy;                 //�G
	Pad    pad;                   //�p�b�h
	Map    map;                   //�}�b�v
	Bootht Boot;
	TITLE  title;				  //�^�C�g�����
	Fade   g_fade;
	

	CPrimitive prim;              //�v���~�e�B�u
	Sprite*    sprite;            //�X�v���C�g

	LPD3DXSPRITE spt;
	D3DXVECTOR3  rag;

	std::list<Bullet*> PlayerBullets;
	std::list<Bullet*> EnemyBullets;
	std::list<Enemy*>  Enemys;


	SceneState         Scene = Title_Scene;    //�V�[���̊Ǘ��ϐ�

};

extern Game* game;