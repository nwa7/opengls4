#include <GL/glew.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <iostream>

using namespace glimac;
// BZBZBZBZBZ
struct Vertex2DUV {
  glm::vec2 position;
  glm::vec2 texture;

  Vertex2DUV(glm::vec2 pos, glm::vec2 text) {
    position = pos;
    texture = text;
  };
};

int main(int argc, char **argv) {
  // Initialize SDL and open a window
  SDLWindowManager windowManager(800, 600, "GLImac");

  // Initialize glew for OpenGL3+ support
  GLenum glewInitError = glewInit();
  if (GLEW_OK != glewInitError) {
    std::cerr << glewGetErrorString(glewInitError) << std::endl;
    return EXIT_FAILURE;
  }

  FilePath applicationPath(argv[0]);
  Program program =
      loadProgram(applicationPath.dirPath() + "shaders/tex2D.vs.glsl",
                  applicationPath.dirPath() + "shaders/tex2D.fs.glsl");
  program.use();

  std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/
  // Création d'un vbo
  GLuint vbo;
  // La variable vbo contient désormais l'identifiant d'un vbo
  glGenBuffers(1, &vbo);
  // Binding d'un vbo sur la cible gl_array_buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // On peut maintenant modifier le vbo en passant par la cible gl_array_buffer

  Vertex2DUV vertices[] = {Vertex2DUV(glm::vec2(-1., -1.), glm::vec2(0, 0)),
                           Vertex2DUV(glm::vec2(1., -1.), glm::vec2(0, 0)),
                           Vertex2DUV(glm::vec2(0., 1.), glm::vec2(0, 0))};
  glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices,
               GL_STATIC_DRAW);

  // Debinding dun VBO sur la cible gl_array_buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Création d'un vao
  GLuint vao;
  glGenVertexArrays(1, &vao);
  // "vao" devient LE vao courant
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Activation de l'attribut de vertex 0
  const GLuint VERTEX_ATTR_POSITION = 0;
  const GLuint VERTEX_ATTR_TEXTURE = 1;
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

  glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DUV),
                        (const GLvoid *)offsetof(Vertex2DUV, position));
  glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DUV),
                        (const GLvoid *)offsetof(Vertex2DUV, texture));

  // Binding d'un vbo sur la cible gl array buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
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
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Update the display
    windowManager.swapBuffers();
  }
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  return EXIT_SUCCESS;
}
