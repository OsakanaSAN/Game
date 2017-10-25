/*!
 * @brief	スキンモデルシェーダー。(4ボーンスキニング)
 */

#include "LightingFunction.h"

//スキン行列。
#define MAX_MATRICES  26
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    g_mViewProj : VIEWPROJECTION;     //カメラのビュープロジェクション行列
float		g_numBone;			//骨の数。
float3      g_Eyeposition;         //視点

int g_isShadowReciever;				//シャドウレシーバー？１ならシャドウレシーバー。
float4x4 	g_lightViewMatrix;			//ライトビュー行列。
float4x4 	g_lightProjectionMatrix;	//ライトプロジェクション行列。

float4x4	g_worldMatrix;			//!<ワールド行列。
float4x4	g_rotationMatrix;		//!<回転行列。
float4x4	g_viewMatrixRotInv;		//!<カメラの回転行列の逆行列。

bool  g_isHasNormalMap;			//法線マップ保持している？
bool  g_isWave;                 //波の判定
float g_moveUV;                 //UVの移動量   
float g_far;
float g_Near;              

texture g_diffuseTexture;		//ディフューズテクスチャ。
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

texture g_shadowMapTexture;		//シャドウマップテクスチャ。
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

//法線マップ
texture g_normalTexture;		//法線マップ。
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


/*!
 * @brief	入力頂点
 */
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  color           : COLOR0;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3	Tangent			: TANGENT;		//接ベクトル
    float3  Tex0            : TEXCOORD0;
    float4  uv              : TEXCOORD1;
};

/*!
 * @brief	出力頂点。
 */
struct VS_OUTPUT
{
	float4  Pos     		: POSITION;
    float3  Normal			: NORMAL;
    float4	color			: COLOR0;
    float2  Tex0   			: TEXCOORD0;
    float3	Tangent			: TEXCOORD1;		//接ベクトル
    float3	worldPos 	    : TEXCOORD2;		//ワールド空間での頂点座標
    float4	lightViewPos 	: TEXCOORD3;		//ワールド空間->ライトビュー空間->ライト射影空間に変換された座標。
    float2	uv				: TEXCOORD4;
    float4  ZCalcTex 		: TEXCOORD5;		// Z値算出用テクスチャ
};
/*!
 *@brief	ワールド座標とワールド法線をスキン行列から計算する。
 *@param[in]	In		入力頂点。
 *@param[out]	Pos		ワールド座標の格納先。
 *@param[out]	Normal	ワールド法線の格納先。
 *@param[out]	Tangent	ワールド接ベクトルの格納先。
 */
void CalcWorldPosAndNormalFromSkinMatrix( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent )
{
	Pos = 0.0f;
	Normal = 0.0f;
	Tangent = 0.0f;
	//ブレンドするボーンのインデックス。
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);
	
	//ブレンドレート。
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
 *@brief	ワールド座標とワールド法線を計算。
 *@param[in]	In		入力頂点。
 *@param[out]	Pos		ワールド座標の格納先。
 *@param[out]	Normal	ワールド法線の格納先。
 *@param[out]	Tangent	ワールド接ベクトルの格納先。
 */
void CalcWorldPosAndNormal( VS_INPUT In, out float3 Pos, out float3 Normal, out float3 Tangent )
{
	Pos = mul(In.Pos, g_worldMatrix );
	Normal = mul(In.Normal, g_rotationMatrix );
	Tangent = mul(In.Tangent, g_rotationMatrix );
}
/*!
 *@brief	頂点シェーダー。
 *@param[in]	In			入力頂点。
 *@param[in]	hasSkin		スキンあり？
 */
VS_OUTPUT VSMain( VS_INPUT In, uniform bool hasSkin )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	float3 Pos, Normal, Tangent;
	if(hasSkin){
		//スキンあり。
	    CalcWorldPosAndNormalFromSkinMatrix( In, Pos, Normal, Tangent );
	}else{
		//スキンなし。
		CalcWorldPosAndNormal( In, Pos, Normal, Tangent );
	}
	o.worldPos = Pos;
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mViewProj);
    o.Normal = normalize(Normal);
    o.Tangent = normalize(Tangent);
    o.Tex0 = In.Tex0;
    o.uv = In.uv;
    if(g_isShadowReciever == 1){
     	float4 worldPos; 
		worldPos = mul( In.Pos, g_worldMatrix );		//モデルのローカル空間からワールド空間に変換。
		//シャドウレシーバー。
		//ワールド座標をライトカメラから見た射影空間に変換する。
		o.lightViewPos = mul(worldPos, g_lightViewMatrix);
		o.lightViewPos = mul(o.lightViewPos, g_lightProjectionMatrix);
	}
    
	return o;
}

