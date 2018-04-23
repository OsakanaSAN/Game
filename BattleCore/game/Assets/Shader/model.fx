/*!
 * @brief	�X�L�����f���V�F�[�_�[�B(4�{�[���X�L�j���O)
 */

#include "LightingFunction.h"

//�X�L���s��B
#define MAX_MATRICES  26
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    g_mViewProj : VIEWPROJECTION;     //�J�����̃r���[�v���W�F�N�V�����s��
float		g_numBone;			//���̐��B
float3      g_Eyeposition;         //���_
int		g_alphaZero;

int         g_isShadowReciever;				    //�V���h�E���V�[�o�[�H�P�Ȃ�V���h�E���V�[�o�[�B
float4x4 	g_lightViewMatrix;			//���C�g�r���[�s��B
float4x4 	g_lightProjectionMatrix;	//���C�g�v���W�F�N�V�����s��B


float4x4	g_worldMatrix;			//!<���[���h�s��B
float4x4	g_rotationMatrix;		//!<��]�s��B
float4x4	g_viewMatrixRotInv;		//!<�J�����̉�]�s��̋t�s��B

bool  g_isHasNormalMap;			//�@���}�b�v�ێ����Ă���H
bool  g_isWave;                 //�g�̔���
float g_moveUV;                 //UV�̈ړ���   
float2 g_Fog;
              

texture g_diffuseTexture;		//�f�B�t���[�Y�e�N�X�`���B
sampler g_diffuseTextureSampler = 
sampler_state
{
	Texture = <g_diffuseTexture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = Wrap;
	AddressV = Wrap;
};

texture g_shadowMapTexture;		//�V���h�E�}�b�v�e�N�X�`���B
sampler g_shadowMapTextureSampler = 
sampler_state
{
	Texture = <g_shadowMapTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
	AddressV = CLAMP;
};

texture g_shadowMapTexture2;		//�V���h�E�}�b�v�e�N�X�`��2���ځB
sampler g_shadowMapTextureSecondSampler = 
sampler_state
{
	Texture = <g_shadowMapTexture2>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
	AddressV = CLAMP;
};

