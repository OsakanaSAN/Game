float2 m_CenterTexel;       //�u���[�̒��S�̃e�N�Z�����W
float  m_BlurPower;         //�ڂ����x(0.0f �Ń{�P�Ȃ�)
float m_TU;                 //X�����ׂ̗̃e�N�Z���ʒu
float m_TV;                 //Y�����ׂ̗̃e�N�Z���ʒu

sampler s0 : register(s0);  //�o�b�N�o�b�t�@�̃C���[�W

struct VS_OUTPUT
{
   float4 Pos : POSITION;
   float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS( float4 Pos : POSITION,
              float2 Tex : TEXCOORD0 )
{
   VS_OUTPUT Out;

   Out.Pos = Pos;
   Out.Tex = Tex;

   return Out;
}

float4 PS( VS_OUTPUT In ) : COLOR0
{
   float4 Color[10];
   
   //�u���[�̒��S�ʒu �� ���݂̃e�N�Z���ʒu
   float2 dir = m_CenterTexel - In.Tex;
   
   //�������v�Z����
   float len = length( dir );
   
   //�����x�N�g���̐��K�����A�P�e�N�Z�����̒����ƂȂ�����x�N�g�����v�Z����
   dir = normalize( dir ) * float2( m_TU, m_TV );
   
   //m_BlurPower �Ń{�P��𒲐�����
   //������ώZ���邱�Ƃɂ��A�����̒��S�ʒu�ɋ߂��قǃu���[�̉e�����������Ȃ�悤�ɂ���
   dir *= m_BlurPower * len;

   //��������
   Color[0]  = tex2D( s0, In.Tex              ) * 0.19f;
   Color[1]  = tex2D( s0, In.Tex + dir        ) * 0.17f;
   Color[2]  = tex2D( s0, In.Tex + dir * 2.0f ) * 0.15f;
   Color[3]  = tex2D( s0, In.Tex + dir * 3.0f ) * 0.13f;
   Color[4]  = tex2D( s0, In.Tex + dir * 4.0f ) * 0.11f;
   Color[5]  = tex2D( s0, In.Tex + dir * 5.0f ) * 0.09f;
   Color[6]  = tex2D( s0, In.Tex + dir * 6.0f ) * 0.07f;
   Color[7]  = tex2D( s0, In.Tex + dir * 7.0f ) * 0.05f;
   Color[8]  = tex2D( s0, In.Tex + dir * 8.0f ) * 0.03f;
   Color[9]  = tex2D( s0, In.Tex + dir * 9.0f ) * 0.01f;   
   
   return Color[0] + Color[1] + Color[2] + Color[3] + Color[4] + Color[5] + Color[6] + Color[7] + Color[8] + Color[9];
}

technique ShockBluer
{
   pass P0
   {
      VertexShader = compile vs_1_1 VS();
      PixelShader  = compile ps_2_0 PS();
   }
}