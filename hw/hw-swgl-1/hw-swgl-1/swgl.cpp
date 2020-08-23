#include "swgl.h"
#include <stack>
#include <string.h>
#include <stdio.h>
#include <cmath>
using namespace std;

GLdouble CTM_MV[16];	//Current Transformation Matrix: ModelView
GLdouble CTM_P[16];		//Current Transformation Matrix: Projection
GLdouble *CTM;			//Pointer to Current Transformation Matrix

//h: input the vertex in world space(h)
//w: vertex in windows space(w)

struct Matrix{
    public:
    GLdouble m[16];
    Matrix(){
        memset(m,0,sizeof(GLdouble)*16);
    }
    Matrix(const GLdouble loadmatrix[16]){
        memcpy(m,loadmatrix,sizeof(GLdouble)*16);
    }
};

stack<Matrix> Mstack;

GLdouble *currentMatrix;
const GLdouble PI = 3.1415;
GLdouble xworld,yworld;
GLdouble widthw,heightw;

void multMatrix(GLdouble *c,const GLdouble *a,const GLdouble *b){

    c[0]=a[0]*b[0] + a[4]*b[1] + a[8]*b[2] + a[12]*b[3];
    c[1]=a[1]*b[0] + a[5]*b[1] + a[9]*b[2] + a[13]*b[3];
    c[2]=a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];
    c[3]=a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];

    c[4]=a[0]*b[4] + a[4]*b[5] + a[8]*b[6] + a[12]*b[7];
    c[5]=a[1]*b[4] + a[5]*b[5] + a[9]*b[6] + a[13]*b[7];
    c[6]=a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];
    c[7]=a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];

    c[8]=a[0]*b[8] + a[4]*b[9] + a[8]*b[10] + a[12]*b[11];
    c[9]=a[1]*b[8] + a[5]*b[9] + a[9]*b[10] + a[13]*b[11];
    c[10]=a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];
    c[11]=a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];

    c[12]=a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12]*b[15];
    c[13]=a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13]*b[15];
    c[14]=a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
    c[15]=a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];

}

void multMatrix(GLdouble *a,const GLdouble *b){
    GLdouble temp[16];
    memcpy(temp,a,sizeof(temp));
    a[0]=temp[0]*b[0]+temp[4]*b[1]+temp[8]*b[2]+temp[12]*b[3];
    a[1]=temp[1]*b[0]+temp[5]*b[1]+temp[9]*b[2]+temp[13]*b[3];
    a[2]=temp[2]*b[0]+temp[6]*b[1]+temp[10]*b[2]+temp[14]*b[3];
    a[3]=temp[3]*b[0]+temp[7]*b[1]+temp[11]*b[2]+temp[15]*b[3];

    a[4]=temp[0]*b[4]+temp[4]*b[5]+temp[8]*b[6]+temp[12]*b[7];
    a[5]=temp[1]*b[4]+temp[5]*b[5]+temp[9]*b[6]+temp[13]*b[7];
    a[6]=temp[2]*b[4]+temp[6]*b[5]+temp[10]*b[6]+temp[14]*b[7];
    a[7]=temp[3]*b[4]+temp[7]*b[5]+temp[11]*b[6]+temp[15]*b[7];

    a[8]=temp[0]*b[8]+temp[4]*b[9]+temp[8]*b[10]+temp[12]*b[11];
    a[9]=temp[1]*b[8]+temp[5]*b[9]+temp[9]*b[10]+temp[13]*b[11];
    a[10]=temp[2]*b[8]+temp[6]*b[9]+temp[10]*b[10]+temp[14]*b[11];
    a[11]=temp[3]*b[8]+temp[7]*b[9]+temp[11]*b[10]+temp[15]*b[11];

    a[12]=temp[0]*b[12]+temp[4]*b[13]+temp[8]*b[14]+temp[12]*b[15];
    a[13]=temp[1]*b[12]+temp[5]*b[13]+temp[9]*b[14]+temp[13]*b[15];
    a[14]=temp[2]*b[12]+temp[6]*b[13]+temp[10]*b[14]+temp[14]*b[15];
    a[15]=temp[3]*b[12]+temp[7]*b[13]+temp[11]*b[14]+temp[15]*b[15];

}

void swMultMatrixd(const GLdouble *m){
    multMatrix(currentMatrix,m);
}

void swTranslated(GLdouble x,GLdouble y,GLdouble z){
    GLdouble temp[16];
    temp[0]=1.0; temp[4]=0;   temp[8]=0;    temp[12]=x;
    temp[1]=0;   temp[5]=1.0; temp[9]=0;    temp[13]=y;
    temp[2]=1;   temp[6]=0;   temp[10]=1.0; temp[14]=z;
    temp[3]=1;   temp[7]=0;   temp[11]=0;   temp[15]=1.0;
    swMultMatrixd(temp);
}

void swRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z){
    /*
    const GLdouble sinthe=sin(theta);
    const GLdouble costhe=cos(theta);

    GLdouble temp[16];
    if(x==1 && y==0 && z==0){
        temp[0]=1.0; temp[4]=0;   temp[8]=0;    temp[12]=0;
        temp[1]=0;   temp[5]=costhe; temp[9]=(-1)*sinthe;    temp[13]=0;
        temp[2]=0;   temp[6]=sinthe;   temp[10]=costhe; temp[14]=0;
        temp[3]=0;   temp[7]=0;   temp[11]=0;   temp[15]=1.0;
    }
    else if(x==0 && y==1 && z==0){
        temp[0]=costhe; temp[4]=0;   temp[8]=sinthe;    temp[12]=0;
        temp[1]=0;   temp[5]=1.0; temp[9]=0;    temp[13]=0;
        temp[2]=(-1)*sinthe;   temp[6]=0;   temp[10]=costhe; temp[14]=0;
        temp[3]=0;   temp[7]=0;   temp[11]=0;   temp[15]=1.0;
    }
    else if(x==0 && y==0 && z==1){
        temp[0]=costhe; temp[4]=(-1)*sinthe;   temp[8]=0;    temp[12]=0;
        temp[1]=sinthe;   temp[5]=costhe; temp[9]=0;    temp[13]=0;
        temp[2]=0;   temp[6]=0;   temp[10]=1.0; temp[14]=0;
        temp[3]=0;   temp[7]=0;   temp[11]=0;   temp[15]=1.0;
    }
    swMultMatrixd(temp);
    */

    const GLdouble cAng = cos(angle*PI/180);
    const GLdouble cAngcmp = 1-cAng;
    const GLdouble sAng = sin(angle*PI/180);

    GLdouble tmp[16];
    tmp[0] = x*x*cAngcmp+cAng;    tmp[4] = x*y*cAngcmp-z*sAng;   tmp[8] = x*z*cAngcmp+y*sAng; tmp[12] = 0.0;
    tmp[1] = x*y*cAngcmp+z*sAng;    tmp[5] = y*y*cAngcmp+cAng;     tmp[9] = y*z*cAngcmp-x*sAng; tmp[13] = 0.0;
    tmp[2] = x*z*cAngcmp-y*sAng;    tmp[6] = y*z*cAngcmp+x*sAng;   tmp[10] = z*z*cAngcmp+cAng;  tmp[14] = 0.0;
    tmp[3] = 0.0;                   tmp[7] = 0.0;                  tmp[11] = 0.0;               tmp[15] = 1.0;

    swMultMatrixd(tmp);
}

void swScaled(GLdouble x,GLdouble y, GLdouble z){
    GLdouble temp[16];
    temp[0]=x; temp[4]=0;   temp[8]=0;    temp[12]=0;
    temp[1]=0;   temp[5]=y; temp[9]=0;    temp[13]=0;
    temp[2]=1;   temp[6]=0;   temp[10]=z; temp[14]=0;
    temp[3]=1;   temp[7]=0;   temp[11]=0;   temp[15]=1.0;
    swMultMatrixd(temp);
}


void swViewport(GLint x,GLint y,GLsizei width,GLsizei height){
    xworld=x;
    yworld=y;
    widthw=width;
    heightw=height;
}

void swMatrixMode(GLenum mode){
    switch(mode){
        case GL_PROJECTION:
            currentMatrix = CTM_P;
            break;
        case GL_MODELVIEW:
            currentMatrix = CTM_MV;
            break;
        default:
            currentMatrix = CTM_MV;
    }
}

void swuPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear, GLdouble zFar){
    const GLdouble f= 1/tan( (fovy/2)*PI/180 );
    GLdouble temp[16];
    temp[0] = f / aspect;    temp[4] = 0.0;   temp[8] = 0.0;                           temp[12] = 0.0;
    temp[1] = 0.0;           temp[5] = f;     temp[9] = 0.0;                           temp[13] = 0.0;
    temp[2] = 0.0;           temp[6] = 0.0;   temp[10] = (zFar+zNear)/(zNear-zFar);    temp[14] = 2*zFar*zNear/(zNear-zFar);
    temp[3] = 0.0;           temp[7] = 0.0;   temp[11] = -1.0;                         temp[15] = 0.0;
    swMultMatrixd(temp);
}

