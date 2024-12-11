#include "task/compute_area_error.hpp"
#include "error/area_weighted_mean_error.hpp"
#include "error/max_relative_area_error.hpp"
#include "map/map.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

void compute_area_error(const argparse::ArgumentParser &arguments)
{
  const std::string geo_file = arguments.get<std::string>("--map");

  const std::string target_area_file =
    arguments.get<std::string>("--target_area_csv");

  std::cout << "Calculating area error:\n";
  std::cout << "GeoJSON File: " << geo_file << "\n";
  std::cout << "Target Area CSV File: " << target_area_file << "\n";

  Map map(geo_file);
  map.make_total_area_one();

  map.store_target_areas(target_area_file);
  map.make_total_target_area_one();

  print_max_relative_area_error_report(map);
  print_area_weighted_mean_error(map);
}
