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

#include <array>

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
    // Si l'agent est sur de la nourriture
    EntityPtr item = _env.cell( _agent.pos() )->entity();
    if( dynamic_cast<Food*>( item.get() ) ) {
      _agent.eat();
      _env.erase( item );
    }

    // Perception
    // Status d'une case : YELLOW, RED, BLUE, GREEN, NOTSEEN
    // Devant
    CellPtr front = _env.cell( _agent.pos() + _agent.dir().vec() );
    CellPtr left  = _env.cell( _agent.pos() + rotate_left(_agent.dir()).vec() );
    CellPtr right = _env.cell( _agent.pos() + rotate_right(_agent.dir()).vec() );
    // left, front, right
    std::array<Stimulus, 3> near{YELLOW, YELLOW, YELLOW};
    if( left and not left->is_empty() ) {
      near[0] = left->entity()->stimulus();
    }
    if( front and not front->is_empty() ) {
      near[1] = front->entity()->stimulus();
    }
    if( right and not right->is_empty() ) {
      near[2] = right->entity()->stimulus();
    }
    // De la gauche vers la droite
    std::array<Stimulus, 5> far{NOTSEEN, NOTSEEN, NOTSEEN, NOTSEEN, NOTSEEN};
    CellPtr cell;
    if( left and left->is_transparent() ) {
      // leftmost
      cell = _env.cell( left->pos() + rotate_left(_agent.dir()).vec() );
      if( cell and not cell->is_empty() ) {
	far[0] = cell->entity()->stimulus();
      }
      else if( cell and cell->is_empty() ) {
	far[0] = YELLOW;
      }
      // leftfront
      cell = _env.cell( left->pos() + _agent.dir().vec() );
      if( cell and not cell->is_empty() ) {
	far[1] = cell->entity()->stimulus();
      }
      else if( cell and cell->is_empty() ) {
	far[1] = YELLOW;
      }
    }
    if( right and right->is_transparent() ) {
      // rightmost
      cell = _env.cell( right->pos() + rotate_right(_agent.dir()).vec() );
      if( cell and not cell->is_empty() ) {
	far[4] = cell->entity()->stimulus();
      }
      else if( cell and cell->is_empty() ) {
	far[4] = YELLOW;
      }
      // rightfront
      cell = _env.cell( right->pos() + _agent.dir().vec() );
      if( cell and not cell->is_empty() ) {
	far[3] = cell->entity()->stimulus();
      }
      else if( cell and cell->is_empty() ) {
	far[3] = YELLOW;
      }
    }
    if( front and front->is_transparent() ) {
      // far front
      cell = _env.cell( front->pos() + _agent.dir().vec() );
      if( cell and not cell->is_empty() ) {
	far[2] = cell->entity()->stimulus();
      }
      else if( cell and cell->is_empty() ) {
	far[2] = YELLOW;
      }
      // leftfront
      if( far[1] == NOTSEEN ) {
	cell = _env.cell( front->pos() + rotate_left(_agent.dir()).vec() );
	if( cell and not cell->is_empty() ) {
	  far[1] = cell->entity()->stimulus();
	}
	else if( cell and cell->is_empty() ) {
	  far[1] = YELLOW;
	}
      }
      else if( cell and cell->is_empty() ) {
	far[1] = YELLOW;
      }
      // rightfront
      if( far[3] == NOTSEEN ) {
	cell = _env.cell( front->pos() + rotate_right(_agent.dir()).vec() );
	if( cell and not cell->is_empty() ) {
	  far[3] = cell->entity()->stimulus();
	}
	else if( cell and cell->is_empty() ) {
	  far[3] = YELLOW;
	}
      }
    }

    // DUMP
    std::cout << "PER_NEAR =";
    for( auto& per: near) {
      std::cout << " " << per.name();
    }
    std::cout << "  PER_FAR =";
    for( auto& per: far) {
      std::cout << " " << per.name();
    }
    std::cout << std::endl;

  };
  // ********************************************************* Simu::attributs
  Environment& env() const { return _env; };
  Agent& agent() const { return _agent; };
private:
  Environment& _env;
  Agent& _agent;
};



#endif // SIMULATOR_HPP
