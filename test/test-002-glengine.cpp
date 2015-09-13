/* -*- coding: utf-8 -*- */

/** 
 * Teste GLEngine avec un GLSimu comme concept::GLScreen
 * pour mettre en œuvre le GLEnvironment.
 */

#include <iostream>                       // std::cout

#include <gl_engine.hpp>
#include <gl_simu.hpp>

// Model
#include <environment.hpp>
#include <agent.hpp>

// ***************************************************************************
int main( int argc, char *argv[] )
{
  std::cout << "__WINDOW" << std::endl;
  GLEngine app( "Simulateur", 800, 800);

  std::cout << "__ENVIRONMENT" << std::endl;
  Environment env(3);
  std::cout << "__AGENT" << std::endl;
  Agent agent;

  std::cout << "__SIMULATOR" << std::endl;
  GLSimu simu_screen( app, env, agent);

  std::cout << "__GO" << std::endl;
  app.run<GLSimu,bool>( simu_screen );
  
  return 0;
}
