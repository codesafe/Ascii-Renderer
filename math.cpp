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

// 이동
void Translation(MAT* m, float x, float y, float z)
{
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}

void Transform(Vec3* out, const Vec3& v, const MAT& mat)
{
	out->x = v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + mat.m[3][0];
	out->y = v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + mat.m[3][1];
	out->z = v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + mat.m[3][2];
}

void Transform4(Vec4* out, const Vec3& v, const MAT& mat)
{
	out->x = v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + mat.m[3][0];
	out->y = v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + mat.m[3][1];
	out->z = v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + mat.m[3][2];
	out->w = v.x * mat.m[0][3] + v.y * mat.m[1][3] + v.z * mat.m[2][3] + mat.m[3][3];
}

void Multiply(MAT* out, const MAT& m1, const MAT& m2)
{
	out->m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	out->m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	out->m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	out->m[0][3] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	out->m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	out->m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	out->m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	out->m[1][3] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	out->m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	out->m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	out->m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	out->m[2][3] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	out->m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	out->m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	out->m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	out->m[3][3] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
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
	//out->m[3][0] = eye.x; //-Dot(right, eye);
	out->m[3][0] = -Dot(right, eye);

	out->m[0][1] = up.x;
	out->m[1][1] = up.y;
	out->m[2][1] = up.z;
	//out->m[3][1] = eye.y; //-Dot(up, eye);
	out->m[3][1] = -Dot(up, eye);

	out->m[0][2] = forward.x;
	out->m[1][2] = forward.y;
	out->m[2][2] = forward.z;
	//out->m[3][2] = eye.z; //-Dot(forward, eye);
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

// view Port 구성은 gl과 완전 다르다.
// dx : https://docs.microsoft.com/ko-kr/windows/win32/direct3d9/viewports-and-clipping
// gl : https://gofo-coding.tistory.com/entry/Rendering-Pipeline-Modeling-Viewing-Projection-Viewport#title-27
void MatrixSetViewPort(MAT* out, float x, float y, float w, float h)
{
	out->m[0][0] = w * 0.5f;
	out->m[1][1] = -h * 0.5f;
	out->m[2][2] = 1;
	out->m[3][0] = x + w * 0.5f;
	out->m[3][1] = y + h * 0.5f;
	out->m[3][2] = 0;

// 	out->m[0][0] = w * 0.5f;
// 	out->m[0][3] = x + w * 0.5f;
// 	out->m[1][1] = -h * 0.5f;
// 	out->m[1][3] = y + h * 0.5f;
// 	out->m[2][2] = 1;
// 	out->m[2][3] = 0;
}

void Transform_Homogenize(Vec3 *out, Vec4 &in, float x, float y, float w, float h)
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

void PerspectiveDivide(Vec3* out, const Vec4 &in)
{
	float rhw = 1 / in.w;
	out->x = in.x * rhw;
	out->y = in.y * rhw;
	out->z = in.z * rhw;
}

void MatRotate(MAT* m, float x, float y, float z, float theta) 
{
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	Vec3 vec(x, y, z);

	float w = qcos;
	Normalize(&vec);

	x = vec.x * qsin;
	y = vec.y * qsin;
	z = vec.z * qsin;
	m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m->m[1][0] = 2 * x * y - 2 * w * z;
	m->m[2][0] = 2 * x * z + 2 * w * y;
	m->m[0][1] = 2 * x * y + 2 * w * z;
	m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m->m[2][1] = 2 * y * z - 2 * w * x;
	m->m[0][2] = 2 * x * z - 2 * w * y;
	m->m[1][2] = 2 * y * z + 2 * w * x;
	m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
	m->m[3][3] = 1.0f;
}


void MatrixRotationY(MAT* pOut, float rad)
{
	float cs = (float)cos(rad);
	float ss = (float)sin(rad);

	pOut->m[0][0] = cs;
	pOut->m[0][2] = -ss;
	pOut->m[2][0] = ss;
	pOut->m[2][2] = cs;
}

