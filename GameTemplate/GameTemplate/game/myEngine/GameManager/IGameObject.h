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

	//D3DXVECTOR3			m_Position;				//���W
	//D3DXQUATERNION		m_Rotation;				//��]
	//SkinModel				m_Skin_model;			//�X�L�����f��
	//SkinModelData			m_Skin_model_data;		//�X�L�����f���f�[�^
	//Light					m_Light;				//���C�g
	//CharacterController	m_CharacterController	//�L�����N�^�[�R���g���[���[


};

