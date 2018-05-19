#include "LoadImageFile.h"
#include "../Utilities/FileIO/FileUtilities.h"
#include "../Utilities/PrintFunctions.h"
#include "../Libraries/stb_image.h"
#include "../Libraries/lodepng/lodepng.h"
#include "../Utilities/ImageHelpers.h"

unsigned char* QwerkE_stb_image_loadImage(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically)
{
	unsigned char* pngbuffer = 0;
	int nrChannels = 4;

	// Note: must cast values to signed ints. Could lead to errors.
	// buffer file data
	pngbuffer = stbi_load(path, (signed int*)&imageWidth, (signed int*)&imageHeight, &nrChannels, channels); // can set 0 to desired comp (rgba == 4)
	// if (pngbuffer == nullptr || strcmp((const char*)pngbuffer, "") == 0)
	if(!pngbuffer)
	{
		stbi_image_free(pngbuffer);
		return nullptr; // null handle
	}

	if (nrChannels == 3)
		channels = GL_RGB;
	else if(nrChannels == 4)
		channels = GL_RGBA;

	// cleanup
	//unsigned char* r = new unsigned char[sizeof(pngbuffer)];
	//memcpy_s(r, sizeof(r), pngbuffer, sizeof(pngbuffer));
	//stbi_image_free(pngbuffer);
	return pngbuffer;
}

unsigned char* QwerkE_lodepng_loadImage(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically)
{
	unsigned char* pngbuffer = 0;
	long filesize;
	unsigned int result = -1;
	unsigned char* filebuffer = (unsigned char*)LoadCompleteFile(path, &filesize);
	if (filebuffer != NULL) // decode file data
	{
		result = lodepng_decode32(&pngbuffer, imageWidth, imageHeight, filebuffer, filesize);
	}

	if (result != 0) // error decoding image data
	{
		free(pngbuffer);
		delete[] filebuffer;
		return nullptr; // exit
	}

	channels = GL_RGBA;

	delete[] filebuffer;
	return pngbuffer;
}

//char* QwerkE_soil_loadImage()
//{
//	// TODO:
//}