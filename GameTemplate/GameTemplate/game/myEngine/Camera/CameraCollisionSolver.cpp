#include "stdafx.h"
#include "CameraCollisionSolver.h"
#include "myEngine/Physics/CollisionAttr.h"

struct SConvexSweepCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	D3DXVECTOR3 m_rayDir;
	SConvexSweepCallback(D3DXVECTOR3 rayDir) :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f)),
		m_rayDir(rayDir) {}
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		D3DXVECTOR3 normal;
		normal.x = convexResult.m_hitNormalLocal.x();
		normal.y = convexResult.m_hitNormalLocal.y();
		normal.z = convexResult.m_hitNormalLocal.z();

		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character) {
			return 1.0f;
		}
		return btCollisionWorld::ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	}
};
CameraCollisionSolver::CameraCollisionSolver()
{
}
CameraCollisionSolver::~CameraCollisionSolver()
{
}
void CameraCollisionSolver::Init(float radius)
{
	m_radius = radius;
	m_collider.Create(radius);
}
bool CameraCollisionSolver::Execute(D3DXVECTOR3& result, const D3DXVECTOR3& position, const D3DXVECTOR3& target)
{
	result = position;
	D3DXVECTOR3 vWk;
	D3DXVec3Subtract( &vWk,&target, &position);
	if (D3DXVec3LengthSq(&vWk) < FLT_EPSILON) {
		//視点と注視点がほぼ同じ座標にある。
		return false;
	}
	D3DXVec3Normalize( &vWk,&vWk);
	//レイを作成する。
	btTransform btStart, btEnd;
	btStart.setIdentity();
	btEnd.setIdentity();
	btStart.setOrigin(btVector3(target.x, target.y, target.z));
	btEnd.setOrigin(btVector3(position.x, position.y, position.z));
	SConvexSweepCallback callback(vWk);
	//	callback.m_collisionFilterGroup = 
	g_physicsWorld->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), btStart, btEnd, callback);
	if (callback.hasHit()) {
		D3DXVECTOR3 vHitPos;
		vHitPos.x = callback.m_hitPointWorld.x();
		vHitPos.y = callback.m_hitPointWorld.y();
		vHitPos.z = callback.m_hitPointWorld.z();
		D3DXVECTOR3 vOffset;
		vOffset.x = callback.m_hitNormalWorld.x();
		vOffset.y = callback.m_hitNormalWorld.y();
		vOffset.z = callback.m_hitNormalWorld.z();
		D3DXVec3Scale(&vOffset, &vOffset, m_radius);
		D3DXVec3Add(&result, &vHitPos, &vOffset);
		return true;
	}
	return false;
}