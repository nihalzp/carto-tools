#include "parse/parse_arguments.h"
#include <argparse/argparse.hpp>

void add_arguments(argparse::ArgumentParser &arguments)
{
  arguments.add_argument("--similarity", "-s")
    .help("Calculate similarity between two GeoJSON files")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--intersection", "-i")
    .help("Calculate intersection report of a GeoJSON file")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--create_csv", "-c")
    .help("Create a CSV file for area error calculation")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--area_error", "-a")
    .help("Calculate area error between a cartogram map")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--map_1", "-m1")
    .help(
      "First GeoJSON file. For area error calculation, this is the sole "
      "geojson cartogram file")
    .required();

  arguments.add_argument("--map_2", "-m2").help("Second GeoJSON file");

  arguments.add_argument("--cart_area_csv", "-cd")
    .help("Cartogram csv data to be used for area error calculation");
}

argparse::ArgumentParser parse_arguments(int argc, char *argv[])
{
  argparse::ArgumentParser arguments;
  add_arguments(arguments);
  arguments.parse_args(argc, argv);
  return arguments;
}