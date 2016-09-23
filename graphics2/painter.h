#ifndef PAINTER_H
#define PAINTER_H
#include "Edge.h"
#include "Polygon.h"
#include <vector>
#include <queue>
#include <algorithm>


struct ColoredEdge {
  Edge edge;
  Color color;
};

//class to have clipping and scanline logic and return points
class Painter {
  // window edges
  const Point win_tl, win_br;

  std::vector<Polygon> polys;
  std::vector<Polygon> clipped_polys;
  Polygon::Builder unfinished_poly;
  bool clipping_mode;

public:
  explicit Painter(Point window_bottom_right);

  //resets all polygons
  void clear();

  queue<ColoredEdge> draw_polygons();
  void toggle_clipping();
  void handle_left_release(int x, int y);
  void handle_right_press(int x, int y);
  void handle_left_press(int x, int y);
  void handle_key_press(unsigned char key);

  // data struct for scanlines
  struct ActiveEdgeList {
    vector<Edge> active_edge_list; // should be sorted

  public:
    std::vector<Edge>& operator+=(const std::list<Edge>& a);
    void remove_ending_at(int line);
    void sort();
    std::vector<Edge> get_horizontals(int line) const;
  };
};

#endif /* PAINTER_H */
