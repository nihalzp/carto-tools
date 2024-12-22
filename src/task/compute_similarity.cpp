#include "task/compute_similarity.hpp"
#include "distance/frechet_distance.hpp"
#include "distance/hausdorff_distance.hpp"
#include "distance/symmetric_difference.hpp"
#include "map/map.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

void compute_similarity(const argparse::ArgumentParser &arguments)
{
  const std::string geo_file_1 = arguments.get<std::string>("--map_1");
  const std::string geo_file_2 = arguments.get<std::string>("--map_2");

  std::cout << "Calculating similarity between two GeoJSONs:\n";
  std::cout << "GeoJSON File 1: " << geo_file_1 << "\n";
  std::cout << "GeoJSON File 2: " << geo_file_2 << "\n";

  Map map1(geo_file_1);
  Map map2(geo_file_2);

  map1.sort_pwhs_in_each_region_by_area();
  map2.sort_pwhs_in_each_region_by_area();

  map1.standardize_each_pwh_independently();
  map2.standardize_each_pwh_independently();

  const double frechet_distance = calculate_frechet_distance(map1, map2);
  const double hausdorff_distance = calculate_hausdorff_distance(map1, map2);
  const double symmetric_difference =
    calculate_symmetric_difference(map1, map2);

  std::cout << "Frechet distance: " << frechet_distance << "\n";
  std::cout << "Hausdorff distance: " << hausdorff_distance << "\n";
  std::cout << "Symmetric difference: " << symmetric_difference << "\n";
}
