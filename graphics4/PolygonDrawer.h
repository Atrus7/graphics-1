#ifndef POLYGON_DRAWER_H
#define POLYGON_DRAWER_H

#define ImageW 500
#define ImageH 500
#include <vector>
#include <cmath>
#include <stdio.h>

using namespace std;

static enum SHADING { Flat, Gourand, Phong } shading;

 class Vertex
 {
 public:
 	float x, y, z;

 	Vertex ( void )
 	{
 		x = y = z = 0;
 	}

 	Vertex ( float X, float Y, float Z )
 	{
 		x = X;
 		y = Y;
 		z = Z;

 		averageNorm = new Vertex;
 	}

 	inline void make_unit_vector() {
 		float k = 1.0 / sqrt(x * x + y * y + z * z);
 		x *= k; y *= k; z *= k;
 	}
 	inline Vertex& operator+=(const Vertex &v2);

 	Vertex* averageNorm;
 	int numNorms;
 };

 inline Vertex operator+(const Vertex &v1, const Vertex &v2) {
 	return Vertex(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
 }

 inline Vertex operator-(const Vertex &v1, const Vertex &v2) {
 	return Vertex(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
 }

 inline Vertex operator*(const Vertex &v1, const Vertex &v2) {
 	return Vertex(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
 }

 inline Vertex operator*(float t, const Vertex &v) {
 	return Vertex(t*v.x, t*v.y, t*v.z);
 }

 inline Vertex operator*(const Vertex &v, float t) {
 	return Vertex(t*v.x, t*v.y, t*v.z);
 }

 inline Vertex operator/(const Vertex& v, float t) {
 	return Vertex(v.x / t, v.y / t, v.z / t);
 }

 inline Vertex& Vertex::operator+=(const Vertex &v) {
 	x += v.x;
 	y += v.y;
 	z += v.z;
 	return *this;
 }

 inline float dot(const Vertex &v1, const Vertex &v2) {
 	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
 }

 inline Vertex cross(const Vertex &v1, const Vertex &v2) {
 	return Vertex((v1.y * v2.z - v1.z * v2.y),
 		(-(v1.x * v2.z - v1.z * v2.x)),
 		(v1.x * v2.y - v1.y * v2.x));
 }

const Vertex AMB_INTENSITY(.5, .5, .5);
const Vertex LS_INTENSITY( 1, 1, 1 );
const Vertex AMB_REFLECTION( .1, 0,0 );
const Vertex DIFF_REFLECTION( .7, 0, 0 );
const Vertex SPEC_REFLECTION( .5, .5, .5 );

const Vertex LS_VECTOR(-1.0 / sqrt(3), (-1.0 / sqrt(3)), (1.0 / sqrt(3)));
const Vertex EYE_VECTOR(0, 0, -1);
const int SPEC_EXP = 5;

class Pt
{
public:
	float x, y, z;

  int normal_avg_count;
  Vertex normal;
  Vertex color;

	Pt ( void )
	{
		normal_avg_count = x = y = z = 0.0;
	}

	Pt ( float nX, float nY, float nZ) : normal(0, 0, 0), color(0,0,0), normal_avg_count(0)
	{
		x = nX;
		y = nY;
		z = nZ;
  }

  void update_normal(Vertex face_normal){
    int old_avg_count = normal_avg_count;
    ++normal_avg_count;

    normal = ((normal * old_avg_count) + face_normal) / normal_avg_count;
    normal.make_unit_vector();
  }

};

class WinPt
{
public:
  int x, y, z;
  //Vertex attr;

  WinPt ( void )
  {
    x = y = z = 0;
  }

  WinPt ( int nX, int nY, int nZ)//, Vertex _attr)
  {
    x = nX;
    y = nY;
    z = nZ;
    //attr = _attr;
  }
};

struct Polygon {
  const Vertex *p1, *p2, *p3;
  //sometimes it's easier to work in vertexes
  Vertex normal;
  Polygon(Vertex *_p1, Vertex *_p2, Vertex *_p3): p1(_p1), p2(_p2), p3(_p3){

    normal = cross(*p2-*p1, *p3-*p1);
    //convert to unit vector
    normal.make_unit_vector();
  }

  static WinPt calculatePointToWindow(const Vertex &point){
    const float coord_size = 2;
    //Convert coords to window.
    printf("Converted %f, %f ", point.x, point.y);
    //shift coords over by 1, since screen is not centered at 0
    float x = point.x + 1;
    float y = point.y + 1;

    float x_per_pixel = coord_size / ImageW;
    float y_per_pixel = coord_size / ImageH;

    int x2 = round(x / x_per_pixel);
    int y2 = round(y / y_per_pixel);
    WinPt window_point(x2, y2, point.z);

    printf("to %d, %d \n", x2, y2);
    return window_point;
  }
  vector<WinPt> getWindowPoints(){
    vector<WinPt> window_points;
    window_points.push_back(calculatePointToWindow(*p1));
    window_points.push_back(calculatePointToWindow(*p2));
    window_points.push_back(calculatePointToWindow(*p3));
    return window_points;
  }
  vector<Vertex> getPoints(){
    vector<Vertex> points;
    points.push_back(Vertex(*p1));
    points.push_back(Vertex(*p2));
    points.push_back(Vertex(*p3));
    return points;
  }

};

void drawPolygon ( Polygon poly );

#endif
