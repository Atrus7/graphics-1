#ifndef LIGHTING_H
#define LIGHTING_H
#include "vertex.h"
#include <cmath>

using namespace std;

struct SurfaceProperties {
  const Vertex AMB_COEFF;
  const Vertex DIFF_COEFF;
  const Vertex SPEC_COEFF;
  SurfaceProperties(Vertex amb_co, Vertex diff_co, Vertex spec_co): AMB_COEFF(amb_co), DIFF_COEFF(diff_co), SPEC_COEFF(spec_co) {};

  void print() const {
    AMB_COEFF.print();
    DIFF_COEFF.print();
    SPEC_COEFF.print();
  }

};

const Vertex AMB_INTENSITY(.5, .5, .5);
const int SPEC_EXP = 5;

struct Light {
  const Vertex L_LOCATION, L_INTENSITY;
  Light(Vertex loc, Vertex _intensity): L_LOCATION(loc), L_INTENSITY(_intensity){};
};



#endif
