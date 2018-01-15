#pragma once
#include "WavFile.h"
#include "CNameKey.h"
//#include "CSoundEngine.h"
#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//最大出力チャンネル数。

class CSoundSource
{
public:
	/*!
	* @brief	コンストラクタ。
	*/
	CSoundSource();
	/*!
	* @brief	デストラクタ。
	*/
	~CSoundSource();
	/*!
	* @brief	初期化。
	*@details
	* オンメモリ再生向けの初期化。
	*@param[in]	filePath	ファイルパス。対応しているファイルフォーマット(*.wave)
	*@param[in]	is3DSound	3Dサウンド？
	*/
	void Init(char* filePath, bool is3DSound = false);
	/*!
	* @brief	初期化。
	*@details
	* オンメモリ再生向けの初期化。こちらを使う方がちょっとだけ速い。
	*@param[in]	nameKey		名前キー。
	*@param[in]	is3DSound	3Dサウンド？
	*/
	void Init(const NameKey& nameKey, bool is3DSound = false);
	/*!
	* @brief	初期化。
	*@details
	* ストリーミング再生向けの初期化。
	*@details
	* リングバッファにストリーミング読み込みを行って、再生を行っていきます。</br>
	* 一度に読み込まれるデータの最大サイズはbufferingSizeです。</br>
	* 読み込まれたデータはリングバッファにコピーされていきます。</br>
	*@param[in]	filePath		ファイルパス。対応しているファイルフォーマット(*.wave)
	*@param[in] is3DSound		3Dサウンド？
	*@param[in] ringBufferSize	リングバッファのサイズ。(bufferSizeの倍数になっていると無駄なく活用できます。)
	*@param[in]	bufferSize		ストリーミングの最大バッファリングサイズ。
	*/
	void InitStreaming(char* filePath, bool is3DSound = false, unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);
	/*!
	* @brief	開放。
	*@details
	* デストラクタから自動的に呼ばれます。明示的に開放を行いたい場合に使用してください。
	*/
	void Release();

	/*!
	* @brief	再生。
	*@param[in]	isLoop		ループ再生フラグ。
	*/
	void Play(bool isLoop);
	/*!
	* @brief	停止。
	*/
	void Stop()
	{
		m_sourceVoice->Stop();
		m_isPlaying = false;
	}
	/*!
	* @brief	一時停止。
	*/
	void Pause()
	{
		m_sourceVoice->Stop(/*XAUDIO2_PLAY_TAILS*/);
	}
	/*!
	* @brief	再生中？。
	*/
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/*!
	* @brief	更新。
	*@param[in]	isLoop		ループ再生フラグ。
	*/
	void Update() ;
	/*!
	* @brief	ボリュームを設定。
	*@param[in]	vol		ボリューム。
	*/
	void SetVolume(float vol)
	{
		m_sourceVoice->SetVolume(vol);
	}
	/*!
	* @brief	音源の座標を設定。
	* @details
	*  3Dサウンドの時に必要になります。
	*  2Dサウンドでは無視されます。
	*@param[in] pos		音源の座標。
	*/
	void SetPosition(const D3DXVECTOR3& pos)
	{
		m_position = pos;
		if (m_isSetPositionFirst) {
			m_lastFramePosition = m_position;
			m_isSetPositionFirst = false;
		}
	}
	/*!
	* @brief	音源の座標を取得。
	*/
	D3DXVECTOR3 GetPosition() const
	{
		return m_position;
	}
	/*!
	* @brief	音源の移動速度を取得。
	*/
	D3DXVECTOR3 GetVelocity() const
	{
		return m_velocity;
	}
	/*!
	* @brief	ループフラグを取得。
	*/
	bool GetLoopFlag() const
	{
		return m_isLoop;
	}
	/*!
	* @brief	ボイスの周波数調整比
	*@details
	* 詳細はIXAudio2SourceVoiceのSetFrequencyRatioを参照してください。
	*@param[in]	ratio		周波数比。
	*/
	void SetFrequencyRatio(float ratio)
	{
		if (m_sourceVoice != nullptr) {
			m_sourceVoice->SetFrequencyRatio(ratio);
		}
	}
	IXAudio2SourceVoice* GetXAudio2SourceVoice()
	{
		return m_sourceVoice;
	}
	//入力チャンネル数を取得。
	int GetNumInputChannel()const
	{
		return m_waveFile->GetFormat()->nChannels;
	}
	FLOAT32* GetEmitterAzimuths()
	{
		return m_emitterAzimuths;
	}
	FLOAT32* GetMatrixCoefficients()
	{
		return m_matrixCoefficients;
	}
	X3DAUDIO_DSP_SETTINGS* GetDspSettings()
	{
		return &m_dspSettings;
	}
	//サウンドを破棄するための関数
	void SoundDete();
private:
	void InitCommon();
	//ストリーミング再生中の更新処理。
	void UpdateStreaming();
	//オンメモリ再生中の更新処理。
	void UpdateOnMemory();
	void Play(char* buff, unsigned int bufferSize);
	/*!
	* @brief	ストリーミングバッファリングの開始。
	*/
	void StartStreamingBuffring();
	void Remove3DSound();
private:
	enum EnStreamingStatus {
		enStreamingBuffering,	//バッファリング中。
		enStreamingQueueing,	//キューイング中。
	};
	std::shared_ptr<CWaveFile>	m_waveFile;					//!<波形データ。
	IXAudio2SourceVoice*		m_sourceVoice = nullptr;	//!<ソースボイス。
	bool						m_isLoop = false;			//!<ループフラグ。
	bool						m_isPlaying = false;		//!<再生中フラグ。
	int							m_Number = 0;				//番号
	bool					m_isStreaming = false;		//!<ストリーミング再生？
	unsigned int			m_streamingBufferSize = 0;	//!<ストリーミング用のバッファリングサイズ。
	unsigned int			m_currentBufferingSize = 0;	//!<現在のバッファリングのサイズ。
	unsigned int			m_readStartPos = 0;			//!<読み込み開始位置。
	unsigned int			m_ringBufferSize = 0;		//!<リングバッファのサイズ。
	EnStreamingStatus		m_streamingState = enStreamingBuffering;	//!<ストリーミングステータス。
	bool					m_is3DSound = false;		//!<3Dサウンド？
	D3DXVECTOR3				m_position = {0,0,0};	//!<音源の座標。3Dサウンドの時に必要。
	D3DXVECTOR3				m_lastFramePosition = { 0,0,0 };//!<音源の1フレーム前の座標。3Dサウンドの時に必要。
	D3DXVECTOR3				m_velocity = { 0,0,0 };	//!<速度。3Dサウンドの時に必要・
	FLOAT32 m_emitterAzimuths[INPUTCHANNELS];
	FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	X3DAUDIO_DSP_SETTINGS m_dspSettings;
	bool m_isSetPositionFirst = true;	//!<一番最初のsetPosition?
};

