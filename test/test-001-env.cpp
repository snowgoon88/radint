/* -*- coding: utf-8 -*- */
/** 
 * TODO
 */

#include <iostream>                         // std::cout

#include <global.hpp>
#include <environment.hpp>
#include <entity.hpp>

// ******************************************************************** MAIN
int main( int argc, char *argv[] )
{
  // Création environnement
  Environment env(3);
  std::cout << env.str_dump() << std::endl;

  // Ajout d'Entity
  EntityPtr item1 = EntityPtr( new Entity( {1,1} ));
  std::cout << "ITEM1 = " << item1->str_dump() << std::endl;
  env.add( item1 );
  std::cout << env.str_dump() << std::endl;

  try {
    // Ajout d'Entity
    EntityPtr item2 = EntityPtr( new Entity( {1,1} ));
    std::cout << "ITEM2 = " << item2->str_dump() << std::endl;
    env.add( item2 );
    std::cout << env.str_dump() << std::endl;
  }
  catch(Exception::Env& e) {
    std::cout << "Exception normale" << std::endl;
    std::cout << e.what() << std::endl;
  }

  // Ajout d'Entity
  EntityPtr item3 = EntityPtr( new Entity( {0,2},false,false ));
  std::cout << "ITEM3 = " << item3->str_dump() << std::endl;
  env.add( item3 );
  std::cout << env.str_dump() << std::endl;


  try {
    // Ajout d'Entity
    EntityPtr item4 = EntityPtr( new Entity( {3,3} ));
    std::cout << "ITEM4 = " << item4->str_dump() << std::endl;
    env.add( item4 );
    std::cout << env.str_dump() << std::endl;
  }
  catch(Exception::Env& e) {
    std::cout << "Exception normale" << std::endl;
    std::cout << e.what() << std::endl;
  }

  std::cout << "CHERCHER CELL" << std::endl;
  Vec2 v{0,0};
  CellPtr c = env.cell( v );
  std::cout << "c " << str_vec( v ) << " : " << c->str_dump() << std::endl;

  
  return 0;
}
