#include "write/write_csv.hpp"
#include "map/map.hpp"
#include "region/region.hpp"
#include <csv_parser/csv.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<std::vector<std::string>> extract_csv_rows(const Map &map)
{
  std::vector<std::vector<std::string>> csv_rows;

  const std::vector<Region> &regions = map.get_regions();

  std::vector<std::string> column_names;
  for (const auto &[header, _] : regions[0].get_properties()) {
    column_names.push_back(header);
  }

  column_names.push_back("Cartogram Data (eg. Population)");

  csv_rows.push_back(column_names);

  for (const auto &region : regions) {
    std::vector<std::string> row;
    for (const auto &[_, value] : region.get_properties()) {
      row.push_back(value);
    }
    row.push_back("");
    csv_rows.push_back(row);
  }

  return csv_rows;
};

void write_csv(const Map &map)
{
  const std::string csv_file_name = map.get_map_name() + "_template.csv";

  std::cout << "Writing Target Template CSV File: " << csv_file_name << "\n";

  std::ofstream out_file_csv(csv_file_name);

  std::vector<std::vector<std::string>> csv_rows = extract_csv_rows(map);

  auto writer = csv::make_csv_writer(out_file_csv);

  for (const auto &row : csv_rows) {
    writer << row;
  }

  out_file_csv.close();
}
