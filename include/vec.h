#ifndef VEC_H
#define VEC_H


struct point{
	double x=0.0,y=0.0;
	point() { x = y = 0.0; }
	point(double i1,double i2){
		x=i1;
		y=i2;
		//z=i3;
	}

};

struct Vec{
	double x,y;
	Vec(){
        x=0.0,y=0.0;
	}
	Vec(double i1,double i2){
		x=i1;
		y=i2;
	}

};

double dist(point a,point b);

point rotate(point p,double x);

Vec tovec(point a,point b);

Vec scale(Vec v,double s);

Vec scaleToMagnitude(Vec v,double magnitude);

point translate(point p,Vec v);
double hypotSq(Vec v);

double dot(Vec a,Vec b);

double cross(Vec a,Vec b);

bool ccw(point p,point q,point r);

bool collinear(point p,point q,point r);

double getShortestAngle(point a, point o, point b) ;

double getAngleClockwise(point a,point o,point b);

Vec rotateVec(Vec v,double x);



double RadToDegree(double rad);

#endif
