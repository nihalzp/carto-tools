#include "task/report_intersections.hpp"
#include "intersection/overlap_intersection.hpp"
#include "intersection/self_intersection.hpp"
#include "map/map.hpp"
#include "write/write_svg.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <vector>
#include <fstream>

void report_intersections(const argparse::ArgumentParser &arguments)
{
  const std::string geo_file = arguments.get<std::string>("--map");

  std::cout << "Calculating intersection report for the GeoJSON file:\n";
  std::cout << "GeoJSON File: " << geo_file << "\n";

  Map map(geo_file);
  map.adjust_map_for_plotting();

  const std::vector<Point> self_intersections = get_self_intersections(map);
  const std::vector<Point> overlap_intersections =
    get_overlap_intersections(map);

  std::cout << "Number of self-intersection points: "
            << self_intersections.size() << "\n";
  std::cout << "Number of overlap intersection points: "
            << overlap_intersections.size() << "\n";

  // Extract the name for the CSV file
  std::string csv_filename = "intersections.csv";

  // Output results to CSV file
  std::ofstream csv_file;
  csv_file.open(csv_filename, std::ios::app);

  // Check if the file is empty and write headers if necessary
  if (csv_file.tellp() == 0) {
    csv_file << "map_name,self-intersections,overlap intersections\n";
  }

  std::string map_filename = geo_file.substr(geo_file.find_last_of("/\\") + 1);
  csv_file << map_filename << "," << self_intersections.size() << "," << overlap_intersections.size() << "\n";
  csv_file.close();

  if (self_intersections.size() > 0 || overlap_intersections.size() > 0) {
    plot_map_with_intersections(map, self_intersections, overlap_intersections);
  }
}
