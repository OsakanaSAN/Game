#pragma once
#include "myEngine/Graphics/Sprite/Primitive.h"
class CPostEffect
{
public:
	CPostEffect();
	~CPostEffect();
	void InitMainRenderTarget();	//���C�������_�����O�^�[�Q�b�g���������B
	void InitPrimitive();			//�|���̏�����
	void MainRTToCurrentRT();		//���C�������_�����O�^�[�Q�b�g�̓��e�����݂̃����_�����O�^�[�Q�b�g�ɃR�s�[�B
	void LoadShader();				//�V�F�[�_�[�����[�h�B
	void Render();
	void RenderPrimitive();
	CRenderTarget* GetMainRenderTarget();
private:
	CBloom          bloom;
	LPD3DXEFFECT m_CopyEffct;
	LPD3DXEFFECT m_SepiaEffct;			//�Z�s�A���̃t�B���^�[������V�F�[�_�[�B

};

extern CPostEffect* g_PostEffect;