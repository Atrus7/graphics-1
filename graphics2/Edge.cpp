#include "Edge.h"

Edge::Edge(Point p1, Point p2) {
    start = p1;
    end = p2;

  if(end.x == start.x){
    slope = 0;
  }
  else{
    slope = (double(end.y - start.y) / double(end.x - start.x));
  }
}

#include <iostream>
// update currentX
float Edge::calc_current_x(int currentY) const {
  if(currentY < get_low().y){
    throw("Bad update of line");
  }
  if(get_start().x == get_end().x){
    return get_start().x;
  }
  float result = get_low().x + (currentY - get_low().y)/slope;
  return result;
}
