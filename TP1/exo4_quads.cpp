#include <GL/glew.h>
#include <cstddef>

#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/glm.hpp>
#include <iostream>

using namespace glimac;

struct Vertex2DColor {
  glm::vec2 position;
  glm::vec3 color;

  Vertex2DColor() {
    position = {0, 0};
    color = {1, 0, 1};
  };

  Vertex2DColor(glm::vec2 pos, glm::vec3 col) {
    position = pos;
    color = col;
  };
};

int main(int argc, char **argv) {
  // Initialize SDL and open a window
  SDLWindowManager windowManager(800, 600, "EXO_1");

  // Initialize glew for OpenGL3+ support
  GLenum glewInitError = glewInit();
  if (GLEW_OK != glewInitError) {
    std::cerr << glewGetErrorString(glewInitError) << std::endl;
    return EXIT_FAILURE;
  }

  FilePath applicationPath(argv[0]);
  Program program =
      loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                  applicationPath.dirPath() + "shaders/triangle.fs.glsl");
  program.use();

  std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  Vertex2DColor vertices[] = {
      Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(1, 0, 0)), // premier sommet
      Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 1, 0)),  // deuz sommet
      Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(0, 0, 1)),
      Vertex2DColor(glm::vec2(-0.5, -0.5),
                    glm::vec3(1, 0, 0)),                       // premier sommet
      Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)), // deuz sommet
      Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 0, 1))};
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices,
               GL_STATIC_DRAW); // Changer nombre sommets
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  const GLuint VERTEX_ATTR_POSITION = 0;
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(8);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor),
                        (const GLvoid *)offsetof(Vertex2DColor, position));
  glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor),
                        (const GLvoid *)offsetof(Vertex2DColor, color));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // Update the display
    windowManager.swapBuffers();
  }
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  return EXIT_SUCCESS;
}
