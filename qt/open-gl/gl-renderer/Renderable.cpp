/*
 * Renderable.cpp
 *
 *  Created on: 29 Apr 2019
 *      Author: mc18g13
 */

#include "Renderable.h"
#include <iostream>

Renderable::~Renderable() {
	delete[] indices;
	delete[] vertices;
}

void Renderable::setup(unsigned int size,
					   unsigned int vertexArraySize,
					   unsigned int indexArraySize) {
	vertexCount = size;
	createVertexArray(vertexArraySize);
	createIndexArray(indexArraySize);
}

void Renderable::createVertexArray(unsigned int size) {
	vertexArrayLength = size * 6;
	vertices = new float[vertexArrayLength];
}

void Renderable::createIndexArray(unsigned int size) {
	indexArrayLength = size;
	totalVertexElements = size * 6;
	indices = new unsigned int[size];
}

unsigned int Renderable::getVertexCount() {
	return vertexCount;
}

float* Renderable::getVertices() {
	return vertices;
}

unsigned int* Renderable::getIndices() {
	return indices;
}

unsigned int Renderable::getVertexArrayLength() {
	return vertexArrayLength;
}

unsigned int Renderable::getIndexArrayLength() {
	return indexArrayLength;
}

unsigned int Renderable::getTotalVertexElements() {
	return totalVertexElements;
}

void Renderable::printVertices() {
	for (unsigned int i = 0; i < vertexArrayLength; i++) {
		int vertexIndex = i % arrayElementsForOneVertex;
		if (vertexIndex < 3) {
			std::cout << *(vertices + i) << " ";
		} else if (vertexIndex == 3) {
			std::cout << std::endl;
		}
	}
}

void Renderable::printIndices() {
	for (unsigned int i = 0; i < vertexCount; i++) {
		std::cout << *(indices + i) << std::endl;
	}
}

