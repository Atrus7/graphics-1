#ifndef POLYGON_H
#define POLYGON_H
#include "Edge.h"
#include <vector>
#include <map>
#include <iostream>
#include <list>

using namespace std;
class Polygon;

enum RectEdge{ TOP, RIGHT, BOTTOM, LEFT};
class Clipper {
  Edge rect_edges[4];
  Point min, max;

public:
  Clipper(Point tl, Point br) :rect_edges({
      Edge(tl, Point(br.x, tl.y)),
        Edge(Point(br.x, tl.y), br),
        Edge(br, Point(tl.x, br.y)),
        Edge(Point(tl.x, br.y), tl)
        }
    ){}

  //does sutherland hodgeman
  Polygon clip_polygon(const Polygon& p1);
  vector<Point> in_out_converter(Edge e, RectEdge r);
};

struct Color {
  float r, g, b;		// Color (R,G,B values)
};

//class to handle edges and scan line logic
class Polygon {
  friend class Clipper;
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
  void print_info(){
    cout << "Polygon : ";
    map<int, list<Edge> >::iterator it = active_edge_table.begin();
    for(; it != active_edge_table.end(); ++it){
      for( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
        cout << "(";
        cout << it2->get_start().x << ", " << it2->get_start().y;
        cout << " -> ";
        cout << it2->get_end().x << ", " << it2->get_end().y;
        cout << ")\n";
      }
    }
  }


  class Builder {
    vector<Point> points;
    Color color;
  public:
    Builder(){
      float red = (rand() % 90);
      float green = (rand() % 90);
      float blue = (rand() % 90);
      color = Color{red / 100, green / 100, blue / 100};
    };
    Polygon build();
    void add_point(Point p);
  };
};

#endif /* POLYGON_H */
