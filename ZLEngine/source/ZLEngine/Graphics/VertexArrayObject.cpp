#include "ZLEngine/Graphics/VertexArrayObject.h"
#include "GLEW/glew.h"

VertexArrayObject::VertexArrayObject()
{
	ID = EAB = VAB = 0;

	Shape.PositionMatrix = CirclePositions;
	Shape.IndicesMatrix = CircleIndices;

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
		sizeof(float) * 3,	// stride - the length it takes to get to each number
		(void*)0			// offset of how many numbers to skip in the matrix
	);

	//enable the vertex array
	glEnableVertexAttribArray(0);

	//clear the buffer
	glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject()
{
	// clean up the VAO in OpenGL
	glDeleteVertexArrays(1, &ID);

	// clean up the vectors
	Shape.PositionMatrix.clear();
	Shape.IndicesMatrix.clear();

	cout << "Deleted VAO..." << endl;
}

void VertexArrayObject::Draw()
{
	// bind our VAO to the current buffer
	glBindVertexArray(ID);

	//draw the 3D object
	glDrawElements(
		GL_TRIANGLES,					// what type of objects are we drawing
		Shape.IndicesMatrix.size(),		// how many vertices do we draw
		GL_UNSIGNED_INT,				// what is the type of data that's being input
		(void*)0						// how many should we skip
	);

	//clear the VAO from the current array to allow for the next object
	glBindVertexArray(0);
}
