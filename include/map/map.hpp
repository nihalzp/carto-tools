#pragma once

#include "region/region.hpp"
#include <map>
#include <string>
#include <vector>

class Map
{
private:
  std::string map_name_;
  std::vector<Region> regions_;

public:
  Map(std::string);
  double compute_area();
  std::string get_map_name() const;
  void make_total_area_one();
  void scale(const double);
  void standardize_each_pwh_independently();
  void store_target_areas(const std::string);
  void translate(const double, const double);
  void update_regions_target_areas(
    const std::string,
    const std::map<std::string, double>);
  const std::vector<Region> &get_regions() const;
  void make_total_target_area_one();
  double get_total_target_area() const;
  Region find_matching_region(const Region &) const;
  double compute_xmin() const;
  double compute_xmax() const;
  double compute_ymin() const;
  double compute_ymax() const;
  void adjust_map_for_plotting();
};