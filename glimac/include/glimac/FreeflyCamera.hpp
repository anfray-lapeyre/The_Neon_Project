#ifndef FREEFLY_CAMERA_H
#define FREEFLY_CAMERA_H

#include <glimac/glm.hpp>

class FreeflyCamera {
private:
    
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors();

public:
	float m_fPhi;
    float m_fTheta;
    glm::vec3 m_Position;
    FreeflyCamera();
    void moveLeft(float t);
    void moveFront(float t);

    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
};

#endif //FREEFLY_CAMERA_H
