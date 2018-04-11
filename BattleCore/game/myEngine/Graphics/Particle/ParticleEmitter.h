#pragma once

class CParticle;
/*!
* @brief	パーティクル生成パラメータ
*/
struct SParicleEmitParameter {
	//初期化。
	void Init()
	{
		memset(this, 0, sizeof(SParicleEmitParameter));
		brightness = 1.0f;
		life = 1.0f;
		initPositionRandomMargin = { 0.0f,0.0f,0.0f };
	}
	const char* texturePath;			//!<テクスチャのファイルパス。
	float	w;							//!<パーティクルの幅。
	float	h;							//!<パーティクルの高さ。
	float	intervalTime;				//!<パーティクルの発生間隔。
	float	Endtimer;						//パーティクルを生成するまでの時間
	float	brightness;					//輝度
	float	life;						//一つのパーティクルの寿命
	D3DXVECTOR3	initPositionRandomMargin;
	D3DXVECTOR3 initSpeed;				//!<初速度

};
/*!
* @brief	パーティクルの発生機
*/

class CParticleEmitter
{
public:
	/*!
	* @brief	コンストラクタ。
	*/
	CParticleEmitter();
	/*!
	* @brief	デストラクタ。
	*/
	~CParticleEmitter();
	/*!
	*@brief	初期化。
	*@param[in]	camera		パーティクルの描画で使用するカメラ。
	*@param[in]	param		パーティクル生成用のパラメータ。
	*/
	void Init(const SParicleEmitParameter& param);
	void Update();
	void Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix);
	void SetPosition(D3DXVECTOR3 SetPosition)
	{
		Position = SetPosition;
	}
	void SetLockPosition(bool set)
	{
		lockPosition = set;
	}
private:
	SParicleEmitParameter	param;			//!<パラメータ。
	std::list<CParticle*>	particleList;	//!<パーティクルのリスト。
	float					timer = 0;			//!<タイマー
	float					EndTime = 0.0f;
	D3DXVECTOR3				Position;
	LPDIRECT3DTEXTURE9		texture;			//!<テクスチャ。
	static ID3DXEffect*		shaderEffect;		//!<シェーダーエフェクト。
	bool					lockPosition = false;
};

