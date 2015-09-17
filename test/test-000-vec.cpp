/* -*- coding: utf-8 -*- */

/** 
 * Vérifie opérateur < sur Vec et les map...
 */

#include <iostream>                         // std::cout

#include <global.hpp>

#include <list>

// ********************************************************************** MAIN
int main( int argc, char *argv[] )
{
  std::list<Vec2> l_vec;
  for( int i = -2; i < 3; ++i) {
    for( int j = -2; j < 3; ++j) {
      l_vec.push_back( Vec2({i,j}) );
    }
  }

  std::cout << "__LIST" << std::endl;
  for( auto& vec: l_vec) {
    std::cout << str_vec(vec) << std::endl;
  }

  for( auto& v1: l_vec) {
    for( auto& v2: l_vec) {
      std::cout << str_vec(v1) << " < " << str_vec(v2) << " ? " << (v1 < v2)  << std::endl;
    }
  }
  
  return 0;
}
