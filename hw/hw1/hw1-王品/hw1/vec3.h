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

	inline vec3 operator*(const float t)const;
	inline vec3 operator*(const vec3 &v2)const;
	inline vec3 operator/(const float t)const;
	inline vec3 operator+(const vec3 &v2)const;
	inline vec3 operator-(const vec3 &v2)const;

	//inline vec3& operator*=(const float t);
	//inline vec3& operator/=(const float t);
   
	inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	inline void make_unit_vector();

	float e[3];
};


inline std::ostream& operator<<(std::ostream &os, const vec3 &t) 
{
   
}

inline vec3& vec3:: operator+=(const vec3 &v2){
	float v_2 = 0.0;
	for (int i = 0;i<3;i++){
       if(i==0){
		   v_2 = v2.x();
	   } 

       else if(i==1){
		   v_2 = v2.y();
	   }
       else if (i==2){
		   v_2 = v2.z();
	   }
	   e[i] += v_2;
	}
	return *this;
}

inline vec3& vec3:: operator-=(const vec3 &v2){
    float v_2 = 0.0;
	for (int i = 0;i<3;i++){
      if(i==0){
		   v_2 = v2.x();
	   } 

       else if(i==1){
		   v_2 = v2.y();
	   }
       else if (i==2){
		   v_2 = v2.z();
	   }
	   e[i] -= v_2;
	}
	return *this;
}

inline vec3& vec3:: operator*=(const vec3 &v2){
      
}

inline vec3& vec3:: operator/=(const vec3 &v2){
     
}
inline vec3 vec3:: operator*(const float t)const{
	return vec3(e[0]*t,e[1]*t,e[2]*t);
}

inline vec3 vec3:: operator*(const vec3 &v2)const{
     return vec3(e[0]*v2.r(),e[1]*v2.g(),e[2]*v2.b());

}



inline vec3 vec3:: operator/(const float t)const{
	return vec3(e[0]/t,e[1]/t,e[2]/t);
}

inline vec3 vec3:: operator+(const vec3 &v2)const{
   return vec3(e[0]+v2.x(),e[1]+v2.y(),e[2]+v2.z());  	
}
inline vec3 vec3:: operator-(const vec3 &v2)const{
   return vec3(e[0]-v2.x(),e[1]-v2.y(),e[2]-v2.z());  	
}

/*
inline vec3& vec3:: operator*=(const float t){
	
}
*/
/*
inline vec3& vec3:: operator/=(const float t){
	
}
*/

inline void vec3::make_unit_vector() 
{
      
}

inline float dot(const vec3 &v1, const vec3 &v2)  
{
	float dot_product = v1.x()* v2.x() + v1.y() * v2.y() + v1.z()*v2.z();
	return dot_product;
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) 
{
	float Nx = v1.y() * v2.z() - v1.z() * v2.y();
	float Ny = v1.z() * v2.x() - v1.x() * v2.z();
	float Nz = v1.x() * v2.y() - v1.y() * v2.x();
	vec3 N(Nx,Ny,Nz);
	return N;
}

inline vec3 unit_vector(vec3 v) 
{
	float len = v.length();
	
	return vec3(v.x()/len,v.y()/len,v.z()/len);  	
}

#endif
