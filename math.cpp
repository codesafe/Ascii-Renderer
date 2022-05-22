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


void Identity(MAT* pOut)
{
	pOut->_11 = 1.0f;
	pOut->_12 = 0.0f;
	pOut->_13 = 0.0f;
	pOut->_14 = 0.0f;

	pOut->_21 = 0.0f;
	pOut->_22 = 1.0f;
	pOut->_23 = 0.0f;
	pOut->_24 = 0.0f;

	pOut->_31 = 0.0f;
	pOut->_32 = 0.0f;
	pOut->_33 = 1.0f;
	pOut->_34 = 0.0f;

	pOut->_41 = 0.0f;
	pOut->_42 = 0.0f;
	pOut->_43 = 0.0f;
	pOut->_44 = 1.0f;
}

// 이동
void Translation(MAT* pOut, float x, float y, float z)
{
	pOut->_41 = x;
	pOut->_42 = y;
	pOut->_43 = z;
}

void Transform(Vec3* pOut, const Vec3& v, const MAT& m)
{
	pOut->x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
	pOut->y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
	pOut->z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;

}

void Transform4(Vec4* pOut, const Vec3& v, const MAT& m)
{
	pOut->x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
	pOut->y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
	pOut->z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;
	pOut->w = v.x * m._14 + v.y * m._24 + v.z * m._34 + m._44;

}

void Multiply(MAT* pOut, const MAT& m1, const MAT& m2)
{
	pOut->_11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41;
	pOut->_12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42;
	pOut->_13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43;
	pOut->_14 = m1._11 * m2._13 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44;

	pOut->_21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41;
	pOut->_22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42;
	pOut->_23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43;
	pOut->_24 = m1._21 * m2._13 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44;

	pOut->_31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41;
	pOut->_32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42;
	pOut->_33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43;
	pOut->_34 = m1._31 * m2._13 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44;

	pOut->_41 = m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41;
	pOut->_42 = m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42;
	pOut->_43 = m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43;
	pOut->_44 = m1._41 * m2._13 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44;
}

// view
void MatrixLookAtLH(MAT* out, const Vec3& eye, const Vec3& at, const Vec3& up)
{
	Vec3 zaxis; // forward;
	zaxis = at - eye;
	Normalize(&zaxis);

	Vec3 _up(0, 1, 0);

	Vec3 xaxis;// right;
	Cross(&xaxis, _up, zaxis);
	Normalize(&xaxis);

	Vec3 yaxis;//upVector;
	Cross(&yaxis, zaxis, xaxis);
	Normalize(&yaxis);

	out->_11 = xaxis.x;
	out->_21 = xaxis.y;
	out->_31 = xaxis.z;
	out->_41 = -Dot(xaxis, eye);

	out->_12 = yaxis.x;
	out->_22 = yaxis.y;
	out->_32 = yaxis.z;
	out->_42 = -Dot(yaxis, eye);

	out->_13 = zaxis.x;
	out->_23 = zaxis.y;
	out->_33 = zaxis.z;
	out->_43 = -Dot(zaxis, eye);

	out->_14 = 0;
	out->_24 = 0;
	out->_34 = 0;
	out->_44 = 1.0f;
}

// perspective
void MatrixPerspectiveFovLH(MAT* out, float fovY, float aspect, float zn, float zf)
{
	//float tanfov = (float)tan(fovY * 0.5f);
	float yScale = 1.0f / (float)tan(fovY * 0.5f);

	out->_11 = yScale / aspect;
	out->_12 = 0;
	out->_13 = 0;
	out->_14 = 0;

	out->_21 = 0;
	out->_22 = yScale;
	out->_23 = 0;
	out->_24 = 0;

	out->_31 = 0;
	out->_32 = 0;
	out->_33 = zf / (zf - zn);
	out->_34 = 1;

	out->_41 = 0;
	out->_42 = 0;
	out->_43 = -zn * zf / (zf - zn);
	out->_44 = 0;
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
void MatrixSetViewPort(MAT* out, float x, float y, float w, float h, float minz, float maxz)
{
	out->_11 = w * 0.5f;
	out->_22 = -h * 0.5f;
	out->_33 = maxz - minz;
	out->_41 = x + w * 0.5f;
	out->_42 = y + h * 0.5f;
	out->_43 = minz;

}

void Transform_Homogenize(Vec3* out, Vec4& in, float x, float y, float w, float h)
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

// to ndc
void PerspectiveDivide(Vec4* out, const Vec4& in)
{
	// x = x/w;
	float rhw = 1 / in.w;
	out->x = in.x * rhw;
	out->y = in.y * rhw;
	out->z = in.z * rhw;
	out->w = in.w;
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
	m->_11 = 1 - 2 * y * y - 2 * z * z;
	m->_21 = 2 * x * y - 2 * w * z;
	m->_31 = 2 * x * z + 2 * w * y;
	m->_12 = 2 * x * y + 2 * w * z;
	m->_22 = 1 - 2 * x * x - 2 * z * z;
	m->_32 = 2 * y * z - 2 * w * x;
	m->_13 = 2 * x * z - 2 * w * y;
	m->_24 = 2 * y * z + 2 * w * x;
	m->_33 = 1 - 2 * x * x - 2 * y * y;
	m->_14 = m->_24 = m->_34 = 0.0f;
	m->_41 = m->_42 = m->_43 = 0.0f;
	m->_44 = 1.0f;
}

void MatrixRotationX(MAT* p_out, float rad)
{
	float cs = (float)cos(rad);
	float ss = (float)sin(rad);

	p_out->_11 = 1.0f;	p_out->_12 = 0.0f;	p_out->_13 = 0.0f;		p_out->_14 = 0.0f;
	p_out->_21 = 0.0f;	p_out->_22 = cs;	p_out->_23 = ss;		p_out->_24 = 0.0f;
	p_out->_31 = 0.0f;	p_out->_32 = -ss;	p_out->_33 = cs;		p_out->_34 = 0.0f;
	p_out->_41 = 0.0f;	p_out->_42 = 0.0f;	p_out->_43 = 0.0f;		p_out->_44 = 1.0f;
}


void MatrixRotationY(MAT* p_out, float rad)
{
	float cs = (float)cos(rad);
	float ss = (float)sin(rad);

	p_out->_11 = cs;	p_out->_12 = 0.0f;	p_out->_13 = -ss;		p_out->_14 = 0.0f;
	p_out->_21 = 0.0f;	p_out->_22 = 1.0f;	p_out->_23 = 0.0f;		p_out->_24 = 0.0f;
	p_out->_31 = ss;	p_out->_32 = 0.0f;	p_out->_33 = cs;		p_out->_34 = 0.0f;
	p_out->_41 = 0.0f;	p_out->_42 = 0.0f;	p_out->_43 = 0.0f;		p_out->_44 = 1.0f;
}

void MatrixRotationZ(MAT* p_out, float rad)
{
	float cs = (float)cos(rad);
	float ss = (float)sin(rad);

	p_out->_11 = cs;	p_out->_12 = ss;	p_out->_13 = 0.0f;		p_out->_14 = 0.0f;
	p_out->_21 = -ss;	p_out->_22 = cs;	p_out->_23 = 0.0f;		p_out->_24 = 0.0f;
	p_out->_31 = 0.0f;	p_out->_32 = 0.0f;	p_out->_33 = 1.0f;		p_out->_34 = 0.0f;
	p_out->_41 = 0.0f;	p_out->_42 = 0.0f;	p_out->_43 = 0.0f;		p_out->_44 = 1.0f;
}

float lerp(float v1, float v2, float t)
{
	return v1 + (v2 - v1) * t;
}