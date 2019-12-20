#ifndef GRID_H
#define GRID_H

#include "Renderable.h"

class Grid : public Renderable {
public:
	Grid();
	void addNumbersToIndices(unsigned int *array, unsigned int index);
	virtual ~Grid();

    float lineSpacing = 100.0f;
    float totalWidth = 6000.0f;

    int arrayElementsForPairOfVertices = 2 * arrayElementsForOneVertex;

};

#endif
