#ifndef __IMAGE_H
#define __IMAGE_H

#include <list>
#include <GL/glut.h>
#include "common.h"


class CImage {

public:

  CImage(void) : _pixels(NULL) {
  }

  // constructor - creates an image with given resolution and format
  CImage(int width, int height, GLenum format = GL_RGB) : _pixels(NULL) {
    Create(width, height, format);
  }

  // creates an empty image (uninitialized)
  void Create(int width = 320, int height = 200, GLenum format = GL_RGB);

  // deletes all image pixels from memory
  void Destroy(void) {
    if(_pixels != NULL) {
      delete[] _pixels;
      _pixels = NULL;
    }
  }

  // desctructor
  virtual ~CImage() {
    Destroy();
  }

  // returns color of a given pixel
  void GetPixel(int row, int col, BYTE *color) {
   BYTE *pixelComponent = _pixels + row*_rowWidth + col * _components;

    for(int comp=0; comp<_components; comp++)
      color[comp] = *pixelComponent++;
  }

  // set the color of given pixel
  void SetPixel(int row, int col, const BYTE *color) {
   BYTE *pixelComponent = _pixels + row*_rowWidth + col * _components;

    for(int comp=0; comp<_components; comp++)
      *pixelComponent++ = color[comp];
  }

  // returns pointer to the beginning of a given row
  BYTE *GetRow(int row) {
    return _pixels + row*_rowWidth;
  }

  // returns number of components per pixel
  int GetComponents(void) {
    return _components;
  }

  // returns image width
  int GetWidth(void) {
    return _width;
  }

  // return image height
  int GetHeight(void) {
    return _height;
  }

  // returns image format - GL_RGB or GL_RGBA
  GLenum GetFormat(void) {
    return _format;
  }

  // returns pointer to pixel data
  BYTE *GetPixels(void) {
    return _pixels;
  }

private:

 int _width;
 int _height;
 int _components;
 GLenum _format;

 BYTE *_pixels;

 long int _rowWidth; // in bytes

};  /* class CImage */

#endif /* of __IMAGE_H */
