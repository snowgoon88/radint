/* -*- coding: utf-8 -*- */

#ifndef AGENT_HPP
#define AGENT_HPP

/** 
 * Un Agent est caratérisé par une position (_pos) et une orientation
 * (Direction).
 *
 * Il peut bouger : turn_right(), turn_left(), advance().
 * Il mémorise ses interactions : _l_interactions
 */
#include <global.hpp>

#include <entity.hpp>
#include <interaction.hpp>

// ***************************************************************************
// ********************************************************************* Agent
// ***************************************************************************
class Agent : public Entity
{
public:
  // ********************************************************** Agent::typedef
  typedef std::list<Interaction::PrimaryPtr> CInteraction;
public:
  // ********************************************************* Agent::creation
  Agent( const Vec2& pos = {0,0}, const Direction& dir = _dir_no ) : 
    Entity(pos,true,false), _dir(dir), _proprio(Stimulus::NOTHING),
    _interaction(nullptr), _memory_max(20)
  {};
  // ************************************************************** Agent::str
  std::string str_dump() 
  {
    std::stringstream dump;
  
    dump << "Agent : " << str_vec( pos() );
    dump << " [" << dir().str() << "]";
    dump << " pro=" << str_enum(_proprio);
  
  return dump.str();
  }
  std::string str_interactions()
  {
    // Lire en partant de la fin
    std::stringstream inter;

    inter << "   [";
    for( auto id = _l_interactions.rbegin(); id != _l_interactions.rend(); id++ ) {
      inter << (*id)->str_display() << ", ";
    }
    inter << "]";

    return inter.str();
  };
  // ************************************************************** Agent::act
  void turn_left()
  {
    _dir = rotate_left(_dir);
    _proprio = Stimulus::MOVED;
    _interaction = Interaction::_left;
  }
  void turn_right()
  {
    _dir = rotate_right(_dir);
    _proprio = Stimulus::MOVED;
    _interaction = Interaction::_right;
  }
  void advance()
  {
    _pos += dir().vec();
    _proprio = Stimulus::MOVED;
    _interaction = Interaction::_advance;
  }
  void eat()
  {
    _proprio = Stimulus::FED;
    _interaction = Interaction::_eat;
  }
  // *********************************************************** Agent::intent
  Vec2 intent_advance()
  {
    _proprio = Stimulus::BUMPED; // par défaut
    _interaction = Interaction::_bump;
    return pos() + dir().vec();
  }
  // ********************************************************* Agent::memorise
  void memorize_interaction()
  {
    _l_interactions.push_front( _interaction );
    if( _l_interactions.size() > _memory_max ) {
      _l_interactions.pop_back();
    }
  };
  // ******************************************************** Agent::attributs
  const Direction& dir() const {return _dir;};
protected:
  Direction _dir;
  Stimulus _proprio;
  /** A Collection of past Interactions */
  CInteraction _l_interactions;
  /** Current Interaction */
  Interaction::PrimaryPtr _interaction;
  /** taille de la mémoire */
  unsigned int _memory_max;
};

#endif // AGENT_HPP
