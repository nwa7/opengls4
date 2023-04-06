#include "glimac/Sphere.hpp"
#include "glimac/common.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <iostream>
#include <vector>

using namespace glimac;

int main(int argc, char **argv) {
  // Initialize SDL and open a window

  float largeur = 800.;
  float hauteur = 600.;
  SDLWindowManager windowManager(largeur, hauteur, "Sphere");

  // Initialize glew for OpenGL3+ support
  GLenum glewInitError = glewInit();
  if (GLEW_OK != glewInitError) {
    std::cerr << glewGetErrorString(glewInitError) << std::endl;
    return EXIT_FAILURE;
  }
  FilePath applicationPath(argv[0]);
  Program program =
      loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                  applicationPath.dirPath() + "shaders/normal.fs.glsl");
  program.use();

  std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

  GLint loc1 = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
  GLint loc2 = glGetUniformLocation(program.getGLId(), "uMVMatrix");
  GLint loc3 = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

  glEnable(GL_DEPTH_TEST);
  glm::mat4 ProjMatrix;
  glm::mat4 MVMatrix;
  glm::mat4 NormalMatrix;

  ProjMatrix =
      glm::perspective(glm::radians(70.f), largeur / hauteur, 0.f, 100.f);
  MVMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -5.f));
  NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/
  // Création d'un vbo
  GLuint vbo;
  Sphere sphere(1, 32, 16);
  // La variable vbo contient désormais l'identifiant d'un vbo
  glGenBuffers(1, &vbo);
  // Binding d'un vbo sur la cible gl_array_buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // On peut maintenant modifier le vbo en passant par la cible gl_array_buffer

  int tabSize = sphere.getVertexCount();
  const glimac::ShapeVertex *dataPointer = sphere.getDataPointer();
  ShapeVertex vertices[tabSize];
  for (int i = 0; i < tabSize; i++) {
    vertices[i] = dataPointer[i];
  };
  glBufferData(GL_ARRAY_BUFFER, tabSize * sizeof(ShapeVertex), vertices,
               GL_STATIC_DRAW);

  // Debinding dun VBO sur la cible gl_array_buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Création d'un vao
  GLuint vao;
  glGenVertexArrays(1, &vao);

  // "vao" devient LE vao courant
  glBindVertexArray(vao);
  // Activation de l'attribut de vertex 0
  const GLuint VERTEX_ATTR_POSITION = 0;
  const GLuint VERTEX_ATTR_NORMAL = 1;
  const GLuint VERTEX_ATTR_TEX = 2;
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEX);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        (const GLvoid *)offsetof(ShapeVertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        (const GLvoid *)offsetof(ShapeVertex, normal));
  glVertexAttribPointer(VERTEX_ATTR_TEX, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        (const GLvoid *)offsetof(ShapeVertex, texCoords));

  // Debinding d'un vbo sur la cible gl array buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Debinding vao
  glBindVertexArray(0);

  // Application loop:
  bool done = false;
  while (!done) {
    // Event loop:
    SDL_Event e;
    while (windowManager.pollEvent(e)) {
      if (e.type == SDL_QUIT) {
        done = true; // Leave the loop after this iteration
      }
    }

    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(loc1, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(loc2, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(loc3, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
    // Update the display
    windowManager.swapBuffers();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  return EXIT_SUCCESS;
}
