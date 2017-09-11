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
#include "Sprite.h"
#include "C2DImage.h"
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
	void CameraAngle();

	Camera* GetCamera()
	{
		return &camera;
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
private:
	D3DXVECTOR3		toEyePos;		//�����_���王�_�܂ł̃x�N�g���B
	D3DXVECTOR3    oldPos;

	Camera camera;
	Player player;
	Enemy  enemy;
	Enemy  enemy2;
	Pad    pad;
	Map    map;
	CPrimitive prim;
	Sprite*    sprite;
	LPD3DXSPRITE spt;
	D3DXVECTOR3  rag;
	float       Time = 0;

	std::list<Bullet*> PlayerBullets;
	std::list<Bullet*> EnemyBullets;
	std::list<Enemy*>  Enemys;
	float             length;


};

extern Game* game;