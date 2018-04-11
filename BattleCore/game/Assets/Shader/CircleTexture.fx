float4x4		g_WVPmt;
texture g_texture;		//ディフューズテクスチャ。
sampler g_textureSampler = 
sampler_state
{
	Texture = <g_texture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT{
	float4	pos		: POSITION;
	float2	uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4 pos		: POSITION;
	float2	uv		: TEXCOORD0;
};

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	Out.pos = In.pos;
	Out.uv = (In.pos * 0.5f) + 0.5f	;
	Out.uv.y = 1.0f -Out.uv.y;
	return Out;
}

float4 PSMain( VS_OUTPUT In ) : COLOR0
{

	float4 tex = tex2D(g_textureSampler, In.uv);
	float2 m_CenterUv = float2(0.5f,0.5f);
	float2 dir =  m_CenterUv - In.uv;
	//距離を計算する
   	float len = length( dir );
  
   	if(len >= 0.3f)
   	{
   	
   		return float4(0.0f,0.0f,0.0f,0.0f);
	}
   	
	
	return tex;
}

technique creatTex
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMain();
    }
}
