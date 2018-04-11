#pragma once
#include"stdafx.h"
#define WINDOWS_WIDTH 720 //��
#define WINDOWS_HEIGHT 480 //����


//���_�t�H�[�}�b�g(�g�����X�t�H�[���ύ��W�ƃf�B�t�[�Y�J���[)
#define FVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE| D3DFVF_TEX1)

//�J�X�^�����_�f�[�^�p�\����
typedef struct CUSTOMVERTEX
{
	float x, y, z;	// ���_�̕ϊ���̍��W
	DWORD color;		// ���_�J���[
	float u, v;			//�e�N�X�`�����W
}CUSTOMVERTEX;

//���b�V���p�\����
typedef struct MESH
{
	LPDIRECT3DTEXTURE9* pTextures;
	D3DMATERIAL9* pMaterials;
	LPD3DXMESH pMesh;
	DWORD materialNum;

};