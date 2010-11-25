//-----------------------------------------------------------------------------
//  [GSY] Utilities - common functions
//  15/03/2008
//-----------------------------------------------------------------------------
//  Controls: 
//-----------------------------------------------------------------------------


#ifndef __GSY_UTILS_H__
#define __GSY_UTILS_H__

#include <stdio.h>


const char* readFile(const char* fileName) 
{
	FILE* fin;
	char* pText = NULL;

	int count = 0;

	if (fileName != NULL) 
	{
		fin = fopen(fileName,"rt");

		if (fin != NULL) 
		{
			fseek(fin, 0, SEEK_END);
			int fileSize = ftell(fin);
			rewind(fin);

			if (fileSize >= 0) 
			{
				pText = new char[fileSize + 1];
				fileSize = fread(pText, sizeof(char), fileSize, fin);
				pText[fileSize] = '\0';
			}
			fclose(fin);
		}
		else
		{
			return fileName;
		}
	}

	return pText;
}


GLuint createSimpleTexture(int width, int height)
{
	GLuint texId = 0;
	unsigned char* pData = new unsigned char[width*height*4];

	if (pData)
	{
		unsigned char* ptr = pData; 
    
		for (int h = 0; h < height; h++) 
		{              
			for (int w = 0; w < width; w++) 
			{
				unsigned char c=((((w&0x88)==0)^((h&0x88))==0)) * 255;

				*ptr++ = (unsigned char) c; 
				*ptr++ = (unsigned char) c;
				*ptr++ = (unsigned char) c;
				*ptr++ = (unsigned char) 255;
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
		glEnable(GL_TEXTURE_2D);

		delete [] pData;
	}

	return texId;
} 

#endif __GSY_UTILS_H__
