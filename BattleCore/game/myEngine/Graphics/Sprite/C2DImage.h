#pragma once
class C2DImage
{
public:
	C2DImage();
	~C2DImage();
	
	virtual void Initialize();
	virtual void Update() = 0;
	void Draw(LPD3DXSPRITE);
	void SetupMatrices();
	void SetTexture(LPDIRECT3DTEXTURE9 SetTex)
	{
		m_pTexture = SetTex;
	}
protected:
	D3DXVECTOR2 m_position;					//�ʒu���
	float       m_angle;					//��]���
	D3DXVECTOR2 m_Size = { 1.0f,1.0f };		//�傫��
	D3DXVECTOR2 m_scale;					//�X�P�[�����
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`��
	D3DCOLOR	m_backColor;				//�w�i�J���[
	LPCTSTR		m_texFileName;				//�e�N�X�`���t�@�C����
	D3DXVECTOR2 m_texCenter;				//�e�N�X�`���̒��_
	RECT		m_rect;						//�`���`
	D3DXMATRIX	m_transformMatrix;			//�ϊ��s��
	D3DCOLOR	m_Color[4];                 //�J���[
	D3DXVECTOR2 m_TextureSize;				//�e�N�X�`���{���̃T�C�Y
};

