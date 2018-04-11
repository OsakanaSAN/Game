#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "Skydoom.h"
#include "Sea.h"
#include "Sun.h"

class MapChip;

class Map 
{
public:
	Map();
	~Map();
	void Start();
	void Init(int mapserect);
	void Update();
	void Render();
	CSun* GetSun()
	{
		return sun;
	}
	const std::list<CEnemy*> GetGroupenemy()
	{
		return m_Enemy[GroupNo];
	}
	//�V���h�E�}�b�v���������ނ��߂̕`�揈��
	void ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix);
	void RadarMapRender(D3DXMATRIX&  RederCameraMatrix, D3DXMATRIX&	RederCameraProjMatrix);
private:
	std::list<MapChip*>	mapChipList;		//�}�b�v�`�b�v�̃��X�g�B
	Skydoom*                skydoom;		//��
	Sea*					sea;			//����
	CSun*					sun;			//���z
	std::list<CEnemy*>		m_Enemy[4];		//�O���[�v
	int						GroupCount = 0;
	int						GroupNo = 0;

};

