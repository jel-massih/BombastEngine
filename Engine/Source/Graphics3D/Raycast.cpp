#include "Raycast.h"

#include <algorithm>

#include "ModelClass.h"

template <class T>
void InitIntersection(Intersection &intersection, DWORD faceIndex, FLOAT dist, FLOAT u, FLOAT v, ActorId actorId, WORD* pIndices, T* pVertices, const Mat4x4& matWorld)
{
	intersection.m_dwFace = faceIndex;
	intersection.m_fDist = dist;
	intersection.m_fBary1 = u;
	intersection.m_fBary2 = v;

	T* v0 = &pVertices[pIndices[3 * faceIndex + 0]];
	T* v1 = &pVertices[pIndices[3 * faceIndex + 1]];
	T* v2 = &pVertices[pIndices[3 * faceIndex + 2]];

	Vec3 a = v0->position - v1->position;
	Vec3 b = v2->position - v1->position;

	Vec3 cross = a.Cross(b);
	cross /= cross.Length();

	Vec3 actorLoc = XMVectorBaryCentric(v0->position, v1->position, v2->position, intersection.m_fBary1, intersection.m_fBary2);
	intersection.m_actorLoc = actorLoc;
	intersection.m_worldLoc = matWorld.Xform(actorLoc);
	intersection.m_actorId = actorId;
	intersection.m_normal = cross;
}

RayCast::RayCast(Point point, DWORD maxIntersections)
{
	m_maxIntersections = maxIntersections;
	m_intersectionArray.reserve(m_maxIntersections);
	m_bUseD3DIntersect = true;
	m_bAllHits = true;
	m_numIntersections = 0;
	m_point = point;
}

void RayCast::Sort()
{
	std::sort(m_intersectionArray.begin(), m_intersectionArray.end());
}

HRESULT RayCast::Pick(Scene * pScene, ActorId actorId, ModelClass * pMesh)
{
	if (!m_bAllHits && m_numIntersections)
		return S_OK;

	const Mat4x4 matView = pScene->GetCamera()->GetView();
	const Mat4x4 matWorld = pScene->GetTopMatrix();
	const Mat4x4 proj = pScene->GetCamera()->GetProjection();

	//Compute Pickray in screen space
	Vec3 v;
	v.x = (((2.0f * m_point.x) / g_pApp->GetScreenSize().x) - 1) / proj._11;
	v.y = (((2.0f * m_point.y) / g_pApp->GetScreenSize().y) - 1) / proj._22;
	v.z = 1.0f;

	Mat4x4 worldView = matWorld * matView;
	Mat4x4 inverseWorld = worldView.Inverse();

	//Screen space pick ray into 3d space
	m_vPickRayDir.x = v.x * inverseWorld._11 + v.y * inverseWorld._21 + v.z * inverseWorld._31;
	m_vPickRayDir.y = v.x * inverseWorld._12 + v.y * inverseWorld._22 + v.z * inverseWorld._32;
	m_vPickRayDir.z = v.x * inverseWorld._13 + v.y * inverseWorld._23 + v.z * inverseWorld._33;
	m_vPickRayOrigin.x = inverseWorld._41;
	m_vPickRayOrigin.y = inverseWorld._42;
	m_vPickRayOrigin.z = inverseWorld._43;

	if (!m_bAllHits)
	{
		//Find closest intersection
		for (auto meshIt = pMesh->meshes.begin(); meshIt != pMesh->meshes.end(); meshIt++)
		{
			for (auto it = (*meshIt).vertices.begin(); it != (*meshIt).vertices.end(); it++)
			{

			}
		}
	}

	return NULL;
}