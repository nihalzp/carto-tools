#include "intersection/overlap_intersection.hpp"
#include "map/map.hpp"
#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/segment.hpp>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::segment<Point> Segment;

using Ring = std::vector<Point>;

bool is_point_part_of_ring(const Point &point, const std::vector<Point> &ring)
{
  for (unsigned int i = 0; i < ring.size(); ++i) {
    if (boost::geometry::equals(ring[i], point)) {
      return true;
    }
  }
  return false;
}

static std::vector<Segment> collect_segments(const Ring &ring)
{
  std::vector<Segment> segs;
  segs.reserve(ring.size());
  for (unsigned int i = 0; i < ring.size(); ++i)
    segs.emplace_back(ring[i], ring[(i + 1) % ring.size()]);
  return segs;
}

std::vector<Point> get_overlap_intersections(const Map &map)
{
  std::vector<Ring> rings;
  for (const auto &region : map.get_regions()) {
    for (const auto &pwh : region.get_pwhs()) {
      rings.push_back(pwh.outer());
      for (const auto &inner_ring : pwh.inners()) {
        rings.push_back(inner_ring);
      }
    }
  }

  std::vector<Point> overlap_intersections;
  for (unsigned int i = 0; i < rings.size(); ++i) {
    auto segs_i = collect_segments(rings[i]);

    for (unsigned int j = i + 1; j < rings.size(); ++j) {
      auto segs_j = collect_segments(rings[j]);

      for (const auto &si : segs_i) {
        for (const auto &sj : segs_j) {
          std::vector<Point> tmp;
          boost::geometry::intersection(si, sj, tmp);

          for (const auto &p : tmp) {
            if (
              !is_point_part_of_ring(p, rings[i]) &&
              !is_point_part_of_ring(p, rings[j]))
              overlap_intersections.push_back(p);
          }
        }
      }
    }
  }

  auto by_xy = [](const Point &a, const Point &b) {
    return std::tie(a.x(), a.y()) < std::tie(b.x(), b.y());
  };
  std::sort(overlap_intersections.begin(), overlap_intersections.end(), by_xy);
  overlap_intersections.erase(
    std::unique(
      overlap_intersections.begin(),
      overlap_intersections.end(),
      [](const Point &a, const Point &b) {
        return boost::geometry::equals(a, b);
      }),
    overlap_intersections.end());

  return overlap_intersections;
}
