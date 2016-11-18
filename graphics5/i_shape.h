#ifndef ISHAPES_H
#define ISHAPES_H
#include "vertex.h"
#include "lighting.h"
#include <cmath>
using namespace std;

const Vertex INVALID_INTERSECT = {-999, -999, -999};


struct IShape {
  SurfaceProperties surface_properties;
  //A vertex that will be used to as a "NULL" indicator of invalid return
  //Since this should not be a correct intersection (behind the eye) / off screen.

  IShape(SurfaceProperties sp): surface_properties(sp){}

  //returns closest point of intersection
  virtual Vertex get_intersection(Vertex p, Vertex v)=0;
  //returns the normal vector
  virtual Vertex get_normal(Vertex point)=0;
};

static Vertex get_reflected_vector(Vertex L, Vertex N){
  return 2 * (dot(L, N)) * (N-L);
}

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

#endif /* ISHAPES_H */
