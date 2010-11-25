
#ifndef __COMMON_H
#define __COMMON_H

#ifdef _WIN32
#include <windows.h>
#include <ctype.h>
#endif

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>


#ifndef MAXLONG
  #define MAXLONG     0x7fffffff
#endif

#define  PERSPECTIVE_Z_NEAR   0.1f
#define  PERSPECTIVE_Z_FAR  100000.0f

// data types
#ifndef _WIN32

  #ifndef BYTE
    #define BYTE unsigned char
  #endif

  #ifndef WORD
    #define WORD unsigned short
  #endif

#endif

// coordinates to access vectors
#define X 0
#define Y 1
#define Z 2
#define W 3

// coordinates to access colors
#define R 0
#define G 1
#define B 2
#define A 3

// M_PI is not defined at some platforms
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define INV_4_M_PI 0.07957747154594767
#define SQRT_2 1.4142135623730951
#define INV_SQRT_2 0.70710678118654746


class CMath {

public:

  // minimum function
  inline static int Min(int a, int b) {
    return (a < b) ? a : b;
  }

  inline static float Min(float a, float b) {
    return (a < b) ? a : b;
  }

  inline static double Min(double a, double b) {
    return (a < b) ? a : b;
  }

  // maximum function
  inline static int Max(int a, int b) {
    return (a > b) ? a : b;
  }

  inline static float Max(float a, float b) {
    return (a > b) ? a : b;
  }

  inline static double Max(double a, double b) {
    return (a > b) ? a : b;
  }

  // clamp value by minimum and maximum
  inline static float Clamp(float value, float min, float max) {
    return ( (value > max) ? max : ( (value < min) ? min : value ) );
  }

  inline static double Clamp(double value, double min, double max) {
    return ( (value > max) ? max : ( (value < min) ? min : value ) );
  }

  inline static int Clamp(int value, int min, int max) {
    return ( (value > max) ? max : ( (value < min) ? min : value ) );
  }

  // clamp value by maximum
  inline static float ClampMax(float value, float max) {
    return ( (value > max) ? max : value );
  }

  inline static int ClampMax(int value, int max) {
    return ( (value > max) ? max : value );
  }
  
  inline static double ClampMax(double value, double max) {
    return ( (value > max) ? max : value );
  }

  // clamp value by minimum
  inline static double ClampMin(double value, double min) {
    return ( (value < min) ? min : value );
  }
  
  inline static float ClampMin(float value, float min) {
    return ( (value < min) ? min : value );
  }
  
  inline static int ClampMin(int value, int min) {
    return ( (value < min) ? min : value );
  }

  // sign function
  inline static int Sign(double value) {
    return ( (value > 0.0) ? 1 : ( (value < 0.0) ? -1 : 0 ) );
  }

  inline static int Sign(float value) {
    return ( (value > 0.0f) ? 1 : ( (value < 0.0f) ? -1 : 0 ) );
  }

  inline static int Sign(int value) {
    return ( (value > 0) ? 1 : ( (value < 0) ? -1 : 0 ) );
  }

 // constants for conversion radians <=> degrees
 static double _deg2rad;
 static double _rad2deg;

};


class CError {

public:

  CError() {}
  ~CError() {}

  static void FatalError(char *str) {
    std::cerr << str << std::endl;
    exit(1);
  }

  static void Warning(char *str) {
    std::cerr << str << std::endl;
  }

};

#endif // __COMMON_H
