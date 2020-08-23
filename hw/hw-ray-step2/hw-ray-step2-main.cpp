#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include "vec3.h"
//#include "stb_image_write.h"

using namespace std;
struct hit_record {
    float t;
    float reflect;
    float trans;
    vec3 p;
    vec3 normal;
    vec3 col;
};
class hitable {
	public:
	virtual bool hit(const ray& r, hit_record& rec) const = 0;
};
class sphere: public hitable {
public:
	sphere() {};
	sphere(vec3 c, float r) : center(c), radius(r) ,color(vec3(0,0,0)),wref(0),wtra(0){};
	sphere(vec3 c, float r,vec3 col) : center(c), radius(r) ,color(col),wref(0),wtra(0){};
	sphere(vec3 c, float r,vec3 col,float ref) : center(c), radius(r) ,color(col),wref(ref),wtra(0){};
	sphere(vec3 c, float r,vec3 col,float ref,float tra) : center(c), radius(r) ,color(col),wref(ref),wtra(tra){};
	virtual bool hit(const ray& r, hit_record& rec) const;
	vec3 center;
	float radius;
	vec3 color;
	float wref;
	float wtra;
};
bool sphere::hit(const ray& r, hit_record& rec) const {
	float tmin;
	float a = dot(r.direction(),r.direction());
	float b = 2.0 * dot(r.direction(),(r.origin() - center));
	float c = dot((r.origin() - center),(r.origin() - center)) -radius * radius;
	if(b*b - 4*a*c > 0 && dot(r.direction(),(center - r.origin())) > 0){
		if((-b - sqrt(b*b - 4*a*c))/(2*a) < (-b + sqrt(b*b - 4*a*c))/(2*a) && (-b - sqrt(b*b - 4*a*c))/(2*a) > 0){
			tmin = (-b - sqrt(b*b - 4*a*c))/(2*a);
		}
		else{
			tmin = (-b + sqrt(b*b - 4*a*c))/(2*a);
		}
		rec.t = tmin;
		rec.p = center;
		rec.col = color;
		rec.reflect = wref;
		rec.trans = wtra;
		return true;
	}
	else{
		rec.t = 1e9;
		return false;
	}
}
vec3 color(const ray& r,vector<sphere> able,int step)
{
	int tar;
	hit_record re[able.size()];
	hit_record shadre[able.size()];
	bool shoot = false;
	bool ishoot = false;
	bool inside = false;
	for(int i = 0;i < able.size();i++){
		shoot = able[i].hit(r,re[i]);
		if(shoot)
			ishoot = true;
	} 
	if(ishoot && step < 5){ 
		//find the cloest ball
		float t0 = 1e9;
		for(int i = 0;i < able.size();i++){
			if(re[i].t < t0){
				t0 = re[i].t;
				tar = i;	
			}
		}
		vec3 N = unit_vector(r.point_at_parameter(re[tar].t) - re[tar].p);
		vec3 L = unit_vector( vec3(-10,10,0) - r.point_at_parameter(t0));
		vec3 I = vec3(1,1,1);
		vec3 R = I * max(0.0f,dot(N,L)) * re[tar].col;
		vec3 REFre = vec3(0,0,0);
		vec3 TRAre = vec3(0,0,0);
		
		//shadow
		bool shadow = false;
		ray shadowray = ray(r.point_at_parameter(re[tar].t) ,vec3(-10,10,0) - r.point_at_parameter(re[tar].t));
		for(int i = 0;i < able.size();i++){
			if(i == tar)
				continue;
			if(able[i].hit(shadowray,shadre[i]) ){
				shadow = true;
				break;
			}
		}
		if(shadow){
			return vec3(0.1,0.1,0.1);
		}	
		//reflect
		if(re[tar].reflect > 0){
			vec3 REF = N * 2 * dot(N,-(r.direction() - r.origin())) - (-(r.direction() - r.origin()));
			ray refray = ray(r.point_at_parameter(re[tar].t),REF);
			REFre = color(refray,able,step + 1);
		}
		//refract
		if(re[tar].trans > 0){
			float ior = 1.34;
			vec3 uniI = unit_vector(r.origin() - r.point_at_parameter(re[tar].t));
			if(dot(r.origin() - r.point_at_parameter(re[tar].t),N) > 0){
				N = -N;
				inside = true;
			} 
			if(inside)
				ior = 1/ior;
			float k = 1 - ior * ior * (1 - dot(N, uniI) * dot(N, uniI)); 
			if(k < 0)
				return vec3(0,0,0);
			else{
				vec3 TRA =  uniI * ior - N * (ior * dot(N, uniI) + sqrt(k));
				ray fraray = ray(r.point_at_parameter(re[tar].t),TRA);
				TRAre = color(fraray,able,step + 1);	
			}
		//	cout << "TRAre: " << TRAre << "step: " << step<<endl;
		}
		
		for(int i = 0; i < 3;i++){
			if(R.e[i] > 1){
				R.e[i] = 1;
			}
		}
		return TRAre * re[tar].trans +  (R * (1 - re[tar].reflect)  + REFre * re[tar].reflect) * (1 -re[tar].trans); 
	}
	else{
    	vec3 unit_direction = unit_vector(r.direction());
    	float t= 0.5*(unit_direction.y() + 1.0);  
    	return vec3(1, 1, 1) * (1.0-t) + vec3(0.5, 0.7, 1.0) * t;
	} 
}

