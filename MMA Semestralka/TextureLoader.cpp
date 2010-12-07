#include <iostream>
#include "TextureLoader.h"

/////////////////////////////////////////////////////////////////
////////////////////// CTGAImageLoader //////////////////////////
/////////////////////////////////////////////////////////////////

bool CTGAImageLoader::Load(char *filename, CImage *image) {

  _stream.open(filename, std::ios::in | std::ios::binary);

  if(!_stream.good())
    return false;

  /* read the TGA header to the given file */
  _stream.read((char *)(&_header.idFieldLength), 1);
  _stream.read((char *)(&_header.colorMapType), 1);
  _stream.read((char *)(&_header.imageTypeCode), 1);
  GetLittleEndianWORD(_header.colorMapIndex);
  GetLittleEndianWORD(_header.colorMapLength);
  _stream.read((char *)(&_header.colorMapEntrySize), 1);
  GetLittleEndianWORD(_header.xOrigin);
  GetLittleEndianWORD(_header.yOrigin);
  GetLittleEndianWORD(_header.imageWidth);
  GetLittleEndianWORD(_header.imageHeight);
  _stream.read((char *)(&_header.pixelDepth), 1);
  _stream.read((char *)(&_header.imageDescriptor), 1); 

 int components;

  switch(_header.pixelDepth) {
    case 24:
      components = 3;
      break;
    case 32:
      components = 4;
      break;
    default:
      std::cerr << "CTGAImageLoader::Load() - Unsupported color depth - ." << _header.pixelDepth << std::endl;
      return false;
  }

  if(_header.imageTypeCode != 2 && _header.imageTypeCode != 10) { // truecolor & truecolor encoded
    std::cerr << "CTGAImageLoader::Load() - Unsupported image type."<< std::endl; 
    _stream.close();
    return false;
  }

  if(_header.pixelDepth != 24 && _header.pixelDepth != 32) {
    std::cerr << "CTGAImageLoader::Load() - Unsupported pixel size." << std::endl;
    _stream.close();
    return false;
  }

 int rowStart, rowStop, rowAdd;

  // check the screen origin bit
  // 0 = origin in lower left-hand corner
  // 1 = origin in upper left-hand corner
  if(_header.imageDescriptor & 0x20) {
    rowStart = (int)(_header.imageHeight - 1); 
    rowStop = -1; 
    rowAdd = -1;
  }
  else {
    rowStart = 0;
    rowStop = (int)_header.imageHeight;
    rowAdd = 1;
  } 

  // skip Image Identification Field, if exists
  if(_header.idFieldLength != 0)
    _stream.seekg(_header.idFieldLength, std::ios::cur);

  image->Create((int)_header.imageWidth, (int)_header.imageHeight, (components==3) ? GL_RGB : GL_RGBA);

 BYTE red, green, blue, alpha;
 BYTE *pixel;

  if(_header.imageTypeCode == 2) { // Uncompressed

    if(rowAdd > 0) {
      for(int row=rowStart; row<rowStop; row+=rowAdd) {
        pixel = image->GetRow(row);

        for(int col=0; col<(int)_header.imageWidth; col++) {

          _stream.read((char*)&blue, 1);
          _stream.read((char*)&green, 1);
          _stream.read((char*)&red, 1);

          *pixel++ = red;
          *pixel++ = green;
          *pixel++ = blue;

          if(components == 4) {
            _stream.read((char*)&alpha, 1);
            *pixel++ = alpha;
          }
        }
      }
    }
    else { // rowAdd < 0
      for(int row=rowStart; row>rowStop; row+=rowAdd) {
        pixel = image->GetRow(row);

        for(int col=0; col<(int)_header.imageWidth; col++) {

          _stream.read((char*)&blue, 1);
          _stream.read((char*)&green, 1);
          _stream.read((char*)&red, 1);

          *pixel++ = red;
          *pixel++ = green;
          *pixel++ = blue;

          if(components == 4) {
            _stream.read((char*)&alpha, 1);
            *pixel++ = alpha;
          }
        }
      }
    }

  }
  else {  // Runlength encoded
   long totalPixels = (int)_header.imageWidth * (long)_header.imageHeight;
   BYTE tmp;
   BYTE color[4];

    int row = rowStart;
    int col = 0;
    BYTE num = 0;

    while(totalPixels) {

      _stream.read((char*)&tmp, 1);

      num = (tmp & 0x7F) + 1;

      if(tmp & 0x80) {

        _stream.read((char*)(color+B), 1);
        _stream.read((char*)(color+G), 1);
        _stream.read((char*)(color+R), 1);

        if(components == 4)
          _stream.read((char*)(color+3), 1);

        for(int M=0; M<num; M++) {
          image->SetPixel(row, col, color);

          col++;
          totalPixels--;

          if(col == (int)_header.imageWidth) {
            col  = 0;
            row += rowAdd;
          }
        }
      }
      else {
        for (int M=0; M<num; M++) {

          _stream.read((char*)(color+B), 1);
          _stream.read((char*)(color+G), 1);
          _stream.read((char*)(color+R), 1);

          if(components == 4)
            _stream.read((char*)(color+3), 1);

          image->SetPixel(row, col, color);

          col++;
          totalPixels--;

          if(col == (int)_header.imageWidth) {
            col  = 0;
            row += rowAdd;
          }
        }
      }
    }
  }

  _stream.close();

  return true;  
}


