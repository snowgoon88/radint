/* -*- coding: utf-8 -*- */

#ifndef GL_ENVIRONMENT_HPP
#define GL_ENVIRONMENT_HPP

/** 
 * Environnement est une séquence d'hexagones.
 */
#include "gl_utils.hpp"                   // GLUtils

#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Model
#include <environment.hpp>
// Viewer
#include <gl_entity.hpp>

// ***************************************************************************
// ************************************************************* GLEnvironment
// ***************************************************************************
class GLEnvironment
{
  typedef std::pair<float,float> Vec2F;
public:
  // ************************************************* GLEnvironment::creation
  GLEnvironment( Environment& env) :
    _model(env),
    _gl_entity(nullptr)
  {
    // VBO pour les lignes de l'hexagone
    // Essaie de faire 3 hexagone (0,0), (2,0) et (2,1)
    // GLfloat hex_vtx[(6*4)*5];
    GLfloat hex_vtx[(6*4)* _model.l_cell().size() ];
    Vec2F center;
    _vbo_hex_size = 0;
    for( auto& item: _model.l_cell()) {
      center = coord_from_center( item.second->pos() );
      _vbo_hex_size += add_hex( center.first, center.second,
				&(hex_vtx[_vbo_hex_size]) );
    }    
    _vbo_hex_size = _vbo_hex_size/2;
    
    // VBO
    glGenBuffers(1, &_vbo_hexes);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_hexes);
    // Pousse les points dans le VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(hex_vtx),
		 hex_vtx, GL_STATIC_DRAW);

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

    // GLEnvironment
    _gl_entity = new GLEntity( _attribute_coord2d, _uniform_l_color,
			       _uniform_mvp );
  };
  // ********************************************** GLEnvironment::destruction
  ~GLEnvironment()
  {
    // Détruit le programme GLSL
    glDeleteProgram(_program);
    // Et les vbo
    glDeleteBuffers(1, &_vbo_hexes);

    // Et viewer
    delete _gl_entity;
  };
  // *************************************************** GLEnvironment::render
  void render( glm::mat4& projection )
  {
    glUseProgram( _program );
    glUniformMatrix4fv(_uniform_mvp, 1, GL_FALSE,
     		       glm::value_ptr(projection));
    
    // LIGNES **************************************
    // Couleur des lignes : BLACK
    glUniform3f( _uniform_l_color, 0.f, 0.f, 0.f ); 

    // Dessiner la grille
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_hexes );
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
    glDrawArrays(GL_LINES, 0, _vbo_hex_size);

    // ENTITY **************************************
    for( auto& item: _model.l_entity()) {
      _gl_entity->render( projection, item );
    }
  };

  // ************************************************ GLEnvironment::attributs
private:
  /** Model : Environment */
  Environment& _model;
  /** Viewer */
  GLEntity* _gl_entity;
  /** Program GLSL */
  GLuint _program;
  /** Variables globale du Programme GLSL */
  GLint _attribute_coord2d;
  /** Uniform var */
  GLint _uniform_l_color, _uniform_mvp;
  /** Vertex Buffer Object pour lines */
  GLuint _vbo_hexes;
  unsigned int _vbo_hex_size;

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
  unsigned int add_hex( GLfloat xc, GLfloat yc, GLfloat* ptr )
  {
    for( unsigned int side = 0; side < 6; ++side) {
      float angle = (float) side * M_PI /3.0;
      *ptr++ = xc + (float) cosf(angle);
      *ptr++ = yc + (float) sinf(angle);
      angle += M_PI /3.0;
      *ptr++ = xc + (float) cosf(angle);
      *ptr++ = yc + (float) sinf(angle);
    }

    return 6*4;
  }
};



#endif // GL_ENVIRONMENT_HPP