//�@���}�b�v
texture g_normalTexture;		//�@���}�b�v�B
sampler g_normalMapSampler = 
sampler_state
{
	Texture = <g_normalTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

//�@���}�b�v2���ڗp
texture g_normalTexture2;		//�@���}�b�v�B
sampler g_normalMapSampler2 = 
sampler_state
{
	Texture = <g_normalTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

texture g_CubeTexture;		//�L���[�u�e�N�X�`���B
samplerCUBE g_CubeTextureSampler = 
sampler_state
{
	Texture = <g_CubeTexture>;
    MipFilter = Linear;
    MinFilter =  Linear;
    MagFilter = Linear;
   
};


/*!
 * @brief	���͒��_
 */
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  color           : COLOR0;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3	Tangent			: TANGENT;		//�ڃx�N�g��
    float3  Tex0            : TEXCOORD0;
    float4  uv              : TEXCOORD1;
};

/*!
 * @brief	�o�͒��_�B
 */
struct VS_OUTPUT
{
	float4  Pos     		: POSITION;
    float3  Normal			: NORMAL;
    float4	color			: COLOR0;
    float2  Tex0   			: TEXCOORD0;
    float3	Tangent			: TEXCOORD1;		//�ڃx�N�g��
    float3	worldPos 	    : TEXCOORD2;		//���[���h��Ԃł̒��_���W
    float4	lightViewPos 	: TEXCOORD3;		//���[���h���->���C�g�r���[���->���C�g�ˉe��Ԃɕϊ����ꂽ���W�B
    float2	uv				: TEXCOORD4;
    float4  DepthUV 		: TEXCOORD5;		// Z�l�Z�o�p�e�N�X�`��
    float2  Depth 		    : TEXCOORD6;	
    float4  Fog				: COLOR1;
    	
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[����̏o�́B
 */
struct PSOutput{
	float4	color		: COLOR0;		//�����_�����O�^�[�Q�b�g0�ɏ������݁B
	float4	depth		: COLOR1;		//�����_�����O�^�[�Q�b�g1�ɏ������݁B
	float4  velocity 	: COLOR2;		//�����_�����O�^�[�Q�b�g2�ɏ������݁B
};

/*!
 *@brief	���[���h���W�ƃ��[���h�@�����X�L���s�񂩂�v�Z����B
 *@param[in]	In		���͒��_�B
 *@param[out]	Pos		���[���h���W�̊i�[��B
 *@param[out]	Normal	���[���h�@���̊i�[��B
 *@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
 */
void CalcWorldPosAndNormalFromSkinMatrix( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent )
{
	Pos = 0.0f;
	Normal = 0.0f;
	Tangent = 0.0f;
	//�u�����h����{�[���̃C���f�b�N�X�B
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);
	
	//�u�����h���[�g�B
	float BlendWeightsArray[4] = (float[4])In.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    float LastWeight = 0.0f;
    for (int iBone = 0; iBone < g_numBone-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(In.Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(In.Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Tangent += mul(In.Tangent, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 
    
	Pos += (mul(In.Pos, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    Normal += (mul(In.Normal, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    Tangent += (mul(In.Tangent, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
}
/*!
 *@brief	���[���h���W�ƃ��[���h�@�����v�Z�B
 *@param[in]	In		���͒��_�B
 *@param[out]	Pos		���[���h���W�̊i�[��B
 *@param[out]	Normal	���[���h�@���̊i�[��B
 *@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
 */
void CalcWorldPosAndNormal( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent )
{
	Pos = mul(In.Pos, g_worldMatrix );
	Normal = mul(In.Normal, g_rotationMatrix );
	Tangent = mul(In.Tangent, g_rotationMatrix );
}
/*!
 *@brief	���_�V�F�[�_�[�B
 *@param[in]	In			���͒��_�B
 *@param[in]	hasSkin		�X�L������H
 */
VS_OUTPUT VSMain( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent );
	}
	o.worldPos = Pos;
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.Normal = normalize(Normal);
    o.Tangent = normalize(Tangent);
    o.Tex0 = In.Tex0;
    o.uv = In.uv;
    
    //�t�H�O�W���̌v�Z
    
    
    if(g_isShadowReciever == 1){
     	float4 worldPos = float4(Pos.x,Pos.y,Pos.z,1.0f); 
		//�V���h�E���V�[�o�[�B
		//���[���h���W�����C�g�J�������猩���ˉe��Ԃɕϊ�����B
		o.lightViewPos = mul(worldPos, g_lightViewMatrix);
		o.lightViewPos = mul(o.lightViewPos, g_lightProjectionMatrix);
	}
	
    
	return o;
}


/*!
 * @brief	�s�N�Z���V�F�[�_�[�B
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	if(g_alphaZero == 1)
	{
		return float4(1.0f,0.0f,0.0f,0.0f);
	}
	float4 color = tex2D(g_diffuseTextureSampler, In.Tex0);
	
    float3 normal = In.Normal;
   	
   	float4 lig = DiffuseLight(normal);      //�f�B�t���[�Y���C�g�̌v�Z
   	
   	lig.xyz += g_light.ambient.xyz; //�A���r�G���g�̉��Z
	float3 eye = normalize(g_Eyeposition - In.worldPos.xyz); 
	float3 L = -g_light.diffuseLightDir[0]; //���C�g�̌���
	float3 N = normal.xyz;                  //�@���x�N�g�� 
	float3 R = -L + 2.0f * dot(N,L)* N; //���˃x�N�g���̌v�Z
	
	color *= lig + pow(max(0,dot(R,eye)),20.0f);   //�X�y�L�����[�̌v�Z
	
	 //�����t�H�O
	 /*
	 	float  Far = 10.0f;
	 	float  Near = 0.2f;
	 	float4 fogcol = float4(1.0f,1.0f,1.0f,1.0f);
		float z = length(In.worldPos.xyz - g_Eyeposition.xyz);
		z = max(z - Near, 0.0f);
		float t = min( z / Far, 1.0f);
		fogcol.xyz = lerp(fogcol.xyz, float3(0.75f, 0.75f, 0.95f), t);
		
		//color.rgb	*= fogcol.rgb;
		float f = (Far - z ) / (Far - Near);
		f = clamp(f,0.0f,1.0f);
		color = color * f + fogcol * (1.0f - f);
	*/	
		 
	if(g_isShadowReciever == 1)
	{
		//�ˉe���(�X�N���[�����W�n)�ɕϊ����ꂽ���W��w�����Ŋ����Ă���(-1.0f�`1.0)�͈̔͂̐��K�����W�n�ɂȂ�B
		//�����UV���W�n(0.0�`1.0)�ɕϊ����āA�V���h�E�}�b�v���t�F�b�`���邽�߂�UV�Ƃ��Ċ��p����B
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;	//���̌v�Z��(-1.0�`1.0)�͈̔͂ɂȂ�B
		shadowMapUV *= float2(0.5f, -0.5f);								//���̌v�Z��(-0.5�`0.5)�͈̔͂ɂȂ�B
		shadowMapUV += float2(0.5f, 0.5f);								//�����Ă����(0.0�`1.0)�͈̔͂ɂȂ��Ăt�u���W�n�ɕϊ��B
		float4 shadowVal = tex2D(g_shadowMapTextureSampler,shadowMapUV);	//�V���h�E�}�b�v�͉e�������Ă���Ƃ���̓O���[�X�P�[���ɂȂ��Ă���B
		
		
		if(shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f && shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f)
		{
			float Depth  = In.lightViewPos.z  / In.lightViewPos.w;  		//���C�g���猩��Z�l
			Depth =  min(1.0f ,Depth);
			if(Depth > shadowVal.x)
			{
				color = float4(0.5f,0.5f,0.5f,1.0f);
				
				return color;
			}
		}
	 }
	 
	 
	//lig.xyz += g_light.ambient.xyz; //�A���r�G���g�̉��Z
	//color *= lig ;
	
	
	return color;
}

//�V���h�E�}�b�v�p�̒��_�V�F�[�_�[
VS_OUTPUT VSMainShadow( VS_INPUT In, uniform bool hasSkin)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//�X�L������B
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent );
	}else{
		//�X�L���Ȃ��B
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent );
	}
	
    Out.Pos = mul(float4(Pos,1.0f), g_mViewProj);
    Out.Tex0 = In.Tex0;
    
     // �f�v�X�o�b�t�@�̐[�x�����擾
    Out.DepthUV  = Out.Pos; 
    
  return Out;
  
}

/*
 *@brief	�V���h�E�}�b�v�������ݗp�̃s�N�Z���V�F�[�_�[�B
 */
float4 PSRenderToShadowMapMain(VS_OUTPUT In ) : COLOR
{
   float4 col = 1.0f;;
   col.rgb = In.DepthUV.z / In.DepthUV.w; //Z�l������
   return float4(In.DepthUV.z / In.DepthUV.w,0,0,1);
}

//���ʗp�̒��_�V�F�[�_�[
VS_OUTPUT VSMainWave( VS_INPUT In)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	//�X�L���Ȃ��B
	CalcWorldPosAndNormal( In, Pos, Normal, Tangent );
	
	o.worldPos = Pos;
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.Normal = normalize(Normal);
    o.Tangent = normalize(Tangent);
    o.Tex0 = In.Tex0;
    o.uv = In.uv;
    
     if(g_isShadowReciever == 1){
     	float4 worldPos = float4(Pos.x,Pos.y,Pos.z,1.0f); 
		//�V���h�E���V�[�o�[�B
		//���[���h���W�����C�g�J�������猩���ˉe��Ԃɕϊ�����B
		o.lightViewPos = mul(worldPos, g_lightViewMatrix);
		o.lightViewPos = mul(o.lightViewPos, g_lightProjectionMatrix);
	}
      
	return o;

}

//���ʗp�̃s�N�Z���V�F�[�_�[
float4 PSWaveMain( VS_OUTPUT In ) : COLOR
{

    	float3 normal = float3(0.0f, 1.0f, 0.0f);
    	float4 Wcolor = float4(0.5f,0.5f,0.5f,1.0f);//tex2D(g_diffuseTextureSampler, In.Tex0.xy * 5.0f);
    	
  
      	 float3 Wnormal = In.Normal;
    	//�@���}�b�v������B
    	float2 baseUV = In.Tex0.xy * 5.0f;
		float3 tangent = normalize(In.Tangent);
		float2 moveUV = g_moveUV;
		
		//�@���}�b�v3���g�p
		float3 binSpaceNormal2 = tex2D( g_normalMapSampler, (baseUV + moveUV));
		moveUV.x *= -1.0f;
		float3 binSpaceNormal = tex2D( g_normalMapSampler2, (baseUV + moveUV));
		moveUV.y *= -1.0f;
		float3 binSpaceNormal3 = tex2D( g_normalMapSampler, (baseUV + moveUV));
		
		/*float4x4 tangentSpaceMatrix;
		//�@���ƃ^���W�F���g����]�@�������߂�
		float3 biNormal = normalize( cross( tangent, normal) );
		//�^���W�F���g�X�y�[�X���烏�[���h�X�y�[�X�ɕϊ�����s������߂�B
		tangentSpaceMatrix[0] = float4( tangent, 0.0f);
		tangentSpaceMatrix[1] = float4( biNormal, 0.0f);
		tangentSpaceMatrix[2] = float4( normal, 0.0f);
		tangentSpaceMatrix[3] = float4( 0.0f, 0.0f, 0.0f, 1.0f );*/
		
		
		
	#if 1
	
		//�^���W�F���g�X�y�[�X�@��
		//float3 tangent = normalize(In.Tangent);
		
		//�@���ƃ^���W�F���g����]�@�������߂�
		float3 biNormal = normalize(cross(In.Tangent, In.Normal));
		
		//-1.0�`1.0�͈̔͂Ƀ}�b�s���O����B
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
		binSpaceNormal2 = (binSpaceNormal2 * 2.0f) - 1.0f;
		binSpaceNormal3 = (binSpaceNormal3 * 2.0f) - 1.0f;
		
		//�^���W�F���g�X�y�[�X���烏�[���h�X�y�[�X�ɕϊ�����
		//
		Wnormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z; 
		Wnormal += tangent * binSpaceNormal2.x + biNormal * binSpaceNormal2.y + normal * binSpaceNormal2.z; 
		Wnormal += tangent * binSpaceNormal3.x + biNormal * binSpaceNormal3.y + normal * binSpaceNormal3.z;
		Wnormal = normalize(Wnormal);
		float4 lig = DiffuseLight(Wnormal);//pow(DiffuseLight(Wnormal), 15.0f);     //�f�B�t���[�Y���C�g�̌v�Z
		
	#else 
		//�I�u�W�F�N�g�X�y�[�X�@���B
		Wnormal = binSpaceNormal;
		Wnormal += binSpaceNormal2; 
		Wnormal += binSpaceNormal3;
		Wnormal = normalize(Wnormal);
		float4 lig = DiffuseLight(Wnormal);      //�f�B�t���[�Y���C�g�̌v�Z
		lig.xyz += g_light.ambient.xyz; 		//�A���r�G���g�̉��Z
		
		
	#endif
	
		/*
	    float3 toSun = normalize(g_Eyeposition - In.worldPos.xyz); 
	    for(int i = 0;i < 4 ;i++)
	    {
			float3 L = -g_light.diffuseLightDir[0]; //���C�g�̌���
			float3 N = Wnormal.xyz;                  //�@���x�N�g�� 
			float3 R = -L + 2.0f * dot(N,L)* N; 	//���˃x�N�g���̌v�Z
			lig *= pow(max(0.0f,dot(R,toSun)),10.0f);   //�X�y�L�����[�̌v�Z
		}
		*/
		float3 toSun = normalize(In.worldPos.xyz - g_Eyeposition); 
		
	/*	Wcolor.xyz *= lig.xyz;
		Wcolor.xyz += g_light.ambient.xyz; 					//�A���r�G���g�̉��Z
	*/
	
		
		
		//���˃x�N�g�������߂�B
		float3 reflectVector = reflect( toSun, Wnormal);
		//
		float4 envColor = texCUBE(g_CubeTextureSampler, reflectVector );
		
		//���z�̔��˂��l������B
		float3 toSunDir = -g_light.diffuseLightDir[0];;
		float sunRate = pow( max( 0.0f, dot(reflectVector, toSunDir)), 10.0f);
		envColor.xyz += 1.0f * sunRate;
		
		//�t���l�����ˌW�����v�Z����B
		float rate = abs(dot(toSun, Wnormal));
		
		Wcolor.xyz = lerp( envColor.xyz, float3(0.2f, 0.2f, 0.4f), min( 1.0f, rate + 0.5f));
		
		float  Far = 1200.0f;
	 	float  Near = 0.2f;
		float z = length(In.worldPos.xyz - g_Eyeposition.xyz);
		z = max(z - Near, 0.0f);
		float t =min( z / Far, 1.0f);
		
		Wcolor.xyz = lerp(Wcolor.xyz, float3(0.75f, 0.75f, 0.95f), t);
		
	if(g_isShadowReciever == 1)
	{
		//�ˉe���(�X�N���[�����W�n)�ɕϊ����ꂽ���W��w�����Ŋ����Ă���(-1.0f�`1.0)�͈̔͂̐��K�����W�n�ɂȂ�B
		//�����UV���W�n(0.0�`1.0)�ɕϊ����āA�V���h�E�}�b�v���t�F�b�`���邽�߂�UV�Ƃ��Ċ��p����B
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;	//���̌v�Z��(-1.0�`1.0)�͈̔͂ɂȂ�B
		shadowMapUV *= float2(0.5f, -0.5f);								//���̌v�Z��(-0.5�`0.5)�͈̔͂ɂȂ�B
		shadowMapUV += float2(0.5f, 0.5f);								//�����Ă����(0.0�`1.0)�͈̔͂ɂȂ��Ăt�u���W�n�ɕϊ��B
		float4 shadowVal = tex2D(g_shadowMapTextureSampler,shadowMapUV);	//�V���h�E�}�b�v�͉e�������Ă���Ƃ���̓O���[�X�P�[���ɂȂ��Ă���B
		
		
		if(shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f && shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f)
		{
			float Depth  = In.lightViewPos.z  / In.lightViewPos.w;  		//���C�g���猩��Z�l
			Depth =  min(1.0f ,Depth);
			if(Depth > shadowVal.x)
			{
				Wcolor.xyz *= float3(0.5f,0.5f,0.5f);
				return Wcolor;
			}
		}
	 }
		
		Wcolor.a = 0.5f;
		return Wcolor;
		

}

//��p�̒��_�V�F�[�_�[
VS_OUTPUT VSMainSky( VS_INPUT In)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	//�X�L���Ȃ��B
	CalcWorldPosAndNormal( In, Pos, Normal, Tangent );
	
	o.worldPos = Pos;
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.lightViewPos.w = o.Pos.w;
    o.Normal = normalize(Normal);
    //o.velocity = mul(float4(o.Pos.xyz, 1.0f), g_mViewProj);
    o.Tangent = normalize(Tangent);
    o.Tex0 = In.Tex0;
    o.uv = In.uv;
      
	return o;

}

//��`��p�̃s�N�Z���V�F�[�_�[
float4 PSskyMain(VS_OUTPUT In ): COLOR
{   
    float3 normal = In.Normal;
	float4 diffuseColor = texCUBE(g_CubeTextureSampler, -normal );
	
	 
		float  MaxHeight = 200.0f;
	 	float  MinHeight = 40.0f;
		float  HeightFog = max(In.worldPos.y  - MinHeight, 0.0f);
		float  t = min(HeightFog / MaxHeight, 1.0f);
		diffuseColor.xyz = lerp( float3(0.75f, 0.75f, 0.95f),diffuseColor.xyz, t);
		
	/*
	float4 color = 0;
	//��̃e�N�X�`���𔒍���
	float3 monochrome = float3(0.29900f, 0.58700f, 0.11400f );
	float Y  =  dot(monochrome, diffuseColor);
	//�����������e�N�X�`����n�悵�Ĕ��ɋ߂��������������o���B
	float cloudRate = pow(Y, 3.0f );
	color = 1.0f + 0.25f * 0.5f;
	//��C�̐F�����m�N����
	float colorY = max( 0.0f, dot(monochrome, color) );
	float nightRate = 0.0f;
	//�_�̐F�B���Ԃ�1.0f�Ŗ�Ԃ�0.3f
	float cloudColor = lerp(3.0f, 0.1f,pow( 1.0f - nightRate, 3.0f));
	//��̐F�Ɖ_�̐F�Ƃ̊Ԃ��_���Ő��`�⊮�B
	color.xyz = lerp( color.xyz, cloudColor, cloudRate ) ;
	PSOutput psOut = (PSOutput)0;
	psOut.color = color * 1.1f;
	psOut.depth = In.lightViewPos.w;
	//psOut.velocity.xy =  In.velocity.xy / In.velocity.w - In.Pos.xy / In.Pos.w;
	psOut.velocity.xy *= 0.5f;
	psOut.velocity.xy += 0.5f;
	psOut.velocity.zw = 0.0f;
	*/
	return diffuseColor;
}


/*!
 *@brief	�X�L�����胂�f���p�̃e�N�j�b�N�B
 */
technique SkinModel
{
    pass p0
    {
        VertexShader = compile vs_3_0 VSMain(true);
        PixelShader = compile ps_3_0 PSMain();
    }
}
/*!
 *@brief	�X�L���Ȃ����f���p�̃e�N�j�b�N�B
 */
technique NoSkinModel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain(false);
		PixelShader = compile ps_3_0 PSMain();
	}
}

/*!
 *@brief	�V���h�E�}�b�v�������ݗp�̃e�N�j�b�N
 */
technique SkinModelRenderToShadowMap
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMainShadow(true);
		PixelShader 	= compile ps_3_0 PSRenderToShadowMapMain(); 
	}
}
//�V���h�E�}�b�v�������ݗp�e�N�j�b�N(�X�L������)

technique NoSkinModelRenderToShadowMap
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMainShadow(false);
		PixelShader 	= compile ps_3_0 PSRenderToShadowMapMain(); 
	}
}

//���ʗp�̃e�N�j�b�N
technique NoSkinModelRenderToWave
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMainWave();
		PixelShader		 = compile ps_3_0 PSWaveMain(); 
	}

}

//�󏑂����ݗp�̃e�N�j�b�N
technique NoSkinModelRenderToSkyMap
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMainSky();
		PixelShader		 = compile ps_3_0 PSskyMain(); 
	}
}

//�n�`�p�̏������݃e�N�j�b�N
technique NoSkinModelRenderTerrain
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMainSky();
		PixelShader		 = compile ps_3_0 PSskyMain(); 
	}

}