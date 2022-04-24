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
	float m[4][4]; 

	MAT operator*(const MAT& m) const
	{
		MAT result = *this;
		return result *= m;
	}

	MAT operator*=(const MAT& m2)
	{
		MAT out;
		MAT m1 = *this;

		out.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
		out.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
		out.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
		out.m[0][3] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

		out.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
		out.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
		out.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
		out.m[1][3] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

		out.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
		out.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
		out.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
		out.m[2][3] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

		out.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
		out.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
		out.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
		out.m[3][3] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

		return out;
	}

	Vec3 operator*(const Vec3& v)
	{
		Vec3 out;
		out.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
		out.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
		out.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
		return out;
	}
};

void Identity(MAT *m);
void Translation(MAT* m, float x, float y, float z);
void Transform(Vec3* out, const Vec3& v, const MAT& m);
void Transform4(Vec4* out, const Vec3& v, const MAT& m);
void Multiply(MAT* out, const MAT& m1, const MAT& m2);
void MatRotate(MAT* m, float x, float y, float z, float theta);
void MatrixRotationY(MAT* pOut, float rad);

void MatrixLookAtLH(MAT* out, const Vec3& eye, const Vec3& at, const Vec3& up);
void MatrixPerspectiveFovLH(MAT* out, float fovY, float aspect, float zn, float zf);
void MatrixPerspectiveFovRH(MAT* out, float fovY, float aspect, float zn, float zf);

void MatrixSetViewPort(MAT* out, float x, float y, float w, float h);
void Transform_Homogenize(Vec3* out, Vec4& in, float x, float y, float w, float h);
void PerspectiveDivide(Vec3* out, const Vec4 &in);

#endif