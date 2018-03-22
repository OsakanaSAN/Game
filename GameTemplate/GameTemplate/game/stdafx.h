#pragma once
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <time.h>
#include <X3daudio.h>
#include <Xaudio2.h>
#include <Xaudio2fx.h>
#pragma comment(lib, "winmm.lib")	// winmm.libを使用する
#pragma comment(lib, "X3DAudio.lib")

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

#include "myEngine/System.h"

#include "myEngine/Graphics/Camera.h"
#include "myEngine/Graphics/SkinModel.h"
#include "myEngine/Graphics/SkinModelData.h"
#include "myEngine/Graphics/Animation.h"
#include "myEngine/Graphics/Light.h"

#include "game.h"
#include "myEngine/Graphics/ShadowMap.h"
#include "myEngine/Random/MyRandom.h"
#include "myEngine/Graphics/PostEffect/PostEffect.h"



extern CShadowMap g_shadowMap;	//シャドウマップ。
extern MyRandom* g_Random;

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p) = NULL; } }	//インスタンスリリース
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p) = NULL; } }	//インスタンス配列削除
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p) = NULL; } }	//インスタンス削除

