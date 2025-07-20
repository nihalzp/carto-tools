#include "intersection/self_intersection.hpp"
#include "map/map.hpp"
#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::segment<Point> Segment;

bool share_endpoint(const Segment &seg1, const Segment &seg2)
{
  return boost::geometry::equals(seg1.first, seg2.first) ||
         boost::geometry::equals(seg1.first, seg2.second) ||
         boost::geometry::equals(seg1.second, seg2.first) ||
         boost::geometry::equals(seg1.second, seg2.second);
}

std::vector<Point> get_self_intersections(const std::vector<Point> &ring)
{
  std::vector<Segment> segments;
  for (unsigned int i = 0; i < ring.size(); ++i) {
    segments.emplace_back(ring[i], ring[(i + 1) % ring.size()]);
  }
  std::vector<Point> intersection_points;
  for (unsigned int i = 0; i < segments.size(); ++i) {
    for (unsigned int j = i + 1; j < segments.size(); ++j) {
      if (share_endpoint(segments[i], segments[j])) {
        continue;
      }

      std::vector<Point> segment_intersections;
      boost::geometry::intersection(
        segments[i],
        segments[j],
        segment_intersections);

      for (const auto &point : segment_intersections) {
        intersection_points.push_back(point);
      }
    }
  }

  return intersection_points;
}

std::vector<Point> get_self_intersections(const Polygon_with_holes &pwh)
{
  std::vector<Point> intersections;
  for (const auto &inner_ring : pwh.inners()) {
    std::vector<Point> inner_intersections =
      get_self_intersections(inner_ring);
    intersections.insert(
      intersections.end(),
      inner_intersections.begin(),
      inner_intersections.end());
  }
  std::vector<Point> outer_intersections = get_self_intersections(pwh.outer());
  intersections.insert(
    intersections.end(),
    outer_intersections.begin(),
    outer_intersections.end());

  return intersections;
}

std::vector<Point> get_self_intersections(const Region &region)
{
  std::vector<Point> intersection_points;
  const std::vector<Polygon_with_holes> &pwhs = region.get_pwhs();
  for (const auto &pwh : pwhs) {
    std::vector<Point> pwh_intersections = get_self_intersections(pwh);
    intersection_points.insert(
      intersection_points.end(),
      pwh_intersections.begin(),
      pwh_intersections.end());
  }
  return intersection_points;
}

std::vector<Point> get_self_intersections(const Map &map)
{
  std::vector<Point> intersection_pts;
  for (const auto &region : map.get_regions()) {
    std::vector<Point> intersection_pts_region =
      get_self_intersections(region);
    intersection_pts.insert(
      intersection_pts.end(),
      intersection_pts_region.begin(),
      intersection_pts_region.end());
  }

  auto by_xy = [](const Point &a, const Point &b) {
    return std::tie(a.x(), a.y()) < std::tie(b.x(), b.y());
  };

  std::sort(intersection_pts.begin(), intersection_pts.end(), by_xy);
  intersection_pts.erase(
    std::unique(
      intersection_pts.begin(),
      intersection_pts.end(),
      [](const Point &a, const Point &b) {
        return a.x() == b.x() && a.y() == b.y();
      }),
    intersection_pts.end());
  return intersection_pts;
}
