#ifndef ISHAPES_H
#define ISHAPES_H
#include "vertex.h"
#include "lighting.h"
#include <cmath>
using namespace std;

//A vertex that will be used to as a "NULL" indicator of invalid return
const Vertex INVALID_INTERSECT = {-999, -999, -999};
const Vertex INVALID_DEFORMATION = {0,0,0};
/*
  Assume D(x,y,z) is simple… a matrix
  First deform line L(t) by inverse of D
  Calculate intersection with undeformed surface S
  Transform intersection point and normal by D
 */

struct IShape {
  SurfaceProperties surface_properties;
  //Since this should not be a correct intersection (behind the eye) / off screen.
  Vertex deformation = INVALID_DEFORMATION;
  Vertex inverse_deformation = INVALID_DEFORMATION;

  IShape(SurfaceProperties sp): surface_properties(sp){}

  void add_deformation(Vertex d){
    deformation = d;
    inverse_deformation = inverse(d);
  }

  //returns closest point of intersection
  virtual Vertex get_intersection(Vertex p, Vertex v)=0;
  //returns the normal vector
  virtual Vertex get_normal(Vertex point)=0;
  //transform_L_t()
};

Vertex orthogonal_projection(Vertex p, Vertex o, Vertex v);

class IPlane: public IShape{
  Vertex normal;
  Vertex point;
public:
  Vertex get_intersection(Vertex p, Vertex v);
  Vertex get_normal(Vertex point);
  IPlane(SurfaceProperties sp, Vertex n, Vertex p): IShape(sp), normal(n), point(p){}
};

class ISphere: public IShape{
  float radius;
  Vertex center;
public:
  Vertex get_intersection(Vertex p, Vertex v);
  Vertex get_normal(Vertex point);
  ISphere(SurfaceProperties sp, float r, Vertex c): IShape(sp), radius(r), center(c){}
};

class ICylinder: public IShape{
  float radius;
  Vertex center;
  Vertex axis;
public:
  Vertex get_intersection(Vertex p, Vertex v);
  Vertex get_normal(Vertex point);
  ICylinder(SurfaceProperties sp, float r, Vertex c, Vertex a): IShape(sp), radius(r), center(c), axis(a){}
};

#endif /* ISHAPES_H */
