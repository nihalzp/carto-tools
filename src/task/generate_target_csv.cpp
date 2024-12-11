#include "task/generate_target_csv.hpp"
#include "map/map.hpp"
#include "write/write_csv.hpp"
#include <argparse/argparse.hpp>
#include <string>

void generate_target_csv(const argparse::ArgumentParser &arguments)
{
  std::string geo_file = arguments.get<std::string>("--map_1");

  Map map(geo_file);
  write_csv(map);
}
