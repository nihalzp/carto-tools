#include "task/report_intersections.hpp"
#include "intersection/overlap_intersection.hpp"
#include "intersection/self_intersection.hpp"
#include "map/map.hpp"
#include "write/write_svg.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <vector>

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

  plot_map_with_intersections(map, self_intersections, overlap_intersections);
}
