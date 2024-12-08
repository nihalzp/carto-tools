#pragma once

#include <fstream>
#include <string>

class SVG
{
private:
  std::ofstream svg_file;
  int width, height;

public:
  SVG(const std::string &filename, int w, int h);

  void add_segment(
    double x1,
    double y1,
    double x2,
    double y2,
    const std::string &color = "black",
    double stroke_width = 0.4);

  void add_point(
    double x,
    double y,
    const std::string &color = "red",
    double radius = 0.9);

  ~SVG();
};