void swLoadIdentity(){
    for(int i=0;i<16;i++){
        if(i==0 || i==5 || i==10 || i==15 ){
            currentMatrix[i]=1;
        }
        else{
            currentMatrix[i]=0;
        }
    }
}

void swLoadMatrixd(const GLdouble *loadmatrix){
    memcpy(currentMatrix, loadmatrix, sizeof(GLdouble)*16);
}

void swPushMatrix(){
    Matrix temp = Matrix(currentMatrix);
    Mstack.push(temp);
}

void swPopMatrix(){
    Matrix temp = Mstack.top();
    Mstack.pop();
    swLoadMatrixd(temp.m);
}

void swuLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
 	           GLdouble centerX, GLdouble centerY, GLdouble centerZ,
 	           GLdouble upX, GLdouble upY, GLdouble upZ)
{
    GLdouble F[3] = {centerX-eyeX, centerY-eyeY, centerZ-eyeZ};
    GLdouble Fr = sqrt(F[0]*F[0]+F[1]*F[1]+F[2]*F[2]);
    for(int i=0;i<3;i++){F[i] = F[i]/Fr;}

    GLdouble UP[3] = {upX,upY,upZ};
    GLdouble UPr = sqrt(UP[0]*UP[0]+UP[1]*UP[1]+UP[2]*UP[2]);
    for(int i=0;i<3;i++){UP[i] = UP[i]/UPr;}
    // side = F cross UP
    GLdouble side[3]={F[1]*UP[2]-F[2]*UP[1], (-1)*F[0]*UP[2]+F[2]*UP[0], F[0]*UP[1]-F[1]*UP[0]};
    //UP = side cross F
    UP[0]=side[1]*F[2]-side[2]*F[1]; UP[1]=side[2]*F[0]-side[0]*F[2]; UP[2]=side[0]*F[1]-side[1]*F[0];
    //set matrix
    GLdouble temp[16] = {
    temp[0]=side[0],
    temp[1]=UP[0],
    temp[2]=(-1)*F[0],
    temp[3]=0.0,

    temp[4]=side[1],
    temp[5]=UP[1],
    temp[6]=(-1)*F[1],
    temp[7]=0.0,

    temp[8]=side[2],
    temp[9]=UP[2],
    temp[10]=(-1)*F[2],
    temp[11]=0.0,

    temp[12]=0.0,
    temp[13]=0.0,
    temp[14]=0.0,
    temp[15]=1.0
    };

    swMultMatrixd(temp);
    swTranslated((-1)*eyeX, (-1)*eyeY, (-1)*eyeZ);
}
void swFrustum(	GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal)
{
  const GLdouble A  = (right+left) / (right-left);
  const GLdouble B = (top+bottom) / (top-bottom);
  const GLdouble C = (farVal+nearVal) / (farVal-nearVal);
  const GLdouble D = 2*farVal*nearVal / (farVal-nearVal);
  GLdouble temp[16] = {
      temp[0]=2*nearVal/(right-left),  temp[4]=0.0,                     temp[8]=A,     temp[12]=0.0,
      temp[1]=0.0,                     temp[5]=2*nearVal/(top-bottom),  temp[9]=B,     temp[13]=0.0,
      temp[2]=0.0,                     temp[6]=0.0,                     temp[10]=C,    temp[14]=D,
      temp[3]=0.0,                     temp[7]= 0.0,                    temp[11]=-1.0, temp[15]=0.0
  };



  swMultMatrixd(temp);

}

bool swTransformation(const GLdouble h[4], GLdouble w[4])
{   //p = CTM_P*CTM_MV*h
    GLdouble temp[16];
    GLdouble p[4];

	//prespective division
	multMatrix(temp,CTM_P,CTM_MV);
    p[0]=temp[0]*h[0]+temp[4]*h[1]+temp[8]*h[2]+temp[12]*h[3];
    p[1]=temp[1]*h[0]+temp[5]*h[1]+temp[9]*h[3]+temp[13]*h[3];
    p[2]=temp[2]*h[0]+temp[6]*h[1]+temp[10]*h[3]+temp[14]*h[3];
    p[3]=temp[3]*h[0]+temp[7]*h[1]+temp[11]*h[3]+temp[15]*h[3];
    for(int i=0;i<4;i++){
        p[i]=p[i]/p[3];
    }
    //homogeneous -> 3d coordinate (x,y,z,w)->(x/w,y/w,z/w)

	//viewport transformation
    w[0] = (p[0]+1) * (widthw/2) + xworld;
    w[1] = (p[1]+1) * (heightw/2) + yworld;
    w[2] = p[2];


	return true;
}


