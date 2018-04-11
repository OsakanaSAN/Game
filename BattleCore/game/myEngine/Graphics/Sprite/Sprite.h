#pragma once
#include "C2DImage.h"
class Sprite : public C2DImage 
{
public:
	Sprite();
	~Sprite();
	 void Initialize() override;
	 void Update()override;
	 void Loadtex(const char* filePath);
	 void Setposition(D3DXVECTOR2 pos)
	 {
		this->m_position = pos;
		position = pos;
	 }
	 D3DXVECTOR2 GetSize()
	 {
		 return m_Size;
	 }
	 void SetSize(D3DXVECTOR2 size)
	 {
		 m_Size = size;
	 }
	 //スプライトのカラーの設定 最大1 最低0
	 void Setcolor(float col0, float col1, float col2, float col3 )
	 {
		 m_Color[0] = (int)(col0 * 255);
		 m_Color[1] = (int)(col1 * 255);
		 m_Color[2] = (int)(col2 * 255);
		 m_Color[3] = (int)(col3 * 255);
	 }
	 void TexRelease()
	 {
		 m_pTexture->Release();
	 }
	 
private:
	D3DXVECTOR2 position = { 0,0 }; // スプライトの座標

};

