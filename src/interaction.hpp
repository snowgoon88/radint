/* -*- coding: utf-8 -*- */

#ifndef INTERACTION_HPP
#define INTERACTION_HPP

/** 
 * Premières implémentation d'intéraction.
 */

#include <memory>                         // std::shared_ptr

#include <entity.hpp>

namespace Interaction
{
// ***************************************************************************
// ****************************************************** Interaction::Primary
// ***************************************************************************
typedef std::pair<Act,Stimulus> PrimaryPair;
class Primary : public PrimaryPair
{
public:
  // ****************************************** Interaction::Primary::creation
  Primary( const Act act, const Stimulus stim, std::string display ) :
    PrimaryPair(act,stim), _str_display(display)
  {
  };
  // *********************************************** Interaction::Primary::str
  std::string str_display() const
  {
    return _str_display;
  };
  // ***************************************** Interaction::Primary::attributs
private:
  std::string _str_display;
};
typedef std::shared_ptr<Primary> PrimaryPtr;

  static PrimaryPtr _advance = PrimaryPtr( new Primary{Act::FORWARD, Stimulus::MOVED, "ADV"});
  // static PrimaryPtr _eat     {Act::FORWARD, Stimulus::FED, "EAT"};
  // static PrimaryPtr _bump    {Act::FORWARD, Stimulus::BUMPED, "BUM"};
  // static PrimaryPtr _left    {Act::LEFT30, Stimulus::MOVED, "L30"};
  // static PrimaryPtre _right   {Act::RIGHT30, Stimulus::MOVED, "R30"};

  // static std::array<Primary,5> _l_primary {_advance,_eat,_bump,_left,_right};

  // static PrimaryPtr
  // static std::array<std::string,5> _l_st_primary {"ADV", "EAT", "BUM", "L30", "R30"};
  
// ***************************************************************************
// ********************************************************* Interaction::Full
// ***************************************************************************
class Full
{
public:
  // ********************************************* Interaction::Full::creation
  /** Création avec Act,Stimulus */
  Full( const Act act, const Stimulus stim ) : _primary(nullptr)
  {
  };
  // ************************************************** Interaction::Full::str
  std::string str_dump() const
  {
    std::stringstream dump;

    dump << "<" << _primary->str_display() << ", ";
    dump << ">";

    return dump.str();
  }
  /** On peut l'enacter sur un agent */
  /** On peut la compléter par un stimulus */

  // ************************************************** Interaction::attributs
private:
  PrimaryPtr _primary;
};

}; // Interaction
#endif // INTERACTION_HPP
