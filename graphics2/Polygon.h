#ifndef POLYGON_H
#define POLYGON_H
#include "Edge.h"
#include <vector>
#include <map>
#include <list>

using namespace std;

struct Color {
	float r, g, b;		// Color (R,G,B values)
};

//class to handle edges and scan line logic
class Polygon {
  Polygon(Color c): color(c){}; //use a builder
  void add_edge(int key, const Edge &e); // builder usage

  // expecting a sparse table(<= 10 polys), so just use a map from
  // y -> list of edges starting at y
  map<int, list<Edge> > active_edge_table;
  const Color color;

public:
  list<Edge> get_edges_starting_at(int line);
  list<Edge> get_edges_ending_at(int line);
  Color get_color() const {
    return color;
  }


  class Builder {
    vector<Point> points;
    Color color;
  public:
    Builder(){};
    Polygon build();
    void set_color(Color c);
    void add_point(Point p);
  };
};

#endif /* POLYGON_H */
