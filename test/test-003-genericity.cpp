/* -*- coding: utf-8 -*- */

/** 
 * Test s'il est possible d'utiliser une sorte de généricité à
 * l'exécution en fonction de la classe d'Entity.
 */

#include <global.hpp>

#include <iostream>                         // std::cout
#include <list>                             // std::list
#include <entity.hpp>


void render(Wall w)
{
  std::cout << "render(Wall w) : " << w.str_dump() << std::endl;
}
void render(Wall* w)
{
    std::cout << "render(Wall* w) : " << w->str_dump() << std::endl;
}
// void render(Wall& w)
// {
//   std::cout << "render(Wall& w) : " << w.str_dump() << std::endl;
// }
void render(EntityPtr e)
{
  std::cout << "render(EntityPtr e) : " << e->str_dump() << std::endl;
}
void render(Entity e)
{
  std::cout << "render(Entity e) : " << e.str_dump() << std::endl;
}
// void render(Entity& e)
// {
//   std::cout << "render(Entity& e) : " << w.str_dump() << std::endl;
// }
// ********************************************************************** MAIN
int main( int argc, char *argv[] )
{
  std::list<EntityPtr> l_entity;
  EntityPtr item1 = EntityPtr( new Entity( {1,1} ));
  l_entity.push_back( item1 );
  EntityPtr item2 = EntityPtr( new Wall( {1,2} ));
  l_entity.push_back( item2 );
  EntityPtr item3 = EntityPtr( new Algae( {1,3} ));
  l_entity.push_back( item3 );

  for( auto& item: l_entity) {
    std::cout << item->str_dump() << std::endl;
    // dynamic_cast en cas de polymorphisme
    // sinon un static_cast marcherait aussi.
    if( Wall* w = dynamic_cast<Wall*>(item.get()) ) {
      render(w);
      render(*w);
    }
    else {
      render(item);
      render(*item);
    }
  }
  
  return 0;
}

