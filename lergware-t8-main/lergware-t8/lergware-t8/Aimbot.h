#pragma once
#include "includes.h"
#define M_PI       3.14159265358979323846
#define ByteToFloat(a) ((float)(a)/255.0f)
#define FloatToByte(a) ((BYTE)(a)*255.0f)
#define DwordFromBytes(a) (((BYTE)(a)[0])|((BYTE)(a)[1]<<8)|((BYTE)(a)[2]<<16)|((BYTE)(a)[3]<<24))
#define GetSign(a) ((a)?((*(int*)&(a)>>31)|1):0)
#define DegreesToRadians(a) ((a)*((float)M_PI/180.0f))
#define RadiansToDegrees(a) ((a)*(180.0f/(float)M_PI))
#define AngleToShort(a) ((int)((a)*(65536/360.0f))&65535)
#define ShortToAngle(a) ((float)((a)*(360.0f/65536)))
#define AngleNormalize(a) (ShortToAngle(AngleToShort((a))))

namespace aim
{
	extern bool need_shoot;
	vec3_t customScan(player player);
	void ClampAngles(int angles[2]);
	void Standard(vec3_t pos);
	vec3_t CalcAngles(vec3_t dest);
}