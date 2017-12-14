#pragma once

/*!
* @brief	�o�l�J�����B
* @details
*  �o�l�Ɉ���������悤�ȃJ�����̋�������������N���X�ł��B</br>
*  SetTarTarget�֐���SetTarPosition�֐����g�p����</br>
*  �J�����̖ڕW�ƂȂ钍���_�Ǝ��_��ݒ肵�Ă��������B</br>
*  Update�֐������s���邱�ƂŁA�J�������ݒ肳�ꂽ�ڕW���W�ɒǏ]���Ă����܂��B</br>
* @code
*		CSpringCamera springCamera;
//�������B
void Init()
{
springCamera.Init(
CVector3(0.0f, 0.0f, 0.0f),
CVector3(0.0f, 0.0f, 100.0f),
100.0f
);
}
//�X�V�B
void Update()
{
�E
�E
�E
//�J�����̒����_��ݒ肷��B
springCamera.SetTarTarget(target);
springCamera.SetTarPosition(position);

//�J�����𓮂����B
springCamera.Update();
}
* @endcode
*/
class SpringCamera {
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	SpringCamera();
	/*!
	* @brief	�f�X�g���N�^
	*/
	~SpringCamera();
	/*!
	* @brief	�������B
	*/
	void Init(const D3DXVECTOR3& target, const D3DXVECTOR3& position, float maxMoveSpeed);
	/*!
	* @brief	�ڕW�ƂȂ钍���_��ݒ�B
	*/
	void SetTarTarget(const D3DXVECTOR3& target)
	{
		m_target = target;
	}
	/*!
	* @brief	�ڕW�ƂȂ鎋�_��ݒ�B
	*/
	void SetTarPosition(const D3DXVECTOR3 position)
	{
		m_position = position;
	}
	/*!
	* @brief	�����_��ݒ�B
	*/
	void SetTarget(const D3DXVECTOR3& target)
	{
		m_camera.SetLookatPt(target);
	}
	/*!
	* @brief	���_��ݒ�B
	*/
	void SetPosition(const D3DXVECTOR3 position)
	{
		m_camera.SetEyePt(position);
	}
	/*!
	* @brief	�����ʂ�ݒ�B
	*/
	void SetFar(float _far)
	{
		m_camera.SetFar(_far);
	}
	/*!
	* @brief	�J�����̉�]�s����擾�B
	*/
	const D3DXMATRIX& GetCameraRotation() const
	{
		return m_camera.GetCameraRotation();
	}
	/*!
	* @brief	�����_���擾�B
	*/
	const D3DXVECTOR3& GetTarget()
	{
		return m_camera.GetLookatPt();

	}
	/*!
	* @brief	���_���擾�B
	*/
	const D3DXVECTOR3& GetPosition() 
	{
		return m_camera.GetEyePt();
	}

	/*!
	* @brief	�X�V�B
	*/
	void Update();
	/*!
	* @brief	�J�������擾�B
	*/
	const Camera* GetCamera() const
	{
		return &m_camera;
	}
	/*!
	* @brief	�o�l�̌�������ݒ�B
	*@detail
	* �l���傫���قǃJ�������x��ĕt���Ă��܂��B
	*/
	void SetDampingRate(float dampingRate)
	{
		m_targetDampingRate = dampingRate;
	}
	/*!
	* @brief	�J�������擾�B
	*/
	Camera* GetCamera()
	{
		return &m_camera;
	}
	void SetViewAngle(float angle)
	{
		m_camera.SetViewAngle(angle);
	}
	float GetViewAngle() const
	{
		return m_camera.GetViewAngle();
	}

	/*!
	* @brief	�ڕW�ƂȂ钍���_���擾�B
	*/
	const D3DXVECTOR3& GetTarTarget() const
	{
		return m_target;
	}
	/*!
	* @brief	�ڕW�ƂȂ鎋�_���擾�B
	*/
	const D3DXVECTOR3& GetTarPosition() const
	{
		return m_position;
	}
	/*!
	* @brief	�o�l�J�����p�̃p�����[�^���N���A�B
	*/
	void ClearSpringParame()
	{
		m_targetMoveSpeed = {0.0f,0.0f,0.0f};
		m_positionMoveSpeed = { 0.0f,0.0f,0.0f };

	}
	/*!
	* @brief	�o�l�J�����̍X�V�����B
	* @details
	*  Update�֐�����Ă΂�Ă��܂��B</br>
	*  �o�l�J�����̍X�V���������ʂŌĂт����Ȃǂ̓���ȃP�[�X�Ŏg�p���Ă��������B</br>
	*/
	void UpdateSpringCamera();
	/*!
	* @brief	�J�����̍X�V�B
	* @details
	*  Update�֐�����Ă΂�Ă��܂��B</br>
	*  �J�����̍X�V���������ʂŌĂт����Ȃǂ̓���ȃP�[�X�Ŏg�p���Ă��������B</br>
	*/
	void UpdateCamera()
	{
		m_camera.Update();
	}
	//�ō����x�Z�b�g
	void SetMaxMoveSpeed(float speed)
	{
		m_maxMoveSpeed = speed;

	}
	//�����_�̈ړ����x�Z�b�g
	void SetTagPosMoveSpeed(D3DVECTOR speed)
	{
		m_targetMoveSpeed = speed;

	}
private:
	Camera		m_camera;						//!<�J�����B
	D3DXVECTOR3	m_target = { 0.0f,0.0f,0.0f };		//!<�ڕW�ƂȂ钍���_�B
	D3DXVECTOR3	m_position = { 0.0f,0.0f,0.0f };	//!<�ڕW�ƂȂ鎋�_�B
	D3DXVECTOR3	m_targetMoveSpeed = { 0.0f,0.0f,0.0f };		//!<�����_�̈ړ����x�B
	D3DXVECTOR3	m_positionMoveSpeed = { 0.0f,0.0f,0.0f };	//!<���_�̈ړ����x�B

	float		m_maxMoveSpeed = 0.0f;					//!<�ō��ړ����x�B
	float		m_targetDampingRate = 5.0f;				//!<�������B�l���傫���قǃJ�������x��t���Ă���B
	float		m_dampingRate = 2.3f;					//!<�������B
	float		m_dampingRateVel = 5.0f;
};

