#pragma once
enum RankChecker
{
	RANK_A,
	RANK_B,
	RANK_C,
};

class CScoreCheckre
{
public:
	CScoreCheckre();
	~CScoreCheckre();
	//�X�V
	void Update();
	//�C���X�^�������ێ��Ɉ�x�����Ăяo�����
	void Start();
	//�`��
	void Render(LPD3DXSPRITE spt);
	//�v���C���[�̎c��HP���v�Z���ĕ\�����邽�߂̊֐�
	void DamageCheck();
	//�X�e�[�W�N���A�[���Ԃ��v�Z����֐�
	void TimeCheck();
	//�N���A�[���̕]��������֐�
	void RankCheck();
	//�X�R�A�̌v�Z������֐�
	void SCheck();
	//�G�̋@�̐�������
	void CountUp()
	{
		m_Count++;
	}
	void SetScore()
	{

		m_Score += 50;

	}

private:
	Sprite* m_Number[4];			//���Ԃ�\�����邽�߂̃X�v���C�g
	Sprite* m_Dameg_img[4];			//�c��HP��\�����邽�߂̃X�v���C�g
	Sprite* m_Score_img[3];			//�X�R�A��\�����邽�߂̃X�v���C�g
	Sprite* m_Rank_img;				//�����N��\�����邽�߂̃X�v���C�g
	int		m_Score;				//�X�R�A�̌v�Z�p�̕ϐ�
	int		m_DamageChecker[4];		//�_���[�W�v�Z�p�̕ϐ�
	int		m_TimeChecker = 0;		//�Q�[���I���܂ł̎��Ԍv�Z�ϐ�
	float	m_Time = 0;				//���Ԃ��i�[����ϐ�
	float	m_num = 0;				
	char    TextName[255];			//�e�N�X�`���̖��O�Ŕ��ʂ��邽�߂̕ϐ�
	int		m_Rank;					//�����N�����邽�߂̕ϐ�
	int		m_Count = 0;
	
	
};

