#pragma once
#include "ZLEngine/CoreMinimal.h"

// hold the image data for conversion to the texture file
struct ImportImageData {
	int w = 0;
	int h = 0;
	int channels = 0;
};

class Texture {
public: 
	Texture();
	~Texture();

	// create texture file from imported image path
	bool CreateTextureFromFilePath(const char* FilePath);

	// return the OpenGL texture ID
	zluint GetID() const { return TextureID; };

	// bind the texture as the current texture in OpenGL
	void BindTexture();

	// assigns the relevant data to the current loaded texture
	void ActivateTexture();

	// clear the texture in OpenGL
	static void ResetTexture();

	const char* GetFilePath() const { return FilePath; };

private:
	// hold the ID for the texture in OpenGL
	zluint TextureID;
	// hold the filepath to avoid duplicates
	const char* FilePath;


};
