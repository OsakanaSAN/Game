#pragma once

/*!
 *@brief	カメラクラス。
 */
class Camera {
private:
	D3DXMATRIX				viewMatrix;			//!<ビュー行列。カメラ行列とも言う。
	D3DXMATRIX              viewMatrixInv;      //ビュー行列の逆行列
	D3DXMATRIX				viewProjectionMatrix;//!<ビュープロジェクション行列。
	D3DXMATRIX              CameraRot;          //!<カメラの回転行列。
	D3DXMATRIX				projectionMatrix;	//!<プロジェクション行列。ビュー空間から射影空間に変換する行列。
	D3DXVECTOR3				vEyePt;				//!<カメラの視点。
	D3DXVECTOR3				vLookatPt;			//!<カメラの注視点。
	D3DXVECTOR3				vUpVec;				//!<カメラの上方向。
	float					Far;				//!<遠平面。
	float					Near;				//!<近平面。
	float			        m_viewAngle;		//!<画角(ラジアン)。
	float					aspect;				//!<アスペクト比
	bool		            m_isNeedUpdateProjectionMatrix;

public:
	/*!
	 *@brief	コンストラクタ。
	 */
	Camera();
	/*!
	 *@brief	デストラクタ
	 */
	~Camera();
	/*!
	 *@brief	アスペクト比を設定。
	 */
	void SetAspect(float _aspect);
	/*!
	 *@brief	アスペクト比を取得。
	 */
	float GetAspect();
	/*!
	 *@brief	ファーを設定。
	 */
	void SetFar(float _far);
	
	/*!
	 *@brief	ニアを取得。
	 */
	float GetNear();
	/*!
	 *@brief	ファーを取得。
	 */
	float GetFar();
	/*!
	 *@brief	ニアを設定。
	 */
	void SetNear(float _near);
	/*!
	 *@brief	視点を設定。
	 */
	void SetEyePt(const D3DXVECTOR3 pt);
	/*!
	 *@brief	視点を取得。
	 */
	D3DXVECTOR3 GetEyePt();
	/*!
	 *@brief	注視点を設定。
	 */
	void SetLookatPt(const D3DXVECTOR3 pt);
	/*!
	 *@brief	注視点の取得。
	 */
	D3DXVECTOR3 GetLookatPt();
	/*!
	 *@brief	上方向の設定。
	 */
	void SetUpVec(D3DXVECTOR3 up);
	/*!
	 *@brief	上方向の取得。
	 */
	D3DXVECTOR3 GetUpVec();
	/*!
	 *@brief	ビュー行列の設定。
	 */
	void SetViewMatrix(D3DXMATRIX mView);
	/*!
	 *@brief	ビュー行列の取得。
	 */
	D3DXMATRIX GetViewMatrix();
	/*!
	* @brief	ビュー行列の逆行列を取得。
	*/
	D3DXMATRIX GetViewMatrixInv()
	{
		return viewMatrixInv;
	}
	/*!
	* @brief	カメラの回転行列を取得。
	*/
	const D3DXMATRIX& GetCameraRotation() const
	{
		return CameraRot;
	}
	/*!
	* @brief	プロジェクション行列を取得。
	*/
	const D3DXMATRIX& GetProjectionMatrix() const
	{
		return projectionMatrix;
	}
	/*!
	* @brief	画角を設定。
	*/
	void SetViewAngle(float viewAngle)
	{
		m_viewAngle = viewAngle;
	}
	/*!
	* @brief	画角を取得。
	*/
	float GetViewAngle() const
	{
		return m_viewAngle;
	}
	/*!
	 *@brief	プロジェクション行列の設定。
	 */
	void SetProjectionMatrix(D3DXMATRIX mProj);
	/*!
	 *@brief	プロジェクション行列の取得。
	 */
	D3DXMATRIX GetProjectionMatrix();
	/*!
	 *@brief	カメラの更新処理。
	 */
	void Update();
	/*!
	 *@brief	カメラの初期化。
	 */
	void Init();

};