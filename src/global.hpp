/* -*- coding: utf-8 -*- */

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

/** 
 * Types globaux:
 *  - Vec2
 *  - Directiob
 */

#include <string>
#include <array>

#include <sstream>                        // std::stringstream
#include <tuple>                          // std::tie

// ***************************************************************** TYPEDEF
typedef int Coord;
typedef struct {
  Coord x,y;
} Vec2;
std::string str_vec( const Vec2 &vec)
{
  std::stringstream str;
  str << "(" << vec.x << ", " << vec.y << ")";
  return str.str();
};
bool operator< ( const Vec2& v, const Vec2& w )
{
  return std::tie(v.x, v.y) < std::tie(w.x, w.y);
};
// ***************************************************************************
// ***************************************************************** Direction
// ***************************************************************************
class Direction
{
public:
  Direction( unsigned int index, const Vec2 &dir, const std::string &str)
  {
    _index = index;
    _vec = dir;
    _str = str;
  };

  // ***************************************************************** Variables
  const std::string str() const { return _str; };
  const Vec2 vec() const {return _vec; };
private:
  /** Unique index */
  unsigned int _index;
  /** Direction vector */
  Vec2 _vec;
  /** string pour afficher le nom de cette Direction */
  std::string _str;
};

static Direction _dir_no = { 0, {0, 1}, "NO" };
static Direction _dir_en = { 1, {1, 0}, "EN"};
static Direction _dir_es = { 2, {1, -1}, "ES"};
static Direction _dir_so = { 3, {0, -1}, "SO" };
static Direction _dir_os = { 4, {-1, 0}, "OS"};
static Direction _dir_on = { 5, {-1, 1}, "ON"};

static std::array<Direction, 6> _l_dir = {_dir_no, _dir_en, _dir_es, _dir_so, _dir_os, _dir_on};

#endif // GLOBAL_HPP
