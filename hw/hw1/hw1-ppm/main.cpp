#include <iostream>
#include <fstream>
#include <math.h>
#include "vec3.h"

using namespace std;

class ray {
public:
	ray() {}
	ray(const vec3& a, const vec3& b) { O = a; D = b; }
	vec3 origin() const { return O; }
	vec3 direction() const { return D; }
	vec3 point_at_parameter(float t) const {
		vec3 pnew(O.e[0] + t * D.e[0], O.e[1] + t * D.e[1], O.e[2] + t * D.e[2]);
		return pnew;
	}  //??

	vec3 O;
	vec3 D;

};

vec3 color(const ray& r);

int main()
{
	int width = 200;
	int height = 100;
	vec3 lower_left_corner(-2, -1, -1);
	vec3 origin(0, 0, 0);
	vec3 horizontal(4, 0, 0);
	vec3 vertical(0, 2, 0);
	fstream file;
	//file.open("C:\\Users\k9nn5\source\repos\Project1\Project1\ray.ppm", ios::out);
	file.open("ray.ppm", ios::out);

	file << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float u = float(i) / float(width);
			float v = float(j) / float(height);
			
			ray r(origin , lower_left_corner + horizontal*u + vertical*v);
			vec3 col = color(r);
			
			//float f = float(i) / float(width);
			//vec3 col(u, v, f);
			file << int(col.e[0] * 255) << " " << int(col.e[1] * 255) << " " << int(col	.e[2] * 255) << "\n";

		}
	}

	return 0;
}

float  hit_sphere(const vec3 &center, float radius, const ray& r) {
	float t,a,b,c,judge;
	a = dot(r.D, r.D);
	b = dot((r.O - center), r.D)*2.0;
	c = dot(r.O - center, r.O - center) - radius * radius;
	judge = b * b - 4 * a * c;
	if (judge < 0) {
		return -1;
	}
	t = ((-b) - sqrt(b*b - 4 * a*c)) / (2 * a);
	return t;
}

vec3 color(const ray& r)
{	
	float u = hit_sphere(vec3(0, 0, -1), 0.5, r);
	vec3 center(0, 0, -1);
	vec3 Lightsource(1,1,0);
	if (u > 0.0) {
		vec3 N = unit_vector(r.point_at_parameter(u) - center);
		//return vec3(N.x() + 1, N.y() + 1, N.z() + 1)*0.5;
		vec3 L = unit_vector(Lightsource - r.point_at_parameter(u));
		vec3 I = vec3(1, 1, 1);
		return I * sqrt(dot(N,L)*dot(N,L));
	}
	vec3 unit_direction = unit_vector(r.direction());
	float	v = 0.5*(unit_direction.y() + 1.0);
	return vec3(1, 1, 1)*float(1.0 - v) + vec3(0.5, 0.7, 1.0)*v;
}
