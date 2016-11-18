#include "i_shape.h"
#include <cstdio>

Vertex IPlane::get_intersection(Vertex p, Vertex v){
  if(dot(v,normal) == 0){ //parallel
    return INVALID_INTERSECT;
  }
  float t = dot((point - p), normal) / dot(v, normal);
  Vertex intersect = (v * t) + p;

  return (t > 0)? intersect : INVALID_INTERSECT;
}

Vertex IPlane::get_normal(Vertex point){
  return normal;
}

Vertex ISphere::get_intersection(Vertex p, Vertex v){
  float a = dot(v,v);
  float b = dot((2 * v), (p - center));
  float c = dot((p - center), (p - center)) - pow(radius, 2);
  float discriminant = pow(b,2) - 4 * a * c;
  if(discriminant <= 0){
    return INVALID_INTERSECT;
  }
  float t1 = (-b + sqrt(discriminant)) / (2 * a);
  float t2 = (-b - sqrt(discriminant)) / (2 * a);
  Vertex intersect;
  if(t1 < t2 ){//}&& t1 >= 0){ // pick the closer of the two.
    intersect = (v * t1) + p;
  } else {
    intersect = (v * t2) + p;
  }
  return intersect;
}

Vertex ISphere::get_normal(Vertex point){
  return (point - center);
}
