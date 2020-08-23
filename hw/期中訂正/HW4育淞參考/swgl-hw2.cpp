#include "swgl.h"
#include <stdlib.h>     /* abs */
#include <algorithm>    // std::swap

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

bool BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble r, GLdouble g, GLdouble b)
{
    //int p,dx,dy,x,y;
    GLdouble dx, dy, i, e;
	GLdouble incx, incy, inc1, inc2;
	GLdouble x,y;

	dx = x2-x1;
	dy = y2-y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1) incx = -1;
	incy = 1;
	if (y2 < y1) incy = -1;
	x = x1; y = y1;
	if (dx > dy) {
		writepixel(x,y,r,g,b);
		e = 2 * dy-dx;
		inc1 = 2*(dy-dx);
		inc2 = 2*dy;
		for (i=0; i<dx; i++) {
			if (e >= 0) {
				y += incy;
				e += inc1;
			}
			else
				e += inc2;
			x += incx;
			writepixel(x,y,r,g,b);
		}

	} else {
		writepixel(x,y,r,g,b);
		e = 2*dx-dy;
		inc1 = 2*(dx-dy);
		inc2 = 2*dx;
		for (i=0; i<dy; i++) {
			if (e >= 0) {
				x += incx;
				e += inc1;
			}
			else
				e += inc2;
			y += incy;
			writepixel(x,y,r,g,b);
		}
	}


    /*
    GLdouble p,dx,dy,x,y;
    writepixel(x1, y1, r, g, b);
    writepixel(x2, y2, r, g, b);

    dx=x2-x1;
    dy=y2-y1;
    p=2*dy-dx;

    x=x1;
    y=y1;

    while(x<=x2){
      writepixel(x,y,r,g,b);
      x++;
      if(p<0){
         p=p+2*dy;
      }
      else{
        p=p+2*dy-2*dx;
        y++;
      }
    }
    */
   /*
    for(int i=0; i<100; i++) {
        float a = 0.01*i;
        writepixel(a*x1+(1-a)*x2, a*y1+(1-a)*y2, r, g, b);
    }
    */

	return true;
}

bool fillBottomFlatTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b){
   float invslope1 = (x2 -x1)/(y2-y1);
   float invslope2 = (x3-x1)/(y3-y1);

   float curx1 = x1;
   float curx2 = x1;

   for(int scanlineY = y1 ; scanlineY<=y2 ;scanlineY++){
        BresenhamLine((int)curx1,scanlineY,(int)z1,(int)curx2,scanlineY,(int)z2,r,g,b);
        curx1 +=invslope1;
        curx2 +=invslope2;
   }
   return true;
}


bool fillTopFlatTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b){

    float invslope1 = (x3-x1)/(y3-y1);
    float invslope2 = (x3-x2)/(y3-y2);

    float curx1 = x3;
    float curx2 = x3;

    for(int scanlineY = y3; scanlineY> y1; scanlineY--)
    {
        BresenhamLine((int)curx1,scanlineY,(int)z3,(int)curx2,scanlineY,(int)z3,r,g,b);
        curx1 -=invslope1;
        curx2 -=invslope2;
    }

     return true;
}


bool swTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b)
{

    writepixel(x1, y1, r, g, b);
    writepixel(x2, y2, r, g, b);
    writepixel(x3, y3, r, g, b);

    BresenhamLine(x1, y1, z1, x2, y2, z2, r, g, b);
    BresenhamLine(x2, y2, z2, x3, y3, z3, r, g, b);
    BresenhamLine(x3, y3, z3, x1, y1, z1, r, g, b);

    if(y2 == y3){
      fillBottomFlatTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,r,g,b);
    }
    /**
    else if(){


    }
    else{


    }
    */


	return true;
}


bool swInitZbuffer(int width, int height)
{

    return true;
}



bool swClearZbuffer()
{

    return true;
}
