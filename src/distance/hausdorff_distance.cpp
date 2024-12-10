#include "distance/hausdorff_distance.h"
#include "distance/distance.h"
#include "map/map.h"
#include "region/region.h"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/discrete_hausdorff_distance.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
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
  const Region &region1,
  const Region &region2)
{
  double max_distance = 0.0;

  for (int i = 0; i < region1.num_pwhs(); ++i) {
    const auto &pwh1 = region1.get_pwhs()[i];
    const auto &pwh2 = region2.get_pwhs()[i];

    double outer_distance =
      calculate_hausdorff_distance(pwh1.outer(), pwh2.outer());
    max_distance = combiner_maximum(max_distance, outer_distance);

    for (int j = 0; j < pwh1.inners().size(); ++j) {
      double hole_distance =
        calculate_hausdorff_distance(pwh1.inners()[j], pwh2.inners()[j]);
      max_distance = combiner_maximum(max_distance, hole_distance);
    }
  }

  return max_distance;
}

double calculate_hausdorff_distance(const Map &map1, const Map &map2)
{
  return calculate_distance(
    map1,
    map2,
    static_cast<double (*)(const Region &, const Region &)>(
      calculate_hausdorff_distance),
    combiner_maximum);
}
