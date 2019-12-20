/*
 * Grid.cpp
 *
 *  Created on: 11 Apr 2019
 *      Author: mc18g13
 */

#include "Grid.h"
#include <iostream>
#include <math.h>

Grid::Grid() {

	vertexArrayLength = 6000;
	vertexCount = vertexArrayLength / arrayElementsForOneVertex;
	setup(vertexCount, vertexCount, vertexCount);
	float numberOfLines = vertexCount / 2;
	float numberOfLinesInOneDirection = numberOfLines / 2;
	float extentWidth = round(numberOfLinesInOneDirection * lineSpacing / 100) * 100;
	float startPosition = -extentWidth / 2;

//	std::cout << "numberOfVertices" << numberOfVertices << std::endl;
//	std::cout << "numberOfLines" << numberOfLines << std::endl;
//	std::cout << "numberOfLinesInOneDirection" << numberOfLinesInOneDirection << std::endl;
//	std::cout << "extentWidth" << extentWidth << std::endl;
//	std::cout << "startPosition" << startPosition << std::endl;

    float currentGridLineX = startPosition;
    float currentGridLineZ = startPosition;

	for (unsigned int i = 0; i < vertexArrayLength; i++) {

		int vertexIndex = i % arrayElementsForOneVertex;
		bool isEvenIteration = i % arrayElementsForPairOfVertices >= arrayElementsForOneVertex;

		if (i <= vertexArrayLength / 2) {

			if (i % 12 == 0) currentGridLineX += lineSpacing;
			if (vertexIndex == 0) *(vertices + i) = currentGridLineX;
			if (vertexIndex == 2) *(vertices + i) = isEvenIteration ? -totalWidth : totalWidth;

		} else {

			if (i % 12 == 0) currentGridLineZ += lineSpacing;
			if (vertexIndex == 0) *(vertices + i) = isEvenIteration ? -totalWidth : totalWidth;
			if (vertexIndex == 2) *(vertices + i) = currentGridLineZ;

		}

		if (vertexIndex == 1) *(vertices + i) = 0.0f;
		if (vertexIndex  > 2) *(vertices + i) = 0.0f;



	}

    Grid::addNumbersToIndices(indices, 0);

}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

void Grid::addNumbersToIndices(unsigned int *array, unsigned int index) {
	bool isIndexForLinesParralelToY = index > vertexArrayLength / 2;
	if (index == vertexCount) {
		return;
	} else if (isIndexForLinesParralelToY) {
		*array = index - 1;
	} else {
		*array = index;
	}

	addNumbersToIndices(array + 1, index + 1);
}

