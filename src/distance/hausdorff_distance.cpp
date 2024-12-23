#include "distance/hausdorff_distance.hpp"
#include "constants.hpp"
#include "distance/distance.hpp"
#include "map/map.hpp"
#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/discrete_hausdorff_distance.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <utility>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::linestring<Point> LineString;

double calculate_hausdorff_distance(
  const std::vector<Point> &ring1,
  const std::vector<Point> &ring2)
{
  LineString line1(ring1.begin(), ring1.end());
  LineString line2(ring2.begin(), ring2.end());

  return boost::geometry::discrete_hausdorff_distance(line1, line2);
}

double calculate_hausdorff_distance(
  const Polygon_with_holes &pwh1,
  const Polygon_with_holes &pwh2)
{
  return calculate_hausdorff_distance(pwh1.outer(), pwh2.outer());
}

std::pair<int, double> calculate_hausdorff_distance(
  const Region &region1,
  const Region &region2)
{
  return calculate_distance(
    region1,
    region2,
    [](const Polygon_with_holes &pwh1, const Polygon_with_holes &pwh2) {
      return calculate_hausdorff_distance(pwh1, pwh2);
    });
}

double calculate_hausdorff_distance(const Map &map1, const Map &map2)
{
  return calculate_distance(
    map1,
    map2,
    [](const Region &region1, const Region &region2) {
      return calculate_hausdorff_distance(region1, region2);
    });
}
