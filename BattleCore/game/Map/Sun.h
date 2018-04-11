#pragma once
class CSun
{
public:
	CSun();
	~CSun();
	void Start(D3DXVECTOR3 pos, D3DXQUATERNION rot);
	void Update();
	void Render();
	D3DXVECTOR3 GetPosition() const
	{
		return m_position;
	}
private:
	Light			m_light;
	D3DXVECTOR3		m_position;
	D3DXQUATERNION	m_rot;
	SkinModel		m_skinmodel;
	SkinModelData   m_skinmodeldata;
};

