#pragma once

#include "polygon_with_holes/polygon_with_holes.h"
#include <map>
#include <nlohmann/json.hpp>
#include <string>

class Region
{
public:
  Region(const nlohmann::json, const std::vector<std::string>);
  std::map<std::string, std::string> properties;
  std::vector<Polygon_with_holes> pwhs;
  double target_area_;
  double area() const;
  void scale(const double);
  void translate(const double, const double);
  void standardize_each_pwh_independently();
  void update_target_area(
    const std::string,
    const std::map<std::string, double>);
  void update_target_area(const double);
  double get_target_area() const;
  unsigned int num_pwhs() const;
  bool operator==(const Region &) const;
  double get_xmin() const;
  double get_xmax() const;
  double get_ymin() const;
  double get_ymax() const;
};