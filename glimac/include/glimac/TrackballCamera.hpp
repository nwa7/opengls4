#pragma once
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class TrackballCamera {
private:
  float m_fDistance;
  float m_fAngleX;
  float m_fAngleY;

public:
  void moveFront(float delta) { this->m_fDistance = m_fDistance - delta; };
  void rotateLeft(float degrees) { this->m_fAngleX = m_fAngleX + degrees; };
  void rotateUp(float degrees) { this->m_fAngleY = m_fAngleY + degrees; };
  glm::mat4 getViewMatrix() const {
    glm::mat4 MVMatrix = glm::mat4(1.f);
    MVMatrix = glm::translate(MVMatrix, glm::vec3(0, 0, m_fDistance));
    MVMatrix =
        glm::rotate(MVMatrix, glm::radians(m_fAngleX), glm::vec3(1, 0, 0));

    MVMatrix =
        glm::rotate(MVMatrix, glm::radians(m_fAngleY), glm::vec3(0, 1, 0));
    return MVMatrix;
  };
};