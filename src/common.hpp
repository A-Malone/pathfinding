#ifndef COMMON_H
#define COMMON_H

// STL
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <stack>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// BOOST
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
namespace bg = boost::geometry;

#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)

typedef bg::model::point<int, 2, bg::cs::cartesian> point_t;
typedef bg::model::box<point_t> box_t;

template <class T, class D>
bg::model::point<T, D, bg::cs::cartesian>& bg::model::point<T, D, bg::cs::cartesian>::operator+(bg::model::point<T, D, bg::cs::cartesian> arg) // copy/move constructor is called to construct arg
{
    this->set<0>(this->get<0>() + arg.get<0>());
    this->set<1>(this->get<1>() + arg.get<1>());
    return *this;
}

#endif /* COMMON_H */
