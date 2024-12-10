#include "distance/frechet_distance.h"
#include "distance/hausdorff_distance.h"
#include "distance/symmetric_difference.h"
#include "error/area_weighted_mean_error.h"
#include "error/max_relative_area_error.h"
#include "intersection/overlap_intersection.h"
#include "intersection/self_intersection.h"
#include "map/map.h"
#include "parse/parse_arguments.h"
#include "write/write_csv.h"
#include "write/write_svg.h"
#include <argparse/argparse.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
  argparse::ArgumentParser arguments = parse_arguments(argc, argv);

  if (arguments.get<bool>("--similarity")) {
    std::string geofile1 = arguments.get<std::string>("--map_1");
    std::string geofile2 = arguments.get<std::string>("--map_2");

    std::cout << "Comparing files:\n";
    std::cout << "File 1: " << geofile1 << "\n";
    std::cout << "File 2: " << geofile2 << "\n";

    Map map1(geofile1);
    Map map2(geofile2);

    map1.standardize_each_pwh_independently();
    map2.standardize_each_pwh_independently();

    double frechet_distance = calculate_frechet_distance(map1, map2);
    double hausdorff_distance = calculate_hausdorff_distance(map1, map2);
    double symmetric_difference = calculate_symmetric_difference(map1, map2);

    std::cout << "Frechet distance: " << frechet_distance << "\n";
    std::cout << "Hausdorff distance: " << hausdorff_distance << "\n";
    std::cout << "Symmetric difference: " << symmetric_difference << "\n";

    return 0;
  }

  if (arguments.get<bool>("--create_csv")) {
    std::string geofile = arguments.get<std::string>("--map_1");

    Map map(geofile);

    write_csv(map);
    return 0;
  }

  if (arguments.get<bool>("--area_error")) {
    std::string geofile = arguments.get<std::string>("--map_1");

    Map map(geofile);
    map.make_total_area_one();

    std::string target_area_file =
      arguments.get<std::string>("--cart_area_csv");

    map.store_target_areas(target_area_file);
    map.make_total_target_area_one();

    print_max_relative_area_error_report(map);
    print_area_weighted_mean_error(map);
    return 0;
  }

  if (arguments.get<bool>("--intersection")) {
    std::string geofile = arguments.get<std::string>("--map_1");

    Map map(geofile);

    map.adjust_map_for_plotting();

    const std::vector<Point> self_intersections = get_self_intersections(map);
    const std::vector<Point> overlap_intersections =
      get_overlap_intersections(map);

    std::cout << "Number of self-intersection points: "
              << self_intersections.size() << "\n";

    std::cout << "Number of overlap intersection points: "
              << overlap_intersections.size() << "\n";

    plot_map_with_intersections(
      map,
      self_intersections,
      overlap_intersections);
    return 0;
  }
}
