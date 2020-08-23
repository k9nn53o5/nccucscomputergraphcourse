#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "vec3.h"

using namespace std;

struct hit_record {
	float reT;
	float reReflect;
	float reTrans;
	vec3 reP;		//ball center
	vec3 reNormal; //normal vector
	vec3 reCol; // color
};

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
private:

};

class sphere{
public:
	sphere() {};
	sphere(vec3 c, float r) : center(c), radius(r), color(vec3(0, 0, 0)), texre(0), textra(0) {};
	sphere(vec3 c, float r, vec3 col) : center(c), radius(r), color(col), texre(0), textra(0) {};
	sphere(vec3 c, float r, vec3 col, float ref) : center(c), radius(r), color(col), texre(ref), textra(0) {};
	sphere(vec3 c, float r, vec3 col, float ref, float tra) : center(c), radius(r), color(col), texre(ref), textra(tra) {};
	float radius;
	vec3 color;
	vec3 center;
	float texre;
	float textra;
	virtual bool hit(const ray& r, hit_record& rec)const;

private:
};

/*
bool sphere::hit(const ray& r,hit_record& rec)const{
	float  a, b, c,judge;
	a = dot(r.D, r.D);
	b = dot((r.O - center), r.D)*2.0;
	c = dot(r.O - center, r.O - center) - radius * radius;
	judge = b * b - 4 * a * c;
	if (judge >= 0  && dot(r.O - center, r.O - center) > 0) {
		float tmin, tmax;
		tmin = (-b - sqrt(b*b - 4*a*c)) / (2*a);
		tmax = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
		if (tmin > 0) {
			rec.reT = tmin;
		}
		else if (tmin < 0 && tmax > 0) {
			rec.reT = tmax; 
			return false; // camara is in the ball !!
		}
		else if (tmin < 0 && tmax <0) {
			rec.reT = -999999;
			return false; //camara is behind the ball !!
		}
		rec.reReflect = texre;
		rec.reTrans = textra;
		rec.reCol = color;
		rec.reP = center;
		
		rec.reNormal = unit_vector(r.point_at_parameter(rec.reT)-center);
		return true;
	}
	else {
		rec.reT = -999999;
		return false;
	}
}

*/

bool sphere::hit(const ray& r, hit_record& rec)const {
	float tmin;
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), (r.origin() - center))*2.0f;
	float c = dot(r.origin() - center, r.origin() - center) - radius * radius;
	float discriminant = b * b - 4.0*a*c;
	if (discriminant > 0 && dot(r.direction(), (center - r.origin())) > 0) {
		if ((-b - sqrt(discriminant)) / (2.0*a) < (-b + sqrt(discriminant)) / (2.0*a) && (-b - sqrt(discriminant)) / (2.0*a) > 0) {
			tmin = (-b - sqrt(discriminant)) / (2.0*a);
		}

		else {
			tmin = (-b + sqrt(discriminant)) / (2.0*a);
		}
		rec.reT = tmin;
		rec.reP = center;
		rec.reCol = color;
		rec.reReflect = texre;
		rec.reTrans = textra;
		return true;
	}
	else {
		rec.reT = 1e9;
		return false;
	}

}
vec3 color(const ray& r,vector<sphere> able,int step) {
	int tar;
	int ballnum = able.size();
	hit_record* re = new hit_record[ballnum];
	hit_record* shadre = new hit_record[ballnum];
	bool shoot = false;
	bool ishoot = false;
	bool inside = false;

	for (int i = 0;i < ballnum;i++) {
		if (able[i].hit(r, re[i])) {
			ishoot = true;		
		}
	}
	if (ishoot && step < 5) {
		//prepare vector
		//find the closest point(tclosest) to camera where reT is the smallest
		float tcloest = 99999;
		int tar = -1;
		for (int i = 0;i < ballnum;i++) {
			if (re[i].reT < tcloest) {
				tcloest = re[i].reT;
				tar = i;
			}
		}
		vec3 N = unit_vector(re[tar].reNormal);
		const vec3 lsource(-10,10,0);
		vec3 L = unit_vector(lsource - r.point_at_parameter(tcloest));
		vec3 I = vec3(1, 1, 1);
		//what is R
		vec3 R = I * max(0.0f, dot(N, L)) * re[tar].reCol;
		vec3 REFre(0,0,0);
		vec3 TRAre(0,0,0);
		//shadow
		bool ishadow = false;
		ray shadowray(r.point_at_parameter(re[tar].reT),lsource - r.point_at_parameter(re[tar].reT));
		
		for (int i = 0;i < ballnum;i++) {
			if (i != tar && able[i].hit(shadowray,shadre[i])) {
				ishadow = true;
				break;
			}
		}
		
		if (ishadow) {
			delete[] re;
			delete[] shadre;
			return vec3(0, 0, 0);
		}
		//reflect 
		if (re[tar].reReflect>0) {
			vec3 refvec = N * 2 * dot(N, (r.direction()) * (-1)) - (r.direction()) * (-1);
			ray refray(r.point_at_parameter(re[tar].reT), refvec);
			REFre = color(refray, able, step + 1);
		}
		//refract
		float ior = 1.34;
		vec3 uniI = unit_vector(r.D) * (-1);
		if (re[tar].reTrans>0) {
			if (dot(uniI, N) > 0) {
				N = N * (-1);
				inside = true;
			}
			if (inside) {
				ior = 1 / ior;
			}
			float k = 1 - ior * ior*(1 - dot(N, uniI) *dot(N, uniI));
			if (k < 0) {
				delete[] re;
				delete[] shadre;
				return vec3(0, 0, 0);
			}
			else {
				vec3 travector = uniI * ior - N * (ior * dot(N, uniI) + sqrt(k));
				ray fraray(r.point_at_parameter(re[tar].reT), travector);
				TRAre = color(fraray, able, step + 1);
			}
		}
		for (int i = 0; i < 3;i++) {
			if (R.e[i] > 1) {
				R.e[i] = 1;
			}
		}
		
		return TRAre * re[tar].reTrans + (R * (1 - re[tar].reReflect) + REFre * re[tar].reReflect) * (1 - re[tar].reTrans);
		
	}
	else  {
		//didn't hit the ball return background
		vec3 unit_direction = unit_vector(r.direction());
		float	v = 0.5*(unit_direction.y() + 1.0);
		delete[] re;
		delete[] shadre;
		return vec3(1, 1, 1)*float(1.0 - v) + vec3(0.5, 0.7, 1.0)*v;
	}
	
}
/*
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
*/
//vec3 color(const ray& r);


