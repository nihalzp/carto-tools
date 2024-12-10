#include "region/region.h"
#include "parse/parse_geojson.h"
#include "polygon_with_holes/polygon_with_holes.h"
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

Region::Region(
  nlohmann::json feature,
  std::vector<std::string> unique_property_headers)
{
  for (const auto &header : unique_property_headers) {
    auto property_json = feature["properties"][header];
    std::string property = property_json.is_string()
                             ? property_json.get<std::string>()
                             : property_json.dump();
    properties_[header] = property;
  }
  pwhs_ = extract_pwhs_from_feature(feature);
}

double Region::area() const
{
  double total_area = 0.0;
  for (const auto &pwh : pwhs_) {
    total_area += pwh.area();
  }
  return total_area;
}

unsigned int Region::num_pwhs() const
{
  return pwhs_.size();
}

void Region::scale(const double factor)
{
  for (auto &pwh : pwhs_) {
    pwh.scale(factor);
  }
}

const std::map<std::string, std::string> &Region::get_properties() const
{
  return properties_;
}

const std::vector<Polygon_with_holes> &Region::get_pwhs() const
{
  return pwhs_;
}

void Region::standardize_each_pwh_independently()
{
  for (auto &pwh : pwhs_) {
    pwh.standardize();
  }
}

void Region::update_target_area(
  const std::string header,
  const std::map<std::string, double> property_to_target_area)
{
  double target_area = property_to_target_area.at(properties_.at(header));
  this->update_target_area(target_area);
}

double Region::get_target_area() const
{
  return target_area_;
}

void Region::update_target_area(const double target_area)
{
  target_area_ = target_area;
}

void Region::translate(const double dx, const double dy)
{
  for (auto &pwh : pwhs_) {
    pwh.translate(dx, dy);
  }
}

bool Region::operator==(const Region &other) const
{
  if (properties_ == other.get_properties()) {
    return true;
  }
  for (auto &[header, property] : properties_) {
    for (auto &[other_header, other_property] : other.get_properties()) {
      if (property == other_property) {
        return true;
      }
    }
  }
  return false;
}

double Region::get_xmin() const
{
  double xmin = std::numeric_limits<double>::max();
  for (const auto &pwh : pwhs_) {
    xmin = std::min(xmin, pwh.get_xmin());
  }
  return xmin;
}

double Region::get_xmax() const
{
  double xmax = std::numeric_limits<double>::lowest();
  for (const auto &pwh : pwhs_) {
    xmax = std::max(xmax, pwh.get_xmax());
  }
  return xmax;
}

double Region::get_ymin() const
{
  double ymin = std::numeric_limits<double>::max();
  for (const auto &pwh : pwhs_) {
    ymin = std::min(ymin, pwh.get_ymin());
  }
  return ymin;
}

double Region::get_ymax() const
{
  double ymax = std::numeric_limits<double>::lowest();
  for (const auto &pwh : pwhs_) {
    ymax = std::max(ymax, pwh.get_ymax());
  }
  return ymax;
}