/*!
 * @brief	�Q�[���N���X�B
 */

#pragma once

#include "myEngine/Physics/Physics.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "myEngine/HID/Pad.h"
#include "Map/Map.h"
#include "Item/Bullet.h"
#include <list>
#include "myEngine/Graphics/Sprite/Primitive.h"
#include "GameCamera/GameCamera.h"
#include "myEngine/Graphics/Sprite/Sprite.h"
#include "HUD/Hud.h"
#include "myEngine/Graphics/Sprite/C2DImage.h"
#include "Scene/TITLE.h"
#include "Fade/Fade.h"
#include "myEngine/GameManager/GameObjectManager.h"
#include "myEngine/Graphics/PostEffect/Bloom.h"
#include "myEngine/Sound/CSoundEngine.h"
#include "Scene/GameScene.h"
#include "Scene/Result.h"
#include "HUD/ScoreCheckre.h"
#include "Scene/SelectScene.h"

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
	 * @brief	3D���f���̕`��B
	 */
	void Render();

	//���X�g�̍X�V����
	void ListUpdate();

	//			2D�̕`��
	void Render2D();

	void NewGameCamera()
	{
		Gamecamera = new GameCamera;
	 }
	GameCamera* GetGameCamara()
	{
		return Gamecamera;
	}

	Pad* GetPad()
	{
		return &pad;
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
	void DeleteBullet()
	{
		for (Bullet* Pbullet : PlayerBullets)
		{
			delete Pbullet;
		}
		PlayerBullets.clear();

		for (Bullet* Ebullet : EnemyBullets)
		{
			delete Ebullet;
		}
		EnemyBullets.clear();
	}

	//�G�l�~�[���X�g
	const std::list<CEnemy*>& GetEnemys()
	{
		return Enemys;
	}
	const void Sound_Dete(int No)
	{
		
		Sounds.erase(Sounds.begin() + No);
	}
	/*!
	* @brief	�v���C���[���������e��ǉ��B
	*/
	void AddPlayerBullets(Bullet* bullet)
	{
		PlayerBullets.push_back(bullet);
	}
	//�G�̕������e��ǉ�
	void AddEnemyBullets(Bullet* bullet)
	{
		EnemyBullets.push_back(bullet);
	}
	//�G�l�~�[���G�l�~�[�̃��X�g�ɒǉ�
	void AddEnemy(CEnemy* enemy)
	{
		Enemys.push_back(enemy);
	}
	//���y���X�g�ɒǉ�����
	void AddSound(CSoundSource* Sound)
	{
		Sounds.push_back(Sound);
	}

	//���݂̃}�b�v���擾
	Map* GetMap()
	{
		return map;
	}
	//�}�b�v�̃C���X�^���X�쐬
	void NewMap()
	{
		map = new Map;
	}

	//���݂̃v���C���[���擾
	void NewPlayer()
	{
		m_player = new CPlayer;
	}
	void DeletePlayer()
	{
		delete m_player;
		m_player = NULL;
	}

	CPlayer* GetPlayer()
	{
		return m_player;
	}
	CFade* GetFade()
	{
		return &g_fade;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Scene�֌W

	//�Q�[���V�[���̎擾
	TITLE* GetTitle()
	{
		return m_title;
	}

	void NewTitle()
	{
		m_title = new TITLE;
	}

	CSelectScene* GetSelect()
	{

		return m_SelectScene;

	}

	void NewSelect()
	{
		m_SelectScene = new CSelectScene;
	}




	//�Q�[���V�[���̎擾
	CGameScene* GetGameScene()
	{
		return m_GameScene;
	}
	void NewGameScene()
	{
		m_GameScene = new CGameScene;
	}

	//���U���g�V�[���̎擾
	CResult* GetResult()
	{
		return m_Result;
	}
	//���U���g�̐���
	void NewResult()
	{
		m_Result = new CResult;
	}
	//���U���g�V�[���̎擾
	CScoreCheckre* GetScorecheckre()
	{
		return m_ScoreCheck;
	}
	

	void SetTitleScene()
	{
		Scene = Title_Scene;
	}
	void SetSelectScene()
	{
		Scene = Select_Scene;
	}
	void SetGameScene()
	{
		Scene = Game_Scene;
	}
	void SetResult()
	{
		Scene = Result_Scene;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void NewScoreCheckre()
	{
		m_ScoreCheck = new CScoreCheckre;
	}


	//�G�𐶐����邽�߂̊֐�
	void InitEnemy(D3DXVECTOR3 Setpos) 
	{
		CEnemy* enemy = new CEnemy;
		Enemys.push_back(enemy);
		enemy->Start();
		enemy->SetPos(Setpos);
	}

	void DeleteEnemy()
	{
		for (CEnemy* enemy : Enemys)
		{
			delete enemy;
		}
		Enemys.clear();
	}

	CSoundEngine* GetSoundEngine()
	{
		return m_SoundEngine;
	}
	CHud*  GetHud()
	{
		return &Hud;
	}

private:
	enum SceneState {
		Title_Scene,	//�^�C�g�����
		Select_Scene,	//�Z���N�g���
		Game_Scene,		//�Q�[�����
		Result_Scene	//���U���g��ʂɈڍs
	};

	
	D3DXVECTOR3		toEyePos;		//�����_���王�_�܂ł̃x�N�g���B
	D3DXVECTOR3		oldPos;
	GameCamera*		Gamecamera;		//�J����
	CPlayer*		m_player;					//�v���C���[

	Pad				pad;						//�p�b�h
	Map*			map;						//�}�b�v
	
	CHud			Hud;						//HUD
	TITLE*			m_title;					//�^�C�g�����
	CSelectScene*	m_SelectScene;
	CGameScene*     m_GameScene;				//�Q�[���V�[��
	CResult*        m_Result;					//���U���g���
	CScoreCheckre*	m_ScoreCheck;				//�X�R�A�`�F�b�N
	CFade			g_fade;
	CSoundEngine*	m_SoundEngine;
	

	CPrimitive prim;				//�v���~�e�B�u
	Sprite*    sprite;				//�X�v���C�g

	LPD3DXSPRITE spt;
	D3DXVECTOR3  rag;

	std::list<Bullet*>  PlayerBullets;			//�v���C���[�̒e�̃��X�g
	std::list<Bullet*>  EnemyBullets;			//�G�̒e�̃��X�g
	std::list<CEnemy*>  Enemys;					//�G�̃��X�g
	std::vector<CSoundSource*>	Sounds;			//�T�E���h���X�g
	GameObjectManager  GameManager;				//�Q�[���}�l�[�W���[
	SceneState         Scene = Title_Scene;		//�V�[���̊Ǘ��ϐ�

};

extern Game* game;