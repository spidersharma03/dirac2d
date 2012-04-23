#include <iostream>

#include "Dirac2D/Dirac2D.h"
USE_NAMESPACE_DIRAC2D


int main (int argc, char * const argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
	
	Vector2f v(1,2);
	Vector2f v1(1,2);
	
	v *= v1;
	v = v + v1;
    return 0;
}
