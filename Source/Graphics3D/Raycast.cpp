#include "Raycast.h"

#include <algorithm>

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