int main()
{
	int width = 1200;
	int height = 600;
	vector<sphere> hitable_list;

	vec3 colorlist[8] = { vec3(0.8, 0.3, 0.3), vec3(0.3, 0.8, 0.3), vec3(0.3, 0.3, 0.8),
	vec3(0.8, 0.8, 0.3), vec3(0.3, 0.8, 0.8), vec3(0.8, 0.3, 0.8),
	vec3(0.8, 0.8, 0.8), vec3(0.3, 0.3, 0.3) };

	hitable_list.push_back(sphere(vec3(0, -100.5, -2), 100, vec3(0, 0.8, 0), 0.5));
	hitable_list.push_back(sphere(vec3(0, 0, -2), 0.5, vec3(0, 0, 0), 0, 0.9));
	hitable_list.push_back(sphere(vec3(1, 0, -1.75), 0.5, vec3(0, 0, 0), 1));
	hitable_list.push_back(sphere(vec3(-1, 0, -2.25), 0.5, vec3(0.5, 0.35, 0.7), 0.1));
	srand(1234);

	for (int i = 0; i < 48; i++) {
		int cindex = rand() % 8;
		float rand_reflec = ((float)rand() / (float)(RAND_MAX));
		float rand_refrac = ((float)rand() / (float)(RAND_MAX));
		float xr = ((float)rand() / (float)(RAND_MAX)) * 6.0f - 3.0f;
		float zr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;
		if (i % 4 == 0)
			hitable_list.push_back(sphere(vec3(xr, -0.45, zr - 2), 0.05, colorlist[cindex], rand_reflec));
		else if (i % 4 == 1)
			hitable_list.push_back(sphere(vec3(xr, -0.45, zr - 2), 0.05, colorlist[cindex]));
		else if (i % 4 == 2)
			hitable_list.push_back(sphere(vec3(xr, -0.45, zr - 2), 0.05, colorlist[cindex], 0, rand_refrac));
		else if (i % 4 == 3)
			hitable_list.push_back(sphere(vec3(xr, -0.45, zr - 2), 0.05, colorlist[cindex], rand_reflec, rand_refrac));
	}
	fstream file;
	file.open("ray1.ppm", ios::out);
	vec3 lower_left_corner(-2, -1, -1);
	vec3 origin(0, 0, 1);
	vec3 horizontal(4, 0, 0);
	vec3 vertical(0, 2, 0);
	file << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float u = float(i) / float(width);
			float v = float(j) / float(height);
			ray r(origin, lower_left_corner + horizontal * u + vertical * v);
			vec3 color1 = color(r, hitable_list, 1);
			file << int(color1.e[0] * 255) << " " << int(color1.e[1] * 255) << " " << int(color1.e[2] * 255) << "\n";
			}
	}

	/*
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
	*/
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
/*
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
*/
