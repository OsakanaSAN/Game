float2 m_CenterTexel;       //ブラーの中心のテクセル座標
float  m_BlurPower;         //ぼけ強度(0.0f でボケなし)
float m_TU;                 //X方向の隣のテクセル位置
float m_TV;                 //Y方向の隣のテクセル位置

sampler s0 : register(s0);  //バックバッファのイメージ

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
   
   //ブラーの中心位置 ← 現在のテクセル位置
   float2 dir = m_CenterTexel - In.Tex;
   
   //距離を計算する
   float len = length( dir );
   
   //方向ベクトルの正規化し、１テクセル分の長さとなる方向ベクトルを計算する
   dir = normalize( dir ) * float2( m_TU, m_TV );
   
   //m_BlurPower でボケ具合を調整する
   //距離を積算することにより、爆発の中心位置に近いほどブラーの影響が小さくなるようにする
   dir *= m_BlurPower * len;

   //合成する
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