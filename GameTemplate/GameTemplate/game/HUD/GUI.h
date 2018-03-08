#pragma once

class CGUI
{
public:
	CGUI();
	~CGUI();
	void Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix);
	void Update();
	void Start(const D3DXVECTOR2 setsize, const char* texturePath);
	void SetPosition(const D3DXVECTOR3 setpos)
	{
		m_position = setpos;
	}
	void setSize(D3DXVECTOR2 setsize);
private:

	ID3DXEffect*		shaderEffect;		//�V�F�[�_�[�G�t�F�N�g�B
	CPrimitive			m_primitive;		//�v���~�e�B�u�B
	LPDIRECT3DTEXTURE9	m_texture;			//�e�N�X�`���B
	D3DXVECTOR3			m_position;			//���W�B
	D3DXVECTOR2			m_size;				//�T�C�Y
	float				Time;
	bool                isDete = false;
	float               oldsize[2] ;
	float				m_oldHp;
};

