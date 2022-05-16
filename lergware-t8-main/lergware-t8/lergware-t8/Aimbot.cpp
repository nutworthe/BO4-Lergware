#include "Aimbot.h"

vec3_t SubtractVec(vec3_t src, vec3_t dest)
{
	vec3_t vec;
	vec.x = 0;
	vec.y = 0;
	vec.z = 0;
	vec.x = dest.x - src.x;
	vec.y = dest.y - src.y;
	vec.z = dest.z - src.z;

	return vec;
}

float GetMagnitude(vec3_t vec)
{
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

vec3_t NormalizeVec(vec3_t vec)
{
	float Length = GetMagnitude(vec);

	vec.x /= Length;
	vec.y /= Length;
	vec.z /= Length;

	return vec;
}

vec3_t MultiplyVec(vec3_t src, vec3_t dest)
{
	src.x *= dest.x;
	src.y *= dest.y;
	src.z *= dest.z;

	return src;
}
float distanceBetweenTwoPoints(float x, float y, float a, float b) {
	return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

void ClampAngles(vec3_t& angles)
{
	while (angles.x > 180.0f)
		angles.x -= 360.0f;

	while (angles.x < -180.0f)
		angles.x += 360.0f;

	while (angles.y > 180.0f)
		angles.y -= 360.0f;

	while (angles.y < -180.0f)
		angles.y += 360.0f;

	if (angles.z != 0.0f)
		angles.z = 0.0f;
}

void VectorAngles(vec3_t direction, vec3_t& angles)
{
	float flTemp, flYaw, flPitch;

	if (direction.x == 0.0f && direction.y == 0.0f)
	{
		flYaw = 0.0f;

		if (direction.z > 0.0f)
			flPitch = 90.0f;

		else
			flPitch = 270.0f;
	}

	else
	{
		flYaw = RadiansToDegrees(atan2f(direction.y, direction.x));

		if (flYaw < 0.0f)
			flYaw += 360.0f;

		flTemp = sqrtf(direction.x * direction.x + direction.y * direction.y);
		flPitch = RadiansToDegrees(atan2f(direction.z, flTemp));

		if (flPitch < 0.0f)
			flPitch += 360.0f;
	}

	angles.x = -flPitch;
	angles.y = flYaw;
	angles.z = 0.0f;
}

namespace aim
{
	bool need_shoot = false;
	

	void ClampAngles(int angles[2])
	{
		while (angles[0] > 180.0f)
			angles[0] -= 360.0f;

		while (angles[0] < -180.0f)
			angles[0] += 360.0f;

		while (angles[1] > 180.0f)
			angles[1] -= 360.0f;

		while (angles[1] < -180.0f)
			angles[1] += 360.0f;
	}

	vec3_t customScan(player player) {
		vec3_t bestpos{};
		auto bones = player.bone_array;
		auto centity_t = player.address;

		vec3_t ViewOrigin = { *(float*)(cgArray + 1479192) , *(float*)(cgArray + 1479196) , *(float*)(cgArray + 1479200) };

		for (size_t i = 0; i < IM_ARRAYSIZE(boneSel); i++) {
			if (selected[i]) {
				if (boneSel[i] == "j_head") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_headpos)) {
						bestpos = bones.j_headpos;
					}
				}
				else if (boneSel[i] == "j_neck") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_neckpos)) {
						bestpos = bones.j_neckpos;
					}
				}
				else if (boneSel[i] == "j_spineupper") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_spineupperpos)) {
						bestpos = bones.j_spineupperpos;
					}
				}
				else if (boneSel[i] == "j_spinelower") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_spinelowerpos)) {
						bestpos = bones.j_spinelowerpos;
					}
				}
				else if (boneSel[i] == "j_shoulder_le") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_shoulder_lepos)) {
						bestpos = bones.j_shoulder_lepos;
					}
				}
				else if (boneSel[i] == "j_shoulder_ri") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_shoulder_ripos)) {
						bestpos = bones.j_shoulder_ripos;
					}
				}
				else if (boneSel[i] == "j_knee_le") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_knee_lepos)) {
						bestpos = bones.j_knee_lepos;
					}
				}
				else if (boneSel[i] == "j_knee_ri") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_knee_ripos)) {
						bestpos = bones.j_knee_ripos;
					}
				}
				else if (boneSel[i] == "j_elbow_le") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_elbow_lepos)) {
						bestpos = bones.j_elbow_lepos;
					}
				}
				else if (boneSel[i] == "j_elbow_ri") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_elbow_ripos)) {
						bestpos = bones.j_elbow_ripos;
					}
				}
				else if (boneSel[i] == "j_wrist_le") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_wrist_lepos)) {
						bestpos = bones.j_wrist_lepos;
					}
				}
				else if (boneSel[i] == "j_wrist_ri") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_wrist_ripos)) {
						bestpos = bones.j_wrist_ripos;
					}
				}
				else if (boneSel[i] == "j_ankle_le") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_ankle_lepos)) {
						bestpos = bones.j_ankle_lepos;
					}
				}
				else if (boneSel[i] == "j_ankle_ri") {
					if (IsTargetVisible(centity_t, ViewOrigin, bones.j_ankle_ripos)) {
						bestpos = bones.j_ankle_ripos;
					}
				}
				else {
					return { 0, 0, 0 };
				}
			}
		}

		return bestpos;
	}

	vec3_t CalcAngles(vec3_t dest)
	{
		/*vec3_t ViewAxis[3];
		ViewAxis[0].x = *(float*)(cgArray + 1479220);
		ViewAxis[0].y = *(float*)(cgArray + 1479224);
		ViewAxis[0].z = *(float*)(cgArray + 1479228);

		ViewAxis[1].x = *(float*)(cgArray + 1479232);
		ViewAxis[1].y = *(float*)(cgArray + 1479236);
		ViewAxis[1].z = *(float*)(cgArray + 1479240);

		ViewAxis[2].x = *(float*)(cgArray + 1479244);
		ViewAxis[2].y = *(float*)(cgArray + 1479248);
		ViewAxis[2].z = *(float*)(cgArray + 1479252);
		


		aimAt = MultiplyVec(ViewAxis[1], normalized);
		float yaw = (float)(asin(aimAt.x + aimAt.y + aimAt.z) * (180 / M_PI));
		aimAt = MultiplyVec(ViewAxis[2], normalized);
		float pitch = (float)(-asin(aimAt.x + aimAt.y + aimAt.z) * (180 / M_PI));*/

		vec3_t src = { *(float*)(cgArray + 1479192) , *(float*)(cgArray + 1479196) , *(float*)(cgArray + 1479200) };
		vec3_t angles;

		vec3_t aimAt = SubtractVec(src, dest);
		vec3_t normalized = NormalizeVec(aimAt);

		VectorAngles(normalized, angles);

		ClampAngles(angles);

		angles.x -= *(float*)(cgArray + 0x166FD0);
		angles.y -= *(float*)(cgArray + 0x166FD4);

		ClampAngles(angles);

		return angles;
	}

	void Standard(vec3_t position)
	{
		vec3_t ViewAxis[3];
		ViewAxis[0].x = *(float*)(cgArray + 1479220);
		ViewAxis[0].y = *(float*)(cgArray + 1479224);
		ViewAxis[0].z = *(float*)(cgArray + 1479228);

		ViewAxis[1].x = *(float*)(cgArray + 1479232);
		ViewAxis[1].y = *(float*)(cgArray + 1479236);
		ViewAxis[1].z = *(float*)(cgArray + 1479240);

		ViewAxis[2].x = *(float*)(cgArray + 1479244);
		ViewAxis[2].y = *(float*)(cgArray + 1479248);
		ViewAxis[2].z = *(float*)(cgArray + 1479252);

		auto angles = CalcAngles(position);

		GetClientActivet()->viewAngles.x += angles.x;
		GetClientActivet()->viewAngles.y += angles.y;

		if (Auto_Fire)
			aim::need_shoot = true;
	}
}