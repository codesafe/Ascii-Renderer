#ifndef MATH_H
#define MATH_H

#define	_PI			3.141592f
#define _DEGREE		(3.141592f / 180.0f)

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