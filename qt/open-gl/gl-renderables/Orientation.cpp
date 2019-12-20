/*
 * Orientation.cpp
 *
 *  Created on: 25 Apr 2019
 *      Author: mc18g13
 */

#include "Orientation.h"
#include <iostream>
Orientation::Orientation() {
//	const static unsigned int vertexCount = 6;
	setup(6, 6, 6);

	float verticesBuffer[36] = {
		0.0f  , 0.0f  , 0.0f  , 1.000f,  0.000f,  0.000f,
		100.0f, 0.0f  , 0.0f  , 1.000f,  0.000f,  0.000f,
		0.0f  , 0.0f  , 0.0f  , 0.000f,  1.000f,  0.000f,
		0.0f  , 100.0f, 0.0f  , 0.000f,  1.000f,  0.000f,
		0.0f  , 0.0f  , 0.0f  , 0.000f,  0.000f,  1.000f,
		0.0f  , 0.0f  , 100.0f, 0.000f,  0.000f,  1.000f,
	 };

	unsigned int indicesBuffer[6] = {
		0,1,
		2,3,
		4,5
	};

	for (int i = 0; i < 6; i++) {
		int index = i * 6;

		*(vertices + index	  ) = verticesBuffer[index];
		*(vertices + index + 1) = verticesBuffer[index + 1];
		*(vertices + index + 2) = verticesBuffer[index + 2];
		*(vertices + index + 3) = verticesBuffer[index + 3];
		*(vertices + index + 4) = verticesBuffer[index + 4];
		*(vertices + index + 5) = verticesBuffer[index + 5];

		*(indices + i) = indicesBuffer[i];

	}

}

Orientation::~Orientation() {
	// TODO Auto-generated destructor stub
}
