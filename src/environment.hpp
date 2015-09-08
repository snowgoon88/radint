/* -*- coding: utf-8 -*- */

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

/** 
 * TODO
 */

#include <global.hpp>

#include <memory>                         // std::shared_ptr
#include <map>                            // std::map

// ***************************************************************************
// ********************************************************************** Cell
// ***************************************************************************
class Cell
{
public:
  // ********************************************************** Cell::creation
  Cell( const Vec2 &pos = {0,0} ) : _pos(pos) {};
  Cell( const Cell &cell ) : _pos( cell.pos()) {};

  // **************************************************************** Cell:str
  /** dump */
  std::string str_dump() const
  {
    return "Cell "+str_vec(_pos);
  };

  // ********************************************************* Cell::attributs
  const Vec2& pos() const {return _pos;};

private:
  /** position */
  Vec2 _pos;
};
typedef std::shared_ptr<Cell> CellPtr;

// ***************************************************************************
// *************************************************************** Environment
// ***************************************************************************
class Environment
{
  typedef std::map<Vec2,CellPtr> CCellPtr;
  typedef std::pair<Vec2,CellPtr> KeyCell;
  
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
	  position = {position.x + _l_dir[j].vec().x,
		      position.y + _l_dir[j].vec().y};
	  std::cout << "Création en " << str_vec(position) << std::endl;
	  _l_cell.insert( KeyCell(position, CellPtr( new Cell(position))) );
	}
      }
    }

    for( auto& item: _l_cell) {
      std::cout << str_vec(item.first) << " : " << item.second->str_dump() << std::endl;
    }
  };

  // ************************************************** Environment::attributs
private:
  CCellPtr _l_cell;
};

#endif // ENVIRONMENT_HPP
