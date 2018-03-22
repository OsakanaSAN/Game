#pragma once

enum SceneState
{
	e_Upadte,		//アップデート中
	e_Clear,		//クリア判定
	e_End,			//終了判定
};

class CSelectScene
{
public:
	CSelectScene();
	~CSelectScene();
	void Update();
	void Start();
	void Render(LPD3DXSPRITE spt);


private:
	Sprite*			m_Select_imag[5];
	Pad				m_pad;
	char			TextName[255];
	int				m_mapselect = 1;
	SceneState		m_state = e_Upadte;

};

