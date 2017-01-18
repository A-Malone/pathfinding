#ifndef SRC_CORE_VORONOI_HPP_
#define SRC_CORE_VORONOI_HPP_

#include "../common.hpp"
#include "../core/world.hpp"

#include <boost/polygon/voronoi.hpp>
#include <SFML/Graphics.hpp>

using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;

struct Point {
  int a;
  int b;
  Point(int x, int y) : a(x), b(y) {}
};

struct Segment {
  Point p0;
  Point p1;
  Segment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
  Segment(Point a, Point b) : p0(a), p1(b) {}
};

namespace boost {
namespace polygon {

template <>
struct geometry_concept<Point> {
  typedef point_concept type;
};

template <>
struct point_traits<Point> {
  typedef int coordinate_type;

  static inline coordinate_type get(
      const Point& point, orientation_2d orient) {
    return (orient == HORIZONTAL) ? point.a : point.b;
  }
};

template <>
struct geometry_concept<Segment> {
  typedef segment_concept type;
};

template <>
struct segment_traits<Segment> {
  typedef int coordinate_type;
  typedef Point point_type;

  static inline point_type get(const Segment& segment, direction_1d dir) {
    return dir.to_int() ? segment.p1 : segment.p0;
  }
};
}  // polygon
}  // boost


class VoronoiMap
{
public:
    VoronoiMap();

    bool update_diagram(const World* world);
    void render(sf::RenderWindow* window, float scale = 1.0);

private:
    voronoi_diagram<double>* m_map;    // Used to represent the map through which the user navigates

    voronoi_diagram<double>::edge_container_type m_filtered_edges;
};

#endif /* SRC_CORE_VORONOI_HPP_ */
