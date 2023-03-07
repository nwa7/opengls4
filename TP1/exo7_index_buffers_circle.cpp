#include <GL/glew.h>
#include <cstddef>

#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/glm.hpp>
#include <iostream>
#include <vector>

using namespace glimac;

struct Vertex2DColor {
  glm::vec2 position;
  glm::vec3 color;

  Vertex2DColor() {
    position = {0.f, 0.f};
    color = {1.f, 0.f, 0.f};
  };

  Vertex2DColor(glm::vec2 pos, glm::vec3 col) {
    position = pos;
    color = col;
  };
};

int main(int argc, char **argv) {
  // Initialize SDL and open a window
  SDLWindowManager windowManager(800, 600, "AYAAA");

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

  int NB_TRIANGLES = 50;

  // Création d'un seul VBO:
  GLuint vbo;
  glGenBuffers(1, &vbo);
  // A partir de ce point, la variable vbo contient l'identifiant d'un VBO

  // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

  std::vector<Vertex2DColor> vertices;

  float angle = 2.0f * glm::pi<float>() / NB_TRIANGLES;
  vertices.push_back(Vertex2DColor(glm::vec2(0, 0), glm::vec3(0, 1, 0)));
  for (int i = 0; i < NB_TRIANGLES; ++i) {
    // vertices.push_back(Vertex2DColor(glm::vec2(0, 0), glm::vec3(1, 0, 0)));
    vertices.push_back(Vertex2DColor(
        glm::vec2(0.5f * glm::cos(i * angle), 0.6f * glm::sin(i * angle)),
        glm::vec3(0, 0, 1)));
    /*vertices.push_back(
        Vertex2DColor(glm::vec2(0.5f * glm::cos((i + 1) * angle),
                                0.6f * glm::sin((i + 1) * angle)),
                      glm::vec3(0, 0, 1)));*/
  }

  glBufferData(GL_ARRAY_BUFFER, (NB_TRIANGLES + 1) * sizeof(Vertex2DColor),
               vertices.data(), GL_STATIC_DRAW);

  // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // => Creation du IBO
  GLuint ibo;
  glGenBuffers(1, &ibo);

  // => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible reservée pour les IBOs
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  // => Tableau d'indices: ce sont les indices des sommets à dessiner
  // On en a 6 afin de former deux triangles
  // Chaque indice correspond au sommet correspondant dans le VBO

  uint32_t indices[NB_TRIANGLES * 3];
  int incr = 1;
  for (int i = 0; i < (3 * NB_TRIANGLES); i += 3) {
    indices[i] = 0;
    indices[i + 1] = incr;
    incr++;
    if (i == (3 * NB_TRIANGLES) - 3) {
      indices[i + 2] = 1;
    } else {
      indices[i + 2] = incr;
    }
  }

  // => On remplit l'IBO avec les indices:
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, NB_TRIANGLES * 3 * sizeof(uint32_t),
               indices, GL_STATIC_DRAW);

  // => Comme d'habitude on debind avant de passer à autre chose
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint vao;
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  // => On bind l'IBO sur GL_ELEMENT_ARRAY_BUFFER; puisqu'un VAO est
  // actuellement bindé, cela a pour effet d'enregistrer l'IBO dans le VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  const GLuint VERTEX_ATTR_POSITION = 3;
  const GLuint VERTEX_ATTR_COLOR = 8;
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DColor),
                        (const GLvoid *)offsetof(Vertex2DColor, position));
  glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DColor),
                        (const GLvoid *)offsetof(Vertex2DColor, color));

  // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Debinding VAO
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

    // => On utilise glDrawElements à la place de glDrawArrays
    // Cela indique à OpenGL qu'il doit utiliser l'IBO enregistré dans le VAO
    glDrawElements(GL_TRIANGLES, 3 * NB_TRIANGLES, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    // Update the display
    windowManager.swapBuffers();
  }
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  return EXIT_SUCCESS;
}
