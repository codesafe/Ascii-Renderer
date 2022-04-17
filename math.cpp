#include <math.h>
#include "math.h"

void Subtract(Vec3* out, const Vec3& v1, const Vec3& v2)
{
	out->x = v1.x - v2.x;
	out->y = v1.y - v2.y;
	out->z = v1.z - v2.z;
}

void Normalize(Vec3* out, const Vec3& v)
{
	float mag = 1.0f / (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	out->x = v.x * mag;
	out->y = v.y * mag;
	out->z = v.z * mag;
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
	m->_11 = 1.0f;
	m->_12 = 0.0f;
	m->_13 = 0.0f;
	m->_14 = 0.0f;

	m->_21 = 0.0f;
	m->_22 = 1.0f;
	m->_23 = 0.0f;
	m->_24 = 0.0f;

	m->_31 = 0.0f;
	m->_32 = 0.0f;
	m->_33 = 1.0f;
	m->_34 = 0.0f;

	m->_41 = 0.0f;
	m->_42 = 0.0f;
	m->_43 = 0.0f;
	m->_44 = 1.0f;
}

// ÀÌµ¿
void Translation(MAT* m, float x, float y, float z)
{
	m->_41 = x;
	m->_42 = y;
	m->_43 = z;
}

void Multiply(MAT* out, const MAT& m1, const MAT& m2)
{
	out->_11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41;
	out->_12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42;
	out->_13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43;
	out->_14 = m1._11 * m2._13 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44;

	out->_21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41;
	out->_22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42;
	out->_23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43;
	out->_24 = m1._21 * m2._13 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44;

	out->_31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41;
	out->_32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42;
	out->_33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43;
	out->_34 = m1._31 * m2._13 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44;

	out->_41 = m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41;
	out->_42 = m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42;
	out->_43 = m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43;
	out->_44 = m1._41 * m2._13 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44;
}

// view
void MatrixLookAtLH(MAT* out, const Vec3& eye, const Vec3& at, const Vec3& up)
{
	Vec3 forward;
	Subtract(&forward, eye, at);
	Normalize(&forward, forward);

	Vec3 _up;
	_up.x = 0;	_up.y = 1;	_up.z = 0;

	Vec3 right;
	Cross(&right, forward, _up);

	Vec3 upVector;
	Cross(&upVector, forward, _up);

	out->_11 = right.x;
	out->_11 = right.y;
	out->_13 = right.z;
	out->_21 = up.x;
	out->_22 = up.y;
	out->_23 = up.z;
	out->_31 = forward.x;
	out->_32 = forward.y;
	out->_33 = forward.z;
	out->_41 = eye.x;
	out->_42 = eye.y;
	out->_43 = eye.z;
}

// perspective
void MatrixPerspectiveFovLH(MAT* out, float fovY, float aspect, float zn, float zf)
{
	float tanfov = (float)tan(fovY * 0.5f);

	out->_11 = (1.0f / tanfov) / aspect;
	out->_12 = 0.0f;
	out->_13 = 0.0f;
	out->_14 = 0.0f;

	out->_21 = 0.0f;
	out->_22 = (1.0f / tanfov);
	out->_23 = 0.0f;
	out->_24 = 0.0f;

	out->_31 = 0.0f;
	out->_32 = 0.0f;
	out->_33 = zf / (zf - zn);
	out->_34 = 1.0f;

	out->_41 = 0.0f;
	out->_42 = 0.0f;
	out->_43 = -zn * zf / (zf - zn);
	out->_44 = 0.0f;

}