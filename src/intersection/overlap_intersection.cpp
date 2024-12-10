#include "intersection/overlap_intersection.h"
#include "map/map.h"
#include "polygon_with_holes/polygon_with_holes.h"
#include "region/region.h"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/segment.hpp>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::segment<Point> Segment;

bool is_point_part_of_ring(const Point &point, const std::vector<Point> &ring)
{
  for (unsigned int i = 0; i < ring.size(); ++i) {
    if (boost::geometry::equals(ring[i], point)) {
      return true;
    }
  }
  return false;
}

bool is_point_part_of_pwh(const Point &point, const Polygon_with_holes &pwh)
{
  if (is_point_part_of_ring(point, pwh.outer())) {
    return true;
  }

  for (const auto &inner_ring : pwh.inners()) {
    if (is_point_part_of_ring(point, inner_ring)) {
      return true;
    }
  }

  return false;
}

std::vector<Point> get_overlap_intersections(const Map &map)
{
  std::vector<Point> overlap_intersections;

  const auto &regions = map.get_regions();

  for (unsigned int i = 0; i < regions.size(); ++i) {
    for (unsigned int j = i + 1; j < regions.size(); ++j) {
      const auto &region_i_pwhs = regions[i].get_pwhs();
      const auto &region_j_pwhs = regions[j].get_pwhs();

      for (const auto &pwh1 : region_i_pwhs) {
        for (const auto &pwh2 : region_j_pwhs) {
          std::vector<Point> intersections;
          boost::geometry::intersection(pwh1, pwh2, intersections);

          // Filter out shared boundary points
          for (const auto &point : intersections) {
            if (
              !is_point_part_of_pwh(point, pwh1) or
              !is_point_part_of_pwh(point, pwh2)) {
              overlap_intersections.push_back(point);
            }
          }
        }
      }
    }
  }

  return overlap_intersections;
}
