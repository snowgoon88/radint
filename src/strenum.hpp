/* -*- coding: utf-8 -*- */

#ifndef STRENUM_HPP
#define STRENUM_HPP

/** 
 * Fonctions pour faciliter la conversion de class enum en std::string
 * ATTENTION : fait l'hypothèse que les enum sont numéroté de 0 à n.
 * ATTENTION : fait l'hypothèse que les enum ont des underlying_type de int.
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
 * Inspiré de 
 * http://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header
 */

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
  return EnumString<T>::data[static_cast<typename std::underlying_type<T>::type>(e)];
};

#endif // STRENUM_HPP
