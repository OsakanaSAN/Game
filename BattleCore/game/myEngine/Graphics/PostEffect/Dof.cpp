#include "stdafx.h"
#include "Dof.h"


CDof::CDof()
{
	//�[�x���������ރ����_�����O�̏�����
	m_DepthRenderTarget.Create
	(
		1280.0f,
		720.0f,
		1,
		D3DFMT_R16F,
		D3DFMT_UNKNOWN,
		D3DMULTISAMPLE_NONE,
		0
	);


}


CDof::~CDof()
{
}


void CDof::Render()
{



}
