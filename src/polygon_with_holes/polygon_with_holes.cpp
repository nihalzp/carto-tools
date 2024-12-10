#include "polygon_with_holes/polygon_with_holes.h"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/strategies/transform.hpp>
#include <cmath>
#include <utility>

typedef boost::geometry::model::d2::point_xy<double> Point;

Polygon_with_holes::Polygon_with_holes(
  const boost::geometry::model::polygon<Point> &polygon)
    : boost::geometry::model::polygon<Point>(polygon)
{
}

double Polygon_with_holes::compute_area() const
{
  return abs(boost::geometry::area(*this));
}

void Polygon_with_holes::scale(const double factor)
{
  boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaler(
    factor);

  Polygon_with_holes scaled_polygon;
  boost::geometry::transform(*this, scaled_polygon, scaler);

  *this = std::move(scaled_polygon);
}

void Polygon_with_holes::translate(const double dx, const double dy)
{
  boost::geometry::strategy::transform::translate_transformer<double, 2, 2>
    translator(dx, dy);

  Polygon_with_holes translated_polygon;
  boost::geometry::transform(*this, translated_polygon, translator);

  *this = std::move(translated_polygon);
}

void Polygon_with_holes::standardize()
{
  Point centroid;
  boost::geometry::centroid(*this, centroid);
  translate(-centroid.x(), -centroid.y());

  const double area = compute_area();
  scale(1.0 / std::sqrt(std::abs(area)));
}

double Polygon_with_holes::compute_xmin() const
{
  boost::geometry::model::box<Point> bounding_box;
  boost::geometry::envelope(*this, bounding_box);
  return bounding_box.min_corner().get<0>();
}

double Polygon_with_holes::compute_xmax() const
{
  boost::geometry::model::box<Point> bounding_box;
  boost::geometry::envelope(*this, bounding_box);
  return bounding_box.max_corner().get<0>();
}

double Polygon_with_holes::compute_ymin() const
{
  boost::geometry::model::box<Point> bounding_box;
  boost::geometry::envelope(*this, bounding_box);
  return bounding_box.min_corner().get<1>();
}

double Polygon_with_holes::compute_ymax() const
{
  boost::geometry::model::box<Point> bounding_box;
  boost::geometry::envelope(*this, bounding_box);
  return bounding_box.max_corner().get<1>();
}
