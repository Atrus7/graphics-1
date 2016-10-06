#ifndef PAINTER_H
#define PAINTER_H
#include "Edge.h"
#include "Polygon.h"
#include <vector>
#include <queue>
#include <algorithm>
class EdgeSorter {
  int line;
public:
  EdgeSorter(int _line) : line(_line) {}

  void set_line(int _line) {line = _line;}
  bool operator()(Edge  o1, Edge  o2) const {
    return o1.calc_current_x(line) < o2.calc_current_x(line);
  }
};

struct ColoredEdge {
  Edge edge;
  Color color;
};

//class to have clipping and scanline logic and return points
class Painter {
  // window edges
  Point clip_tl, clip_br;
  const Point win_br;

  std::vector<Polygon> polys;
  std::vector<Polygon> clipped_polys;
  Polygon::Builder unfinished_poly;

  // 0 means not clipping, 1 means currently getting clipping rectangle, 2 means done clipping
  int clipping_mode;
  void clip();
public:
  explicit Painter(Point window_bottom_right);
  void set_tl_clipping(Point clip_point){clip_tl = clip_point;}
  void set_br_clipping(Point clip_point){clip_br = clip_point;}

  //resets all polygons
  void clear();

  queue<ColoredEdge> draw_polygons();
  void toggle_clipping(int i);
  void handle_left_release(int x, int y);
  void handle_right_press(int x, int y);
  void handle_left_press(int x, int y);
  void handle_mouse_move(int x, int y);
  void handle_key_press(unsigned char key);

  int get_clipping() const {return clipping_mode;}
  Point get_tl_clipping() const {return clip_tl;}
  Point get_br_clipping() const {return clip_br;}
  // data struct for scanlines
  struct ActiveEdgeList {
    vector<Edge> active_edge_list; // should be sorted

  public:
    std::vector<Edge>& operator+=(const std::list<Edge>& a);
    void remove_ending_at(int line);
    std::vector<Edge> get_horizontals(int line) const;
  };
};

#endif /* PAINTER_H */
