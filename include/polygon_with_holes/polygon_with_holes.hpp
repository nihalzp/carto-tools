#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

typedef boost::geometry::model::d2::point_xy<double> Point;

class Polygon_with_holes : public boost::geometry::model::polygon<Point>
{
public:
  Polygon_with_holes() = default;
  Polygon_with_holes(const boost::geometry::model::polygon<Point> &);
  double compute_area() const;
  double compute_xmax() const;
  double compute_xmin() const;
  double compute_ymax() const;
  double compute_ymin() const;
  void scale(const double);
  void standardize();
  void translate(const double, const double);
};

// Register the new class as a polygon in Boost.Geometry
namespace boost
{
namespace geometry
{
namespace traits
{

template <> struct tag<Polygon_with_holes> {
  using type = polygon_tag;
};

template <> struct point_type<Polygon_with_holes> {
  using type = Point;
};

template <> struct ring_const_type<Polygon_with_holes> {
  using type = const boost::geometry::ring_type<
    boost::geometry::model::polygon<Point>>::type &;
};

template <> struct ring_mutable_type<Polygon_with_holes> {
  using type =
    boost::geometry::ring_type<boost::geometry::model::polygon<Point>>::type &;
};

template <> struct interior_const_type<Polygon_with_holes> {
  using type = const boost::geometry::interior_type<
    boost::geometry::model::polygon<Point>>::type &;
};

template <> struct interior_mutable_type<Polygon_with_holes> {
  using type = boost::geometry::interior_type<
    boost::geometry::model::polygon<Point>>::type &;
};

template <> struct exterior_ring<Polygon_with_holes> {
  static auto get(Polygon_with_holes &polygon) -> decltype(polygon.outer())
  {
    return polygon.outer();
  }

  static auto get(const Polygon_with_holes &polygon)
    -> decltype(polygon.outer())
  {
    return polygon.outer();
  }
};

template <> struct interior_rings<Polygon_with_holes> {
  static auto get(Polygon_with_holes &polygon) -> decltype(polygon.inners())
  {
    return polygon.inners();
  }

  static auto get(const Polygon_with_holes &polygon)
    -> decltype(polygon.inners())
  {
    return polygon.inners();
  }
};

}  // namespace traits
}  // namespace geometry
}  // namespace boost
