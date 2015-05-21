#include <qapplication.h>
#include "viewer.h"
#include "vect3.h"

int main(int argc, char** argv)
{
    Vect3 v(1.0, -2.0, 3);
    Vect3 v2(1.0, 5.0);

    std::cout << v * v2 << std::endl;

    return 0;
}
