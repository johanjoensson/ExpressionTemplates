#include <matrix_reduced.h>
#include <iostream>

int main()
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    auto n = -m;
    std::cout << "n[0, 0] = " << n[0, 0] << "\n";
    return 0;
}

