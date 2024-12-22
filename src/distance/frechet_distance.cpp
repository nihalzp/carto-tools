#include "distance/frechet_distance.hpp"
#include "constants.hpp"
#include "distance/distance.hpp"
#include "map/map.hpp"
#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/discrete_frechet_distance.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::linestring<Point> LineString;

double calculate_frechet_distance(
  const std::vector<Point> &ring1,
  const std::vector<Point> &ring2)
{
  LineString line1(ring1.begin(), ring1.end());
  LineString line2(ring2.begin(), ring2.end());
  return boost::geometry::discrete_frechet_distance(line1, line2);
}

double calculate_frechet_distance(const Region &region1, const Region &region2)
{
  double total_distance = 0.0;
  const unsigned int possible_num_pwhs_to_compare = std::min(
    {region1.get_num_pwhs(), region2.get_num_pwhs(), NUM_PWH_TO_COMPARE});
  for (unsigned int i = 0; i < possible_num_pwhs_to_compare; ++i) {
    const Polygon_with_holes &pwh1 = region1.get_pwhs()[i];
    const Polygon_with_holes &pwh2 = region2.get_pwhs()[i];
    double outer_distance =
      calculate_frechet_distance(pwh1.outer(), pwh2.outer());
    total_distance = combiner_addition(total_distance, outer_distance);
    for (unsigned int j = 0; j < pwh1.inners().size(); ++j) {
      double hole_distance =
        calculate_frechet_distance(pwh1.inners()[j], pwh2.inners()[j]);
      total_distance = combiner_addition(total_distance, hole_distance);
    }
  }
  return total_distance;
}

double calculate_frechet_distance(const Map &map1, const Map &map2)
{
  return calculate_distance(
    map1,
    map2,
    static_cast<double (*)(const Region &, const Region &)>(
      calculate_frechet_distance),
    combiner_addition);
}