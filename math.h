#ifndef MATH_H
#define MATH_H

#include <algorithm>


struct Vec3
{
	float x, y, z;
	Vec3()
	{
		x = y = z = 0;
	}

	Vec3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3	operator+(const Vec3& rhs) const
	{
		return Vec3(*this) += rhs;
	}

	Vec3 operator-(const Vec3& rhs) const
	{
		return Vec3(*this) -= rhs;
	}

	Vec3	operator*(const float rhs) const
	{
		return Vec3(*this) *= rhs;
	}

	Vec3	operator/(const float rhs) const
	{
		return Vec3(*this) /= rhs;
	}


	Vec3& operator+=(const Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	Vec3& operator-=(const Vec3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vec3& operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	Vec3& operator/=(const float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
};

struct Vec4
{
	float x, y, z, w;
	Vec4()
	{
		x = y = z = w = 0;
	}

	Vec4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

};

void Subtract(Vec3* out, const Vec3& v1, const Vec3& v2);
void Normalize(Vec3* vec);
float Dot(const Vec3& v1, const Vec3& v2);
void Cross(Vec3* out, const Vec3& v1, const Vec3& v2);


////////////////////////////////////////////////////////////////////////////////////


struct MAT 
{ 
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	MAT operator*(const MAT& m) const
	{
		MAT result = *this;
		return result *= m;
	}

	MAT operator*=(const MAT& m2)
	{
		MAT pOut;
		MAT m1 = *this;

		pOut._11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41;
		pOut._12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42;
		pOut._13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43;
		pOut._14 = m1._11 * m2._13 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44;

		pOut._21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41;
		pOut._22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42;
		pOut._23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43;
		pOut._24 = m1._21 * m2._13 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44;

		pOut._31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41;
		pOut._32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42;
		pOut._33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43;
		pOut._34 = m1._31 * m2._13 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44;

		pOut._41 = m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41;
		pOut._42 = m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42;
		pOut._43 = m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43;
		pOut._44 = m1._41 * m2._13 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44;

		return pOut;
	}

	Vec3 operator*(const Vec3& v)
	{
		Vec3 out;
		out.x = v.x * _11 + v.y * _21 + v.z * _31 + _41;
		out.y = v.x * _12 + v.y * _22 + v.z * _32 + _42;
		out.z = v.x * _13 + v.y * _23 + v.z * _33 + _43;
		return out;
	}
};

void Identity(MAT *m);
void Translation(MAT* m, float x, float y, float z);
void Transform(Vec3* out, const Vec3& v, const MAT& m);
void Transform4(Vec4* out, const Vec3& v, const MAT& m);
void Multiply(MAT* out, const MAT& m1, const MAT& m2);
void MatRotate(MAT* m, float x, float y, float z, float theta);

void MatrixRotationX(MAT* pOut, float rad);
void MatrixRotationY(MAT* pOut, float rad);
void MatrixRotationZ(MAT* pOut, float rad);

void MatrixLookAtLH(MAT* out, const Vec3& eye, const Vec3& at, const Vec3& up);
void MatrixPerspectiveFovLH(MAT* out, float fovY, float aspect, float zn, float zf);
void MatrixPerspectiveFovRH(MAT* out, float fovY, float aspect, float zn, float zf);

void MatrixSetViewPort(MAT* out, float x, float y, float w, float h, float minz, float maxz);
void Transform_Homogenize(Vec3* out, Vec4& in, float x, float y, float w, float h);
void PerspectiveDivide(Vec3* out, const Vec4 &in);


////////////////////////////////////////////////////////////////////////////////////

float lerp(float v1, float v2, float t);

struct EDGE
{
	float x, z, w;
	float r, g, b, a;
	float u, v;
};

#endif