int main()
{
	int width = 1200;
	int height = 600;
	vector<sphere> hitable_list;
	vec3 colorlist[8] = { vec3(0.8, 0.3, 0.3), vec3(0.3, 0.8, 0.3), vec3(0.3, 0.3, 0.8),
 	vec3(0.8, 0.8, 0.3), vec3(0.3, 0.8, 0.8), vec3(0.8, 0.3, 0.8),
 	vec3(0.8, 0.8, 0.8), vec3(0.3, 0.3, 0.3) };

	hitable_list.push_back(sphere(vec3(0, -100.5, -2), 100,vec3(0,0.8,0),0.5));
	hitable_list.push_back(sphere(vec3(0, 0, -2), 0.5,vec3(0,0,0),0,0.9));
	hitable_list.push_back(sphere(vec3(1, 0, -1.75), 0.5,vec3(0,0,0),1));
	hitable_list.push_back(sphere(vec3(-1, 0, -2.25), 0.5,vec3(0.5,0.35,0.7),0.1));
	srand(1234);
	for (int i = 0; i < 48; i++) {
		int cindex = rand() % 8;
		float rand_reflec = ((float)rand() / (float)(RAND_MAX));
		float rand_refrac = ((float)rand() / (float)(RAND_MAX));
		float xr = ((float)rand() / (float)(RAND_MAX)) * 6.0f - 3.0f;
		float zr = ((float)rand() / (float)(RAND_MAX)) * 3.0f - 1.5f;
		if(i % 4 == 0)
			hitable_list.push_back(sphere(vec3(xr, -0.45, zr-2), 0.05,colorlist[cindex],rand_reflec));
		else if(i % 4 == 1)
			hitable_list.push_back(sphere(vec3(xr, -0.45, zr-2), 0.05,colorlist[cindex]));
		else if(i % 4 == 2)
			hitable_list.push_back(sphere(vec3(xr, -0.45, zr-2), 0.05,colorlist[cindex],0,rand_refrac));
		else if(i % 4 == 3)
		hitable_list.push_back(sphere(vec3(xr, -0.45, zr-2), 0.05,colorlist[cindex],rand_reflec,rand_refrac));
	}
	unsigned char *data = new unsigned char[width*height * 3];
//	fstream file;
	file.open("E:\computer graph\hw-ray-step2", ios::out);
//	file.open("ray1.ppm", ios::out);
	vec3 lower_left_corner(-2, -1, -1);
	vec3 origin(0,0,1);
	vec3 horizontal(4, 0, 0);
	vec3 vertical(0, 2, 0);
	file << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float u = float(i) / float(width);
			float v = float(j) / float(height);
			ray r(origin, lower_left_corner + horizontal * u + vertical * v);
			vec3 color1 = color(r,hitable_list,1);
			file << int(color1.e[0] * 255) << " " << int(color1.e[1]  * 255) << " " << int(color1.e[2] * 255) << "\n";
			int index = ((height -j-1)*width + i) * 3;
        	data[index + 0] = (unsigned char)(color1.e[0] * 255);
        	data[index + 1] = (unsigned char)(color1.e[1] * 255);
        	data[index + 2] = (unsigned char)(color1.e[2] * 255);
    	}
	}
//	stbi_write_bmp("ray.bmp", width, height, 3, data);
	return 0;
}
