


texture g_scene;	//シーンテクスチャ。
sampler g_blurSampler = 
sampler_state
{
    Texture = <g_scene>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

struct VS_INPUT
{
	float4 		pos : POSITION;
};
struct VS_OUTPUT
{
   float4 pos       : POSITION;
   float2 tex0	: TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
};

float2 g_luminanceTexSize;		//輝度テクスチャのサイズ。
float2 g_offset;				//オフセット
float2 g_renderTargetSize;		//レンダリングターゲットのサイズ。
float  g_weight[8];				//ガウスフィルタの重み。

//Xブラー
VS_OUTPUT VSXBlur(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = In.pos;
	float2 tex = (In.pos * 0.5f) + 0.5f;;
	tex.y = 1.0f - tex.y;
	tex += float2( 0.5 / g_renderTargetSize.x, 0.5 / g_renderTargetSize.y);
	Out.tex0 = tex + float2( - 1.0f / g_luminanceTexSize.x, 0.0f );
    Out.tex1 = tex + float2( - 3.0f / g_luminanceTexSize.x, 0.0f );
    Out.tex2 = tex + float2( - 5.0f / g_luminanceTexSize.x, 0.0f );
    Out.tex3 = tex + float2( - 7.0f / g_luminanceTexSize.x, 0.0f );
    Out.tex4 = tex + float2( - 9.0f / g_luminanceTexSize.x, 0.0f );
    Out.tex5 = tex + float2( -11.0f / g_luminanceTexSize.x, 0.0f );
    Out.tex6 = tex + float2( -13.0f / g_luminanceTexSize.x, 0.0f );
    Out.tex7 = tex + float2( -15.0f / g_luminanceTexSize.x, 0.0f );
    return Out;
}

float4 PSXBlur( VS_OUTPUT In ) : COLOR
{
	float4 Color;
	Color  = g_weight[0] * (tex2D( g_blurSampler, In.tex0 )
	                 + tex2D( g_blurSampler, In.tex7 + g_offset ));
	Color += g_weight[1] * (tex2D( g_blurSampler, In.tex1 )
	                 + tex2D( g_blurSampler, In.tex6 + g_offset ));
	Color += g_weight[2] * (tex2D( g_blurSampler, In.tex2 )
	            + tex2D( g_blurSampler, In.tex5 + g_offset ));
	Color += g_weight[3] * (tex2D( g_blurSampler, In.tex3 )
	                 + tex2D( g_blurSampler, In.tex4 + g_offset ));
	Color += g_weight[4] * (tex2D( g_blurSampler, In.tex4 )
	                 + tex2D( g_blurSampler, In.tex3 + g_offset ));
	Color += g_weight[5] * (tex2D( g_blurSampler, In.tex5 )
	                 + tex2D( g_blurSampler, In.tex2 + g_offset ));
	Color += g_weight[6] * (tex2D( g_blurSampler, In.tex6 )
	                 + tex2D( g_blurSampler, In.tex1 + g_offset ));
	Color += g_weight[7] * (tex2D( g_blurSampler, In.tex7 )
	                 + tex2D( g_blurSampler, In.tex0 + g_offset ));
	return Color;
}

/*!
 * @brief	横ブラー
 */
technique XBlur
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSXBlur();
		PixelShader = compile ps_3_0 PSXBlur();
	}
}