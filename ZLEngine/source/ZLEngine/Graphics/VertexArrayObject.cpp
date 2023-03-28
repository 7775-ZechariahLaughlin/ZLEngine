#include "ZLEngine/Graphics/VertexArrayObject.h"
#include "GLEW/glew.h"

VertexArrayObject::VertexArrayObject(GeometricShapes ChosenShape)
{
	ID = EAB = VAB = 0;

	// localised version of chosen matrices
	PositionMatrix ChosenPositions = PositionMatrix();
	IndicesMatrix ChosenIndices = IndicesMatrix();

	// switch the chosen matrices depending on the selected geometric shape type
	switch (ChosenShape) {
	case GeometricShapes::Triangle: 
		ChosenPositions = TrianglePositions;
		ChosenIndices = TriangleIndices;
		break;
	case GeometricShapes::Polygon: 
		ChosenPositions = PolyPositions;
		ChosenIndices = PolyIndices;
		break;
	case GeometricShapes::Circle: 
		ChosenPositions = CirclePositions;
		ChosenIndices = CircleIndices;
		break;
	case GeometricShapes::Pentagon:
		ChosenPositions = PentagonPositions;
		ChosenIndices = PentagonIndices;
		break;
	case GeometricShapes::Cube:
		ChosenPositions = CubePositions;
		ChosenIndices = CubeIndicies;
		break;
	default:
		break;
	}

	Shape.PositionMatrix = ChosenPositions;
	Shape.IndicesMatrix = ChosenIndices;

	// create the ID for our VAO
	glGenVertexArrays(1, &ID);

	//bind the data to this vertex array
	glBindVertexArray(ID);

	// HANDLE THE POSITIONS
	// create an id for our array buffer
	glGenBuffers(1, &VAB);

	// bind the above IDs to OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VAB);

	// run throught the data and attach the vertices to our VAB
	glBufferData(
		GL_ARRAY_BUFFER,
		Shape.PositionMatrix.size() * sizeof(float),
		&Shape.PositionMatrix[0],
		GL_STATIC_DRAW
	);

	// HANDLE THE INDICES
	// create an id for our element array buffer
	glGenBuffers(1, &EAB);

	// bind the above ID to OpenGL as the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);

	// run throught the data and attach the indices to our EAB
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		Shape.IndicesMatrix.size() * sizeof(zluint),
		&Shape.IndicesMatrix[0],
		GL_STATIC_DRAW
	);

	// assign the vertices and indices to the VAO
	glVertexAttribPointer(
		0,					// Data Set - 0 = the first data set in the array
		3,					// How many numbers in our matrix to make a triangle
		GL_FLOAT, GL_FALSE, // data type, whether you want to normalise the values
		sizeof(float) * 8,	// stride - the length it takes to get to each number
		(void*)0			// offset of how many numbers to skip in the matrix
	);

	// enable the vertex array
	glEnableVertexAttribArray(0);

	// assign the colour to the shader
	glVertexAttribPointer(
		1,							// Data Set - 1 = the second data set in the array
		3,							// How many numbers in our matrix
		GL_FLOAT, GL_FALSE,			// data type, whether you want to normalise the values
		sizeof(float) * 8,			// stride - the length it takes to get to each number
		(void*)(3 * sizeof(float))	// offset of how many numbers to skip in the matrix
	);

	//enabling the colour array
	glEnableVertexAttribArray(1);

	// assign the texture coordinates to the shader
	glVertexAttribPointer(
		2,							// Data Set - 1 = the second data set in the array
		2,							// How many numbers in our matrix
		GL_FLOAT, GL_FALSE,			// data type, whether you want to normalise the values
		sizeof(float) * 8,			// stride - the length it takes to get to each number
		(void*)(6 * sizeof(float))	// offset of how many numbers to skip in the matrix
	);

	//enabling the texture coordinates array
	glEnableVertexAttribArray(2);

	// clear the buffer
	glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject(vector<Vertex> Vertices, vector<zluint> Indices)
{
	ID = EAB = VAB = 0;

	// assign the vertices and the indices to the class
	this->Vertices = Vertices;
	this->Indices = Indices;

	// create the ID for our VAO
	glGenVertexArrays(1, &ID);

	//bind the data to this vertex array
	glBindVertexArray(ID);

	// HANDLE THE POSITIONS
	// create an id for our array buffer
	glGenBuffers(1, &VAB);

	// bind the above IDs to OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VAB);

	// run throught the data and attach the vertices to our VAB
	glBufferData(
		GL_ARRAY_BUFFER,
		Vertices.size() * sizeof(Vertex),
		&Vertices[0],
		GL_STATIC_DRAW
	);

	// HANDLE THE INDICES
	// create an id for our element array buffer
	glGenBuffers(1, &EAB);

	// bind the above ID to OpenGL as the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);

	// run throught the data and attach the indices to our EAB
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		Indices.size() * sizeof(zluint),
		&Indices[0],
		GL_STATIC_DRAW
	);

	// assign the vertices and indices to the VAO
	glVertexAttribPointer(
		0,					// Data Set - 0 = the first data set in the array
		3,					// How many numbers in our matrix to make a triangle
		GL_FLOAT, GL_FALSE, // data type, whether you want to normalise the values
		sizeof(float) * 8,	// stride - the length it takes to get to each number
		(void*)0			// offset of how many numbers to skip in the matrix
	);

	// enable the vertex array
	glEnableVertexAttribArray(0);

	// assign the normals of the mesh vertices to the shader
	glVertexAttribPointer(
		1,							// Data Set - 1 = the second data set in the array
		3,							// How many numbers in our matrix
		GL_FLOAT, GL_FALSE,			// data type, whether you want to normalise the values
		sizeof(float) * 8,			// stride - the length it takes to get to each number
		(void*)(3 * sizeof(float))	// offset of how many numbers to skip in the matrix
	);

	//enabling the normals array
	glEnableVertexAttribArray(1);

	// assign the texture coordinates to the shader
	glVertexAttribPointer(
		2,							// Data Set - 1 = the second data set in the array
		2,							// How many numbers in our matrix
		GL_FLOAT, GL_FALSE,			// data type, whether you want to normalise the values
		sizeof(float) * 8,			// stride - the length it takes to get to each number
		(void*)(6 * sizeof(float))	// offset of how many numbers to skip in the matrix
	);

	//enabling the texture coordinates array
	glEnableVertexAttribArray(2);

	// clear the buffer
	glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject()
{
	// clean up the VAO in OpenGL
	glDeleteVertexArrays(1, &ID);

	// clean up the vectors
	Shape.PositionMatrix.clear();
	Shape.IndicesMatrix.clear();
	Vertices.clear();
	Indices.clear();

	cout << "Vertex Array Object | Deleted VAO..." << endl;
}

void VertexArrayObject::Draw()
{
	vector<zluint> IndicesToUse;

	// decide which indices to use based on what's assigned
	if (Vertices.size() > 0) {
		IndicesToUse = Indices;
	}
	else {
		IndicesToUse = Shape.IndicesMatrix;
	}

	// bind our VAO to the current buffer
	glBindVertexArray(ID);

	//draw the 3D object
	glDrawElements(
		GL_TRIANGLES,					// what type of objects are we drawing
		IndicesToUse.size(),		// how many vertices do we draw
		GL_UNSIGNED_INT,				// what is the type of data that's being input
		(void*)0						// how many should we skip
	);

	//clear the VAO from the current array to allow for the next object
	glBindVertexArray(0);
}
