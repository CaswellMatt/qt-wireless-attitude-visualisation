/*
 * Cylinder.cpp
 *
 *  Created on: 26 Apr 2019
 *      Author: mc18g13
 */

#include "Cylinder.h"
#include <math.h>
#include <iostream>

Cylinder::Cylinder(vec3 unitVectorDirectionOfCylinder, vec3 startPointCoordinate, vec3 color, float length, float radius) {
	unsigned int vertexCount = 20;
	unsigned int indexCount =  3 * vertexCount;
	setup(vertexCount, vertexCount, indexCount);

	calculateVertices(unitVectorDirectionOfCylinder, startPointCoordinate, color, length, radius);
	calculateIndices();
}


void Cylinder::calculateVertices(vec3 unitVectorDirectionOfCylinder, vec3 startPointCoordinate, vec3 color,
								 float length, float radius) {

	float currentAngle = 0.0f;
	unsigned int iterationsForPairsOfVertices = (vertexCount / 2);

	vec3 centreLineOfCylinder(0.0f, 0.0f, 1.0f);
	quat rotationToUpdateDirection = RotationBetweenVectors(unitVectorDirectionOfCylinder, centreLineOfCylinder);

	float angle = 2*M_PI / (float)(iterationsForPairsOfVertices - 1);
	for (unsigned int i = 0; i < iterationsForPairsOfVertices; i++) {
		int j = i * 12;

	    float x = cos(currentAngle) * radius;
	    float y = sin(currentAngle) * radius;

	    quat unmappedCoordinateClose = quat(0.0f, x, y, 0.0f);
	    quat unmappedCoordinateFar = quat(0.0f, x, y, length);

	    quat remappedCoordinateClose = inverse(rotationToUpdateDirection) * unmappedCoordinateClose * rotationToUpdateDirection;
	    quat remappedCoordinateFar = inverse(rotationToUpdateDirection) * unmappedCoordinateFar * rotationToUpdateDirection;

	    *(vertices + j)      = remappedCoordinateClose.x;
	    *(vertices + j + 1)  = remappedCoordinateClose.y;
	    *(vertices + j + 2)  = remappedCoordinateClose.z;
	    *(vertices + j + 3)  = color.x;
	    *(vertices + j + 4)  = color.y;
	    *(vertices + j + 5)  = color.z;
	    *(vertices + j + 6)  = remappedCoordinateFar.x;
	    *(vertices + j + 7)  = remappedCoordinateFar.y;
	    *(vertices + j + 8)  = remappedCoordinateFar.z;
	    *(vertices + j + 9)  = color.x;
	    *(vertices + j + 10) = color.y;
	    *(vertices + j + 11) = color.z;

		currentAngle += angle;
	}
}

void Cylinder::calculateIndices() {
	for (unsigned int i = 0; i < vertexCount; i++) {
		if (i % 2 == 0) {
			*(indices + i) = i;
		} else {
			*(indices + i) = 0;
		}
	}

	for (unsigned int i = 0; i < vertexCount; i++) {
		unsigned int j = vertexCount;
		*(indices + j + i)= i;
	}

	for (unsigned int i = 0; i < vertexCount; i++) {
		unsigned int j = 2 * vertexCount;
		if (i % 2 == 1) {
			*(indices + j + i) = i;
		} else {
			*(indices + j + i) = 1;
		}
	}
}
