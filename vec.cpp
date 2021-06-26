/* This module contains necessary functions about vectors */

#include "include/vec.h"
#include <math.h>
#include "include/consts.h"

double dist(point a,point b){
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

point rotate(point p,double x){
	return {p.x*cos(x)-p.y*sin(x),p.y*cos(x)+p.x*sin(x)};
}



Vec tovec(point a,point b){         // convert 2 points to vector a->b
    return Vec(b.x-a.x,b.y-a.y);
}

Vec scale(Vec v,double s){
	return Vec(v.x*s,v.y*s);
}

Vec scaleToMagnitude(Vec v,double magnitude){
	double mul= magnitude/sqrt(v.x*v.x+v.y*v.y);
	return {v.x*mul,v.y*mul};
}

point translate(point p,Vec v){
	return point(p.x+v.x,p.y+v.y);
}

double hypotSq(Vec v){
	return v.x*v.x+v.y*v.y;
}

double dot(Vec a,Vec b){
	return a.x*b.x+a.y*b.y;
}

double cross(Vec a,Vec b){
	return a.x*b.y-b.x*a.y;
}

bool ccw(point p,point q,point r){
	return cross(tovec(p,q),tovec(p,r))>0.0;
}

// returns true if point r is on the same line as the line pq
bool collinear(point p,point q,point r){
	return fabs(cross(tovec(p,q),tovec(p,r)))<EPS;
}

double getShortestAngle(point a, point o, point b) { // returns angle aob in rad
	Vec oa = tovec(o, a), ob = tovec(o, b);
	return acos(dot(oa, ob) / sqrt(hypotSq(oa) * hypotSq(ob)));
}

double getAngleClockwise(point a,point o,point b){
    double res=getShortestAngle(a,o,b);
    if(!ccw(o,a,b))res=2.0*PI-res;
    return res;
}

Vec rotateVec(Vec v,double x){
	point p=rotate({v.x,v.y},x);
	return {p.x,p.y};
}

double RadToDegree(double rad){
    return (180.0*rad)/acos(-1);
}
