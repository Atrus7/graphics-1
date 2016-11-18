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

float SphereIntersection(Vertex center, float radius, Vertex p, Vertex v){
  float a = dot(v,v);
  float b = dot((2 * v), (p - center));
  float c = dot((p - center), (p - center)) - pow(radius, 2);
  float discriminant = pow(b,2) - 4 * a * c;
  if(discriminant < 0){
    return -1;//invalid
  }

  float t1 = (-b + sqrt(discriminant)) / (2 * a);
  float t2 = (-b - sqrt(discriminant)) / (2 * a);
  Vertex intersect;
  if(t1 < t2 && t1 >= 0){ // pick the closer of the two.
    return t1;
  } else {
    return t2;
  }
}


Vertex ISphere::get_intersection(Vertex p, Vertex v){
  float t = SphereIntersection(center, radius, p, v);
  if(t <= 0){
    return INVALID_INTERSECT;
  }
  return (v * t) + p;
}

Vertex ISphere::get_normal(Vertex point){
  return (point - center);
}

Vertex ICylinder::get_intersection(Vertex p, Vertex v){
  //orthogonal projection of p/v
  Vertex p_proj = orthogonal_projection(p, center, axis);
  Vertex v_proj = orthogonal_projection(v, center, axis);
  //circle intersection
  float t = SphereIntersection(center, radius, p_proj, v_proj);
  if(t <= 0){
    return INVALID_INTERSECT;
  }
  else{
    return (v*t) + p;
  }
}
Vertex orthogonal_projection(Vertex p, Vertex o, Vertex v){
  return p - (dot(p-o, v) * v);
}


Vertex ICylinder::get_normal(Vertex point){
  return (point - center - dot((point - center), axis) * axis) / radius;
}
