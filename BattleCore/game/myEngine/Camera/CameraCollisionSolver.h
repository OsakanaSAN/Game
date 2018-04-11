#pragma once
#include "myEngine/Physics/SphereCollider.h"


/*!
*@brief	�J�����̃R���W�����\���o�[�B
*@details
* ��ʓI�ȎO�l�̎��_�̃Q�[���J�����̃R���W�����\���o�[��񋟂��܂��B
*/
class CameraCollisionSolver {
public:
	/*!
	*@brief	�R���X�g���N�^�B
	*/
	CameraCollisionSolver();
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~CameraCollisionSolver();
	/*!
	*@brief	�������B
	*@param[in]	radius		�J�����̃o�E���f�B���O�X�t�B�A�̔��a�B
	*/
	void Init(float radius);
	/*!
	*@brief	�R���W���������̎��s�B
	*@param[out]	result		�R���W�����������s�������ʂ̃J�����̎��_�̍��W���i�[�����B
	*@param[in]		position	�J�����̎��_�B
	*@param[in]		target		�J�����̒����_�B
	*@return	�R���W�����������s�����ꍇ��true���Ԃ��Ă���B
	*/
	bool Execute(D3DXVECTOR3& result, const D3DXVECTOR3& position, const D3DXVECTOR3& target);
private:
	SphereCollider		m_collider;		 //�R���C�_�[�B
	float				m_radius = 0.0f; //���a�B
};


