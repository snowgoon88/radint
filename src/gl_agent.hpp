/* -*- coding: utf-8 -*- */

#ifndef GL_AGENT_HPP
#define GL_AGENT_HPP

/** 
 * Un triangle orienté.
 */
#include "gl_utils.hpp"                   // GLUtils

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Model
#include <agent.hpp>

// ***************************************************************************
// ******************************************************************* GLAgent
// ***************************************************************************
class GLAgent
{
  typedef std::pair<float,float> Vec2F;
public:
  // ******************************************************* GLAgent::creation
  GLAgent( Agent& agent ) :
    _model(agent)
  {
    // VBO pour les lignes des 2 triangles de l'agent
    GLfloat agent_vtx[(3*2*2)];
    _vbo_agent_size = 0;
    agent_vtx[_vbo_agent_size++] = -0.2f;
    agent_vtx[_vbo_agent_size++] =  0.f;
    agent_vtx[_vbo_agent_size++] = -0.4f;
    agent_vtx[_vbo_agent_size++] = -0.3f;
    agent_vtx[_vbo_agent_size++] = 0.7f;
    agent_vtx[_vbo_agent_size++] = 0.f;

    agent_vtx[_vbo_agent_size++] = -0.2f;
    agent_vtx[_vbo_agent_size++] = 0.f;
    agent_vtx[_vbo_agent_size++] = 0.7f;
    agent_vtx[_vbo_agent_size++] = 0.f;
    agent_vtx[_vbo_agent_size++] = -0.4f;
    agent_vtx[_vbo_agent_size++] = 0.3f;
    
    _vbo_agent_size = _vbo_agent_size/2;
    
    // VBO
    glGenBuffers(1, &_vbo_agent);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_agent);
    // Pousse les points dans le VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(agent_vtx),
		 agent_vtx, GL_STATIC_DRAW);

    // Delier les VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Charger les Shaders LINE ou TRIANGLES
    GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    /** Vertex Shader */
    if ((vs = GLUtils::create_shader("src/shaders/line.v.glsl", GL_VERTEX_SHADER))   == 0)
      exit( EXIT_FAILURE );
    /** Fragment Shader */
    if ((fs = GLUtils::create_shader("src/shaders/line.f.glsl", GL_FRAGMENT_SHADER)) == 0)
      exit( EXIT_FAILURE );
    /** GLSL program */
    // link Vertex et Fragment Shaders
    // program est une variable GLOBALE
    _program = glCreateProgram();
    glAttachShader(_program, vs);
    glAttachShader(_program, fs);
    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
      std::cerr << "__GLWorld: Pb avec glLinkProgam "<< std::endl;
      std::cerr << GLUtils::str_compiler_log(_program) << std::endl;
      exit( EXIT_FAILURE );;
    }

    // Lier les variables des shaders
    // l_color : couleur des lignes
    const char* uniform_name = "l_color";
    _uniform_l_color = glGetUniformLocation(_program, uniform_name);
    if (_uniform_l_color == -1) {
      std::cerr <<  "Pb pour lier l'uniform " << uniform_name << std::endl;
      exit( EXIT_FAILURE );
    }
    // la variable uniforme mvp
    uniform_name = "mvp";
    _uniform_mvp = glGetUniformLocation(_program, uniform_name);
    if (_uniform_mvp == -1) {
      std::cerr <<  "Pb pour lier l'uniform " << uniform_name << std::endl;
      exit( EXIT_FAILURE );
    }
    // coord2d : coordonnées du vertex
    const char* attribute_name = "coord2d";
    _attribute_coord2d= glGetAttribLocation(_program, attribute_name);
    if (_attribute_coord2d == -1) {
      std::cerr <<  "Pb pour lier l'attribut " << attribute_name << std::endl;
      exit( EXIT_FAILURE );
    }
  };
  // ********************************************** GLEnvironment::destruction
  ~GLAgent()
  {
    // Détruit le programme GLSL
    glDeleteProgram(_program);
    // Et les vbo
    glDeleteBuffers(1, &_vbo_agent);
  };
  // *************************************************** GLEnvironment::render
  void render( glm::mat4& projection )
  {
    glUseProgram( _program );

    // Translation
    Vec2F center = coord_from_center( _model.pos() );
    glm::mat4 trans = glm::translate(glm::mat4(1.0f),
				     glm::vec3( center.first,
						center.second,
						0.0));
    // Rotation
    glm::mat4 rotate = glm::rotate( glm::mat4(1.0f),
				    (float) M_PI/2.f - _model.dir().index() * (float) M_PI/3.f,
				    glm::vec3( 0.f, 0.f, 1.0f));

    // Et finalement
    glm::mat4 mvp = projection * trans * rotate;

    glUniformMatrix4fv(_uniform_mvp, 1, GL_FALSE,
     		       glm::value_ptr(mvp));
    
    // Triangles Bleus
    glUniform3f( _uniform_l_color, 0.f, 0.f, 1.f ); 

    // Dessiner la grille
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_agent );
    glEnableVertexAttribArray( _attribute_coord2d );
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
      _attribute_coord2d, // attribute
      2,                 // number of elements per vertex, here (x,y)
      GL_FLOAT,          // the type of each element
      GL_FALSE,          // take our values as-is
      0,                 // stride
      0                  // offset of first element
			  );
    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, _vbo_agent_size);
  };
  // ************************************************ GLEnvironment::attributs
private:
  /** Model : Environment */
  Agent& _model;
  /** Program GLSL */
  GLuint _program;
  /** Variables globale du Programme GLSL */
  GLint _attribute_coord2d;
  /** Uniform var */
  GLint _uniform_l_color, _uniform_mvp;
  /** Vertex Buffer Object pour lines */
  GLuint _vbo_agent;
  unsigned int _vbo_agent_size;

  // ************************************************** GLEnvironment::convert
  Vec2F coord_from_center( const Vec2& pos ) const
  {
    // Déplacement du centre en fonction de x
    float x = (float) pos.x * (1.f+cosf(M_PI/3.f));
    float y = (float) pos.x * (sinf(M_PI/3.f));
    // Puis on ajoute y
    y += (float) pos.y * (2.f * sinf(M_PI/3.f));

    return Vec2F(x,y);
  };
};

#endif // GL_AGENT_HPP

