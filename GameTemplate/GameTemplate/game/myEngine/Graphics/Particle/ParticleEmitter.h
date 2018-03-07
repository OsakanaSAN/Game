#pragma once

class CParticle;
/*!
* @brief	�p�[�e�B�N�������p�����[�^
*/
struct SParicleEmitParameter {
	//�������B
	void Init()
	{
		memset(this, 0, sizeof(SParicleEmitParameter));
	}
	const char* texturePath;			//!<�e�N�X�`���̃t�@�C���p�X�B
	float w;							//!<�p�[�e�B�N���̕��B
	float h;							//!<�p�[�e�B�N���̍����B
	float intervalTime;					//!<�p�[�e�B�N���̔����Ԋu�B
	float timer;						//������܂ł̎���
	D3DXVECTOR3 initSpeed;				//!<�����x

};
/*!
* @brief	�p�[�e�B�N���̔����@
*/

class CParticleEmitter
{
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	CParticleEmitter();
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~CParticleEmitter();
	/*!
	*@brief	�������B
	*@param[in]	camera		�p�[�e�B�N���̕`��Ŏg�p����J�����B
	*@param[in]	param		�p�[�e�B�N�������p�̃p�����[�^�B
	*/
	void Init(const SParicleEmitParameter& param);
	void Update();
	void Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix);
	void SetPosition(D3DXVECTOR3 SetPosition)
	{
		Position = SetPosition;
	}
private:
	SParicleEmitParameter	param;			//!<�p�����[�^�B
	std::list<CParticle*>	particleList;	//!<�p�[�e�B�N���̃��X�g�B
	float					timer = 0;			//!<�^�C�}�[
	D3DXVECTOR3				Position;
	LPDIRECT3DTEXTURE9		texture;			//!<�e�N�X�`���B
	static ID3DXEffect*		shaderEffect;		//!<�V�F�[�_�[�G�t�F�N�g�B
};

