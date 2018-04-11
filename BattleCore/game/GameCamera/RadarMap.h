#pragma once
#include "myEngine/Graphics/RenderTarget.h"
#include "myEngine/Camera/CameraCollisionSolver.h"
#include "myEngine/Camera/SpringCamera.h"
#include "myEngine/Graphics/Sprite/Primitive.h"


class CRadarMap
{
public:
	CRadarMap();
	~CRadarMap();
	void InitRender();
	void Update(const D3DXVECTOR3 position);
	void Draw(SkinModel* skinmodel);
	//�e�N�X�`�����擾�B
	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return m_SrenderTarget.GetTexture();
	}


private:
	CPrimitive		m_primitive;
	CRenderTarget m_SrenderTarget;		//���[�_�[�}�b�v���������ރ����_�����O�^�[�Q�b�g�B
	D3DXMATRIX  m_lightViewMatrix;		//���C�g�r���[�}�g���N�X�B
	D3DXMATRIX	m_lightProjMatrix;		//���C�g�v���W�F�N�V�����}�g���N�X�B
	D3DXVECTOR3 m_viewPosition;			//���C�g�r���[�̎��_�B
	D3DXVECTOR3 m_viewTarget;			//���C�g�r���[�̒����_�B
	D3DXVECTOR3 m_lightDirection;		//!<���C�g�̌���
	Camera      m_Scamera;							//�J����
	SpringCamera    m_Springcamera;					//�o�l�J����
	CameraCollisionSolver CameraCol;				//���̕t���J����
	LPDIRECT3DTEXTURE9	texture;			//!<�e�N�X�`���B
	ID3DXEffect*		shaderEffect;		//!<�V�F�[�_�[�G�t�F�N�g�B


	D3DXVECTOR3     Eyepos =	{0.0f,500.0f, 0.0f }; //�J�����̈ʒu
	D3DXVECTOR3     m_targetpos = { 0.0f,0.0f, 0.0f }; //�J�����̈ʒu
	D3DXMATRIX		m_rot;

	float					m_Movelight = 0.0f; //���C�g�̎��_���ړ�������
	float					m_Far;				//!<�����ʁB
	float					m_Near;				//!<�ߕ��ʁB
	float			        m_viewAngle;		//!<��p(���W�A��)�B
	float					m_aspect;			//!<�A�X�y�N�g��
	Pad						pad;     //�p�b�g
};

