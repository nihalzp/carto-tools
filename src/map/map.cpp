#include "map/map.hpp"
#include "constants.hpp"
#include "parse/parse_csv.hpp"
#include "parse/parse_geojson.hpp"
#include "region/region.hpp"
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Map::Map(const std::string &geofile)
{
  const nlohmann::json geojson = read_geojson(geofile);
  regions_ = extract_regions(geojson);
  map_name_ = geofile.substr(0, geofile.find_last_of("."));
}

std::string Map::get_map_name() const
{
  return map_name_;
}

const std::vector<Region> &Map::get_regions() const
{
  return regions_;
}

double Map::compute_area() const
{
  double total_area = 0.0;
  for (const auto &region : regions_) {
    total_area += region.compute_area();
  }
  return total_area;
};

double Map::compute_xmax() const
{
  double xmax = std::numeric_limits<double>::lowest();
  for (const auto &region : regions_) {
    xmax = std::max(xmax, region.compute_xmax());
  }
  return xmax;
}

double Map::compute_xmin() const
{
  double xmin = std::numeric_limits<double>::max();
  for (const auto &region : regions_) {
    xmin = std::min(xmin, region.compute_xmin());
  }
  return xmin;
}

double Map::compute_ymax() const
{
  double ymax = std::numeric_limits<double>::lowest();
  for (const auto &region : regions_) {
    ymax = std::max(ymax, region.compute_ymax());
  }
  return ymax;
}

double Map::compute_ymin() const
{
  double ymin = std::numeric_limits<double>::max();
  for (const auto &region : regions_) {
    ymin = std::min(ymin, region.compute_ymin());
  }
  return ymin;
}

static std::pair<bool, std::string> find_matching_property_header(
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
  for (const auto &[header, property_st] : matched_properties) {
    if (property_st.size() == regions.size()) {
      return {true, header};
    }
  }
  return {false, ""};
}

static std::map<std::string, double> combine_property_with_target_area(
  const std::vector<std::string> &target_areas,
  const std::vector<std::string> &properties)
{
  std::map<std::string, double> property_to_target_area;
  for (unsigned int i = 0; i < properties.size(); ++i) {
    property_to_target_area[properties[i]] = stod(target_areas[i]);
  }
  return property_to_target_area;
}

static std::pair<std::string, std::map<std::string, double>>
match_region_target_areas(
  const std::vector<Region> &regions,
  const std::map<std::string, std::vector<std::string>> &csv_data)
{
  if (csv_data.count(TARGET_AREA_COL_NAME) == 0) {
    throw std::runtime_error(
      "Target areas column must have the name: " + TARGET_AREA_COL_NAME);
  }
  const std::vector<std::string> target_areas =
    csv_data.at(TARGET_AREA_COL_NAME);
  for (const auto &[_, properties] : csv_data) {
    const auto [match, header] =
      find_matching_property_header(regions, properties);
    if (match) {
      return std::make_pair(
        header,
        combine_property_with_target_area(target_areas, properties));
    }
  }
  throw std::runtime_error(
    "No matching property found in GeoJSON for the columns given in the CSV.");
}

void Map::store_target_areas(const std::string &target_area_file)
{
  const std::map<std::string, std::vector<std::string>> csv_data =
    parse_csv(target_area_file);
  const auto [header, property_to_target_area] =
    match_region_target_areas(regions_, csv_data);
  update_regions_target_areas(header, property_to_target_area);
}

void Map::update_regions_target_areas(
  const std::string &header,
  const std::map<std::string, double> &property_to_target_area)
{
  for (auto &region : regions_) {
    region.update_target_area(header, property_to_target_area);
  }
}

double Map::compute_total_target_area() const
{
  double total_target_area = 0.0;
  for (const auto &region : regions_) {
    total_target_area += region.get_target_area();
  }
  return total_target_area;
}

void Map::scale(const double factor)
{
  for (auto &region : regions_) {
    region.scale(factor);
  }
}

void Map::translate(const double dx, const double dy)
{
  for (auto &region : regions_) {
    region.translate(dx, dy);
  }
}

void Map::make_total_target_area_one()
{
  const double total_target_area = compute_total_target_area();
  for (auto &region : regions_) {
    const double cur_target_area = region.get_target_area();
    region.update_target_area(cur_target_area / total_target_area);
  }
}

void Map::standardize_each_pwh_independently()
{
  for (auto &region : regions_) {
    region.standardize_each_pwh_independently();
  }
}

void Map::make_total_area_one()
{
  const double total_area = compute_area();
  const double scale_factor = 1.0 / std::sqrt(total_area);
  scale(scale_factor);
}

void Map::adjust_map_for_plotting()
{
  const double xmin = compute_xmin();
  const double xmax = compute_xmax();
  const double ymin = compute_ymin();
  const double ymax = compute_ymax();

  const double map_width = xmax - xmin;
  const double map_height = ymax - ymin;

  const double map_center_x = xmin + map_width / 2.0;
  const double map_center_y = ymin + map_height / 2.0;

  const double target_center_x = 512.0 / 2.0;
  const double target_center_y = 512.0 / 2.0;

  const double dx = target_center_x - map_center_x;
  const double dy = target_center_y - map_center_y;

  translate(dx, dy);

  const double new_xmin = compute_xmin();
  const double new_xmax = compute_xmax();
  const double new_ymin = compute_ymin();
  const double new_ymax = compute_ymax();

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

std::string get_properties_string(
  const std::map<std::string, std::string> &prop)
{
  std::string prop_str = "";
  for (const auto &[header, value] : prop) {
    prop_str += header + ": " + value + ", ";
  }
  return prop_str;
}

const Region &Map::find_matching_region(const Region &other_region) const
{
  for (const auto &region : regions_) {
    if (region == other_region) {
      return region;
    }
  }
  throw std::runtime_error(
    "Regions could not be matched between two GeoJSON. For the first GeoJSON "
    "region: " +
    get_properties_string(other_region.get_properties()) +
    "no matching region found in the second GeoJSON.");
}