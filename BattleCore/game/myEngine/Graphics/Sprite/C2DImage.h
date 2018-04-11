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
	D3DXVECTOR2 m_position;					//位置情報
	float       m_angle;					//回転情報
	D3DXVECTOR2 m_Size = { 1.0f,1.0f };		//大きさ
	D3DXVECTOR2 m_scale;					//スケール情報
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャ
	D3DCOLOR	m_backColor;				//背景カラー
	LPCTSTR		m_texFileName;				//テクスチャファイル名
	D3DXVECTOR2 m_texCenter;				//テクスチャの中点
	RECT		m_rect;						//描画矩形
	D3DXMATRIX	m_transformMatrix;			//変換行列
	D3DCOLOR	m_Color[4];                 //カラー
	D3DXVECTOR2 m_TextureSize;				//テクスチャ本来のサイズ
};

