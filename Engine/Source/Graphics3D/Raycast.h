#pragma once

#include <vector>
#include "Geometry.h"

class Intersection
{
public:
	FLOAT m_fDist;
	DWORD m_dwFace;
	FLOAT m_fBary1, m_fBary2;
	FLOAT m_tu, m_tv;
	ActorId m_actorId;
	Vec3 m_worldLoc;
	Vec3 m_actorLoc;
	Vec3 m_normal;

	bool const operator< (Intersection const & other) { return m_fDist < other.m_fDist; }
};

template <class T>
void InitIntersection(Intersection &intersection, DWORD faceIndex, FLOAT dist, FLOAT u, FLOAT v, ActorId actorId, WORD* pIndices, T* pVertices, const Mat4x4& matWorld);

typedef std::vector<Intersection> IntersectionArray;

class ModelClass;

class RayCast
{
public:
	RayCast(Point point, DWORD maxIntersections = 16);

	DWORD m_maxIntersections;
	DWORD m_numIntersections;

	bool m_bUseD3DIntersect;
	bool m_bAllHits;
	Point m_point;

	Vec3 m_vPickRayDir;
	Vec3 m_vPickRayOrigin;

	IntersectionArray m_intersectionArray;

	void Sort();

	HRESULT Pick(Scene *pScene, ActorId actorId, ModelClass* pMesh);
};