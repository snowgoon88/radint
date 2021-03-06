/* -*- coding: utf-8 -*- */

/** 
 * Test les méthodes basiques d'interaction.
 */

#include <iostream>                         // std::cout
#include <interaction.hpp>

// ********************************************************************** Main
int main( int argc, char *argv[] )
{

  std::cout <<  Interaction::_advance->str_display() << std::endl;

  for( auto& inter: Interaction::_l_primary) {
    std::cout << inter->str_display()  << std::endl;
  }
  
  
  return 0;
}
