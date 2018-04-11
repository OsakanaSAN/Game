#pragma once
class IGameObject
{
public:
	IGameObject();
	~IGameObject();
	virtual void Update()=0;
	//virtual void Start()=0;
	//virtual void Render()=0;
	
private:

	//D3DXVECTOR3			m_Position;				//座標
	//D3DXQUATERNION		m_Rotation;				//回転
	//SkinModel				m_Skin_model;			//スキンモデル
	//SkinModelData			m_Skin_model_data;		//スキンモデルデータ
	//Light					m_Light;				//ライト
	//CharacterController	m_CharacterController	//キャラクターコントローラー


};

