#ifndef VERTEX_H
#define VERTEX_H
#include <cmath>
#include <cstdio>

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
 	}

 	inline void make_unit_vector() {
 		float k = 1.0 / sqrt(x * x + y * y + z * z);
 		x *= k; y *= k; z *= k;
 	}

   inline Vertex get_unit_vector() const {
     float k = 1.0 / sqrt(x * x + y * y + z * z);
     return Vertex(x * k, y * k, z * k);
   }
   void print() const {
     printf("Vertex: (%f, %f, %f) \n", x,y,z);
   }

 	inline Vertex& operator+=(const Vertex &v2);

 };

 inline Vertex operator+(const Vertex &v1, const Vertex &v2) {
 	return Vertex(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
 }

inline bool operator==(const Vertex &v1, const Vertex &v2) {
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

inline bool operator!=(const Vertex &v1, const Vertex &v2) {
  return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
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

#endif /* VERTEX_H */
