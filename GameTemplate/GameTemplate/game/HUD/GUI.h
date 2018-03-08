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

	ID3DXEffect*		shaderEffect;		//シェーダーエフェクト。
	CPrimitive			m_primitive;		//プリミティブ。
	LPDIRECT3DTEXTURE9	m_texture;			//テクスチャ。
	D3DXVECTOR3			m_position;			//座標。
	D3DXVECTOR2			m_size;				//サイズ
	float				Time;
	bool                isDete = false;
	float               oldsize[2] ;
	float				m_oldHp;
};

