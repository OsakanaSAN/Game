/*!
 * @brief	モノクロフィルター。
 */


struct VS_INPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

float g_sceneTexSizeX;	//シーンテクスチャのサイズ。
float g_sceneTexSizeY;
float g_AlphaPower;
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
float g_blendRate;		//ブレンド率。
/*!
 *@brief	頂点シェーダー。
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = In.pos;
	Out.uv = In.uv + float2( 0.5/g_sceneTexSizeX, 0.5/g_sceneTexSizeY);
	return Out;
}


/*!
 *@brief	ピクセルシェーダー。
 */
float4 PSMain( VS_OUTPUT In ) : COLOR0
{

	float4 color = tex2D(SceneSampler, In.uv);
	float4 MonoColor = float4(1.0f,1.0f,1.0f,1.0f);
	MonoColor.rgb = (0.299f * color.r +  0.587f * color.g + 0.114 * color.b);
	color.rgb = color.rgb * g_AlphaPower + MonoColor.rgb * (1.0f - g_AlphaPower);
	return color;
	
   
   
}

technique Default
{
	pass p0
	{
		VertexShader	= compile vs_3_0 VSMain();
		PixelShader		= compile ps_3_0 PSMain();
	}
}