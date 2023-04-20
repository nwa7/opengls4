#include "glimac/Image.hpp"
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
  SDLWindowManager windowManager(largeur, hauteur, "Moon");

  // Initialize glew for OpenGL3+ support
  GLenum glewInitError = glewInit();
  if (GLEW_OK != glewInitError) {
    std::cerr << glewGetErrorString(glewInitError) << std::endl;
    return EXIT_FAILURE;
  }
  auto moonImg =
      (loadImage("/home/noi7/Documents/IMAC_A2/OpenGL/GLImac-Template/"
                 "assets/textures/MoonMap.jpg"));
  auto earthImg =
      (loadImage("/home/noi7/Documents/IMAC_A2/OpenGL/GLImac-Template/"
                 "assets/textures/EarthMap.jpg"));
  auto cloudsImg =
      (loadImage("/home/noi7/Documents/IMAC_A2/OpenGL/GLImac-Template/"
                 "assets/textures/CloudMap.jpg"));

  FilePath applicationPath(argv[0]);
  Program program =
      loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                  applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl");
  program.use();

  std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/

  Sphere sphere(1, 32, 16);

  GLint loc1 = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
  GLint loc2 = glGetUniformLocation(program.getGLId(), "uMVMatrix");
  GLint loc3 = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
  GLint locTex = glGetUniformLocation(program.getGLId(), "uTex");
  GLint locTex2 = glGetUniformLocation(program.getGLId(), "uTex2");

  glm::mat4 ProjMatrix;
  glm::mat4 MVMatrix;
  glm::mat4 NormalMatrix;

  ProjMatrix =
      glm::perspective(glm::radians(70.f), largeur / hauteur, 0.1f, 100.f);
  MVMatrix = glm::translate(MVMatrix, glm::vec3(0, 0, -5));
  NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

  // Earth img
  GLuint earthTexture;
  glGenTextures(1, &earthTexture);
  glBindTexture(GL_TEXTURE_2D, earthTexture); // la texture earthTexture est
                                              // bindée sur l'unité GL_TEXTURE0
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earthImg->getWidth(),
               earthImg->getHeight(), 0, GL_RGBA, GL_FLOAT,
               earthImg->getPixels());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Clouds img
  GLuint cloudsTexture;
  glGenTextures(1, &cloudsTexture);
  glBindTexture(GL_TEXTURE_2D, cloudsTexture); // la texture cloudTexture est
                                               // bindée sur l'unité GL_TEXTURE1
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloudsImg->getWidth(),
               cloudsImg->getHeight(), 0, GL_RGBA, GL_FLOAT,
               cloudsImg->getPixels());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, earthTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, cloudsTexture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE0
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0); // débind sur l'unité GL_TEXTURE1

  // Moon img
  GLuint moonTexture;
  glGenTextures(1, &moonTexture);
  glBindTexture(GL_TEXTURE_2D, moonTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moonImg->getWidth(),
               moonImg->getHeight(), 0, GL_RGBA, GL_FLOAT,
               moonImg->getPixels());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glUniform1i(locTex, 0);
  // Indique à OpenGL qu'il doit aller chercher sur l'unité de texture 1
  // pour lire dans la texture uEarthTexture:
  glUniform1i(locTex2, 1);

  // Création d'un vbo
  GLuint vbo;
  // La variable vbo contient désormais l'identifiant d'un vbo
  glGenBuffers(1, &vbo);
  // Binding d'un vbo sur la cible gl_array_buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // On peut maintenant modifier le vbo en passant par la cible gl_array_buffer

  int tabSize = sphere.getVertexCount();
  glBufferData(GL_ARRAY_BUFFER, tabSize * sizeof(ShapeVertex),
               sphere.getDataPointer(), GL_STATIC_DRAW);

  // Debinding dun VBO sur la cible gl_array_buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnable(GL_DEPTH_TEST);

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

  std::vector<glm::vec3> axis;
  for (int i = 0; i < 32; i++) {
    axis.push_back(glm::sphericalRand(1.0f));
  };

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
    glBindVertexArray(vao);

    glUniformMatrix4fv(loc1, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * MVMatrix));
    MVMatrix = glm::rotate(MVMatrix, 0.005f,
                           glm::vec3(0.f, 1.f, 0.f)); // Translation * Rotation
    glUniformMatrix4fv(loc2, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(loc3, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    // Textures

    glBindTexture(GL_TEXTURE_2D, earthTexture);
    // Indique à OpenGL qu'il doit aller chercher sur l'unité de texture 0
    // pour lire dans la texture uEarthTexture:
    glUniform1i(locTex, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    // Indique à OpenGL qu'il doit aller chercher sur l'unité de texture 1
    // pour lire dans la texture uEarthTexture:
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cloudsTexture);
    glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

    glBindTexture(GL_TEXTURE_2D, moonTexture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    for (int i = 0; i < 32; i++) {

      glm::mat4 MVMatrix =
          glm::translate(glm::mat4(1), glm::vec3(0, 0, -5)); // Translation
      MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(),
                             axis[i]); // Translation * Rotation
      MVMatrix =
          glm::translate(MVMatrix,
                         axis[i + 1]); // Translation * Rotation * Translation
      MVMatrix =
          glm::translate(MVMatrix,
                         axis[i + 1]); // Translation * Rotation * Translation
      MVMatrix = glm::scale(
          MVMatrix,
          glm::vec3(0.2, 0.2,
                    0.2)); // Translation * Rotation * Translation * Scale

      // Pass the new MVMatrix to the shader
      glUniformMatrix4fv(loc1, 1, GL_FALSE,
                         glm::value_ptr(ProjMatrix * MVMatrix));
      glUniformMatrix4fv(loc2, 1, GL_FALSE, glm::value_ptr(MVMatrix));
      glUniformMatrix4fv(loc3, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

      // Draw the new sphere
      glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
    }
    glBindVertexArray(0);
    // Update the display
    windowManager.swapBuffers();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &earthTexture);
  glDeleteTextures(1, &moonTexture);
  glDeleteTextures(1, &cloudsTexture);
  return EXIT_SUCCESS;
}
