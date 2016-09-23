#include "Edge.h"

Edge::Edge(Point p1, Point p2) {
  if(p1.y < p2.y){
    start = p1;
    end = p2;
  }
  else if(p1.y > p2.y){
    start = p2;
    end = p1;
  } else {
    //horizontal
    throw("Edge error: Horizontal line");
  }
  slope = (double(end.y - start.y) / double(end.x - start.x));
}

// update currentX
int Edge::calc_current_x(int currentY) const {
  if(currentY < start.y){
    throw("Bad update of line");
  }
    return start.x + slope * (currentY - start.y);
}



