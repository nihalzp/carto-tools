#pragma once

#include <map>
#include <string>

std::map<std::string, std::vector<std::string>> parse_csv(
  const std::string &csv_file);