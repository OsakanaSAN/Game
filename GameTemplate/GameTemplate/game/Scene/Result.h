#pragma once

enum ResultState {
	e_Upadte,		//�A�b�v�f�[�g��
	e_Clear,		//�N���A����
	e_End,		//�I������
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

