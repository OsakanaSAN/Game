#pragma once


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
	SceneState m_resultState;
};

