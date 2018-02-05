#pragma once
#include "myEngine/Physics/SphereCollider.h"


/*!
*@brief	カメラのコリジョンソルバー。
*@details
* 一般的な三人称視点のゲームカメラのコリジョンソルバーを提供します。
*/
class CameraCollisionSolver {
public:
	/*!
	*@brief	コンストラクタ。
	*/
	CameraCollisionSolver();
	/*!
	*@brief	デストラクタ。
	*/
	~CameraCollisionSolver();
	/*!
	*@brief	初期化。
	*@param[in]	radius		カメラのバウンディングスフィアの半径。
	*/
	void Init(float radius);
	/*!
	*@brief	コリジョン解決の実行。
	*@param[out]	result		コリジョン解決を行った結果のカメラの視点の座標が格納される。
	*@param[in]		position	カメラの視点。
	*@param[in]		target		カメラの注視点。
	*@return	コリジョン解決を行った場合はtrueが返ってくる。
	*/
	bool Execute(D3DXVECTOR3& result, const D3DXVECTOR3& position, const D3DXVECTOR3& target);
private:
	SphereCollider		m_collider;		 //コライダー。
	float				m_radius = 0.0f; //半径。
};


