#include "task/generate_target_csv.hpp"
#include "map/map.hpp"
#include "write/write_csv.hpp"
#include <argparse/argparse.hpp>
#include <string>

void generate_target_csv(const argparse::ArgumentParser &arguments)
{
  const std::string geo_file = arguments.get<std::string>("--map");

  std::cout
    << "Creating target area template CSV file for the GeoJSON file:\n";
  std::cout << "GeoJSON File: " << geo_file << "\n";

  Map map(geo_file);
  write_csv(map);
}
