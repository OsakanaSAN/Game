#pragma once

/*!
 *@brief	�J�����N���X�B
 */
class Camera {
private:
	D3DXMATRIX				viewMatrix;			//!<�r���[�s��B�J�����s��Ƃ������B
	D3DXMATRIX              viewMatrixInv;      //�r���[�s��̋t�s��
	D3DXMATRIX				viewProjectionMatrix;//!<�r���[�v���W�F�N�V�����s��B
	D3DXMATRIX              CameraRot;          //!<�J�����̉�]�s��B
	D3DXMATRIX				projectionMatrix;	//!<�v���W�F�N�V�����s��B�r���[��Ԃ���ˉe��Ԃɕϊ�����s��B
	D3DXVECTOR3				vEyePt;				//!<�J�����̎��_�B
	D3DXVECTOR3				vLookatPt;			//!<�J�����̒����_�B
	D3DXVECTOR3				vUpVec;				//!<�J�����̏�����B
	float					Far;				//!<�����ʁB
	float					Near;				//!<�ߕ��ʁB
	float			        m_viewAngle;		//!<��p(���W�A��)�B
	float					aspect;				//!<�A�X�y�N�g��
	bool		            m_isNeedUpdateProjectionMatrix;

public:
	/*!
	 *@brief	�R���X�g���N�^�B
	 */
	Camera();
	/*!
	 *@brief	�f�X�g���N�^
	 */
	~Camera();
	/*!
	 *@brief	�A�X�y�N�g���ݒ�B
	 */
	void SetAspect(float _aspect);
	/*!
	 *@brief	�A�X�y�N�g����擾�B
	 */
	float GetAspect();
	/*!
	 *@brief	�t�@�[��ݒ�B
	 */
	void SetFar(float _far);
	
	/*!
	 *@brief	�j�A���擾�B
	 */
	float GetNear();
	/*!
	 *@brief	�t�@�[���擾�B
	 */
	float GetFar();
	/*!
	 *@brief	�j�A��ݒ�B
	 */
	void SetNear(float _near);
	/*!
	 *@brief	���_��ݒ�B
	 */
	void SetEyePt(const D3DXVECTOR3 pt);
	/*!
	 *@brief	���_���擾�B
	 */
	D3DXVECTOR3 GetEyePt();
	/*!
	 *@brief	�����_��ݒ�B
	 */
	void SetLookatPt(const D3DXVECTOR3 pt);
	/*!
	 *@brief	�����_�̎擾�B
	 */
	D3DXVECTOR3 GetLookatPt();
	/*!
	 *@brief	������̐ݒ�B
	 */
	void SetUpVec(D3DXVECTOR3 up);
	/*!
	 *@brief	������̎擾�B
	 */
	D3DXVECTOR3 GetUpVec();
	/*!
	 *@brief	�r���[�s��̐ݒ�B
	 */
	void SetViewMatrix(D3DXMATRIX mView);
	/*!
	 *@brief	�r���[�s��̎擾�B
	 */
	D3DXMATRIX GetViewMatrix();
	/*!
	* @brief	�r���[�s��̋t�s����擾�B
	*/
	D3DXMATRIX GetViewMatrixInv()
	{
		return viewMatrixInv;
	}
	/*!
	* @brief	�J�����̉�]�s����擾�B
	*/
	const D3DXMATRIX& GetCameraRotation() const
	{
		return CameraRot;
	}
	/*!
	* @brief	�v���W�F�N�V�����s����擾�B
	*/
	const D3DXMATRIX& GetProjectionMatrix() const
	{
		return projectionMatrix;
	}
	/*!
	* @brief	��p��ݒ�B
	*/
	void SetViewAngle(float viewAngle)
	{
		m_viewAngle = viewAngle;
	}
	/*!
	* @brief	��p���擾�B
	*/
	float GetViewAngle() const
	{
		return m_viewAngle;
	}
	/*!
	 *@brief	�v���W�F�N�V�����s��̐ݒ�B
	 */
	void SetProjectionMatrix(D3DXMATRIX mProj);
	/*!
	 *@brief	�v���W�F�N�V�����s��̎擾�B
	 */
	D3DXMATRIX GetProjectionMatrix();
	/*!
	 *@brief	�J�����̍X�V�����B
	 */
	void Update();
	/*!
	 *@brief	�J�����̏������B
	 */
	void Init();

};