#include "parse/parse_geojson.hpp"
#include "map/map.hpp"
#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

nlohmann::json read_geojson(const std::string &geofile)
{
  nlohmann::json geojson;
  std::ifstream file(geofile);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file " + geofile);
  }

  file >> geojson;
  file.close();

  return geojson;
}

std::vector<std::string> extract_unique_property_headers(
  nlohmann::json geojson)
{
  std::map<std::string, std::set<std::string>> properties;
  const unsigned int num_regions = geojson["features"].size();
  for (const auto &feature : geojson["features"]) {
    for (const auto &[header, property_json] : feature["properties"].items()) {
      std::string property = property_json.is_string()
                               ? property_json.get<std::string>()
                               : property_json.dump();
      properties[header].insert(property);
    }
  }
  std::vector<std::string> unique_property_headers;
  for (auto &[header, property_st] : properties) {
    if (property_st.size() == num_regions) {
      unique_property_headers.push_back(header);
    }
  }
  return unique_property_headers;
}

Polygon_with_holes extract_pwh(nlohmann::json coordinates)
{
  Polygon_with_holes pwh;

  const auto outer = coordinates[0];
  for (const auto &point : outer) {
    pwh.outer().push_back(
      Point(point[0].get<double>(), point[1].get<double>()));
  }
  for (unsigned int i = 1; i < coordinates.size(); ++i) {
    pwh.inners().resize(i);
    for (const auto &point : coordinates[i]) {
      pwh.inners()[i - 1].push_back(
        Point(point[0].get<double>(), point[1].get<double>()));
    }
  }
  return pwh;
}

std::vector<Polygon_with_holes> extract_pwhs_from_feature(
  nlohmann::json feature)
{
  std::vector<Polygon_with_holes> pwhs;

  auto geometry = feature["geometry"];

  if (geometry["type"] == "MultiPolygon") {
    const auto coordinates_collection = geometry["coordinates"];

    for (const auto &coordinates : coordinates_collection) {
      pwhs.push_back(extract_pwh(coordinates));
    }
  } else if (geometry["type"] == "Polygon") {
    const auto coordinates = geometry["coordinates"];
    pwhs.push_back(extract_pwh(coordinates));
  }
  return pwhs;
}

std::vector<Region> extract_regions(nlohmann::json geojson)
{
  std::vector<Region> regions;
  std::vector<std::string> unique_property_headers =
    extract_unique_property_headers(geojson);
  for (const auto &feature : geojson["features"]) {
    Region region(feature, unique_property_headers);
    regions.push_back(region);
  }
  return regions;
}