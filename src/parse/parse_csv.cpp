#include "parse/parse_csv.h"
#include <csv_parser/csv.hpp>
#include <map>
#include <string>
#include <vector>

csv::CSVReader read_csv(const std::string &csv_file)
{
  csv::CSVReader reader(csv_file);
  return reader;
}

std::map<std::string, std::vector<std::string>> parse_csv(
  const std::string &csv_file)
{
  csv::CSVReader reader = read_csv(csv_file);
  std::map<std::string, std::vector<std::string>> csv_data;
  for (csv::CSVRow &row : reader) {
    for (int i = 0; i < row.size(); ++i) {
      const std::string col_name = reader.get_col_names()[i];
      const std::string cell_value = row[i].get<std::string>();
      csv_data[col_name].push_back(cell_value);
    }
  }
  return csv_data;
}