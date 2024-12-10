#pragma once

#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

std::vector<Polygon_with_holes> extract_pwhs_from_feature(nlohmann::json);
std::vector<Region> extract_regions(nlohmann::json);
nlohmann::json read_geojson(const std::string &);