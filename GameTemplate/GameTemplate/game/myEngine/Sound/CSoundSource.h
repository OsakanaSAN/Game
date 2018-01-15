#pragma once
#include "WavFile.h"
#include "CNameKey.h"
//#include "CSoundEngine.h"
#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//�ő�o�̓`�����l�����B

class CSoundSource
{
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	CSoundSource();
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~CSoundSource();
	/*!
	* @brief	�������B
	*@details
	* �I���������Đ������̏������B
	*@param[in]	filePath	�t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wave)
	*@param[in]	is3DSound	3D�T�E���h�H
	*/
	void Init(char* filePath, bool is3DSound = false);
	/*!
	* @brief	�������B
	*@details
	* �I���������Đ������̏������B��������g������������Ƃ��������B
	*@param[in]	nameKey		���O�L�[�B
	*@param[in]	is3DSound	3D�T�E���h�H
	*/
	void Init(const NameKey& nameKey, bool is3DSound = false);
	/*!
	* @brief	�������B
	*@details
	* �X�g���[�~���O�Đ������̏������B
	*@details
	* �����O�o�b�t�@�ɃX�g���[�~���O�ǂݍ��݂��s���āA�Đ����s���Ă����܂��B</br>
	* ��x�ɓǂݍ��܂��f�[�^�̍ő�T�C�Y��bufferingSize�ł��B</br>
	* �ǂݍ��܂ꂽ�f�[�^�̓����O�o�b�t�@�ɃR�s�[����Ă����܂��B</br>
	*@param[in]	filePath		�t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wave)
	*@param[in] is3DSound		3D�T�E���h�H
	*@param[in] ringBufferSize	�����O�o�b�t�@�̃T�C�Y�B(bufferSize�̔{���ɂȂ��Ă���Ɩ��ʂȂ����p�ł��܂��B)
	*@param[in]	bufferSize		�X�g���[�~���O�̍ő�o�b�t�@�����O�T�C�Y�B
	*/
	void InitStreaming(char* filePath, bool is3DSound = false, unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);
	/*!
	* @brief	�J���B
	*@details
	* �f�X�g���N�^���玩���I�ɌĂ΂�܂��B�����I�ɊJ�����s�������ꍇ�Ɏg�p���Ă��������B
	*/
	void Release();

	/*!
	* @brief	�Đ��B
	*@param[in]	isLoop		���[�v�Đ��t���O�B
	*/
	void Play(bool isLoop);
	/*!
	* @brief	��~�B
	*/
	void Stop()
	{
		m_sourceVoice->Stop();
		m_isPlaying = false;
	}
	/*!
	* @brief	�ꎞ��~�B
	*/
	void Pause()
	{
		m_sourceVoice->Stop(/*XAUDIO2_PLAY_TAILS*/);
	}
	/*!
	* @brief	�Đ����H�B
	*/
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/*!
	* @brief	�X�V�B
	*@param[in]	isLoop		���[�v�Đ��t���O�B
	*/
	void Update() ;
	/*!
	* @brief	�{�����[����ݒ�B
	*@param[in]	vol		�{�����[���B
	*/
	void SetVolume(float vol)
	{
		m_sourceVoice->SetVolume(vol);
	}
	/*!
	* @brief	�����̍��W��ݒ�B
	* @details
	*  3D�T�E���h�̎��ɕK�v�ɂȂ�܂��B
	*  2D�T�E���h�ł͖�������܂��B
	*@param[in] pos		�����̍��W�B
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
	* @brief	�����̍��W���擾�B
	*/
	D3DXVECTOR3 GetPosition() const
	{
		return m_position;
	}
	/*!
	* @brief	�����̈ړ����x���擾�B
	*/
	D3DXVECTOR3 GetVelocity() const
	{
		return m_velocity;
	}
	/*!
	* @brief	���[�v�t���O���擾�B
	*/
	bool GetLoopFlag() const
	{
		return m_isLoop;
	}
	/*!
	* @brief	�{�C�X�̎��g��������
	*@details
	* �ڍׂ�IXAudio2SourceVoice��SetFrequencyRatio���Q�Ƃ��Ă��������B
	*@param[in]	ratio		���g����B
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
	//���̓`�����l�������擾�B
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
	//�T�E���h��j�����邽�߂̊֐�
	void SoundDete();
private:
	void InitCommon();
	//�X�g���[�~���O�Đ����̍X�V�����B
	void UpdateStreaming();
	//�I���������Đ����̍X�V�����B
	void UpdateOnMemory();
	void Play(char* buff, unsigned int bufferSize);
	/*!
	* @brief	�X�g���[�~���O�o�b�t�@�����O�̊J�n�B
	*/
	void StartStreamingBuffring();
	void Remove3DSound();
private:
	enum EnStreamingStatus {
		enStreamingBuffering,	//�o�b�t�@�����O���B
		enStreamingQueueing,	//�L���[�C���O���B
	};
	std::shared_ptr<CWaveFile>	m_waveFile;					//!<�g�`�f�[�^�B
	IXAudio2SourceVoice*		m_sourceVoice = nullptr;	//!<�\�[�X�{�C�X�B
	bool						m_isLoop = false;			//!<���[�v�t���O�B
	bool						m_isPlaying = false;		//!<�Đ����t���O�B
	int							m_Number = 0;				//�ԍ�
	bool					m_isStreaming = false;		//!<�X�g���[�~���O�Đ��H
	unsigned int			m_streamingBufferSize = 0;	//!<�X�g���[�~���O�p�̃o�b�t�@�����O�T�C�Y�B
	unsigned int			m_currentBufferingSize = 0;	//!<���݂̃o�b�t�@�����O�̃T�C�Y�B
	unsigned int			m_readStartPos = 0;			//!<�ǂݍ��݊J�n�ʒu�B
	unsigned int			m_ringBufferSize = 0;		//!<�����O�o�b�t�@�̃T�C�Y�B
	EnStreamingStatus		m_streamingState = enStreamingBuffering;	//!<�X�g���[�~���O�X�e�[�^�X�B
	bool					m_is3DSound = false;		//!<3D�T�E���h�H
	D3DXVECTOR3				m_position = {0,0,0};	//!<�����̍��W�B3D�T�E���h�̎��ɕK�v�B
	D3DXVECTOR3				m_lastFramePosition = { 0,0,0 };//!<������1�t���[���O�̍��W�B3D�T�E���h�̎��ɕK�v�B
	D3DXVECTOR3				m_velocity = { 0,0,0 };	//!<���x�B3D�T�E���h�̎��ɕK�v�E
	FLOAT32 m_emitterAzimuths[INPUTCHANNELS];
	FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	X3DAUDIO_DSP_SETTINGS m_dspSettings;
	bool m_isSetPositionFirst = true;	//!<��ԍŏ���setPosition?
};

