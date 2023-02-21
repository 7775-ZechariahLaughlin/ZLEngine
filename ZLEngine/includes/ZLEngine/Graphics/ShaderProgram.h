#pragma once
#include "ZLEngine/CoreMinimal.h"
#include "GLM/glm.hpp"

class ShaderProgram {
public: 
	ShaderProgram();
	~ShaderProgram();

	// this will initialise our shader as a vertex fragment shader
	bool InitVFShader(VFShaderParams ShaderFilePaths);

	// Run the shader program as the current shader to be interacted with in OpenGL
	void RunShader();

	// returns the program ID
	zluint GetID() const { return ProgramID; }

	// allow changing mat4 variables in shader code
	void SetMat4(const char* ShaderVariable, glm::mat4 Value);

private: 
	// Attach the shader to the VAO in OpenGL
	bool AttachShader(const wchar_t* ShaderFilePath, ShaderTypes Type);
	
	// Link the shader to OpenGL
	bool Link();

private: 
	// OpenGL will assign an ID to our shader
	zluint ProgramID;
	
	/*array that will hold IDs to our unique shaders
	* 0 - Vertex 
	* 1 - Fragment */
	zluint VFShaderIDs[2] = { 0 };
};
