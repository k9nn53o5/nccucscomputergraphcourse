//================================================================================================= 
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {

public:
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }
	float r() const { return e[0]; }
	float g() const { return e[1]; }
	float b() const { return e[2]; }


	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator-=(const vec3 &v2);
	inline vec3& operator*=(const vec3 &v2);
	inline vec3& operator/=(const vec3 &v2);
	inline vec3 operator+(vec3 v2);
	inline vec3 operator-(const vec3 v2)const;

	inline vec3& operator*=(const float t);
	inline vec3& operator/=(const float t);
	inline vec3 operator*(float t);
	inline vec3 operator/(float t);

	inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	inline void make_unit_vector();

	float e[3];
};


inline std::ostream& operator<<(std::ostream &os, const vec3 &t) 
{

}

inline vec3 vec3::operator+(vec3 v2)
{
	vec3 tempv(e[0]+ v2.e[0], e[1] + v2.e[1], e[2] + v2.e[2]);
	return tempv;
}
inline vec3 vec3::operator-(const vec3 v2)const
{
	vec3 tempv(e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2]);
	return tempv;
}

inline vec3 vec3::operator*(float t)
{	
	vec3 tempv(e[0] * t, e[1] * t, e[2] * t);
	return tempv;
}

inline vec3 vec3::operator/(float t)
{
	vec3 tempv(e[0] / t, e[1] / t, e[2] / t);
	return tempv;
}

inline void vec3::make_unit_vector()
{
	this->e[0] = this->e[0] / this->length();
	this->e[1] = this->e[1] / this->length();
	this->e[2] = this->e[2] / this->length();
}

inline float dot(const vec3 &v1, const vec3 &v2) 
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) 
{
	vec3 vnew(v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1] , v1.e[2]*v2.e[0]-v1.e[0]*v2.e[2] , v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0] );
	return vnew;
}

inline vec3 unit_vector(vec3 v) 
{
	vec3 vnew(v.e[0]/v.length() , v.e[1]/v.length() , v.e[2]/v.length());
	return vnew;
}

#endif