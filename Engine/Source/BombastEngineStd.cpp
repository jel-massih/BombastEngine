#include "BombastEngineStd.h"

Vec4 g_White = { 1.0f, 1.0f, 1.0f, fOPAQUE };
Vec4 g_Black = { 0.0f, 0.0f, 0.0f, fOPAQUE };

Vec3 g_Right(1.0f, 0.0f, 0.0f);
Vec3 g_Up(0.0f, 1.0f, 0.0f);
Vec3 g_Forward(0.0f, 0.0f, 1.0f);

Vec4 g_Up4(g_Up.x, g_Up.y, g_Up.z, 0.0f);
Vec4 g_Right4(g_Right.x, g_Right.y, g_Right.z, 0.0f);
Vec4 g_Forward4(g_Forward.x, g_Forward.y, g_Forward.z, 0.0f);

const float fOPAQUE = 1.0f;
const int iOPAQUE = 1;
const float fTRANSPARENT = 0.0f;
const int iTRANSPARENT = 0;