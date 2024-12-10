#pragma once

#include "polygon_with_holes/polygon_with_holes.hpp"
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Region
{
private:
  std::map<std::string, std::string> properties_;
  std::vector<Polygon_with_holes> pwhs_;
  double target_area_;

public:
  Region(const nlohmann::json, const std::vector<std::string>);
  double compute_area() const;
  void scale(const double);
  void translate(const double, const double);
  void standardize_each_pwh_independently();
  void update_target_area(
    const std::string,
    const std::map<std::string, double>);
  void update_target_area(const double);
  double get_target_area() const;
  unsigned int get_num_pwhs() const;
  bool operator==(const Region &) const;
  double compute_xmin() const;
  double compute_xmax() const;
  double compute_ymin() const;
  double compute_ymax() const;
  const std::map<std::string, std::string> &get_properties() const;
  const std::vector<Polygon_with_holes> &get_pwhs() const;
};