#include "stdafx.h"
#include "PostEffect.h"

CRenderTarget* mainRenderTarget;	//!<18-2 ���C�������_�����O�^�[�Q�b�g�B
CPrimitive*	quadPrimitive;			//!<18-3 �l�p�`�̔|���v���~�e�B�u�B

CRenderTarget* CPostEffect::GetMainRenderTarget()
{
	return mainRenderTarget;
}

CPostEffect::CPostEffect()
{
}


CPostEffect::~CPostEffect()
{
}

void CPostEffect::InitMainRenderTarget()
{
	mainRenderTarget = new CRenderTarget;

	mainRenderTarget->Create(
		1280,					//�t���[���o�b�t�@�̕�
		 720,					//�t���[���o�b�t�@�̍���
		   1,					//�~�b�v�}�b�v���x��
		D3DFMT_A16B16G16R16F,	//�J���[�o�b�t�@�̃t�H�[�}�b�g
		D3DFMT_D24S8,			//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�̎��
		0						//�}���`�T���v�����O�̕i��
	);
}

void CPostEffect::InitPrimitive()
{
	quadPrimitive = new CPrimitive;
	//���_�̍\���́B
	struct SVertex {
		float pos[4];	//���_���W�B
		float uv[2];		//UV���W�B
	};
	//���_�o�b�t�@�B
	SVertex vertexBuffer[] = {
		{
			-1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f
		},
		{
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f
		},
		{
			-1.0f, -1.0f, 0.0f, 1.0f,
			0.0f, 1.0f
		},
		{
			1.0f, -1.0f, 0.0f, 1.0f,
			1.0f, 1.0f
		},
	};
	//���_���C�A�E�g�B���_�o�b�t�@�̍\�����ǂ̂悤�ɂȂ��Ă��邩��GPU�ɋ����邽�߂̂��́B
	static const D3DVERTEXELEMENT9 scShapeVertex_PT_Element[] = {
		//���_�X�g���[����0�Ԗڂ�0�o�C�g�ڂ���float�^��4���̃f�[�^�͍��W(D3DDECLUSAGE_POSITION)�ł��邱�Ƃ������Ă���B
		{ 0, 0 ,   D3DDECLTYPE_FLOAT4		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION	, 0 },
		//���_�X�g���[����0�Ԗڂ�16�o�C�g�ڂ���float�^��2���̃f�[�^��UV���W(D3DDECLUSAGE_TEXCOORD)�ł��邱�Ƃ������Ă���B
		{ 0, 16 ,  D3DDECLTYPE_FLOAT2		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD	, 0 },
		D3DDECL_END()
	};
	//�C���f�b�N�X�o�b�t�@�B
	unsigned short indexBuffer[] = { 0, 1, 2, 3 };
	quadPrimitive->Create(
		CPrimitive::eTriangleStrip,	//����̓v���~�e�B�u�̎�ނ̓g���C�A���O���X�g���b�v�B
		4,							//���_�̐��B�l�p�`�̔|���Ńg���C�A���O���X�g���b�v�Ȃ̂łS�B
		sizeof(SVertex),			//���_�X�g���C�h�B��̒��_�̑傫���B�P�ʂ̓o�C�g�B
		scShapeVertex_PT_Element,	//���_���C�A�E�g�B
		vertexBuffer,				//���_�o�b�t�@�B
		4,							//�C���f�b�N�X�̐��B
		D3DFMT_INDEX16,				//�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g�B�����16bit�t�H�[�}�b�g���w�肷��B
									//�C���f�b�N�X�̐���65536�ȉ��̏ꍇ��16bit�ł������A����𒴂���ꍇ��D3DFMT_INDEX32���w�肷��B
		indexBuffer					//�C���f�b�N�X�o�b�t�@�B
	);
}

//���C�������_�����O�^�[�Q�b�g�̓��e�����݂̃����_�����O�^�[�Q�b�g�ɃR�s�[
void CPostEffect::MainRTToCurrentRT()
{
	
	// Z�e�X�g�Ŏ��s���Ă�������獢��̂ŁAZ�e�X�g�͖����ɂ��Ă����B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ���u�����h������Ȃ��B
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	// ���_�X�g���[��0�Ԃɔ|���̒��_�o�b�t�@��ݒ肷��B
	g_pd3dDevice->SetStreamSource(
		0,												//���_�X�g���[���̔ԍ��B
		quadPrimitive->GetVertexBuffer()->GetBody(),	//���_�o�b�t�@�B
		0,												//���_�o�b�t�@�̓ǂݍ��݂��J�n����I�t�Z�b�g�̃o�C�g���B											//����͐擪����ǂݍ��ނ̂�0�ł����B
		quadPrimitive->GetVertexBuffer()->GetStride()	//���_����̃T�C�Y�B�P�ʂ̓o�C�g�B
	);


	//LPD3DXEFFECT shader = SepiaEffct;			//�Z�s�A�t�B���^�[��������

	//�R�s�[�V�F�[�_�[��ݒ�B
	m_CopyEffct->SetTechnique("Default");
	m_CopyEffct->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_CopyEffct->BeginPass(0);
	//�V�[���e�N�X�`����ݒ肷��
	m_CopyEffct->SetTexture("g_tex",mainRenderTarget->GetTexture());
	//�萔���W�X�^�ւ̕ύX���R�~�b�g����B
	m_CopyEffct->CommitChanges();


	//�Z�s�A�V�F�[�_�[�̐ݒ�
	//m_SepiaEffct->SetTechnique("Default");
	//m_SepiaEffct->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	//m_SepiaEffct->BeginPass(0);
	//�V�[���e�N�X�`����ݒ肷��
	m_SepiaEffct->SetTexture("g_tex", mainRenderTarget->GetTexture());
	//�萔���W�X�^�ւ̕ύX���R�~�b�g����B
	m_SepiaEffct->CommitChanges();

	// �C���f�b�N�X�o�b�t�@��ݒ�B
	g_pd3dDevice->SetIndices(quadPrimitive->GetIndexBuffer()->GetBody());
	// ���_��`��ݒ肷��B
	g_pd3dDevice->SetVertexDeclaration(quadPrimitive->GetVertexDecl());
	//�@�������������̂ŕ`��B
	g_pd3dDevice->DrawIndexedPrimitive(
		quadPrimitive->GetD3DPrimitiveType(),	//�v���~�e�B�u�̎�ނ��w�肷��B
		0,										//�x�[�X���_�C���f�b�N�X�B0�ł����B
		0,										//�ŏ��̒��_�C���f�b�N�X�B0�ł����B
		quadPrimitive->GetNumVertex(),			//���_�̐��B
		0,										//�J�n�C���f�b�N�X�B0�ł����B
		quadPrimitive->GetNumPolygon()			//�v���~�e�B�u�̐��B
	);
	//�R�s�[
	m_CopyEffct->EndPass();
	m_CopyEffct->End();
	//�Z�s�A
	//m_SepiaEffct->EndPass();
	//m_SepiaEffct->End();
	// �ύX���������_�����O�X�e�[�g�����ɖ߂��B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

}

