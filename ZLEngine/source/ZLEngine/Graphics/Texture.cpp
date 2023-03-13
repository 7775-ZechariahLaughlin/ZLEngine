#include "ZLEngine/Graphics/Texture.h"
#include "GLEW/glew.h"
#include "stb_image/stb_image.h"
#include <string>

Texture::Texture()
{
	TextureID = 0;
	FilePath = "";

	// assign an OpenGL texture ID
	glGenTextures(1, &TextureID);
	// set the current texture as the current OpenGL texture
	BindTexture();

	/* set default geometry texture setting
	 * set what happens if texture doesn't fit the whole geometry
	 * s = u = x --- t = v = y --- r = z (we don't use z for 2D textures)*/
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//set how the image will blur between pixels as it scales up and down resolution
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	cout << "Textures | Initialised Texture with ID: " << TextureID << endl;

}

Texture::~Texture()
{
	// destroy the texture from memory
	glDeleteTextures(1, &TextureID);

	cout << " Textures | Texture " << TextureID << " deleted." << endl;
}

bool Texture::CreateTextureFromFilePath(const char* FilePath)
{
	BindTexture();

	this->FilePath = FilePath;

	// hold the imported image data
	ImportImageData ImageData;

	// tell stbi to load our images in flipped
	stbi_set_flip_vertically_on_load(true);

	// loading the image using the file path and getting image data
	unsigned char* RawImage = stbi_load(
		FilePath,
		&ImageData.w, &ImageData.h,
		&ImageData.channels,
		STBI_rgb_alpha
	);

	if (!RawImage) {
		cout << "Textures | Image couldn't be loaded." << endl;
		return false;
	}

	//create/convert the image data into an OpenGL texture
	glTexImage2D(
		GL_TEXTURE_2D, 
		0,
		GL_RGBA,
		ImageData.w, ImageData.h,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		RawImage
	);

	// generate smaller versions of the texture for when further away
	glGenerateMipmap(GL_TEXTURE_2D);

	// destroys the raw image from memory
	stbi_image_free(RawImage);

	return true;
}

void Texture::BindTexture()
{
	// check that an id has been assigned then bind the texture as the current 
	if (TextureID != 0) {
		glBindTexture(GL_TEXTURE_2D, TextureID);
	}
}

void Texture::ActivateTexture(zluint Index)
{
	if (TextureID != 0) {
		glActiveTexture(GL_TEXTURE0 + Index);
	}
}

void Texture::ResetTexture()
{
	// assign the first texture to anything without a texture
	glActiveTexture(GL_TEXTURE0);
}