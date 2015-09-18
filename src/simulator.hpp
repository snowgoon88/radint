/* -*- coding: utf-8 -*- */

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

/** 
 * Le Simu fait évoluer le World=Agent+Environment+Entity.
 */

#include <global.hpp>

#include <environment.hpp>
#include <agent.hpp>
#include <entity.hpp>

// ***************************************************************************
// *********************************************************** Exception::Simu
// ***************************************************************************
namespace Exception {
  
  class Sim : public std::exception {
  private:
    std::string message;
  public:
    Sim(const std::string& kind,
	const std::string& msg) {
      message = std::string("Environment exception : ")
	+ kind + " : " + msg;
    }
    
    virtual ~Sim(void) throw () {}
    
    virtual const char * what(void) const throw ()
    {
      return message.c_str();
    }
  };
};

// ***************************************************************************
// ********************************************************************** Simu
// ***************************************************************************
class Simu
{
public:
  // ********************************************************** Simu::creation
  Simu( Environment& env, Agent& agent) :
    _env(env), _agent(agent)
  {
  };
  // ************************************************************* Simu::enact
  /**
   * Agent peut avancer si la case devant est libre ou traversable.
   */
  void apply_advance()
  {
    // Dans quelle case veut aller l'agent.
    std::cout << "apply_advance agent=" << agent().str_dump() << std::endl;
    Vec2 target = _agent.intent_advance();
    std::cout << "apply_advance target=" << str_vec(target) << std::endl;
    std::cout << "apply_advance agent=" << agent().str_dump() << std::endl;
    CellPtr cell = _env.cell( target );
    std::cout << "apply_advance cell=" << cell << std::endl;
    if( cell ) {
      std::cout << "apply_advance cell=" << cell->str_dump() << std::endl;
      if( cell->is_traversable() ) {
	_agent.advance();
      }
    }
    else {
      throw Exception::Sim( "BoundError", "Agent wants to go outside Env");
    }
  };
  /**
   * Tourner fonctionne toujours.
   */
  void apply_turn_left()
  {
    _agent.turn_left();
  };
  /**
   * Tourner fonctionne toujours.
   */
  void apply_turn_right()
  {
    _agent.turn_right();
  };
  // ********************************************************** Simu::perceive
  /**
   * Agent Internal Perception.
   */
  void apply_proprioception()
  {
    _agent.proprioception( _env.cell( _agent.pos() )->entity() );
  };
  // ********************************************************* Simu::attributs
  Environment& env() const { return _env; };
  Agent& agent() const { return _agent; };
private:
  Environment& _env;
  Agent& _agent;
};



#endif // SIMULATOR_HPP
