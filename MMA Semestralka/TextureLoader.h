
#ifndef __TEXTURELOADER_H
#define __TEXTURELOADER_H

#include <string.h>
#include <fstream>
#include <list>
#include "common.h"
#include "Image.h"

class CImageLoader {

public:

  CImageLoader() {}

  virtual ~CImageLoader() {}

  virtual bool Load(char *filename, CImage *image) = 0;

  /* get a word from the file(stream) using little-endian byte order */
  void GetLittleEndianWORD(WORD& word) {
   BYTE low, high;

    //    _stream >> low >> high;
    _stream.read((char *)(&low), 1);
    _stream.read((char *)(&high), 1);

    word = low + (((WORD)high) << 8);
  }

protected:

 std::fstream _stream;

};


class CTGAImageLoader : public CImageLoader {

public:

  CTGAImageLoader() : CImageLoader() {}

  virtual bool Load(char *filename, CImage *image);

protected:

/* TGA 1.0 File Header */
typedef struct _TGAHeaderH {
 BYTE   idFieldLength;       /* Image ID Field Length      */
 BYTE   colorMapType;        /* Color Map Type             */
 BYTE   imageTypeCode;       /* Image Type                 */
 
 /* Color Map Specification */
 WORD   colorMapIndex;       /* First Entry Index          */
 WORD   colorMapLength;      /* Color Map Length           */
 BYTE   colorMapEntrySize;   /* Color Map Entry Size       */
 
 /* Image Specification */
 WORD   xOrigin;             /* X-origin of Image          */
 WORD   yOrigin;             /* Y-origin of Image          */
 WORD   imageWidth;          /* Image Width                */
 WORD   imageHeight;         /* Image Height               */
 BYTE   pixelDepth;          /* Pixel Depth                */
 BYTE   imageDescriptor;     /* Image Descriptor           */

} STGAHeader;

 STGAHeader _header;

}; /* CTGAImageLoader */


#ifdef _WIN32

class CBMPImageLoader : public CImageLoader {

public:

  CBMPImageLoader() : CImageLoader() {}

  virtual bool Load(char *filename, CImage *image);

}; /* CBMPImageLoader */

#endif


class CTextureLoader {

public:

  CTextureLoader() {
    RegisterLoader("tga", new CTGAImageLoader());
#ifdef _WIN32
    RegisterLoader("bmp", new CBMPImageLoader());
#endif
  }

  ~CTextureLoader() {}

  bool Load(char *filename, CImage* &texture);

  void RegisterLoader(char *extension, CImageLoader *loader);

  CImageLoader *FindImageLoader(char *extension);

  void GetExtension(char *filename, char *extension);

private:

 typedef struct _LoaderListEntryH {
   char extension[128];
   CImageLoader *loader;
 } SLoaderListEntry;

 std::list<SLoaderListEntry *> _loaderList;

};  /* class CTextureLoader */

#endif  /* of __TEXTURELOADER_H */
