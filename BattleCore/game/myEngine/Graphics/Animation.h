/*!
 * @brief	アニメーションクラス。
 */
 
#pragma once;
#include <deque>

/*!
 * @brief	アニメーションクラス。
 */
class Animation{
public:
	/*!
	 * @brief	コンストラクタ
	 */
	Animation() :
		pAnimController(nullptr),
		numAnimSet(0),
		isBlending(false),
		isInterpolate(false),
		numMaxTracks(0),
		interpolateTime(0.0f),
		interpolateEndTime(0.0f)
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	~Animation()
	{
	}
	/*!
	 * @brief	初期化。
	 *@param[in]	anim		アニメーションコントローラー。
	 */
	void Init(ID3DXAnimationController* anim);
	/*!
	* @brief	アニメーションの終了タイムを設定する。
	*@param[in]	animationSetIndex		アニメーションインデックス。
	*@param[in] endTime					アニメーションの終了タイム。-1.0を設定するとアニメーションファイルに設定されている終了タイムになる。
	*/
	void SetAnimationEndTime(int animationSetIndex, double endTime)
	{
		animationEndTime[animationSetIndex] = endTime;
	}
	void SetAnimetionLoopFlag(int animationSetIndex, bool loopFlag)
	{
		if (animationSetIndex < numAnimSet) {
			animationLoopFlags[animationSetIndex] = loopFlag;
		}
	}
	/*!
	*@brief	アニメーションの再生。
	*@param[in]		animationIndex		再生したいアニメーションのインデックス。
	*/
	void PlayAnimation(int animationSetIndex);
	/*!
	*@brief	アニメーションの再生。アニメーションの補完が行われます。
	*@param[in]		animationSetIndex	再生したいアニメーションのインデックス。
	*@param[in]		interpolateTime		補間時間。
	*/
	void PlayAnimation(int animationSetIndex, float interpolateTime);

	void PlayAnimationQueue(int animationSetIndex, float interpolateTime)
	{
		RequestPlayAnimation req;
		req.animationSetIndex = animationSetIndex;
		req.interpolateTime = interpolateTime;
		playAnimationRequest.push_back(req);
}

	//アニメーションスピードの設定
	void SetAnimetionSpeed(float SAS)
	{
		animationSpeed = SAS;
	}
	/*!
	*@brief	アニメーションの再生中判定。
	*/
	bool IsPlay() const
	{
		return !isAnimEnd;

    }
	int GetAnimNo() const
	{
		return currentAnimationSetNo;
	}
#if 0
	/*!
	*@brief	アニメーションのブレンディング再生。
	*@param		animationIndex		再生したいアニメーションのインデックス。
	*/
	void BlendAnimation(int animationSetIndex);
#endif
	/*!
	*@brief	アニメーションセットの取得。
	*/
	int GetNumAnimationSet() const
	{
		return numAnimSet;
	}
	/*!
	*@brief	補間時間を元にトラックの重みを更新。
	*/
	void UpdateTrackWeights();
	/*!
	*@brief	アニメーションの更新。
	*@param[in]		deltaTime	更新時間。単位は秒。
	*/
	void Update( float deltaTime );
	void PopRequestPlayeAnimation();
private:
	struct RequestPlayAnimation {
			int animationSetIndex;
			float interpolateTime;
		};
	ID3DXAnimationController*				pAnimController;		//!<アニメーションコントローラ。
	int										numAnimSet;				//!<アニメーションセットの数。
	std::unique_ptr<ID3DXAnimationSet*[]>	animationSets;			//!<アニメーションセットの配列。
	std::unique_ptr<float[]>				blendRateTable;			//!<ブレンディングレートのテーブル。
	std::unique_ptr<double[]>				animationEndTime;		//!<アニメーションの終了タイム。デフォルトは-1.0が入っていて、-1.0が入っている場合はID3DXAnimationSetのアニメーション終了タイムが優先される。
	std::unique_ptr<bool[]>					animationLoopFlags;		//!<DirectX9のアニメーションセットに１秒以下のアニメーションを入れる方法が見つからない。1秒以下のアニメーションはこいつを適時設定。
	double									localAnimationTime;		//!<ローカルアニメーションタイム。
	int										currentAnimationSetNo;	//!<現在再生中のアニメーショントラックの番号。
	int										currentTrackNo;			//!<現在のトラックの番号。
	int										numMaxTracks;			//!<アニメーショントラックの最大数。
	bool									isBlending;				//!<アニメーションブレンディング中？
	bool									isInterpolate;			//!<補間中？
	bool									isAnimEnd;				//!<アニメーションの終了フラグ。
	float									interpolateEndTime;		//!<補間終了時間。
	float									interpolateTime;		//!<補間時間。
	float									animationSpeed = 1.0f;  //アニメーションスピード
	std::deque<RequestPlayAnimation>		playAnimationRequest;	//!<アニメーション再生のリクエスト。

};