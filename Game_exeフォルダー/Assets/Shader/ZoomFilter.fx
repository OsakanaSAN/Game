/*!
 * @brief	ZoomFilter�B
 */

float  	g_BlurPower;         //�ڂ����x(0.0f �Ń{�P�Ȃ�)
float 	g_TU;                 //X�����ׂ̗̃e�N�Z���ʒu
float 	g_TV;                 //Y�����ׂ̗̃e�N�Z���ʒu

struct VS_INPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};
texture g_scene;
sampler SceneSampler = sampler_state
{
	Texture = <g_scene>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
    AddressV = CLAMP;
};
float g_blendRate;		//�u�����h���B
/*!
 *@brief	���_�V�F�[�_�[�B
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = In.pos;
	Out.uv = In.uv;
	return Out;
}


/*!
 *@brief	�s�N�Z���V�F�[�_�[�B
 */
float4 PSMain( VS_OUTPUT In ) : COLOR0
{
	//�u���[�̒��S��ݒ�
	float2 m_CenterTexel = float2(0.5f,0.5f);
	float2 Tuv = float2(1.0f/1280.0f,1.0f/720.0f);
	float4 Color[10];
	float2 dir =  m_CenterTexel - In.uv;
	
	//�������v�Z����
   	float len = length( dir );
   	//�����x�N�g���̐��K�����A�P�e�N�Z�����̒����ƂȂ�����x�N�g�����v�Z����
   dir = normalize( dir ) * float2( Tuv.x, Tuv.y );
   
   //m_BlurPower �Ń{�P��𒲐�����
   //������ώZ���邱�Ƃɂ��A�����̒��S�ʒu�ɋ߂��قǃu���[�̉e�����������Ȃ�悤�ɂ���
   dir *= g_BlurPower * len;
   
   //��������
   Color[0]  = tex2D(SceneSampler, In.uv) * 0.19f;
   Color[1]  = tex2D( SceneSampler, In.uv + dir        ) * 0.17f;
   Color[2]  = tex2D( SceneSampler, In.uv + dir * 2.0f ) * 0.15f;
   Color[3]  = tex2D( SceneSampler, In.uv + dir * 3.0f ) * 0.13f;
   Color[4]  = tex2D( SceneSampler, In.uv + dir * 4.0f ) * 0.11f;
   Color[5]  = tex2D( SceneSampler, In.uv + dir * 5.0f ) * 0.09f;
   Color[6]  = tex2D( SceneSampler, In.uv + dir * 6.0f ) * 0.07f;
   Color[7]  = tex2D( SceneSampler, In.uv + dir * 7.0f ) * 0.05f;
   Color[8]  = tex2D( SceneSampler, In.uv + dir * 8.0f ) * 0.03f;
   Color[9]  = tex2D( SceneSampler, In.uv + dir * 9.0f ) * 0.01f;   
   
   return Color[0] + Color[1] + Color[2] + Color[3] + Color[4] + Color[5] + Color[6] + Color[7] + Color[8] + Color[9];
   
   
}

technique Default
{
	pass p0
	{
		VertexShader	= compile vs_3_0 VSMain();
		PixelShader		= compile ps_3_0 PSMain();
	}
}