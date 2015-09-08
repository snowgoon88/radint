/* -*- coding: utf-8 -*- */
/** 
 * TODO
 */

#include <iostream>                         // std::cout

#include <global.hpp>

// ******************************************************************** MAIN
int main( int argc, char *argv[] )
{
  // Une boucle pour toutes les Directions
  for(const auto& dir: _l_dir) {
    std::cout << "Direction : " << dir.str();
    std::cout << " pos=" << str_vec(dir.vec());
    std::cout << std::endl;
  }
  return 0;
}


