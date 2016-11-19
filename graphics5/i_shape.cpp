#include "i_shape.h"
#include <cstdio>

Vertex IPlane::get_intersection(Vertex p, Vertex v){
  if(deformation != INVALID_DEFORMATION){
    p = p * inverse_deformation;
    // Vertex p2 = (v+p) * inverse_deformation;
    // p  = p2 - p;
    v = v * inverse_deformation;
  }

  if(dot(v,normal) == 0){ //parallel
    return INVALID_INTERSECT;
  }
  float t = dot((point - p), normal) / dot(v, normal);

  if(t <= 0){
    return INVALID_INTERSECT;
  }

  Vertex intersect = (v * t) + p;

  if(deformation != INVALID_DEFORMATION){
    return intersect * deformation;
  }
  return intersect;
}


Vertex IPlane::get_normal(Vertex point){
  if(deformation != INVALID_DEFORMATION){
    return normal * deformation;
  }
  return normal;
}

float SphereIntersection(Vertex center, float radius, Vertex p, Vertex v){
  float a = dot(v,v);
  float b = 2 * dot( v, (p - center));
  float c = dot((p - center), (p - center)) - pow(radius, 2);
  float discriminant = pow(b,2) - 4 * a * c;
  if(discriminant < 0){
    return -1;//invalid
  }

  float t1 = (-b - sqrt(discriminant)) / (2 * a);
  float t2 = (-b + sqrt(discriminant)) / (2 * a);
  Vertex intersect;
  if(t1 < t2 && t1 >= 0){ // pick the closer of the two.
    return t1;
  } else {
    return t2;
  }
}


Vertex ISphere::get_intersection(Vertex p, Vertex v){
  if(deformation != INVALID_DEFORMATION){
    p = p * inverse_deformation;
    v = v * inverse_deformation;
  }

  float t = SphereIntersection(center, radius, p, v);
  if(t <= 0){
    return INVALID_INTERSECT;
  }
  Vertex intersect=  (v * t) + p;

  if(deformation != INVALID_DEFORMATION){
    return intersect * deformation;
  }
  return intersect;
}

Vertex ISphere::get_normal(Vertex point){

  if(deformation != INVALID_DEFORMATION){
    Vertex normal = ((point * inverse_deformation -  center));
    return normal* inverse_deformation;
  }
  return point - center;
}

Vertex ICylinder::get_intersection(Vertex p, Vertex v){
  if(deformation != INVALID_DEFORMATION){
    p = p * inverse_deformation;
    v = v * inverse_deformation;
  }

  //orthogonal projection of p/v
  Vertex p_proj = orthogonal_projection(p, center, axis.get_unit_vector());
  Vertex p2 = v+p;
  Vertex v_proj = orthogonal_projection(p2, center, axis.get_unit_vector()) -p_proj;

  //circle intersection
  float t = SphereIntersection(center, radius, p_proj, v_proj);
  if(t <= 0){
    return INVALID_INTERSECT;
  }
  Vertex intersect = (v * t) + p;


  if(deformation != INVALID_DEFORMATION){
    return intersect * deformation;
  }
  return intersect;
}
Vertex orthogonal_projection(Vertex p, Vertex o, Vertex n){
  Vertex result = p - (dot(p - o, n) * n);
  return result;
}


Vertex ICylinder::get_normal(Vertex point){
  Vertex normal = (point - center - (dot((point - center), axis) * axis)) / radius;

  if(deformation != INVALID_DEFORMATION){
    return normal * inverse_deformation;
  }

  return normal;
}
