#include <iostream>
#include "floodfill.h"

using namespace std;

int main()
{
    int arrayx[4][4] = { {-1,0,0,0},
                        {0,-1,0,0},
                        {0,0,0,0} };
   floodfill yarab;
   yarab.initialize(arrayx);

    cout << "Hello world!" << endl;
   yarab.compute(3,3);

    return 0;
}
