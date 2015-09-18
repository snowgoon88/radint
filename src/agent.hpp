/* -*- coding: utf-8 -*- */

#ifndef AGENT_HPP
#define AGENT_HPP

/** 
 * Un Agent est caratérisé par une position (_pos) et une orientation
 * (Direction).
 *
 * Il peut bouger : turn_right(), turn_left(), advance().
 */
#include <global.hpp>

#include <entity.hpp>

class Stimulus
  {
  public:
    static int _global_id;
  public:
    Stimulus( std::string name ) : _name(name)
    {
      _id = _global_id++;
    };
    bool operator==( const Stimulus& rhs ) const {
      return _id == rhs._id;
    }
    std::string name() const { return _name; };
  private:
    friend std::ostream& operator<<(std::ostream& os, const Stimulus& stim );
    int _id;
    std::string _name;
  };
  int Stimulus::_global_id = 0;
  std::ostream& operator<<( std::ostream& os, const Stimulus& stim ) {
    return os << stim.name();
  }
  static Stimulus NOTHING("NOTHING");
  static Stimulus MOVED("MOVED");
  static Stimulus BUMPED("BUMPED");
  static Stimulus EATEN("EATEN");

// ***************************************************************************
// ********************************************************************* Agent
// ***************************************************************************
class Agent : public Entity
{
public:

  // ********************************************************* Agent::creation
  Agent( const Vec2& pos = {0,0}, const Direction& dir = _dir_no ) : 
    Entity(pos,true,false), _dir(dir), _proprio(NOTHING)
  {};
  // ************************************************************** Agent::str
  std::string str_dump() 
  {
    std::stringstream dump;
  
    dump << "Agent : " << str_vec( pos() );
    dump << " [" << dir().str() << "]";
    dump << " pro=" << _proprio;
  
  return dump.str();
  }
  // ************************************************************* Agent::move
  void turn_left()
  {
    _dir = rotate_left(_dir);
    _proprio = MOVED;
  }
  void turn_right()
  {
    _dir = rotate_right(_dir);
    _proprio = MOVED;
  }
  void advance()
  {
    _pos += dir().vec();
    _proprio = MOVED;
  }
  // *********************************************************** Agent::intent
  Vec2 intent_advance()
  {
    _proprio = BUMPED; // par défaut
    return pos() + dir().vec();
  }
  // ******************************************************* Agent::perception
  void proprioception( EntityPtr item )
  {
    if( item ) {
      if( dynamic_cast<Food*>(item.get()) ) {
	_proprio = EATEN;
      }
    } 
  };
  // ******************************************************** Agent::attributs
  const Direction& dir() const {return _dir;};
protected:
  Direction _dir;
  Stimulus _proprio;
};

#endif // AGENT_HPP
