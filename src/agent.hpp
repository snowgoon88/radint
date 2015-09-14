/* -*- coding: utf-8 -*- */

#ifndef AGENT_HPP
#define AGENT_HPP

/** 
 * Un Agent est caratérisé par une position (_pos) et une orientation
 * (Direction).
 *
 * Il peut bouger : turn_right(), turn_left(), advance().
 */

#include <entity.hpp>

// ***************************************************************************
// ********************************************************************* Agent
// ***************************************************************************
class Agent : public Entity
{
public:
  // ********************************************************* Agent::creation
  Agent( const Vec2& pos = {0,0}, const Direction& dir = _dir_no ) : 
    Entity(pos,true,false), _dir(dir)
  {};
  // ************************************************************** Agent::str
  std::string str_dump() 
  {
    std::stringstream dump;
  
    dump << "Agent : " << str_vec( pos() );
    dump << " [" << dir().str() << "]";
  
  return dump.str();
  }
  // ************************************************************* Agent::move
  void turn_left()
  {
    _dir = rotate_left(_dir);
  }
  void turn_right()
  {
    _dir = rotate_right(_dir);
  }
  void advance()
  {
    _pos += dir().vec();
  }

  // ******************************************************** Agent::attributs
  const Direction& dir() const {return _dir;};
protected:
  Direction _dir;
};

#endif // AGENT_HPP
