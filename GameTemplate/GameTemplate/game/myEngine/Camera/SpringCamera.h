#pragma once

/*!
* @brief	バネカメラ。
* @details
*  バネに引っ張られるようなカメラの挙動を実現するクラスです。</br>
*  SetTarTarget関数とSetTarPosition関数を使用して</br>
*  カメラの目標となる注視点と視点を設定してください。</br>
*  Update関数を実行することで、カメラが設定された目標座標に追従していきます。</br>
* @code
*		CSpringCamera springCamera;
//初期化。
void Init()
{
springCamera.Init(
CVector3(0.0f, 0.0f, 0.0f),
CVector3(0.0f, 0.0f, 100.0f),
100.0f
);
}
//更新。
void Update()
{
・
・
・
//カメラの注視点を設定する。
springCamera.SetTarTarget(target);
springCamera.SetTarPosition(position);

//カメラを動かす。
springCamera.Update();
}
* @endcode
*/
class SpringCamera {
public:
	/*!
	* @brief	コンストラクタ。
	*/
	SpringCamera();
	/*!
	* @brief	デストラクタ
	*/
	~SpringCamera();
	/*!
	* @brief	初期化。
	*/
	void Init(const D3DXVECTOR3& target, const D3DXVECTOR3& position, float maxMoveSpeed);
	/*!
	* @brief	目標となる注視点を設定。
	*/
	void SetTarTarget(const D3DXVECTOR3& target)
	{
		m_target = target;
	}
	/*!
	* @brief	目標となる視点を設定。
	*/
	void SetTarPosition(const D3DXVECTOR3 position)
	{
		m_position = position;
	}
	/*!
	* @brief	注視点を設定。
	*/
	void SetTarget(const D3DXVECTOR3& target)
	{
		m_camera.SetLookatPt(target);
	}
	/*!
	* @brief	視点を設定。
	*/
	void SetPosition(const D3DXVECTOR3 position)
	{
		m_camera.SetEyePt(position);
	}
	/*!
	* @brief	遠平面を設定。
	*/
	void SetFar(float _far)
	{
		m_camera.SetFar(_far);
	}
	/*!
	* @brief	カメラの回転行列を取得。
	*/
	const D3DXMATRIX& GetCameraRotation() const
	{
		return m_camera.GetCameraRotation();
	}
	/*!
	* @brief	注視点を取得。
	*/
	const D3DXVECTOR3& GetTarget()
	{
		return m_camera.GetLookatPt();

	}
	/*!
	* @brief	視点を取得。
	*/
	const D3DXVECTOR3& GetPosition() 
	{
		return m_camera.GetEyePt();
	}

	/*!
	* @brief	更新。
	*/
	void Update();
	/*!
	* @brief	カメラを取得。
	*/
	const Camera* GetCamera() const
	{
		return &m_camera;
	}
	/*!
	* @brief	バネの減衰率を設定。
	*@detail
	* 値が大きいほどカメラが遅れて付いてきます。
	*/
	void SetDampingRate(float dampingRate)
	{
		m_targetDampingRate = dampingRate;
	}
	/*!
	* @brief	カメラを取得。
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
	* @brief	目標となる注視点を取得。
	*/
	const D3DXVECTOR3& GetTarTarget() const
	{
		return m_target;
	}
	/*!
	* @brief	目標となる視点を取得。
	*/
	const D3DXVECTOR3& GetTarPosition() const
	{
		return m_position;
	}
	/*!
	* @brief	バネカメラ用のパラメータをクリア。
	*/
	void ClearSpringParame()
	{
		m_targetMoveSpeed = {0.0f,0.0f,0.0f};
		m_positionMoveSpeed = { 0.0f,0.0f,0.0f };

	}
	/*!
	* @brief	バネカメラの更新処理。
	* @details
	*  Update関数から呼ばれています。</br>
	*  バネカメラの更新処理だけ個別で呼びたいなどの特殊なケースで使用してください。</br>
	*/
	void UpdateSpringCamera();
	/*!
	* @brief	カメラの更新。
	* @details
	*  Update関数から呼ばれています。</br>
	*  カメラの更新処理だけ個別で呼びたいなどの特殊なケースで使用してください。</br>
	*/
	void UpdateCamera()
	{
		m_camera.Update();
	}
	//最高速度セット
	void SetMaxMoveSpeed(float speed)
	{
		m_maxMoveSpeed = speed;

	}
	//注視点の移動速度セット
	void SetTagPosMoveSpeed(D3DVECTOR speed)
	{
		m_targetMoveSpeed = speed;

	}
private:
	Camera		m_camera;						//!<カメラ。
	D3DXVECTOR3	m_target = { 0.0f,0.0f,0.0f };		//!<目標となる注視点。
	D3DXVECTOR3	m_position = { 0.0f,0.0f,0.0f };	//!<目標となる視点。
	D3DXVECTOR3	m_targetMoveSpeed = { 0.0f,0.0f,0.0f };		//!<注視点の移動速度。
	D3DXVECTOR3	m_positionMoveSpeed = { 0.0f,0.0f,0.0f };	//!<視点の移動速度。

	float		m_maxMoveSpeed = 0.0f;					//!<最高移動速度。
	float		m_targetDampingRate = 5.0f;				//!<減衰率。値が大きいほどカメラが遅れ付いてくる。
	float		m_dampingRate = 2.3f;					//!<減衰率。
	float		m_dampingRateVel = 5.0f;
};

