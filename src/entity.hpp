/* -*- coding: utf-8 -*- */

#ifndef ENTITY_HPP
#define ENTITY_HPP

/** 
 * Une entité a une position et connaît l'Environnement.
 * Elle est liée à un Cell.
 *
 * Ses principales propriétés sont:
 * <ul>
 * <li> Position Vec2 _pos</li>
 * <li> isOpaque()</li>
 * <li> isTraversable()</li>
 * </ul>
 */

#include <memory>                         // std::shared_ptr


// ***************************************************************************
// ******************************************************************** Entity
// ***************************************************************************
class Entity
{
public:
  // ******************************************************** Entity::creation
  Entity( const Vec2& pos = {0,0}, bool opaque=true, bool traversable=true) :
    _pos(pos), _opaque(opaque), _traversable(traversable)
  {};
  // ************************************************************* Entity::str
  /** dump */
  virtual std::string str_dump() const
  {
    std::stringstream dump;

    dump << str_vec(pos()) << " - ";
    dump << ( is_opaque() ? " Opaque" : " Transparent");
    dump << ( is_traversable() ? " Traversable" : " Obstacle"); 

    return dump.str();
  };
  // ************************************************************ Entity::move
  void move( const Vec2& pos ) { _pos = pos; };  
  // ******************************************************* Entity::attributs
  const bool is_opaque() const { return _opaque; };
  const bool is_traversable() const { return _traversable; };
  const Vec2& pos() const {return _pos;};
protected:
  Vec2 _pos;           /** position */
  bool _opaque;        /** is Entity opaque */
  bool _traversable;   /** is Entity travesable */
};
// *********************************************************************** Ptr
typedef std::shared_ptr<Entity> EntityPtr;


// ***************************************************************************
// ********************************************************************** Wall
// ***************************************************************************
class Wall : public Entity
{
public:
  // ********************************************************** Wall::creation
  Wall( const Vec2& pos = {0,0}) : Entity(pos,true,false) {};
  // *************************************************************** Wall::str
  /** dump */
  virtual std::string str_dump() const
  {
    return "__WALL "+Entity::str_dump();
  };
};
// ***************************************************************************
// ********************************************************************* Algae
// ***************************************************************************
class Algae : public Entity
{
public:
   // ******************************************************** Algae::creation
  Algae( const Vec2& pos = {0,0}) : Entity(pos,true,true) {};
  // ************************************************************** Algae::str
  /** dump */
  virtual std::string str_dump() const
  {
    return "__ALGA "+Entity::str_dump();
  };
};

#endif // ENTITY_HPP
