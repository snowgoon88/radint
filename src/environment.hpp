/* -*- coding: utf-8 -*- */

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

/** 
 * TODO
 */

#include <global.hpp>

#include <memory>                         // std::shared_ptr
#include <map>                            // std::map
#include <list>                           // std::list

#include <entity.hpp>

// ***************************************************************************
// ********************************************************************** Cell
// ***************************************************************************
class Cell
{
public:
  // ********************************************************** Cell::creation
  Cell( const Vec2 &pos = {0,0} ) : _pos(pos), _entity(nullptr)  {};
  Cell( const Cell &cell ) : _pos( cell.pos()), _entity(nullptr)  {};

  // **************************************************************** Cell:str
  /** dump */
  std::string str_dump() const
  {
    return "Cell "+str_vec(_pos)+" "+( not is_empty() ? "Entity "+_entity->str_dump() : "Vide");
  };

  // ************************************************************ Cell::status
  bool is_empty() const
  {
    return (_entity == nullptr);
  }
  bool is_traversable() const
  {
    if( not is_empty() ) {
      return _entity->is_traversable();
    }
    return true;
  }
  // ************************************************************ Cell::entity
  void add( EntityPtr item )
  {
    _entity = item;
  }
  // ********************************************************* Cell::attributs
  const Vec2& pos() const {return _pos;};

private:
  /** position */
  Vec2 _pos;
  /** Entity sur cette Cell */
  EntityPtr _entity;
};
typedef std::shared_ptr<Cell> CellPtr;

// ***************************************************************************
// **************************************************** Exception::Environment
// ***************************************************************************
namespace Exception {
  
  class Env : public std::exception {
  private:
    std::string message;
  public:
    Env(const std::string& kind,
	const std::string& msg) {
      message = std::string("Environment exception : ")
	+ kind + " : " + msg;
    }
    
    virtual ~Env(void) throw () {}
    
    virtual const char * what(void) const throw ()
    {
      return message.c_str();
    }
  };
};

// ***************************************************************************
// *************************************************************** Environment
// ***************************************************************************
class Environment
{
  typedef std::map<Vec2,CellPtr> CCellPtr;
  typedef std::pair<Vec2,CellPtr> KeyCell;
  typedef std::list<EntityPtr> CEntityPtr;
  
public:
  Environment( unsigned int size )
  {
    Vec2 position = {0,0};
    std::cout << "Création en " << str_vec(position) << std::endl;
    _l_cell.insert( KeyCell(position, CellPtr( new Cell(position))) );
    
    // On empile des "anneaux" de Cell de taille croissante
    for( unsigned int i = 1; i < size; ++i) {
      // position de départ de l'anneau
      position = {(int)-i, 0};
      std::cout << "i = " << i << " >> ";
      // Un anneau est composé de '_d_dir.size()' segment de taille 'i'
      for( unsigned int j = 0; j < _l_dir.size(); ++j) {
	for( unsigned int side = 0; side < i; ++side) {
	  std::cout << "   j=" <<j << " side=" << side << " >> ";
	  position += _l_dir[j].vec();
	  std::cout << "Création en " << str_vec(position) << std::endl;
	  _l_cell.insert( KeyCell(position, CellPtr( new Cell(position))) );
	}
      }
    }

    for( auto& item: _l_cell) {
      std::cout << str_vec(item.first) << " : " << item.second->str_dump() << std::endl;
    }
  }; 
  // ******************************************************** Environment::str
  std::string str_dump()
  {
    std::stringstream dump;
  
    dump << "__ENV__ " << std::endl;
    for( auto& item: _l_cell) {
      dump << str_vec(item.first) << " : " << item.second->str_dump() << std::endl;
    } 
    
    return dump.str();
  }
  // ***************************************************** Environment::entity
  /**
   * Ajoute l'Entity dans l'environnement si la pos() est une Cell valide et 
   * vide.
   */
  void add( EntityPtr item )
  {
    // Est-ce que c'est une Cell de l'Environnement ?
    auto search = _l_cell.find( item->pos() );
    if( search != _l_cell.end()) {
      CellPtr cell = search->second;
      // Est-ce que la Cell est vide ?
      if( cell->is_empty() ) {
	cell->add( item );
	_l_entity.push_back( item );
      }
      else {
	throw Exception::Env( "CellError", "Cell pas vide" );
      }
    }
    else {
      throw Exception::Env( "KeyError", "Cell pas trouvée");
    }
  };
  // ******************************************************* Environment::cell
  /**
   * @return CellPtr ou nullptr.
   */
  CellPtr cell( const Vec2& pos )
  {
    std::cout << "cell pos=" << str_vec(pos) << std::endl;
    auto search = _l_cell.find( pos );
    if( search != _l_cell.end() ) {
      std::cout << "cell " << str_vec(search->first) << " : " << search->second->str_dump() << std::endl;
      return search->second;
    }
    std::cout << "cell null" << std::endl;
    return nullptr;
  };
  // ************************************************** Environment::attributs
  CCellPtr l_cell() const { return _l_cell; };
  CEntityPtr l_entity() const { return _l_entity; };
private:
  CCellPtr _l_cell;
  CEntityPtr _l_entity;
};

#endif // ENVIRONMENT_HPP
