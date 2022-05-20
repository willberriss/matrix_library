#include "matlib.h"

template<size_t num_rows, size_t num_columns, typename T>
std::string transpose(Matrix<num_rows, num_columns, T> x)
{
    return x.transpose();
} 

int main (int argc, char *argv[])
{
    std::cout << "Matrix library demonstration\n" << std::endl;

    Matrix<3, 3, double> A;
    std::string filename{"../matrix_A.txt"};
    std::cout << "Opening file: " << filename << std::endl;
    A.read_from_file(filename);
    std::cout << A << std::endl;

    Vector<3, double> b;
    filename = "../vector_b.txt";
    std::cout << "Opening file: " << filename << std::endl;
    std::string vector_b(filename);
    b.read_from_file(vector_b);
    std::cout << b << std::endl;
    
    Vector<3, double> x;
    x = inverse(A) * b;
    
    std::cout << "solution: " << transpose(x) << std::endl;

    return 0;
}

