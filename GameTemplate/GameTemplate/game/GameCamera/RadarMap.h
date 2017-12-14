#pragma once
class CRadarMap
{
public:
	CRadarMap();
	~CRadarMap();
	void InitRender();
	void Update();
	void Draw();


private:

	CRenderTarget m_SrenderTarget;		//���[�_�[�}�b�v���������ރ����_�����O�^�[�Q�b�g�B
	D3DXMATRIX  m_lightViewMatrix;		//���C�g�r���[�}�g���N�X�B
	D3DXMATRIX	m_lightProjMatrix;		//���C�g�v���W�F�N�V�����}�g���N�X�B
	D3DXVECTOR3 m_viewPosition;			//���C�g�r���[�̎��_�B
	D3DXVECTOR3 m_viewTarget;			//���C�g�r���[�̒����_�B
	D3DXVECTOR3 m_lightDirection;		//!<���C�g�̌���
	Camera      m_Scamera;				//�J����
	float					m_Movelight = 0.0f; //���C�g�̎��_���ړ�������
	float					m_Far;				//!<�����ʁB
	float					m_Near;				//!<�ߕ��ʁB
	float			        m_viewAngle;		//!<��p(���W�A��)�B
	float					m_aspect;			//!<�A�X�y�N�g��

};

