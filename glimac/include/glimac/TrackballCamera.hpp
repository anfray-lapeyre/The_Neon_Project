#pragma once


#include "glm.hpp"

namespace glimac {
	class TrackballCamera {

	private:
		float m_fDistance;
		float m_fAngleX;
		float m_fAngleY;
	   
	public:
	    TrackballCamera(float distance=5.f,float angleX=0.f,float angleY=0.f):
	        m_fDistance(distance), m_fAngleX(angleX), m_fAngleY(angleY) {
	    }

	     void moveFront(float delta);
	     void rotateLeft(float degrees);
	     void rotateUp(float degrees);
	     glm::mat4 getViewMatrix() const;

	};

}
