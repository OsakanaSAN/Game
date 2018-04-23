#include "stdafx.h"
#include "C2DImage.h"
#include "myEngine/system.h"


C2DImage::C2DImage()
{
}


C2DImage::~C2DImage()
{
}


void C2DImage::Initialize()
{
	D3DXIMAGE_INFO imgInfo;										//�摜���i�[�p�\����
	D3DXCreateTextureFromFileEx((g_pd3dDevice), this->m_texFileName, 0, 0, 0, 0, D3DFMT_A1R5G5B5,
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xff000000, &imgInfo, NULL, &this->m_pTexture);	//�e�N�X�`���Ǎ�
	this->m_texCenter = D3DXVECTOR2({ 0.0f,0.0f });	//�e�N�X�`���̒��_�Z�b�g

	//(float)imgInfo.Width / 2, (float)imgInfo.Height / 2;
	m_TextureSize.x = (float)imgInfo.Width + 100;
	m_TextureSize.y = (float)imgInfo.Height + 100;
	RECT rec = { 0, 0, imgInfo.Width, imgInfo.Height };			//�`��̈�
	memcpy(&this->m_rect, &rec, sizeof(RECT));					//�`��̈�Z�b�g
}

void C2DImage::Draw(LPD3DXSPRITE Sprite)
{
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	Sprite->Begin(0);		//�X�v���C�g�`��J�n
	Sprite->SetTransform(&this->m_transformMatrix);	//�ϊ��s��Z�b�g
	Sprite->Draw(this->m_pTexture, NULL, &D3DXVECTOR3(this->m_texCenter.x, this->m_texCenter.y, 0.0f), NULL, D3DCOLOR_ARGB(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));	//�X�v���C�g�Ƀe�N�X�`���\�t��
	Sprite->End();	//�X�v���C�g�`��I��
}

void C2DImage::SetupMatrices()
{

	
	m_scale.x = m_Size.x / m_TextureSize.x;
	m_scale.y = m_Size.y / m_TextureSize.y;
	D3DXMatrixIdentity(&this->m_transformMatrix);	//���[���h�s�񏉊���
	D3DXMatrixTransformation2D(&this->m_transformMatrix,NULL, 0.0f, &this->m_scale, NULL, D3DXToRadian(this->m_angle), &this->m_position);	//�ϊ��s��쐬
}