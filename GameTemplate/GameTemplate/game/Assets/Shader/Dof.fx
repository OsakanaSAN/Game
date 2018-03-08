float g_Near;                 //�u���[���J�n�����ߋ����B���̋�������O�ɂ���I�u�W�F�N�g�ɂ̓u���[���K������Ȃ��B
float g_Far;                  //�ł������u���[�������鉓�����B���̋�����艜�ɂ���I�u�W�F�N�g�͋����u���[���K�������B
float m_Len;                  //m_Far - m_Near

sampler tex0 : register(s0);  //�o�b�N�o�b�t�@�̂��̂܂܂̃e�N�X�`���[
sampler tex1 : register(s1);  //�o�b�N�o�b�t�@��Z�l
sampler tex2 : register(s2);  //�o�b�N�o�b�t�@���ڂ������e�N�X�`��
sampler tex3 : register(s3);  //����ɂڂ������e�N�X�`���[

struct VS_OUTPUT
{
   float4 Pos   : POSITION;
   float2 Tex   : TEXCOORD0;
};

VS_OUTPUT VS( float4 Pos     : POSITION,
              float2 Tex     : TEXCOORD0 )
{
   VS_OUTPUT Out;
   
   Out.Pos    = Pos;
   Out.Tex    = Tex;
      
   return Out;
}

float4 PS( VS_OUTPUT In ) : COLOR
{
   //�o�b�N�o�b�t�@��Z�l���
   float z = tex2D( tex1, In.Tex ).r;

   float4 Out;
   float m_Len = g_Far - g_Near;
   
   //������
   if( m_Far <= z )
      Out = tex2D( tex3, In.Tex );
   
   //�ߋ���
   else if( m_Near >= z )
      Out = tex2D( tex0, In.Tex );
   
   //����
   else
   {  
      //z�l ��0.0f �` 1.0f �͈͓̔��ł̒l�ɕϊ�����B
      float ConvZ = ( z - g_Near ) / m_Len;
      
      if( ConvZ < 0.5f )
      {
         ConvZ*=2.0f;
         Out = tex2D( tex0, In.Tex ) * ( 1.0f - ConvZ ) + tex2D( tex2, In.Tex ) * ConvZ;
      }
      
      else
      {
         ConvZ-=0.5f;
         ConvZ*=2.0f;
         Out = tex2D( tex2, In.Tex ) * ( 1.0f - ConvZ ) + tex2D( tex3, In.Tex ) * ConvZ;
      }      
   }
   
   return Out;    
}

technique Default
{
   pass P0
   {            
      VertexShader = compile vs_1_1 VS();
      PixelShader  = compile ps_2_0 PS();
   }   
}