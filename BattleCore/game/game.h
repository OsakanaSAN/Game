/*!
 * @brief	ゲームクラス。
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
 * @brief	ゲームクラス。
 */



class Game{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	Game();
	/*!
	 * @brief	デストラクタ。
	 */
	~Game();
	/*!
	 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
	 */
	void Start();
	/*!
	 * @brief	更新。
	 */
	void Update();
	/*!
	 * @brief	3Dモデルの描画。
	 */
	void Render();

	//リストの更新処理
	void ListUpdate();

	//			2Dの描画
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
	//プレイヤーの弾のリスト
	const std::list<Bullet*>& GetPlayerBullet()
	{
		return PlayerBullets;
	}
	//敵の弾のリスト
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

	//エネミーリスト
	const std::list<CEnemy*>& GetEnemys()
	{
		return Enemys;
	}
	const void Sound_Dete(int No)
	{
		
		Sounds.erase(Sounds.begin() + No);
	}
	/*!
	* @brief	プレイヤーが放った弾を追加。
	*/
	void AddPlayerBullets(Bullet* bullet)
	{
		PlayerBullets.push_back(bullet);
	}
	//敵の放った弾を追加
	void AddEnemyBullets(Bullet* bullet)
	{
		EnemyBullets.push_back(bullet);
	}
	//エネミーをエネミーのリストに追加
	void AddEnemy(CEnemy* enemy)
	{
		Enemys.push_back(enemy);
	}
	//音楽リストに追加する
	void AddSound(CSoundSource* Sound)
	{
		Sounds.push_back(Sound);
	}

	//現在のマップを取得
	Map* GetMap()
	{
		return map;
	}
	//マップのインスタンス作成
	void NewMap()
	{
		map = new Map;
	}

	//現在のプレイヤーを取得
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
	//Scene関係

	//ゲームシーンの取得
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




	//ゲームシーンの取得
	CGameScene* GetGameScene()
	{
		return m_GameScene;
	}
	void NewGameScene()
	{
		m_GameScene = new CGameScene;
	}

	//リザルトシーンの取得
	CResult* GetResult()
	{
		return m_Result;
	}
	//リザルトの生成
	void NewResult()
	{
		m_Result = new CResult;
	}
	//リザルトシーンの取得
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


	//敵を生成するための関数
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
		Title_Scene,	//タイトル画面
		Select_Scene,	//セレクト画面
		Game_Scene,		//ゲーム画面
		Result_Scene	//リザルト画面に移行
	};

	
	D3DXVECTOR3		toEyePos;		//注視点から視点までのベクトル。
	D3DXVECTOR3		oldPos;
	GameCamera*		Gamecamera;		//カメラ
	CPlayer*		m_player;					//プレイヤー

	Pad				pad;						//パッド
	Map*			map;						//マップ
	
	CHud			Hud;						//HUD
	TITLE*			m_title;					//タイトル画面
	CSelectScene*	m_SelectScene;
	CGameScene*     m_GameScene;				//ゲームシーン
	CResult*        m_Result;					//リザルト画面
	CScoreCheckre*	m_ScoreCheck;				//スコアチェック
	CFade			g_fade;
	CSoundEngine*	m_SoundEngine;
	

	CPrimitive prim;				//プリミティブ
	Sprite*    sprite;				//スプライト

	LPD3DXSPRITE spt;
	D3DXVECTOR3  rag;

	std::list<Bullet*>  PlayerBullets;			//プレイヤーの弾のリスト
	std::list<Bullet*>  EnemyBullets;			//敵の弾のリスト
	std::list<CEnemy*>  Enemys;					//敵のリスト
	std::vector<CSoundSource*>	Sounds;			//サウンドリスト
	GameObjectManager  GameManager;				//ゲームマネージャー
	SceneState         Scene = Title_Scene;		//シーンの管理変数

};

extern Game* game;