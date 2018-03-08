#pragma once
enum RankChecker
{
	RANK_A,
	RANK_B,
	RANK_C,
};

class CScoreCheckre
{
public:
	CScoreCheckre();
	~CScoreCheckre();
	//更新
	void Update();
	//インスタン生成維持に一度だけ呼び出される
	void Start();
	//描画
	void Render(LPD3DXSPRITE spt);
	//プレイヤーの残りHPを計算して表示するための関数
	void DamageCheck();
	//ステージクリアー時間を計算する関数
	void TimeCheck();
	//クリアー時の評価をする関数
	void RankCheck();
	//スコアの計算をする関数
	void SCheck();
	//敵の機体数を入れる
	void CountUp()
	{
		m_Count++;
	}
	void SetScore()
	{

		m_Score += 50;

	}

private:
	Sprite* m_Number[4];			//時間を表示するためのスプライト
	Sprite* m_Dameg_img[4];			//残りHPを表示するためのスプライト
	Sprite* m_Score_img[3];			//スコアを表示するためのスプライト
	Sprite* m_Rank_img;				//ランクを表示するためのスプライト
	int		m_Score;				//スコアの計算用の変数
	int		m_DamageChecker[4];		//ダメージ計算用の変数
	int		m_TimeChecker = 0;		//ゲーム終了までの時間計算変数
	float	m_Time = 0;				//時間を格納する変数
	float	m_num = 0;				
	char    TextName[255];			//テクスチャの名前で判別するための変数
	int		m_Rank;					//ランクをつけるための変数
	int		m_Count = 0;
	
	
};

