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
	if (rigidBody != NULL)
	{
		g_physicsWorld->RemoveRigidBody(rigidBody);
		rigidBody->Release();
		delete rigidBody;
		modelData.Release();
	}
}
void MapChip::Init(SMapChipLocInfo& locInfo)
{
	
	//�ǂݍ��ރ��f���̃t�@�C���p�X���쐬�B
	char modelPath[256];
	sprintf(modelPath, "Assets/modelData/%s.x", locInfo.modelName);
	//���f�������[�h�B
	modelData.LoadModelData(modelPath, NULL);
	//���[�h�������f���f�[�^���g����SkinModel���������B

	

	
	
	position = locInfo.pos;
	rotation = locInfo.rotation;
	model.Init(&modelData);

	if (locInfo.modelName == "Wall" || locInfo.modelName == "Wall2")
	{
		model.SetAlphaZero(true);
	}
	//���C�g���������B
	if (locInfo.modelName == "map7")
	{
		light.SetAmbientLight({ 0.5f,0.5f,0.5f,1.0f });
	}

	else if (locInfo.modelName != "Map4")
	{
		light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, -0.707f, 0.0f, 1.0f));
		light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
		light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
		light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));


		light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
		light.SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
		light.SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
		light.SetDiffuseLightColor(3, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));


		light.SetAmbientLight(D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));

	}
	else
	{

		light.SetAmbientLight({ 0.1f,0.1f,0.1f,1.0f });

	}

	model.SetLight(&light);

	if (locInfo.modelName != "bill2_NR" || locInfo.modelName != "bill3_NR")
	{

		rigidBody = new RigidBody;
		//��������Փ˔��藍�݂̏������B
		//�X�L�����f�����烁�b�V���R���C�_�[���쐬����B
		D3DXMATRIX* rootBoneMatrix = modelData.GetRootBoneWorldMatrix();
		model.UpdateWorldMatrix(position, rotation, { 1.0f, 1.0f, 1.0f });

		meshCollider.CreateFromSkinModel(&model, rootBoneMatrix);
		//�����č��̂��쐬����B
		//�܂��͍��̂��쐬���邽�߂̏���ݒ�B
		RigidBodyInfo rbInfo;
		rbInfo.collider = &meshCollider;	//���̂̃R���W������ݒ肷��B
		rbInfo.mass = 0.0f;					//���ʂ�0�ɂ���Ɠ����Ȃ����̂ɂȂ�B
		rbInfo.pos = position;
		rbInfo.rot = rotation;
		//���̂��쐬�B
		rigidBody->Create(rbInfo);
		//�쐬�������̂𕨗����[���h�ɒǉ��B
		g_physicsWorld->AddRigidBody(rigidBody);

	}


}
	


void MapChip::Update()
{
	//���C�g�̉�]
	float angle = 0.0f;
	if (GetAsyncKeyState('Q')) {
		angle = D3DXToRadian(1.0f);
	}
	else if (GetAsyncKeyState('E')) {
		angle = D3DXToRadian(-1.0f);
	}
	//���C�g����]������s����쐬����B
	D3DXMATRIX mRot;
	D3DXVECTOR3 UP = { 1.0,0.0,0.0 };
	D3DXMatrixRotationAxis(&mRot, &UP, angle);
	//��]������B
	for (int i = 0; i < 4; i++) {
		D3DXVECTOR4 dir;
		D3DXVec4Transform(&dir, &light.GetDiffuseLightDirection()[i], &mRot);
		light.SetDiffuseLightDirection(i, dir);
	}
	
	model.UpdateWorldMatrix(position, rotation, { 1.0f, 1.0f, 1.0f });
}
void MapChip::Render()
{
	model.SetShadowMap(false);
	model.SetShadowRecieve(true);
	model.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
}
void MapChip::LightEyePosRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	model.SetShadowMap(true);
	model.SetShadowRecieve(false);

	model.Draw(&lightViewMatrix, &lightProjMatrix);

}