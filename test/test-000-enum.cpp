/* -*- coding: utf-8 -*- */

/** 
 * Pour simplifier la conversion de class enum en std::string
 *
 * Exemple
 * enum class EE {E1, E2};
 * template<> std::string EnumString<EE>::data[] = {"E1", "E2"};
 *
 * EE e1 = EE::E1;
 * EE e2 = EE::E2;
 * std::cout << "e1=" << str_enum(e1) << std::endl;
 * std::cout << "e2=" << str_enum(e2) << std::endl;
 * 
 * assert( e1 == e1 );
 * assert( e1 != e2 );
 *
 * Inspiré de 
 * http://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header
 */

#include <iostream>                         // std::cout
#include <assert.h>                         // assert
#include <string>

// ***************************** En passant par les Template et les enum class
// **************************************************************** EnumString
/**
 * Une structure de donnée associée à chaque enum class.
 * ATTENTION : fait l'hypothèse que les enum sont numéroté de 0 à n.
 */
template<typename T>
struct EnumString {
  static std::string data[];
};
/**
 * Fonctionne en récupérant le underlying indice du class enum et en 
 * retournant le string à cette position dans EnumString approprié.
 */
template<typename T>
std::string str_enum(const T& e) {
  auto index = static_cast<typename std::underlying_type<T>::type>(e);
  std::cout << "idx = " << index << std::endl;
  std::cout << "Data=";
  for( auto& s: EnumString<T>::data) {
    std::cout << s << "; ";
  }
  std::cout << std::endl;
  return EnumString<T>::data[static_cast<typename std::underlying_type<T>::type>(e)];
};

// ******************************************************** EnumString EXEMPLE
enum class EE {E1, E2};
template<> std::string EnumString<EE>::data[] = {"E1", "E2"};
/**
 * Peut alors écrire
 * EE e1 = EE::E1;
 * EE e2 = EE::E2;
 * std::cout << "e1=" << str_enum(e1) << std::endl;
 * std::cout << "e2=" << str_enum(e2) << std::endl;
 * 
 * assert( e1 == e1 );
 * assert( e1 != e2 );
 */
// ***************************************************************************


// ********************************************************************** Main
int main( int argc, char *argv[] )
{
  EE e1 = EE::E1;
  EE e2 = EE::E2;
  std::cout << "e1=" << str_enum(e1) << std::endl;
  std::cout << "e2=" << str_enum(e2) << std::endl;

  assert( e1 == e1 );
  assert( e1 != e2 );
  
  return 0;
}
