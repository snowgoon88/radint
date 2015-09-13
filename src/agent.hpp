/* -*- coding: utf-8 -*- */

#ifndef AGENT_HPP
#define AGENT_HPP

/** 
 * Un Agent est caratérisé par une position (_pos) et une orientation
 * (Direction).
 *
 * Il peut bouger : turn_right(), turn_left(), advance().
 */

// ***************************************************************************
// ********************************************************************* Agent
// ***************************************************************************
class Agent
{
public:
  // ********************************************************* Agent::creation
  Agent( const Vec2& pos = {0,0}, const Direction& dir = _dir_no ) : 
    _pos(pos), _dir(dir)
  {};

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
  const Vec2& pos() const {return _pos;};
  const Direction& dir() const {return _dir;};
private:
  Vec2 _pos;
  Direction _dir;
};

#endif // AGENT_HPP
