#pragma once

enum SceneState
{
	e_Upadte,		//�A�b�v�f�[�g��
	e_Clear,		//�N���A����
	e_End,			//�I������
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

