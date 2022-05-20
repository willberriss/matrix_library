#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "matlib.h"

TEST(MainTest, GivenMatrixA_WhenMatrixOfMinorsIsCalculated_ThenTheResultMatchesTheGroundTruth) 
{ 
    Matrix<3, 3, int> A;
    std::string matrix_A("../matrix_A.txt");
    A.read_from_file(matrix_A);

    // Read in the ground truth (GT) that was calculated by hand
    Matrix<3, 3, int> GTMinors;
    std::string matrix_of_minors("../matrix_of_minors_of_A.txt");
    GTMinors.read_from_file(matrix_of_minors);

    Matrix<3, 3, int> AMinors;
    std::cout << "Calculating matrix of minors of A" << std::endl;
    AMinors = A.minors33();

    ASSERT_EQ(AMinors, GTMinors);
}

TEST(MainTest, GivenMatrixOfMinors_WhenMatrixofMinorsIsMultipliedByCheckboard_ThenResultMatchesTheMatrixOfCofactorsGroundTruth) 
{ 
    Matrix<3, 3, double> GTMinors;
    std::string matrix_of_minors("../matrix_of_minors_of_A.txt");
    GTMinors.read_from_file(matrix_of_minors);
    
    Matrix<3, 3, double> Checkerboard;
    std::string checkerboard_matrix("../checkerboard_matrix.txt");
    Checkerboard.read_from_file(checkerboard_matrix);

    Matrix<3, 3, double> ACofactors{GTMinors};
    ACofactors.applyCheckerboard(Checkerboard);

    // Read in the ground truth (GT) Cofactors matrix, as calculated by hand
    Matrix<3, 3, double> GTCofactors;
    std::string matrix_of_cofactors("../matrix_of_cofactors_of_A.txt");
    GTCofactors.read_from_file(matrix_of_cofactors);
    
    ASSERT_EQ(ACofactors, GTCofactors);
}

TEST(MainTest, GivenMatrices_A_and_x_ThenMultiplying_A_and_x_gives_b) 
{
    Matrix<3, 3, double> A;
    std::string matrix_A("../matrix_A.txt");
    A.read_from_file(matrix_A);

    Matrix<3, 1, double> x;
    std::string vector_x("../vector_x.txt");
    x.read_from_file(vector_x);
    
    Vector<3, double> b;
    std::string vector_b("../vector_b.txt");
    b.read_from_file(vector_b);
   
    Vector<3, double> myb;
    myb = A * x;

    ASSERT_EQ(myb, b);
}

TEST(MainTest, GivenMatrixA_WhenTheAdjugateMatrixIsCalculated_ThenTheResultMatchesTheGroundTruth) 
{
    Matrix<3, 3, double> A;
    std::string matrix_A("../matrix_A.txt");
    A.read_from_file(matrix_A);
    
    Matrix<3, 3, double> GTAdjugate;
    std::string adjugate_of_a("../adjugate_of_A.txt");
    GTAdjugate.read_from_file(adjugate_of_a);
    
    Matrix<3, 3, double> Adjugate;
    Adjugate = A.adjugate();

    ASSERT_EQ(Adjugate, GTAdjugate);
}

TEST(MainTest, GivenMatrixA_WhenItsInverseIsCalculated_ThenItShouldMatchTheGroundTruth) 
{ 
    Matrix<3, 3, double> A;
    std::string matrix_A("../matrix_A.txt");
    A.read_from_file(matrix_A);
    
    Matrix<3, 3, double> GTInverseA;
    std::string inverse_A("../inverse_of_matrix_A.txt");
    GTInverseA.read_from_file(inverse_A);
    
    Matrix<3, 3, double> invA;
    invA = inverse(A);
    
    ASSERT_EQ(invA, GTInverseA);
}

TEST(MainTest, GivenMatrixAandItsInverseThenMultplyingThemProducesTheIdentityMatrix) 
{ 
    Matrix<3, 3, double> A;
    std::string matrix_A("../matrix_A.txt");
    A.read_from_file(matrix_A);
    
    Matrix<3, 3, double> Identity;
    std::string identity_matrix("../identity_matrix.txt");
    Identity.read_from_file(identity_matrix);
    
    Matrix<3, 3, double> invA;
    invA = inverse(A);

    Matrix<3, 3, double> myIdentity;
    myIdentity = A.dotProductRounded(inverse(A));

    ASSERT_EQ(myIdentity, Identity);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
