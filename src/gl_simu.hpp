/* -*- coding: utf-8 -*- */

#ifndef GL_SIMU_HPP
#define GL_SIMU_HPP

/** 
 * GLSimu implémente concept::GLScreen pour gérer une simulation.
 */

// Model
#include <environment.hpp>
#include <agent.hpp>

// Viewer
#include <gl_engine.hpp>
#include <gl_environment.hpp>
#include <gl_agent.hpp>
#include <gl_text.hpp>

#include <chrono>                      // std::chrono
#include <thread>                      // std::thread

// ******************************************************************** GLOBAL

// ***************************************************************************
// ******************************************************************** GLSimu
// ***************************************************************************
class GLSimu
{
public:
  // ******************************************************** GLSimu::creation
  /** Création avec un GLEngine */
  GLSimu( GLEngine& engine, Environment& env, Agent& agent) :
    _window(engine.window()),
    _env(env), _agent(agent),
    _gl_env(_env), _gl_agent(_agent), _gl_text(),
    _is_running(false),
    _frame_time_min(1000.0), _frame_time_max(0.0), _frame_time_avg(0.0)
  {
  };
  // ************************************************************ GLSimu::init
  void init()
  {
    // Installe les callback pour les touches
    glfwSetWindowUserPointer( _window, this);
    glfwSetKeyCallback(_window, key_callback);

    std::cout << _env.str_dump() << std::endl;

  };
  // ********************************************************** GLSimu::render
  void render()
  {
    // Transparence
    // Enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _gl_text.set_glstate();

    // TODO cbk quand fenêtre redimensionnée
    // FPS
    auto frame_time = std::chrono::steady_clock::now();
    std::stringstream fps_ss;
    fps_ss << "FPS m/avg/M : ";
    fps_ss << _frame_time_min.count() << "/";
    fps_ss << _frame_time_avg.count() << "/";
    fps_ss << _frame_time_max.count();

    while (!glfwWindowShouldClose(_window)) {
      // clock
      auto start_proc = std::chrono::steady_clock::now();

      // world update
      if( _is_running ) {
	//_world.update( 0.020 );
      }

      glfwGetFramebufferSize(_window, &_screen_width, &_screen_height);
      //float ratio = _screen_width / (float) _screen_height;
      
      glViewport(0, 0, _screen_width, _screen_height);
      /* Clear the background as yellow #ffde94 */
      glClearColor(1.0, 1.0, 0.58, 1.0);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      // do_ortho() - Needed by _gl_text
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 1.f, -1.f);
      
      // Needed by _gl_text
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      

      // Projection (to 2D screen)
      glm::mat4 projection = glm::ortho( -10.0f, 10.0f,
					 -10.0f, 10.0f,
					 -1.0f, 1.0f );
      // Prépare GLText
      _gl_text.set_scale( (10.f + 10.f)/(float)_screen_width,
      (10.f + 10.f)/(float)_screen_height );
      glColor3f( 0.f, 0.f, 0.f );
      _gl_text.render( fps_ss.str(), -9.f, 9.f );
      _gl_text.render( _agent.str_dump(), -9.f, 9.f - _gl_text.line_height() );

      // // Display cbk
      _gl_env.render( projection );
      _gl_agent.render( projection );

      // Remove any programm so that glText can "work"
      glUseProgram(0);
      
      glfwSwapBuffers(_window);
      glfwPollEvents();
      
      // clock
      auto end_proc = std::chrono::steady_clock::now();
      // wait period 
      std::chrono::duration<double> elapsed_seconds = end_proc - start_proc;
      std::this_thread::sleep_for(std::chrono::milliseconds(20)
				  - elapsed_seconds );
      //std::cout << "TIME = " <<  elapsed_seconds.count() << std::endl;
      //FPS update
      std::chrono::duration<double> frame_delta_time = end_proc - frame_time;
      if( _frame_time_min > frame_delta_time ) _frame_time_min = frame_delta_time;
      if( _frame_time_max < frame_delta_time ) _frame_time_max = frame_delta_time;
      _frame_time_avg = 0.9 * _frame_time_avg + 0.1 * frame_delta_time;
      frame_time = end_proc;
      
      fps_ss.str("");
      fps_ss << "FPS m/avg/M : ";
      fps_ss << _frame_time_min.count() << "/";
      fps_ss << _frame_time_avg.count() << "/";
      fps_ss << _frame_time_max.count();
    }
  };
  // ***************************************************** GLSimu::final_state
  bool final_state() const { return true; };
  // ******************************************************* GLSimu::attributs
private:
  /** Ptr sur la Fenetre */
  GLFWwindow* _window;
  int _screen_width, _screen_height;
  /** Model */
  Environment& _env;
  Agent& _agent;
  /** Viewer */
  GLEnvironment _gl_env;
  GLAgent _gl_agent;
  GLText _gl_text;
  // ************************************************************** simulation
  bool _is_running;
  // ********************************************************************* fps
  std::chrono::duration<double, std::milli> _frame_time_min;
  std::chrono::duration<double, std::milli> _frame_time_max;
  std::chrono::duration<double, std::milli> _frame_time_avg;
  //***************************************************************** callback
  /**
   * Callback qui gère les événements 'key'
   */
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    // ESC => Quit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    // sinon callback de la classe
    else if( action == GLFW_PRESS ) {
      //std::cout << "key_callback = " << key << std::endl;
      ((GLSimu *)glfwGetWindowUserPointer(window))->on_key_pressed( key );
    }
  }
  // ********************************************************* public_callback
  void on_key_pressed( int key ) 
  {
    std::cout << "GLWindow::key_pressed key=" << key << std::endl;
    if( key == GLFW_KEY_R) {
      // toggle _is_running
      _is_running = !_is_running;
    }
    else if( key == GLFW_KEY_P) {
      // display l'Environnemnt
      std::cout << _env.str_dump() << std::endl;
    }
    else if( key == GLFW_KEY_LEFT or key == GLFW_KEY_A) {
      _agent.turn_left();
    }
    else if( key == GLFW_KEY_RIGHT or key == GLFW_KEY_D) {
      _agent.turn_right();
    }
    else if( key == GLFW_KEY_UP or key == GLFW_KEY_W) {
      _agent.advance();
    }
    
  };
  /**
   * Callback pour gérer les messages d'erreur de GLFW
   */
  static void error_callback(int error, const char* description)
  {
    std::cerr <<  description << std::endl;
  }
};

#endif // GL_SIMU_HPP
