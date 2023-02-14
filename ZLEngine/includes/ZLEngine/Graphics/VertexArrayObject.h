#pragma once
#include "ZLEngine/CoreMinimal.h"

const PositionMatrix TrianglePositions{
	// x	// y	// z
	-0.5f,	-0.5f,	0.0f, //bottom left
	 0.5f,	-0.5f,	0.0f, //bottom right
	 0.0f,	 0.5f,	0.0f  //top middle
};

const IndicesMatrix TriangleIndices{
	0, 1, 2
};

const PositionMatrix PolyPositions{
	// x	// y	// z
	-0.5f,	-0.5f,	0.0f, //bottom left
	 0.5f,	-0.5f,	0.0f, //bottom right
	 0.5f,	 0.5f,	0.0f, //top right
	-0.5f,	 0.5f,  0.0f  //top left

};

const IndicesMatrix PolyIndices{
	0, 3, 1,	//Triangle 1
	3, 2, 1		//Triangle 2
};
const PositionMatrix CirclePositions{
	// x		// y		// z 
	-0.375f,	0.375f,		0.0f, //top left
	 0.0f,		0.5f,		0.0f, //top
	 0.375f,	0.375f,		0.0f, //top right
	-0.5f,		0.0f,		0.0f, //middle left
	 0.0f,		0.0f,		0.0f, //centre
	 0.5f,		0.0f,		0.0f, //middle right
	-0.375f,   -0.375f,		0.0f, //bottom left
	 0.0f,	   -0.5f,		0.0f, //bottom
	 0.375f,   -0.375f,		0.0f  //bottom right

};

const IndicesMatrix CircleIndices{
	3, 0, 4,	//Triangle 1
	0, 1, 4,	//Triangle 2
	1, 2, 4,	//Triangle 3
	2, 5, 4,	//Triangle 4
	5, 8, 4,	//Triangle 5
	8, 7, 4,	//Triangle 6
	7, 6, 4,	//Triangle 7
	6, 3, 4,	//Triangle 8
};

class VertexArrayObject {

public: 
	VertexArrayObject();
	~VertexArrayObject();

	void Draw();

private: 
	ShapeData Shape;

	/* this is our version of unsigned int
	ID - ID for the VAO
	VAB - ID for the Vertices
	EAB - ID for the Indices */
	zluint ID, VAB, EAB;
};

