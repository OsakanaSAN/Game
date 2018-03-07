#pragma once

enum ResultState {
	e_Upadte,		//アップデート中
	e_Clear,		//クリア判定
	e_End,		//終了判定
};

class CResult
{
public:
	CResult();
	~CResult();
	void Start();
	void Update();
	void Render(LPD3DXSPRITE spt);
private:

	Sprite*		m_result_imag[2];
	Sprite*		m_result_Scoretext[5];
	Pad*		m_Pad;
	ResultState m_resultState;
};

