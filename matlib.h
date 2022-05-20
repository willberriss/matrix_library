#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

template<typename T> 
T inverse(const T& m)
{
    return m.myinverse();
}

bool equals(double a, double b, double epsilon = 0.000001)
{
    return std::abs(a-b) < epsilon;
}

template<size_t num_rows, typename T>
class Vector;

template<size_t num_rows, size_t num_columns, typename T>
class Matrix
{
public:

    T _M[num_rows][num_columns];

    bool operator==(const Matrix &rhs) const
    {
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_columns; ++c)
            {
                if ( ! equals(_M[r][c], rhs._M[r][c]) )
                {
                    return false;
                }
            }
        }
        return true;
    }
   
    // Perform the dot product
    // Dimensions: (r1 by c1) . (r2 by c2) => (r1 by c2) == (r by c)
    //
    // Hack: Works ONLY for the following dimensions of matrices:
    // (3 by 1) <= (3 by 3) . (3 by 1)
    Vector<num_rows, T> operator*(const Matrix<num_rows, 1, T> &rhs) const
    {
        Vector<num_rows, T> dotProduct;

        // Loop through each element in the resultant (r by c) matrix
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < 1; ++c)
            {
                dotProduct._M[r][c] = 0;

                // Do the dot product for result element [r][c]
                for (int r1 = 0; r1 < num_rows; ++r1)
                {
                    dotProduct._M[r][c] += _M[r][r1] * rhs._M[r1][c];
                }
            }
        }
        return dotProduct;
    }
    
    // For unittest usage
    Matrix dotProductRounded(const Matrix &M) const
    {
        Matrix dotProduct;

        // Loop through each element in the resultant (r by c) matrix
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_columns; ++c)
            {
                dotProduct._M[r][c] = 0;

                // Do the dot product for result element [r][c]
                for (int r1 = 0; r1 < num_rows; ++r1)
                {
                    dotProduct._M[r][c] += _M[r][r1] * M._M[r1][c];
                }
                dotProduct._M[r][c] = std::round(dotProduct._M[r][c]);
            }
        }
        return dotProduct;
    }
    
    friend std::ostream& operator<<(std::ostream &output, Matrix &M)
    {
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_columns; ++c)
            {
                output << M._M[r][c] << "\t";
            }
            output << std::endl;
        }
        return output;
    }
    
    void applyCheckerboard(const Matrix &checkerboard)
    {
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_columns; ++c)
            {
                _M[r][c] = checkerboard._M[r][c] * _M[r][c];
            }
        }
    }

    bool read_from_file(std::string filename)
    {
        std::ifstream f(filename);
        const int num_elements = num_rows * num_columns;
    
        if ( ! f.is_open() ) 
        {
            std::cerr << "Could not open file: '"
                << filename << "'" << std::endl;
            return false;
        }

        T value;
        int n {0};
        while (f >> value) 
        {
            int row = n / (num_columns);
            int col = n  % (num_columns);
            ++n;
            _M[row][col] = value;
        }
        f.close();

        return true;
    }

    Matrix myinverse() const
    {
        Matrix Cofactors = cofactors33();

        // Calculate determinant of matrix A
        double determinant = determinant33(Cofactors);

        Matrix Adjugate = adjugate();

        Matrix invM;
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_columns; ++c)
            {
                invM._M[r][c] = (Adjugate._M[r][c])/determinant;
            }
        }
        return invM;
    }
    
    Matrix adjugate() const
    {
        Matrix Cofactors = cofactors33(); // Unfortunate recalculation (TODO)
        
        // Multiply adjugate of matrix A by 1/determinant to obtain 
        // the inverse of matrix A
        Matrix Adjugate = Cofactors.transposeMM();

        return Adjugate;
    }

    // transpose an M by M matrix
    Matrix transposeMM() const
    {
        Matrix Transpose;
        for (int r = 0; r < num_rows; ++r)
        {
            for (int c = 0; c < num_columns; ++c)
            {
                 Transpose._M[r][c] = _M[c][r];
            }
        }
        return Transpose;
    }

    // Hacking for now - works only for vector (num_columns=1)
    std::string transpose() const
    {
        assert(num_columns == 1);

        std::ostringstream oss;
        oss.precision(2);
        for (int row = 0; row < num_rows; ++row)
        {
            if (row > 0)
            {
                oss << std::fixed << std::setprecision(1) 
                    << ", " << _M[row][0];
            }
            else
            {
                oss << std::fixed << std::setprecision(1) 
                    << _M[row][0];
            }
        }
        return oss.str();
    }

    // Works only for 3x3 matrices
    Matrix minors33() const
    {
        assert(num_rows == 3);
        assert(num_columns == 3);
        
        Matrix<2, 2, T> Submatrix22;
        Matrix<3, 3, T> Minors;
        for (int r = 0; r < 3; ++r)
        {
            for (int c = 0; c < 3; ++c)
            {
                Submatrix22 = this->submatrix22(r, c);
                Minors._M[r][c] = Submatrix22.determinant22();
            }
        }

        return Minors;
    }
    
    // Works only for 3x3 matrices
    Matrix cofactors33() const
    {
        assert(num_rows == 3);
        assert(num_columns == 3);

        Matrix Cofactors = minors33();

        Matrix Checkerboard;
        std::string checkerboard_matrix("../checkerboard_matrix.txt");
        Checkerboard.read_from_file(checkerboard_matrix);

        Cofactors.applyCheckerboard(Checkerboard);
        return Cofactors;
    }

    // rSKip means skip this row, cSkip means skip this column
    // Works only for 3x3 matrices. 
    Matrix<2, 2, T> submatrix22(int rSkip, int cSkip) const
    {
        assert(num_rows == 3);
        assert(num_columns == 3);

        // Go through this matrix (3x3) and set r and c of submatrix (2x2)
        // by skipping row rSkip and column cSkip of the original matrix
        Matrix<2, 2, T> Submatrix22;
        int r=0;
        for (int row = 0; row < 3; ++row)
        {
            if (row == rSkip)
            {
                // skip row
            }
            else
            {
                int c=0;
                for (int column = 0; column < 3; ++column)
                {
                    if (column == cSkip)
                    {   
                        // skip column
                    }
                    else
                    {
                        Submatrix22._M[r][c] = _M[row][column];
                        ++c;
                    }
                }
                ++r;
            }
        }
        return Submatrix22;
    }
    
    // Works only for 2x2 matrices. 
    T determinant22() const
    {
        assert(num_rows == 2);
        assert(num_columns == 2);
        T det = (_M[0][0] * _M[1][1]) - (_M[0][1] * _M[1][0]);
        return det;
    }
    
    // Calculate determinant using matrix of cofactors
    // Works only for 3x3 matrices. 
    T determinant33(const Matrix &Cofactors) const
    {
        assert(num_rows == 3);
        assert(num_columns == 3);

        T det{0}; 
        for (int c=0; c < num_columns; ++c)
        {
            det += _M[0][c] * Cofactors._M[0][c];
        }

        return det;
    }

};

template<size_t num_rows, typename T>
class Vector : public Matrix<num_rows, 1, T> {};

