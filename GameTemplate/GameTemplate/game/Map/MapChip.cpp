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
	
	//読み込むモデルのファイルパスを作成。
	char modelPath[256];
	sprintf(modelPath, "Assets/modelData/%s.x", locInfo.modelName);
	//モデルをロード。
	modelData.LoadModelData(modelPath, NULL);
	//ロードしたモデルデータを使ってSkinModelを初期化。

	

	
	
	position = locInfo.pos;
	rotation = locInfo.rotation;
	model.Init(&modelData);

	if (locInfo.modelName == "Wall" || locInfo.modelName == "Wall2")
	{
		model.SetAlphaZero(true);
	}
	//ライトを初期化。
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
		//ここから衝突判定絡みの初期化。
		//スキンモデルからメッシュコライダーを作成する。
		D3DXMATRIX* rootBoneMatrix = modelData.GetRootBoneWorldMatrix();
		model.UpdateWorldMatrix(position, rotation, { 1.0f, 1.0f, 1.0f });

		meshCollider.CreateFromSkinModel(&model, rootBoneMatrix);
		//続いて剛体を作成する。
		//まずは剛体を作成するための情報を設定。
		RigidBodyInfo rbInfo;
		rbInfo.collider = &meshCollider;	//剛体のコリジョンを設定する。
		rbInfo.mass = 0.0f;					//質量を0にすると動かない剛体になる。
		rbInfo.pos = position;
		rbInfo.rot = rotation;
		//剛体を作成。
		rigidBody->Create(rbInfo);
		//作成した剛体を物理ワールドに追加。
		g_physicsWorld->AddRigidBody(rigidBody);

	}


}
	


void MapChip::Update()
{
	//ライトの回転
	float angle = 0.0f;
	if (GetAsyncKeyState('Q')) {
		angle = D3DXToRadian(1.0f);
	}
	else if (GetAsyncKeyState('E')) {
		angle = D3DXToRadian(-1.0f);
	}
	//ライトを回転させる行列を作成する。
	D3DXMATRIX mRot;
	D3DXVECTOR3 UP = { 1.0,0.0,0.0 };
	D3DXMatrixRotationAxis(&mRot, &UP, angle);
	//回転させる。
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