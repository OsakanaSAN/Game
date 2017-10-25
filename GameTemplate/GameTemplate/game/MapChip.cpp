#include "stdafx.h"
#include "game.h"
#include "MapChip.h"


MapChip::MapChip() :
	position(0.0f, 0.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f, 1.0f)
{
}


MapChip::~MapChip()
{
	g_physicsWorld->RemoveRigidBody(&rigidBody);
}
void MapChip::Init(SMapChipLocInfo& locInfo)
{
	
	//�ǂݍ��ރ��f���̃t�@�C���p�X���쐬�B
	char modelPath[256];
	sprintf(modelPath, "Assets/modelData/%s.x", locInfo.modelName);
	//���f�������[�h�B
	modelData.LoadModelData(modelPath, NULL);
	//���[�h�������f���f�[�^���g����SkinModel���������B

	

	//���C�g���������B
	//light.SetAmbientLight({ 1,1,1,1 });
	if (locInfo.modelName != "skydoom") {
		
		light.SetAmbientLight(D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
	}

	model.SetLight(&light);
	position = locInfo.pos;
	rotation = locInfo.rotation;
	model.Init(&modelData);
	if (locInfo.modelName == "sea") {
		//light.SetDiffuseLightDirection(0, D3DXVECTOR4( 0.707f, -0.707f, 0.0f, 1.0f));
		light.SetDiffuseLightDirection(0, D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));

		//light.SetDiffuseLightDirection(1, D3DXVECTOR4( -0.3f, 0.2f, -0.3f, 1.0f));

		
		light.SetDiffuseLightColor(0, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		//light.SetDiffuseLightColor(1, D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));

		light.SetAmbientLight(D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
		model.SetLight(&light);
		model.SetWaveTexture();
		model.SetWave(true);
	
	}
	

	model.UpdateWorldMatrix(position, rotation, { 1.0f, 1.0f, 1.0f });
	
	if (locInfo.modelName != "sea")
	{
		//��������Փ˔��藍�݂̏������B
		//�X�L�����f�����烁�b�V���R���C�_�[���쐬����B
		D3DXMATRIX* rootBoneMatrix = modelData.GetRootBoneWorldMatrix();

		meshCollider.CreateFromSkinModel(&model, rootBoneMatrix);
		//�����č��̂��쐬����B
		//�܂��͍��̂��쐬���邽�߂̏���ݒ�B
		RigidBodyInfo rbInfo;
		rbInfo.collider = &meshCollider;	//���̂̃R���W������ݒ肷��B
		rbInfo.mass = 0.0f;					//���ʂ�0�ɂ���Ɠ����Ȃ����̂ɂȂ�B
		rbInfo.pos = position;
		rbInfo.rot = rotation;
		//���̂��쐬�B
		rigidBody.Create(rbInfo);
		//�쐬�������̂𕨗����[���h�ɒǉ��B
		g_physicsWorld->AddRigidBody(&rigidBody);
	}

}
void MapChip::Update()
{
	
	if(GetAsyncKeyState('A'))
	{
		light.SetDiffuseLightDirection(lightNo,Up);
		model.SetLight(&light);
		Up.y += 0.01f;
	}
	if (GetAsyncKeyState('Z'))
	{
		light.SetDiffuseLightDirection(lightNo, Up);
		model.SetLight(&light);
		Up.y -= 0.01f;
	}
	if (GetAsyncKeyState('S'))
	{
		light.SetDiffuseLightDirection(lightNo, Up);
		model.SetLight(&light);
		Up.x += 0.01f;
	}
	if (GetAsyncKeyState('X'))
	{
		light.SetDiffuseLightDirection(lightNo, Up);
		model.SetLight(&light);
		Up.x -= 0.01f;
	}
	if (GetAsyncKeyState('D'))
	{
		light.SetDiffuseLightDirection(lightNo, Up);
		model.SetLight(&light);
		Up.z += 0.01f;
	}
	if (GetAsyncKeyState('C'))
	{
		light.SetDiffuseLightDirection(lightNo, Up);
		model.SetLight(&light);
		Up.z -= 0.01f;
	}


	model.UpdateWorldMatrix(position, rotation, { 1.0f, 1.0f, 1.0f });
}
void MapChip::Draw()
{
	model.SetShadowMap(false);
	model.SetShadowRecieve(true);

	model.Draw(&game->GetCamera()->GetViewMatrix(), &game->GetCamera()->GetProjectionMatrix());
}