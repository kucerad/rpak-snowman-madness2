
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"


CVector3D Normalize(const CVector3D& vectA) {
 double length = Length(vectA);
 
  if(length < EPSD) {
    std::cerr << "Trying to normalize zero length vector ("
         << vectA._vector[X] << ", " << vectA._vector[Y] << ", "
         << vectA._vector[Z] << ", " << vectA._vector[W] << ")"
         << std::endl;
    exit(1);       
  }
 
  return vectA / length;
}


// Overload << operator for C++-style output
std::ostream& operator<< (std::ostream& stream, const CVector3D& vectA) {

  stream << "[_CVector3D_]" << std::endl;
    
  return stream << "(" << vectA._vector[X] << ", " << vectA._vector[Y] << ", " << vectA._vector[Z] << ", " << vectA._vector[W] << ")" << std::endl << std::endl;
}
