#pragma once

#include <fstream>
#include <string>

class SVG
{
private:
  std::ofstream svg_file_;
  unsigned int width_, height_;

public:
  SVG(const std::string &, unsigned int, unsigned int);
  void add_point(double, double, const std::string & = "red", double = 0.9);
  void add_segment(
    double,
    double,
    double,
    double,
    const std::string & = "black",
    double = 0.4);
  ~SVG();
};
