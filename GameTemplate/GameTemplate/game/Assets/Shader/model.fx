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


int g_isShadowReciever;				    //�V���h�E���V�[�o�[�H�P�Ȃ�V���h�E���V�[�o�[�B
float4x4 	g_lightViewMatrix;			//���C�g�r���[�s��B
float4x4 	g_lightProjectionMatrix;	//���C�g�v���W�F�N�V�����s��B


float4x4	g_worldMatrix;			//!<���[���h�s��B
float4x4	g_rotationMatrix;		//!<��]�s��B
float4x4	g_viewMatrixRotInv;		//!<�J�����̉�]�s��̋t�s��B

bool  g_isHasNormalMap;			//�@���}�b�v�ێ����Ă���H
bool  g_isWave;                 //�g�̔���
float g_moveUV;                 //UV�̈ړ���   
float g_Fog_X;
float g_Fog_Y;              

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

texture g_CubeTexture;		//�L���[�u�e�N�X�`���B
sampler g_CubeTextureSampler = 
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
    o.Fog = g_Fog_X + o.Pos.w * g_Fog_Y;
    
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
	//�����t�H�O
	//float z = length(In.worldPos.xyz - g_Eyeposition);
	//z = max(In.Fog.x, 0.0f);
	//float t = min(z / In.Fog.y,1.0f);
	//color.xyz = lerp(color.xyz,float3(0.75f, 0.75f, 0.95f),t);
	
	float4 color = tex2D(g_diffuseTextureSampler, In.Tex0.xy * 6);
	
    float3 normal = In.Normal;
   	
   	float4 lig = DiffuseLight(normal);      //�f�B�t���[�Y���C�g�̌v�Z
   	
	float3 eye = normalize(g_Eyeposition - In.worldPos.xyz); 
	float3 L = -g_light.diffuseLightDir[0]; //���C�g�̌���
	float3 N = normal.xyz;                  //�@���x�N�g�� 
	float3 R = -L + 2.0f * dot(N,L)* N; //���˃x�N�g���̌v�Z
	
	color *= lig + pow(max(0,dot(R,eye)),10.0f);   //�X�y�L�����[�̌v�Z
	
	if(g_isShadowReciever == 1)
	{
		//�ˉe���(�X�N���[�����W�n)�ɕϊ����ꂽ���W��w�����Ŋ����Ă���(-1.0f�`1.0)�͈̔͂̐��K�����W�n�ɂȂ�B
		//�����UV���W�n(0.0�`1.0)�ɕϊ����āA�V���h�E�}�b�v���t�F�b�`���邽�߂�UV�Ƃ��Ċ��p����B
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;	//���̌v�Z��(-1.0�`1.0)�͈̔͂ɂȂ�B
		shadowMapUV *= float2(0.5f, -0.5f);								//���̌v�Z��(-0.5�`0.5)�͈̔͂ɂȂ�B
		shadowMapUV += float2(0.5f, 0.5f);								//�����Ă����(0.0�`1.0)�͈̔͂ɂȂ��Ăt�u���W�n�ɕϊ��B
		float4 shadowVal = tex2D(g_shadowMapTextureSampler,shadowMapUV);	//�V���h�E�}�b�v�͉e�������Ă���Ƃ���̓O���[�X�P�[���ɂȂ��Ă���B
		
		
		//if(shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f && shadowMapUV.y >= 0.0f, shadowMapUV.y <= 1.0f){
			float Depth  = In.lightViewPos.z  / In.lightViewPos.w;  		//���C�g���猩��Z�l
			Depth =  min(1.0f ,Depth);
			
			if(Depth > shadowVal.x )
			{
				color = float4(0.0,0.0f,0.0f,1.0f);
				return color;
			}
		//}
	 }
	lig.xyz += g_light.ambient.xyz; //�A���r�G���g�̉��Z
	color *= lig ;

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
      
	return o;

}

//���ʗp�̃s�N�Z���V�F�[�_�[
float4 PSWaveMain( VS_OUTPUT In ) : COLOR
{

    float3 normal = In.Normal;
    float4 Wcolor = tex2D(g_diffuseTextureSampler, In.Tex0);;
  
      	 float3 Wnormal = In.Normal;
    	//�@���}�b�v������B
    	float2 baseUV = In.Tex0.xy * 10.0f;
		float3 tangent = normalize(In.Tangent);
		float2 moveUV = g_moveUV;
		
		//�@���}�b�v3���g�p
		float3 binSpaceNormal2 = tex2D( g_normalMapSampler, (baseUV + moveUV));
		moveUV.x *= -1.0f;
		float3 binSpaceNormal = tex2D( g_normalMapSampler, (baseUV + moveUV));
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
		float4 lig = pow(DiffuseLight(Wnormal), 20.0f);      //�f�B�t���[�Y���C�g�̌v�Z
		
	#else 
		//�I�u�W�F�N�g�X�y�[�X�@���B
		Wnormal = binSpaceNormal;
		Wnormal += binSpaceNormal2; 
		Wnormal += binSpaceNormal3;
		Wnormal = normalize(Wnormal);
		float4 lig = DiffuseLight(Wnormal);      //�f�B�t���[�Y���C�g�̌v�Z
		
	#endif
		
	    float3 toSun = normalize(g_Eyeposition - In.worldPos.xyz); 
		float3 L = -g_light.diffuseLightDir[0]; //���C�g�̌���
		float3 N = Wnormal.xyz;                  //�@���x�N�g�� 
		float3 R = -L + 2.0f * dot(N,L)* N; //���˃x�N�g���̌v�Z
		//lig += pow(max(0.0f,dot(R,toSun)),10.0f);   //�X�y�L�����[�̌v�Z
		
		//lig.xyz += g_light.ambient.xyz; //�A���r�G���g�̉��Z
		Wcolor.xyz = lig.xyz;
		//Wcolor.a = 5.0f; //���ʂ̓��ߏ���
		return float4(Wcolor.x,Wcolor.y,Wcolor.z,0.5f);
		

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
    o.Normal = normalize(Normal);
    o.Tangent = normalize(Tangent);
    o.Tex0 = In.Tex0;
    o.uv = In.uv;
      
	return o;

}

//��`��p�̃s�N�Z���V�F�[�_�[
float4 PSskyMain(VS_OUTPUT In ): COLOR
{   
    float3 normal = In.Normal;
	float4 color = texCUBE(g_CubeTextureSampler, -normal );
	return color;
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
