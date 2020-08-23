#include "swgl.h"


//---------------------------------------------------------------------------
//cghw2
//---------------------------------------------------------------------------

//You can change it to speed up
void writepixel(int x, int y, GLdouble r, GLdouble g, GLdouble b)
{
	GLubyte map[1]={255};

	glColor3d(r, g, b);
	glWindowPos2i(x, y);
	glBitmap(1, 1, 0, 0, 0, 0, map);
}

GLdouble gldoubleabs(GLdouble a){
    if(a < 0){
        return (-1)*a;
    }
    return a;
}

void plotlowline(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble r, GLdouble g, GLdouble b){
    GLdouble dx=x2-x1;
    GLdouble dy=y2-y1;
    int yi=1;
    if(dy<0){
        yi=-1;
        dy=(-1)*dy;
    }
    GLdouble D = 2*dy - dx;
    GLdouble y = y1;
    GLdouble x = x1;
    for(int i=0;i<dx;i++){
        x=x+1;
        writepixel(x,y,r,g,b);
        if(D > 0){
            y=y+yi;
            D=D-2*dx;
        }
        D=D+2*dy;
    }

}

void plothighline(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble r, GLdouble g, GLdouble b){
    GLdouble dx = x2-x1;
    GLdouble dy = y2-y1;
    GLdouble xi=1;
    if(dx<0){
        xi=-1;
        dx=(-1)*dx;
    }
    GLdouble D=2*dx-dy;
    GLdouble x=x1;
    GLdouble y=y1;
    for(int i=0;i<dy;i++){
        y = y+1;
        writepixel(x,y,r,g,b);
        if(D>0){
            x=x+xi;
            D=D-2*dy;
        }
        D=D+2*dx;
    }
}

bool BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble r, GLdouble g, GLdouble b)
{
    writepixel(x1, y1, r, g, b);
    writepixel(x2, y2, r, g, b);
    //BresenhamLine
    GLdouble dx=x2-x1;
    GLdouble dy=y2-y1;
    if(gldoubleabs(dy) < gldoubleabs(dx)){
        if(x1>x2){
            plotlowline(x2,y2,z2,x1,y1,z1,r,g,b);
        }
        else{
            plotlowline(x1,y1,z1,x2,y2,z2,r,g,b);
        }
    }
    else{
        if(y1>y2){
            plothighline(x2,y2,z2,x1,y1,z1,r,g,b);
        }
        else{
            plothighline(x1,y1,z1,x2,y2,z2,r,g,b);
        }

    }

    /*
    for(int i=0; i<100; i++) {
        float a = 0.01*i;
        writepixel(a*x1+(1-a)*x2, a*y1+(1-a)*y2, r, g, b);
    }
    */

	return true;
}

bool isintriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble px,GLdouble py,GLdouble pz){
//p = p0 + (p1 - p0) * s + (p2 - p0) * t
//The point p is inside the triangle if 0 <= s <= 1 and 0 <= t <= 1 and s + t <= 1.
//s,t and 1 - s - t are called the barycentric coordinates of the point p.

    GLdouble s=0.0;
    GLdouble t=0.0;
    GLdouble area = 0.5 *((-1)*y2*x3 + y1*((-1)*x2 + x3) + x1*(y2 - y3) + x2*y3);
    s = 1 / (2*area)*(y1*x3 - x1*y3 + (y3 - y1)*px + (x1 - x3)*py);
    t = 1 / (2*area)*(x1*y2 - y1*x2 + (y1 - y2)*px + (x2 - x1)*py);

    if(s>=0 && s<=1 && t>=0 && t<=1 && s+t<=1 && s+t>=0){
        return true;
    }
    return false;


}

bool swTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b)
{
    writepixel(x1, y1, r, g, b);
    writepixel(x2, y2, r, g, b);
    writepixel(x3, y3, r, g, b);

    GLdouble bx=-999999,by=-999999;
    GLdouble sx=999999,sy=999999;


    (x1>x2)?bx=x1:bx=x2;
    (bx>x3)?bx=bx:bx=x3;
    (y1>y2)?by=y1:by=y2;
    (by>y3)?by=by:by=y3;

    (x1<x2)?sx=x1:sx=x2;
    (sx<x3)?sx=sx:sx=x3;
    (y1<y2)?sy=y1:sy=y2;
    (sy<y3)?sy=sy:sy=y3;

    for(int i=sx-1;i<bx+1;i++){

        for(int j=sy-1;j<by+1;j++){
            if(isintriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,i,j,-1)  == true){
                writepixel(i,j,r,g,b);
            }
        }
    }

    BresenhamLine(x1, y1, z1, x2, y2, z2, r, g, b);
    BresenhamLine(x2, y2, z2, x3, y3, z3, r, g, b);
    BresenhamLine(x3, y3, z3, x1, y1, z1, r, g, b);



	return true;
}


bool swInitZbuffer(int width, int height)
{
    int** zbuf = new int*[height];
    for(int i=0;i<width;i++){
        zbuf[i] = new int[width];
    }
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            zbuf[i][j]=999999;
        }
    }



    return true;
}



bool swClearZbuffer()
{
    /*
    for(int i=0;i<sizeof(zbuf[0]);i++){
        delete zbuf[i];
    }
    delete[] zbuf;
    */
    return true;
}
