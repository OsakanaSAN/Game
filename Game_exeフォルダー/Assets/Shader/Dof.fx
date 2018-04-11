float g_Near;                 //ブラーが開始される近距離。この距離より手前にあるオブジェクトにはブラーが適応されない。
float g_Far;                  //最も強くブラーがかかる遠距離。この距離より奥にあるオブジェクトは強いブラーが適応される。
float m_Len;                  //m_Far - m_Near

sampler tex0 : register(s0);  //バックバッファのそのままのテクスチャー
sampler tex1 : register(s1);  //バックバッファのZ値
sampler tex2 : register(s2);  //バックバッファをぼかしたテクスチャ
sampler tex3 : register(s3);  //さらにぼかしたテクスチャー

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
   //バックバッファのZ値情報
   float z = tex2D( tex1, In.Tex ).r;

   float4 Out;
   float m_Len = g_Far - g_Near;
   
   //遠距離
   if( m_Far <= z )
      Out = tex2D( tex3, In.Tex );
   
   //近距離
   else if( m_Near >= z )
      Out = tex2D( tex0, In.Tex );
   
   //中間
   else
   {  
      //z値 が0.0f ～ 1.0f の範囲内での値に変換する。
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