//影用の頂点シェーダー
/*VS_OUTPUT VSMainShadow( VS_INPUT In )
{
  VS_OUTPUT Out = (VS_OUTPUT)0;
  // 普通にカメラの目線によるワールドビュー射影変換をする
   float4x4 mat
   mat = mul (g_worldMatrix, g_lightViewMatrix);
   mat = mul (mat,g_lightProjectionMatrix);
}
*/
/*!
 * @brief	ピクセルシェーダー。
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	float4 color = tex2D(g_diffuseTextureSampler, In.Tex0);
    float3 normal = In.Normal;
   if(g_isWave)
   {
       	float4 Wcolor = tex2D(g_diffuseTextureSampler, In.Tex0);
      	 float3 Wnormal = In.Normal;
    	//法線マップがある。
		float3 tangent = normalize(In.Tangent);
		float3 binSpaceNormal2 = tex2D( g_normalMapSampler, In.Tex0.xy + g_moveUV);
		float3 binSpaceNormal = tex2D( g_normalMapSampler, In.Tex0.xy - g_moveUV);
		
		float4x4 tangentSpaceMatrix;
		//法線とタンジェントから従法線を求める
		float3 biNormal = normalize( cross( tangent, normal) );
		//タンジェントスペースからワールドスペースに変換する行列を求める。
		tangentSpaceMatrix[0] = float4( tangent, 0.0f);
		tangentSpaceMatrix[1] = float4( biNormal, 0.0f);
		tangentSpaceMatrix[2] = float4( normal, 0.0f);
		tangentSpaceMatrix[3] = float4( 0.0f, 0.0f, 0.0f, 1.0f );
		
		//-1.0～1.0の範囲にマッピングする。
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
		//-1.0～1.0の範囲にマッピングする。
		binSpaceNormal2 = (binSpaceNormal2 * 2.0f) - 1.0f;
		//タンジェントスペースからワールドスペースの法線に変換する。
		//Wnormal = tangentSpaceMatrix[0] * binSpaceNormal.x + tangentSpaceMatrix[1] * binSpaceNormal.y + tangentSpaceMatrix[2] * binSpaceNormal.z;
		Wnormal = In.Normal;
		return g_light.diffuseLightDir[0];	
		float4 lig = float4(0.0f, 0.0f, 0.0f, 1.0f);//DiffuseLight(Wnormal);      //ディフューズライトの計算
		float3 eye = normalize(g_Eyeposition - In.worldPos.xyz); 
		float3 L = -g_light.diffuseLightDir[0]; //ライトの向き
		float3 N = Wnormal.xyz;                  //法線ベクトル 
		float3 R = -L + 2.0f * dot(N,L)* N; //反射ベクトルの計算
		lig += pow(max(0.0f,dot(R,eye)),0.2f) * 5.0f;   //スペキュラーの計算
		
		/*Wnormal = tangentSpaceMatrix[0] * binSpaceNormal2.x + tangentSpaceMatrix[1] * binSpaceNormal2.y + tangentSpaceMatrix[2] * binSpaceNormal2.z;
		
	   // lig += DiffuseLight(Wnormal);      //ディフューズライトの計算
		N = Wnormal.xyz;                  //法線ベクトル 
		R = -L + 2.0f * dot(N,L)* N; //反射ベクトルの計算
		lig += pow(max(0.0f,dot(R,eye)),2.0f) * 5.0f;   //スペキュラーの計算
		*/
		
		Wcolor *= lig ;
		Wcolor.w = 0.5f; //水面の透過処理
		return Wcolor;
		
   }
   
  if(!g_isWave)
    {	
	float4 lig = DiffuseLight(normal);      //ディフューズライトの計算
	
	float3 eye = normalize(g_Eyeposition - In.worldPos.xyz); 
	float3 L = -g_light.diffuseLightDir[0]; //ライトの向き
	float3 N = normal.xyz;                  //法線ベクトル 
	float3 R = -L + 2.0f * dot(N,L)* N; //反射ベクトルの計算
	
	color *= lig + pow(max(0,dot(R,eye)),5.0f);   //スペキュラーの計算
	}
	
	if(g_isShadowReciever == 1){
		//射影空間(スクリーン座標系)に変換された座標はw成分で割ってやると(-1.0f～1.0)の範囲の正規化座標系になる。
		//これをUV座標系(0.0～1.0)に変換して、シャドウマップをフェッチするためのUVとして活用する。
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;	//この計算で(-1.0～1.0)の範囲になる。
		shadowMapUV *= float2(0.5f, -0.5f);								//この計算で(-0.5～0.5)の範囲になる。
		shadowMapUV += float2(0.5f, 0.5f);								//そしてこれで(0.0～1.0)の範囲になってＵＶ座標系に変換できた。やったね。
		float4 shadowVal = tex2D(g_shadowMapTextureSampler, shadowMapUV);	//シャドウマップは影が落ちているところはグレースケールになっている。
		color *= shadowVal;
	 }
	 
	float4 lig = DiffuseLight(normal);
	color *= lig ;
	return color;
}

/*!
 *@brief	シャドウマップ書き込み用のピクセルシェーダー。
 */
float4 PSRenderToShadowMapMain(VS_OUTPUT In) : COLOR
{
	
	return float4(0.5f, 0.5f, 0.5f, 1.0f);
}


/*!
 *@brief	スキンありモデル用のテクニック。
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
 *@brief	スキンなしモデル用のテクニック。
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
 *@brief	シャドウマップ書き込み用のテクニック
 */
technique SkinModelRenderToShadowMap
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMain(true);
		PixelShader 	= compile ps_3_0 PSRenderToShadowMapMain(); 
	}
}