//�V�F�[�_�[�����[�h
void CPostEffect::LoadShader()
{

	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//�Z�s�A���̃V�F�[�_�[���[�h
	m_SepiaEffct = g_effectManager->LoadEffect("Assets/Shader/SepiaFilter.fx");
	m_CopyEffct = g_effectManager->LoadEffect("Assets/Shader/Copy.fx");


}
void CPostEffect::Render()
{
	
	//���݂̃����_�����O�^�[�Q�b�g�̃o�b�N�A�b�v�̍쐬
	LPDIRECT3DSURFACE9 frameBufferRT;
	LPDIRECT3DSURFACE9 frameBufferDS;
	g_pd3dDevice->GetRenderTarget(0, &frameBufferRT);
	g_pd3dDevice->GetDepthStencilSurface(&frameBufferDS);

	//�����_�����O�^�[�Q�b�g�̐؂�ւ�
	g_pd3dDevice->SetRenderTarget(
		0,									//���Ԗڂ̃����_�����O�^�[�Q�b�g
		mainRenderTarget->GetRenderTarget()  //�ύX���郌���_�����O�^�[�Q�b�g
	);
	//�f�v�X�X�e���V���o�b�t�@�̐؂�ւ�
	g_pd3dDevice->SetDepthStencilSurface(
		mainRenderTarget->GetDepthStencilBuffer()//�ύX����f�v�X�X�e���V���o�b�t�@
	);

	// �����_�����O�^�[�Q�b�g���N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	g_pd3dDevice->BeginScene();

	game->Render();
	bloom.Render();
	monochro.Render();
	if (game->GetPlayer()->GetZoomBlur())
	{

		zoomBlur.Render();

	}

	// �n���Y�I�� 1-2 �����_�����O�^�[�Q�b�g��߂��B
	g_pd3dDevice->SetRenderTarget(0, frameBufferRT);
	g_pd3dDevice->SetDepthStencilSurface(frameBufferDS);

	frameBufferRT->Release();
	frameBufferDS->Release();
	
	// 18-3 �I�t�X�N���[�������_�����O�����G���t���[���o�b�t�@�ɓ\��t����B
	MainRTToCurrentRT();
	

	game->Render2D();
	// �V�[���̕`��I���B
	//g_pd3dDevice->EndScene();

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	//g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

}

void CPostEffect::RenderPrimitive()
{
	// ���_�X�g���[��0�Ԃɔ|���̒��_�o�b�t�@��ݒ肷��B
	g_pd3dDevice->SetStreamSource(
		0,												//���_�X�g���[���̔ԍ��B
		quadPrimitive->GetVertexBuffer()->GetBody(),	//���_�o�b�t�@�B
		0,												//���_�o�b�t�@�̓ǂݍ��݂��J�n����I�t�Z�b�g�̃o�C�g���B����͐擪����ǂݍ��ނ̂�0�ł����B
		quadPrimitive->GetVertexBuffer()->GetStride()	//���_����̃T�C�Y�B�P�ʂ̓o�C�g�B
	);
	// �C���f�b�N�X�o�b�t�@��ݒ�B
	g_pd3dDevice->SetIndices(quadPrimitive->GetIndexBuffer()->GetBody());
	// ���_��`��ݒ肷��B
	g_pd3dDevice->SetVertexDeclaration(quadPrimitive->GetVertexDecl());
	//�@�������������̂ŕ`��B
	g_pd3dDevice->DrawIndexedPrimitive(
		quadPrimitive->GetD3DPrimitiveType(),	//�v���~�e�B�u�̎�ނ��w�肷��B
		0,										//�x�[�X���_�C���f�b�N�X�B0�ł����B
		0,										//�ŏ��̒��_�C���f�b�N�X�B0�ł����B
		quadPrimitive->GetNumVertex(),			//���_�̐��B
		0,										//�J�n�C���f�b�N�X�B0�ł����B
		quadPrimitive->GetNumPolygon()			//�v���~�e�B�u�̐��B
	);
}
