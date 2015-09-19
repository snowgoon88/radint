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
static Stimulus FED("FED");
static Stimulus YELLOW("Y");
static Stimulus RED("R");
static Stimulus BLUE("B");
static Stimulus GREEN("G");

// ***************************************************************************
// ******************************************************************** Entity
// ***************************************************************************
class Entity
{
public:
  // ******************************************************** Entity::creation
  Entity( const Vec2& pos = {0,0}, 
	  bool opaque=true, bool traversable=true,
	  Stimulus& stim = YELLOW ) :
    _pos(pos), _opaque(opaque), _traversable(traversable), _stimulus(stim)
  {};
  // ************************************************************* Entity::str
  /** dump */
  virtual std::string str_dump() const
  {
    std::stringstream dump;

    dump << str_vec(pos()) << " - ";
    dump << ( is_opaque() ? " Opaque" : " Transparent");
    dump << ( is_traversable() ? " Traversable" : " Obstacle");
    dump << " Stim=" << _stimulus.name();

    return dump.str();
  };
  // ************************************************************ Entity::move
  void move( const Vec2& pos ) { _pos = pos; };  
  // ******************************************************* Entity::attributs
  const bool is_opaque() const { return _opaque; };
  const bool is_traversable() const { return _traversable; };
  const Vec2& pos() const {return _pos;};
  const Stimulus& stimulus() const { return _stimulus; };
protected:
  Vec2 _pos;           /** position */
  bool _opaque;        /** is Entity opaque */
  bool _traversable;   /** is Entity travesable */
  Stimulus& _stimulus; /** quel genre de Stimulus évoqué */
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
  Wall( const Vec2& pos = {0,0}) : Entity(pos,true,false,GREEN) {};
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
  Algae( const Vec2& pos = {0,0}) : Entity(pos,true,true,RED) {};
  // ************************************************************** Algae::str
  /** dump */
  virtual std::string str_dump() const
  {
    return "__ALGA "+Entity::str_dump();
  };
};
// ***************************************************************************
// ********************************************************************** Food
// ***************************************************************************
class Food : public Entity
{
public:
   // ********************************************************* Food::creation
  Food( const Vec2& pos = {0,0}) : Entity(pos,false,true,BLUE) {};
  // *************************************************************** Food::str
  /** dump */
  virtual std::string str_dump() const
  {
    return "__FOOD "+Entity::str_dump();
  };
};



#endif // ENTITY_HPP
