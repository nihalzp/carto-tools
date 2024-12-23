#include "distance/symmetric_difference.hpp"
#include "constants.hpp"
#include "distance/distance.hpp"
#include "map/map.hpp"
#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/sym_difference.hpp>
#include <utility>

double calculate_symmetric_difference(
  const Polygon_with_holes &pwh1,
  const Polygon_with_holes &pwh2)
{
  std::vector<Polygon_with_holes> sym_difference_result;
  boost::geometry::sym_difference(pwh1, pwh2, sym_difference_result);

  double symmetric_difference = 0.0;
  for (const auto &diff_poly : sym_difference_result) {
    symmetric_difference += diff_poly.compute_area();
  }

  return symmetric_difference;
}

std::pair<int, double> calculate_symmetric_difference(
  const Region &region1,
  const Region &region2)
{
  return calculate_distance(
    region1,
    region2,
    [](const Polygon_with_holes &pwh1, const Polygon_with_holes &pwh2) {
      return calculate_symmetric_difference(pwh1, pwh2);
    });
}

double calculate_symmetric_difference(const Map &map1, const Map &map2)
{
  return calculate_distance(
    map1,
    map2,
    [](const Region &region1, const Region &region2) {
      return calculate_symmetric_difference(region1, region2);
    });
}
