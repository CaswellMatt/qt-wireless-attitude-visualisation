/*
 * Cube.h
 *
 *  Created on: 25 Apr 2019
 *      Author: mc18g13
 */

#ifndef CUBE_H
#define CUBE_H

#include "Centroid.h"

class Cube : Centroid {
public:
	Cube();
	virtual ~Cube();
	void printVertices();

	const unsigned int size = 36;
	const unsigned int arrayElementsForOneVertex = 6;

	float halfCubeWidth = 30.0f;

	float left   = leftRightCentre 		 - halfCubeWidth;
	float bottom = topBottomCentre 		 - halfCubeWidth;
	float back   = forwardBackwardCentre - halfCubeWidth;
	float right  = leftRightCentre 		 + halfCubeWidth;
	float top    = topBottomCentre       + halfCubeWidth;
	float front  = forwardBackwardCentre + halfCubeWidth;

	unsigned int leftBottomBack   = 0;
	unsigned int leftBottomFront  = 1;
	unsigned int leftTopBack      = 2;
	unsigned int leftTopFront     = 3;
	unsigned int rightTopBack     = 4;
	unsigned int rightTopFront    = 5;
	unsigned int rightBottomBack  = 6;
	unsigned int rightBottomFront = 7;

	float vertices[48] = {
		left , bottom, back , 0.583f,  0.771f,  0.914f,
		left , bottom, front, 0.909f,  0.115f,  0.436f,
		left , top   , back , 0.327f,  0.483f,  0.844f,
		left , top   , front, 0.822f,  0.569f,  0.201f,
		right, top   , back , 0.435f,  0.602f,  0.923f,
		right, top   , front, 0.910f,  0.747f,  0.185f,
		right, bottom, back , 0.597f,  0.770f,  0.761f,
		right, bottom, front, 0.559f,  0.436f,  0.730f
	 };

	unsigned int indices[36] = {
			//front
		leftBottomFront, rightBottomFront, rightTopFront  ,
		leftBottomFront, leftTopFront    , rightTopFront  ,

			//left
		leftBottomBack , leftTopBack     , leftTopFront   ,
		leftBottomBack , leftBottomFront , leftTopFront   ,

			//bottom
		rightBottomBack, rightBottomFront, leftBottomFront,
		rightBottomBack, leftBottomBack  , leftBottomFront,

			//right
		rightBottomBack, rightTopBack    , rightTopFront  ,
		rightBottomBack, rightBottomFront, rightTopFront  ,

			//top
		rightTopBack   , rightTopFront   , leftTopFront   ,
		rightTopBack   , leftTopBack     , leftTopFront   ,

			//back
		leftBottomBack , rightBottomBack , rightTopBack   ,
		leftBottomBack , leftTopBack     , rightTopBack	  ,
	};
};

#endif /* CUBE_H */
