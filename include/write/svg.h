#pragma once

#include <fstream>
#include <string>

class SVG
{
private:
  std::ofstream svg_file;
  int width, height;

public:
  SVG(const std::string &, int, int);

  void add_segment(
    double,
    double,
    double,
    double,
    const std::string & = "black",
    double = 0.4);

  void add_point(double, double, const std::string & = "red", double = 0.9);

  ~SVG();
};
