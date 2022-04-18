#include <math.h>
#include "math.h"

void Subtract(Vec3* out, const Vec3& v1, const Vec3& v2)
{
	out->x = v1.x - v2.x;
	out->y = v1.y - v2.y;
	out->z = v1.z - v2.z;
}

void Normalize(Vec3* vec)
{
	float mag = 1.0f / (float)sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	vec->x *= mag;
	vec->y *= mag;
	vec->z *= mag;
}

float Dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void Cross(Vec3* out, const Vec3& v1, const Vec3& v2)
{
	out->x = v1.y * v2.z - v1.z * v2.y;
	out->y = v1.z * v2.x - v1.x * v2.z;
	out->z = v1.x * v2.y - v1.y * v2.x;
}


void Identity(MAT* m)
{
	m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
	m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}

// ÀÌµ¿
void Translation(MAT* m, float x, float y, float z)
{
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}

void Transform(Vec3* out, const Vec3& v, const MAT& m)
{
// 	out->x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
// 	out->y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
// 	out->z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;
}

void Transform4(Vec4* out, const Vec4& v, const MAT& mat)
{
	float X = v.x, Y = v.y, Z = v.z, W = v.w;
	out->x = X * mat.m[0][0] + Y * mat.m[1][0] + Z * mat.m[2][0] + W * mat.m[3][0];
	out->y = X * mat.m[0][1] + Y * mat.m[1][1] + Z * mat.m[2][1] + W * mat.m[3][1];
	out->z = X * mat.m[0][2] + Y * mat.m[1][2] + Z * mat.m[2][2] + W * mat.m[3][2];
	out->w = X * mat.m[0][3] + Y * mat.m[1][3] + Z * mat.m[2][3] + W * mat.m[3][3];
}

void Multiply(MAT* out, const MAT& m1, const MAT& m2)
{
	for (int i = 0; i < 4; i++) 
		for (int j = 0; j < 4; j++) 
		{
			out->m[j][i] = 
				(m1.m[j][0] * m2.m[0][i]) +
				(m1.m[j][1] * m2.m[1][i]) +
				(m1.m[j][2] * m2.m[2][i]) +
				(m1.m[j][3] * m2.m[3][i]);
		}
}

// view
void MatrixLookAtLH(MAT* out, const Vec3& eye, const Vec3& at, const Vec3& up)
{
	Vec3 forward;
	Subtract(&forward, at, eye);
	Normalize(&forward);

	Vec3 _up;
	_up.x = 0;	_up.y = 1;	_up.z = 0;

	Vec3 right;
	Cross(&right, _up, forward);
	Normalize(&right);

	Vec3 upVector;
	Cross(&upVector, forward, right);
	Normalize(&upVector);

	out->m[0][0] = right.x;
	out->m[1][0] = right.y;
	out->m[2][0] = right.z;
	out->m[3][0] = -Dot(right, eye);

	out->m[0][1] = up.x;
	out->m[1][1] = up.y;
	out->m[2][1] = up.z;
	out->m[3][1] = -Dot(up, eye);

	out->m[0][2] = forward.x;
	out->m[1][2] = forward.y;
	out->m[2][2] = forward.z;
	out->m[3][2] = -Dot(forward, eye);

	out->m[0][3] = out->m[1][3] = out->m[2][3] = 0.0f;
	out->m[3][3] = 1.0f;
}

// perspective
void MatrixPerspectiveFovLH(MAT* out, float fovY, float aspect, float zn, float zf)
{
	float tanfov = (float)tan(fovY * 0.5f);
	out->m[0][0] = (1.0f / tanfov) / aspect;
	out->m[1][0] = 0;
	out->m[2][0] = 0;
	out->m[3][0] = 0;

	out->m[0][1] = 0;
	out->m[1][1] = (1.0f / tanfov);
	out->m[2][1] = 0;
	out->m[3][1] = 0;

	out->m[0][2] = 0;
	out->m[1][2] = 0;
	out->m[2][2] = zf / (zf - zn);
	out->m[3][2] = -zn * zf / (zf - zn);

	out->m[0][3] = 0;
	out->m[1][3] = 0;
	out->m[2][3] = 1;
	out->m[3][3] = 0;
}

void MatrixPerspectiveFovRH(MAT* out, float fovY, float aspect, float zn, float zf)
{
// 	float tanfov = (float)tan(fovY * 0.5f);
// 
// 	out->_00 = (1.0f / tanfov) / aspect;
// 	out->_01 = 0.0f;
// 	out->_02 = 0.0f;
// 	out->_03 = 0.0f;
// 
// 	out->_10 = 0.0f;
// 	out->_11 = (1.0f / tanfov);
// 	out->_12 = 0.0f;
// 	out->_13 = 0.0f;
// 
// 	out->_20 = 0.0f;
// 	out->_21 = 0.0f;
// 	out->_22 = zf / (zf - zn);
// 	out->_23 = -zn * zf / (zf - zn);
// 
// 	out->_30 = 0.0f;
// 	out->_31 = 0.0f;
// 	out->_32 = 1.0f;
// 	out->_33 = 0.0f;
}


void MatrixSetViewPort(MAT* out, float x, float y, float w, float h)
{
// 	out->_11 = w * 0.5f;
// 	out->_41 = x + w * 0.5f;
// 	out->_22 = -h * 0.5f;
// 	out->_42 = y + h * 0.5f;
}

void Transform_Homoenize(Vec3 *out, Vec4 &in, float x, float y, float w, float h)
{
	float rhw = 1.0f / in.w;
	out->x = (in.x * rhw + 1.0f) * (x + w * 0.5f);
	out->y = (1.0f - in.y * rhw) * (y + h * 0.5f);
	out->z = in.z * rhw;
	//out->w = 1.0f;

// 	float rhw = 1.0f / in.w;
// 	out->x = in.x * rhw;
// 	out->y = in.y * rhw;
// 	out->z = in.z * rhw;
// 	//out->w = 1;
}