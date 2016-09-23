#ifndef EDGE_H
#define EDGE_H
#include <algorithm>

// integer point. Any point within the window size would fall within on a pixel
struct Point {
  int x, y;
  Point(int _x, int _y): x(_x), y(_y){};
  Point():x(), y(){};
};

class Edge
{
  double slope;
  Point start, end;
public:
  Edge(Point start, Point end);
  int calc_current_x(int) const;

  Point get_start() const { return start; }
  Point get_end() const { return end; }
};


#endif /* EDGE_H */
