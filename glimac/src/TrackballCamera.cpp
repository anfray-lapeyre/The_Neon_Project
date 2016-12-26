#include "glimac/TrackballCamera.hpp"

namespace glimac {

     void TrackballCamera::moveFront(float delta) {
     	this->m_fDistance += delta;
     }


     void TrackballCamera::rotateLeft(float degrees) {
     	this->m_fAngleX += degrees;
     }


     void TrackballCamera::rotateUp(float degrees) {
     	this->m_fAngleY += degrees;
     }


     glm::mat4 TrackballCamera::getViewMatrix() const{
     	glm::mat4 res = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -this->m_fDistance));
     		res = glm::rotate(res,glm::radians(this->m_fAngleX),glm::vec3(0.f,1.f,0.f));
     		res = glm::rotate(res,glm::radians(this->m_fAngleY),glm::vec3(1.f,0.f,0.f));


     	return res;

     }


 }


