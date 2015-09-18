/* -*- coding: utf-8 -*- */

#ifndef GL_ENTITY_HPP
#define GL_ENTITY_HPP

/** 
 * Conçu pour être appelé depuis un GLEnvironment, il utilise le
 * même programme GLSL.
 */
#include "gl_utils.hpp"                   // GLUtils

#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <entity.hpp>

// ***************************************************************************
// ****************************************************************** GLEntity
// ***************************************************************************
class GLEntity
{
    typedef std::pair<float,float> Vec2F;
public:
  // ****************************************************** GLEntity::creation
  /** Les variables du programme GLSL */
  GLEntity( GLint att_coord2d, GLint uni_color, GLint uni_mvp ) :
    _attribute_coord2d(att_coord2d),
    _uniform_l_color(uni_color),
    _uniform_mvp(uni_mvp)
  {
    // Préparation du VBO pour le mur :GL_TRIANGLE_FAN
    GLfloat wall_vtx[8*2];
    _vbo_wall_size = 0;
    wall_vtx[_vbo_wall_size++] = 0.f;
    wall_vtx[_vbo_wall_size++] = 0.f;
    for( unsigned int i = 0; i < 7; ++i) {
      wall_vtx[_vbo_wall_size++] = cosf( (float)M_PI/3.f * i );
      wall_vtx[_vbo_wall_size++] = sinf( (float)M_PI/3.f * i );
    }
    _vbo_wall_size = _vbo_wall_size / 2;
    // VBO
    glGenBuffers(1, &_vbo_wall);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_wall);
    // Pousse les points dans le VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_vtx),
		 wall_vtx, GL_STATIC_DRAW);

    // Préparation du VBO pour l'alge : 2 x GL_TRIANGLE
    GLfloat algae_vtx[2*(2*3)];
    _vbo_algae_size = 0;
    algae_vtx[_vbo_algae_size++] = -0.4f;
    algae_vtx[_vbo_algae_size++] = 0.f;
    algae_vtx[_vbo_algae_size++] = 0.f;
    algae_vtx[_vbo_algae_size++] = +0.4f;
    algae_vtx[_vbo_algae_size++] = +0.4f;
    algae_vtx[_vbo_algae_size++] = 0.f;

    algae_vtx[_vbo_algae_size++] = -0.4f;
    algae_vtx[_vbo_algae_size++] = 0.f;
    algae_vtx[_vbo_algae_size++] = +0.4f;
    algae_vtx[_vbo_algae_size++] = 0.f;
    algae_vtx[_vbo_algae_size++] = 0.f;
    algae_vtx[_vbo_algae_size++] = -0.4f;

    _vbo_algae_size = _vbo_algae_size / 2;
    // VBO
    glGenBuffers(1, &_vbo_algae);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_algae);
    // Pousse les points dans le VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(algae_vtx),
		 algae_vtx, GL_STATIC_DRAW);

    // Préparation du VBO pour la nourriture : GL_TRIANGLE_FAN
    GLfloat food_vtx[14*2];
    _vbo_food_size = 0;
    food_vtx[_vbo_food_size++] = 0.f;
    food_vtx[_vbo_food_size++] = 0.f;
    for( unsigned int i = 0; i < 13; ++i) {
      food_vtx[_vbo_food_size++] = 0.2f * cosf( (float)M_PI/6.f * i );
      food_vtx[_vbo_food_size++] = 0.2f * sinf( (float)M_PI/6.f * i );
    }
    _vbo_food_size = _vbo_food_size / 2;
    // VBO
    glGenBuffers(1, &_vbo_food);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_food);
    // Pousse les points dans le VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(food_vtx),
		 food_vtx, GL_STATIC_DRAW);
    
    // Delier les VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  };
  ~GLEntity()
  {
    // Et les vbo
    glDeleteBuffers(1, &_vbo_food);
    glDeleteBuffers(1, &_vbo_algae);
    glDeleteBuffers(1, &_vbo_wall);
  }
  // ******************************************************** GLEntity::render
  void render( glm::mat4& projection, EntityPtr item )
  {
    // dynamic_cast en cas de polymorphisme
    // sinon un static_cast marcherait aussi.
    if( Wall* w = dynamic_cast<Wall*>(item.get()) ) {
      render( projection, *w);
    }
    else if( Algae* a = dynamic_cast<Algae*>(item.get()) ) {
      render( projection, *a);
    }
    else if( Food* f = dynamic_cast<Food*>(item.get()) ) {
      render( projection, *f);
    }
    else {
      std::cerr <<  "GLEntity::render avec type inconnu" << std::endl;
    }
  };
  /** Wall */
  void render( glm::mat4& projection, Wall& wall )
  {
    // Translation
    Vec2F center = coord_from_center( wall.pos() );
    glm::mat4 trans = glm::translate(glm::mat4(1.0f),
				     glm::vec3( center.first,
						center.second,
						0.0));
    // Et finalement
    glm::mat4 mvp = projection * trans;

    glUniformMatrix4fv(_uniform_mvp, 1, GL_FALSE,
     		       glm::value_ptr(mvp));

    // Triangles Verts
    glUniform3f( _uniform_l_color, 0.f, 1.f, 0.f );
    // Obstacle
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_wall );
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
    glDrawArrays(GL_TRIANGLE_FAN, 0, _vbo_wall_size);
  };
  /** Algae */
  void render( glm::mat4& projection, Algae& alga )
  {
    // Translation
    Vec2F center = coord_from_center( alga.pos() );
    glm::mat4 trans = glm::translate(glm::mat4(1.0f),
				     glm::vec3( center.first,
						center.second,
						0.0));
    // Et finalement
    glm::mat4 mvp = projection * trans;

    glUniformMatrix4fv(_uniform_mvp, 1, GL_FALSE,
     		       glm::value_ptr(mvp));

    // Triangles Rouges
    glUniform3f( _uniform_l_color, 1.f, 0.f, 0.f );
    // Obstacle
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_algae );
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
    glDrawArrays(GL_TRIANGLES, 0, _vbo_algae_size );
  };
  /** Food */
  void render( glm::mat4& projection, Food& food )
  {
    // Translation
    Vec2F center = coord_from_center( food.pos() );
    glm::mat4 trans = glm::translate(glm::mat4(1.0f),
				     glm::vec3( center.first,
						center.second,
						0.0));
    // Et finalement
    glm::mat4 mvp = projection * trans;

    glUniformMatrix4fv(_uniform_mvp, 1, GL_FALSE,
     		       glm::value_ptr(mvp));

    // Triangles bleu
    glUniform3f( _uniform_l_color, 0.f, 0.f, 1.f );
    // Obstacle
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_food );
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
    glDrawArrays(GL_TRIANGLE_FAN, 0, _vbo_food_size);
  };
  // ***************************************************** GLEntity::attributs
private:
   /** Variables globale du Programme GLSL */
  GLint _attribute_coord2d;
  /** Uniform var */
  GLint _uniform_l_color, _uniform_mvp;
  /** Vertexw buffers */
  GLuint _vbo_algae, _vbo_wall, _vbo_food;
  unsigned int _vbo_algae_size, _vbo_wall_size, _vbo_food_size;

  // ******************************************************** GLEntity::convert
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

#endif // GL_ENTITY_HPP
