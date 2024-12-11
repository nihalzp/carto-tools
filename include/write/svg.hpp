#pragma once

#include <fstream>
#include <string>

class SVG
{
private:
  std::ofstream svg_file_;
  unsigned int width_, height_;

public:
  SVG(const std::string &, const unsigned int, const unsigned int);
  void add_point(
    const double,
    const double,
    const std::string & = "red",
    const double = 0.9);
  void add_segment(
    const double,
    const double,
    const double,
    const double,
    const std::string & = "black",
    const double = 0.4);
  ~SVG();
};
