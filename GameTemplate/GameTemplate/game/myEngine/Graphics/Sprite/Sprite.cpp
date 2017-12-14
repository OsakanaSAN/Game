#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}
void Sprite::Loadtex(const char* filePath)
{

	this->m_texFileName = filePath;

}

void Sprite::Initialize()
{
	m_position = position;
	m_angle = 0;
	m_scale = { 0.1f,0.1f };
	m_texCenter = position;
	C2DImage::Initialize();
}
void Sprite::Update()
{
	this->SetupMatrices();
}