#include <qapplication.h>
#include "viewer.h"
#include "skeleton.h"

int main(int argc, char** argv)
{
    Skeleton s;
    s.loadFromFile("data/bonhomme.txt");

    std::cout << s << std::endl;

    return 0;
}
