/*
 * Cube.cpp
 *
 *  Created on: 25 Apr 2019
 *      Author: mc18g13
 */

#include "Cube.h"
#include <iostream>

Cube::Cube() {
	// TODO Auto-generated constructor stub

}

Cube::~Cube() {
	// TODO Auto-generated destructor stub
}

void Cube::printVertices() {
	for (unsigned int i = 0; i < size; i++) {
		int vertexIndex = i % arrayElementsForOneVertex;
		if (vertexIndex < 3) {
			std::cout << vertices[i] << " ";
		} else if (vertexIndex == 3) {
			std::cout << vertices[i] << std::endl;
		}
	}
}

