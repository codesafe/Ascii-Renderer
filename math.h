#ifndef MATH_H
#define MATH_H

#define	_PI			3.141592f
#define _DEGREE		(3.141592f / 180.0f)

struct Vec3
{
	float x, y, z;
};

void Subtract(Vec3* out, const Vec3& v1, const Vec3& v2);
void Normalize(Vec3* out, const Vec3& v);
float Dot(const Vec3& v1, const Vec3& v2);
void Cross(Vec3* out, const Vec3& v1, const Vec3& v2);


struct MAT
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};



void Identity(MAT *m);
void Translation(MAT* m, float x, float y, float z);
void Transform(Vec3* out, const Vec3& v, const MAT& m);
void Multiply(MAT* out, const MAT& m1, const MAT& m2);
void MatrixLookAtLH(MAT* out, const Vec3& eye, const Vec3& at, const Vec3& up);
void MatrixPerspectiveFovLH(MAT* out, float fovY, float aspect, float zn, float zf);
void MatrixPerspectiveFovRH(MAT* out, float fovY, float aspect, float zn, float zf);

void MatrixSetViewPort(MAT* out, float x, float y, float w, float h);

#endif