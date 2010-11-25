
#include "Image.h"


void CImage::Create(int width, int height, GLenum format) {

  if(_pixels != NULL)
    Destroy();

  _width = width;
  _height = height;
  _format = format;

  switch(_format) {
    case GL_RGB:
      _components = 3;
      break;
    case GL_RGBA:
      _components = 4;
      break;
    default:
      _components = 3;
  }

  _rowWidth = _width * _components;

  _pixels = (BYTE *) new BYTE[_height*_rowWidth];

}

