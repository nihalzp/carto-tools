#include "intersection/self_intersection.h"
#include "map/map.h"
#include "polygon_with_holes/polygon_with_holes.h"
#include "region/region.h"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::segment<Point> Segment;

std::vector<Segment> collect_segments(const std::vector<Point> &ring)
{
  std::vector<Segment> segments;
  for (int i = 0; i < ring.size(); ++i) {
    segments.emplace_back(ring[i], ring[(i + 1) % ring.size()]);
  }
  return segments;
}

std::vector<Segment> collect_segments(const Polygon_with_holes &pwh)
{
  std::vector<Segment> segments;

  auto outer_segments = collect_segments(pwh.outer());
  segments.insert(
    segments.end(),
    outer_segments.begin(),
    outer_segments.end());

  for (const auto &inner_ring : pwh.inners()) {
    auto inner_segments = collect_segments(inner_ring);
    segments.insert(
      segments.end(),
      inner_segments.begin(),
      inner_segments.end());
  }

  return segments;
}

bool share_endpoint(const Segment &seg1, const Segment &seg2)
{
  return boost::geometry::equals(seg1.first, seg2.first) ||
         boost::geometry::equals(seg1.first, seg2.second) ||
         boost::geometry::equals(seg1.second, seg2.first) ||
         boost::geometry::equals(seg1.second, seg2.second);
}

std::vector<Point> get_self_intersections(const Polygon_with_holes &pwh)
{
  std::vector<Point> intersection_points;

  std::vector<Segment> all_segments = collect_segments(pwh);

  for (int i = 0; i < all_segments.size(); ++i) {
    for (int j = i + 1; j < all_segments.size(); ++j) {
      if (share_endpoint(all_segments[i], all_segments[j])) {
        continue;
      }

      std::vector<Point> segment_intersections;
      boost::geometry::intersection(
        all_segments[i],
        all_segments[j],
        segment_intersections);

      for (const auto &point : segment_intersections) {
        intersection_points.push_back(point);
      }
    }
  }

  return intersection_points;
}

std::vector<Point> get_self_intersections(const Region &region)
{
  std::vector<Point> intersection_pts;
  for (const auto &pwh : region.pwhs) {
    std::vector<Point> intersection_pts_pwh = get_self_intersections(pwh);
    intersection_pts.insert(
      intersection_pts.end(),
      intersection_pts_pwh.begin(),
      intersection_pts_pwh.end());
  }
  return intersection_pts;
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
  return intersection_pts;
}