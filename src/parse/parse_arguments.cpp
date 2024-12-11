#include "parse/parse_arguments.hpp"
#include <argparse/argparse.hpp>

void add_arguments(argparse::ArgumentParser &arguments)
{
  arguments.add_argument("--similarity")
    .help("Task: Calculate similarity between two GeoJSON files.")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--intersection")
    .help("Task: Calculate intersection report of a GeoJSON file.")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--area_error")
    .help(
      "Task: Calculate area error of the GeoJSON file with respect to the "
      "target area CSV file.")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--create_csv")
    .help(
      "Task: Create a template target area file for the provided GeoJSON "
      "file. After filling the target area infomation, the CSV file can be "
      "used with area error task.")
    .default_value(false)
    .implicit_value(true);

  arguments.add_argument("--map_1").help(
    "First GeoJSON file for similarity task.");

  arguments.add_argument("--map_2").help(
    "Second GeoJSON file for similarity task.");

  arguments.add_argument("--map").help(
    "GeoJSON file for intersection, area error, create target area CSV "
    "tasks.");

  arguments.add_argument("--target_area_csv")
    .help(
      "Target area CSV data to be used for area error calculation. The "
      "template file can be generated with create_csv task.");
}

argparse::ArgumentParser parse_arguments(const int argc, char *argv[])
{
  argparse::ArgumentParser arguments;
  add_arguments(arguments);
  try {
    arguments.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << "\n";
    std::cerr << arguments;
    std::exit(1);
  }
  return arguments;
}