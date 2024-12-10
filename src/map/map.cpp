#include "map/map.h"
#include "parse/parse_csv.h"
#include "parse/parse_geojson.h"
#include "region/region.h"
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Map::Map(std::string geofile)
{
  const nlohmann::json geojson = read_geojson(geofile);
  regions_ = extract_regions(geojson);
  map_name_ = geofile.substr(0, geofile.find_last_of("."));
}

double Map::area()
{
  double total_area = 0.0;
  for (const auto &region : regions_) {
    total_area += region.area();
  }
  return total_area;
};

Region Map::find_matching_region(const Region &other_region) const
{
  for (const auto &region : regions_) {
    if (region == other_region) {
      return region;
    }
  }
  throw std::runtime_error("No matching region found.");
}

std::string Map::get_map_name() const
{
  return map_name_;
}

const std::vector<Region> &Map::get_regions() const
{
  return regions_;
}

std::pair<bool, std::string> find_matching_property_header(
  const std::vector<Region> &regions,
  const std::vector<std::string> &properties)
{
  std::map<std::string, std::set<std::string>> matched_properties;
  for (const auto &region : regions) {
    for (const auto &[header, region_property] : region.get_properties()) {
      for (const auto &csv_property : properties) {
        if (region_property == csv_property) {
          matched_properties[header].insert(region_property);
        }
      }
    }
  }
  for (auto &[header, property_st] : matched_properties) {
    if (property_st.size() == regions.size()) {
      return {true, header};
    }
  }
  return {false, ""};
}

std::map<std::string, double> combine_property_with_target_area(
  const std::vector<std::string> &target_areas,
  const std::vector<std::string> &properties)
{
  std::map<std::string, double> property_to_target_area;
  for (int i = 0; i < properties.size(); ++i) {
    property_to_target_area[properties[i]] = stod(target_areas[i]);
  }
  return property_to_target_area;
}

std::pair<std::string, std::map<std::string, double>>
match_region_target_areas(
  const std::vector<Region> &regions,
  const std::map<std::string, std::vector<std::string>> csv_data)
{
  const std::string target_area_col_name = "Cartogram Data (eg. Population)";
  if (csv_data.count(target_area_col_name) == 0) {
    throw std::runtime_error(
      "Target areas column must have the name: " + target_area_col_name);
  }
  const std::vector<std::string> target_areas =
    csv_data.at(target_area_col_name);
  for (auto &[_, properties] : csv_data) {
    auto [match, header] = find_matching_property_header(regions, properties);
    if (match) {
      return std::make_pair(
        header,
        combine_property_with_target_area(target_areas, properties));
    }
  }
  throw std::runtime_error("No matching property found.");
}

void Map::scale(const double factor)
{
  for (auto &region : regions_) {
    region.scale(factor);
  }
}

double Map::get_total_target_area() const
{
  double total_target_area = 0.0;
  for (const auto &region : regions_) {
    total_target_area += region.get_target_area();
  }
  return total_target_area;
}

void Map::make_total_target_area_one()
{
  const double total_target_area = get_total_target_area();
  for (auto &region : regions_) {
    const double cur_target_area = region.get_target_area();
    region.update_target_area(cur_target_area / total_target_area);
  }
}

void Map::translate(const double dx, const double dy)
{
  for (auto &region : regions_) {
    region.translate(dx, dy);
  }
}

void Map::standardize_each_pwh_independently()
{
  for (auto &region : regions_) {
    region.standardize_each_pwh_independently();
  }
}

void Map::store_target_areas(const std::string target_area_file)
{
  const std::map<std::string, std::vector<std::string>> csv_data =
    parse_csv(target_area_file);
  auto [header, property_to_target_area] =
    match_region_target_areas(regions_, csv_data);
  update_regions_target_areas(header, property_to_target_area);
}

void Map::update_regions_target_areas(
  const std::string header,
  const std::map<std::string, double> property_to_target_area)
{
  for (auto &region : regions_) {
    region.update_target_area(header, property_to_target_area);
  }
}

void Map::make_total_area_one()
{
  const double total_area = area();
  const double scale_factor = 1.0 / std::sqrt(total_area);
  scale(scale_factor);
}

double Map::get_xmin() const
{
  double xmin = std::numeric_limits<double>::max();
  for (const auto &region : regions_) {
    xmin = std::min(xmin, region.get_xmin());
  }
  return xmin;
}

double Map::get_xmax() const
{
  double xmax = std::numeric_limits<double>::lowest();
  for (const auto &region : regions_) {
    xmax = std::max(xmax, region.get_xmax());
  }
  return xmax;
}

double Map::get_ymin() const
{
  double ymin = std::numeric_limits<double>::max();
  for (const auto &region : regions_) {
    ymin = std::min(ymin, region.get_ymin());
  }
  return ymin;
}

double Map::get_ymax() const
{
  double ymax = std::numeric_limits<double>::lowest();
  for (const auto &region : regions_) {
    ymax = std::max(ymax, region.get_ymax());
  }
  return ymax;
}

void Map::adjust_map_for_plotting()
{
  const double xmin = get_xmin();
  const double xmax = get_xmax();
  const double ymin = get_ymin();
  const double ymax = get_ymax();

  const double map_width = xmax - xmin;
  const double map_height = ymax - ymin;

  const double map_center_x = xmin + map_width / 2.0;
  const double map_center_y = ymin + map_height / 2.0;

  const double target_center_x = 512.0 / 2.0;
  const double target_center_y = 512.0 / 2.0;

  const double dx = target_center_x - map_center_x;
  const double dy = target_center_y - map_center_y;

  translate(dx, dy);

  const double new_xmin = get_xmin();
  const double new_xmax = get_xmax();
  const double new_ymin = get_ymin();
  const double new_ymax = get_ymax();

  const double translated_width = new_xmax - new_xmin;
  const double translated_height = new_ymax - new_ymin;

  const double padding = 50;
  const double x_scale = (512 - 2 * padding) / translated_width;
  const double y_scale = (512 - 2 * padding) / translated_height;
  const double factor = std::min(x_scale, y_scale);

  // Center before scaling
  translate(-target_center_x, -target_center_y);

  // Scale relative to the center
  scale(factor);

  // Translate back to the center
  translate(target_center_x, target_center_y);
}