/////////////////////////////////////////////////////////////////
////////////////////// CBMPImageLoader //////////////////////////
/////////////////////////////////////////////////////////////////

#ifdef _WIN32

// based on a code created by Michal (Sirall) Cervenka (cervem5@fel.cvut.cz)
bool CBMPImageLoader::Load(char *filename, CImage *image) {
 const DWORD bitmapID = 0x4D42;
 BITMAPFILEHEADER fileHeader;
 BITMAPINFOHEADER infoHeader;

  _stream.open(filename, std::ios::in | std::ios::binary);

  if(!_stream.good())
    return false;

  _stream.read((char *)(&fileHeader), sizeof(BITMAPFILEHEADER));

  if(fileHeader.bfType != bitmapID) {
    _stream.close();
    return false;
  }

  _stream.read((char *)(&infoHeader), sizeof(BITMAPINFOHEADER));

  if(infoHeader.biBitCount != 24) {
    _stream.close();
    return false;
  }


  image->Create((int)infoHeader.biWidth, (int)infoHeader.biHeight, GL_RGB);

  unsigned int bitsPerLine = infoHeader.biWidth * 24;
  unsigned int bitsToAdd = 0;
    
  if ((bitsPerLine%32) != 0)
    bitsToAdd = 32 - (bitsPerLine%32);

  unsigned int iStride = (bitsPerLine + bitsToAdd) / 8;

  _stream.seekg(fileHeader.bfOffBits);
  _stream.read((char *)(image->GetPixels()), iStride * infoHeader.biHeight);
  _stream.close();

 BYTE *pixel, tmp;

  for(int row=0; row<infoHeader.biHeight; row++) {

     pixel = image->GetRow(row);
     for(int col=0; col<infoHeader.biWidth; col++) {

       tmp = pixel[R]; // loaded as BGR -> correct to RGB
	   pixel[R] = pixel[B];
	   pixel[B] = tmp;

	   pixel += 3;
    }
  }

  return true;
}


#endif


#ifdef _WIN32

char *rindex(char *a, int c) {
	return(strrchr(a, c));
}

#endif


/////////////////////////////////////////////////////////////////
/////////////////////// CTextureLoader //////////////////////////
/////////////////////////////////////////////////////////////////

void CTextureLoader::GetExtension(char *filename, char *extension) {
 char *str;
 int i;

  str = rindex(filename, '.'); // last occurence of the dot in the filename
  if(str == NULL) {
    strcpy(extension, "");
    return;
  }

  /* copy the extension */
  i = 0;
  str++;
  while(*str != '\0')
    extension[i++] = *str++;
  extension[i] = *str;

  /* to lower case */
  i = 0;
  while(extension[i] != '\0') {
    extension[i] = (isupper(extension[i]) != 0) ? tolower(extension[i]) : extension[i];
    i++;
  }

  return;
}


bool CTextureLoader::Load(char *filename, CImage* &texture) {
 char extension[128];
 CImageLoader *loader;

  texture = NULL;

  GetExtension(filename, extension);

  loader = FindImageLoader(extension);
  if(loader != NULL) {
    texture = new CImage();
    if(loader->Load(filename, texture) == false) {
      delete texture;
      texture = NULL;
    }
  }

  if(texture == NULL)
    return false;

  return true;
}


void CTextureLoader::RegisterLoader(char *extension, CImageLoader *loader) {
 SLoaderListEntry *entry = new SLoaderListEntry;

  strcpy(entry->extension, extension);
  entry->loader = loader;
  _loaderList.push_back(entry);
}


CImageLoader *CTextureLoader::FindImageLoader(char *extension) {

  std::list<SLoaderListEntry *>::iterator it = _loaderList.begin();
  while(it != _loaderList.end()) {
    if(strncmp((*it)->extension, extension, 128) == 0)
      return (*it)->loader;
    it++;
  }

 return NULL;
}
