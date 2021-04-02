#include "gtest/gtest.h"
extern "C"{
    #include "../matrix.h"
}

// Auxillary functions required in testing

// Checks if two values are within 1e-7(tol) of each other
bool isclose(double a,double b){
    return fabs(a-b)<1e-7;
}
// ---------------------------------------
// Tetsing function new_matrix

// Testing invalid row number
TEST(NEW_MATRIX_TEST,negativeInputRow){
     ASSERT_EXIT(new_matrix(-1,2),::testing::ExitedWithCode(EXIT_FAILURE),"In function new_matrix.Numerical argument out of domain");
}
// Testing invalid column number
TEST(NEW_MATRIX_TEST,negativeInputColumn){
    ASSERT_EXIT(new_matrix(1,-5),::testing::ExitedWithCode(EXIT_FAILURE),"In function new_matrix.Numerical argument out of domain");
}
// Testing row number greater than column number
TEST(NEW_MATRIX_TEST,rGTc){
    double at11[]={
        0,
        0,
        0
    };
    matrix mt11=array_to_matrix(3,1,at11);
    matrix mt12=new_matrix(3,1);
    ASSERT_TRUE(matrix_isequal(mt11,mt12));
}
// Testing column number is greater than row number
TEST(NEW_MATRIX_TEST,cGTr){
    double at21[]={
        0,0,0
    };
    matrix mt21=array_to_matrix(1,3,at21);
    matrix mt22=new_matrix(1,3);
    ASSERT_TRUE(matrix_isequal(mt21,mt22));
}
// Testing row number is equal to column number
TEST(NEW_MATRIX_TEST,rEQc){
    double at31[]={
        0,0,0,
        0,0,0,
        0,0,0
    };
    matrix mt31=array_to_matrix(3,3,at31);
    matrix mt32=new_matrix(3,3);
    ASSERT_TRUE(matrix_isequal(mt31,mt32));
}
// ---------------------------
// Testing function get_value

// Testing invalid row index
TEST(GET_VALUE_TEST,invRow){
    double at[]={
        9,7,34,
        45,2,12,
        7,8,12
    };
    matrix m=array_to_matrix(3,3,at);
    ASSERT_EXIT(get_value(m,3,2),::testing::ExitedWithCode(EXIT_FAILURE),"In function get_value.Dimension error.Index out of bounds\n");
}
// Testing invalid column index
TEST(GET_VALUE_TEST,invCol){
    double at[]={
        1,3,5,
        3,7,2
    };
    matrix m=array_to_matrix(2,3,at);
    ASSERT_EXIT(get_value(m,0,3),::testing::ExitedWithCode(EXIT_FAILURE),"In function get_value.Dimension error.Index out of bounds\n");
}
// Testing on lower bounds of indexes
TEST(GET_VALUE_TEST,lowerBoundindex){
    double at11[]={
        1,2,3,4,
        5,6,7,8,
        9,10,11,12
    };
    matrix mt11=array_to_matrix(3,4,at11);
    double v=get_value(mt11,0,0);
    ASSERT_TRUE(isclose(v,1));
}
// Testing on higher bounds of indexes
TEST(GET_VALUE_TEST,upperBoundindex){
    double at21[]={
        1,2,3,4,
        5,6,7,8,
        9,10,11,12
    };
    matrix mt21=array_to_matrix(3,4,at21);
    double v=get_value(mt21,2,3);
    ASSERT_TRUE(isclose(v,12));
}
// Testing on in between indexes
TEST(GET_VALUE_TEST,middleIndex){
    double at31[]={
        1,2,3,4,
        5,6,7,8,
        9,10,11,12
    };
    matrix mt31=array_to_matrix(3,4,at31);
    double v=get_value(mt31,1,2);
    ASSERT_TRUE(isclose(v,7));
}
// Testing on double values
TEST(GET_VALUE_TEST,doubleValues){
    double at41[]={
        1.39,4.57,9.21,
        7.72,6.63,5.53,
        3.12,2.87,0.56
    };
    matrix mt41=array_to_matrix(3,3,at41);
    double v=get_value(mt41,1,2);
    ASSERT_TRUE(isclose(v,5.53));
}
// ---------------------------------
// Testing function set value

// Invalid rowIndex
TEST(SET_VALUE_TEST,invRow){
    double at[]={
        3,7,
        8,12,
        67,11
    };
    matrix m=array_to_matrix(3,2,at);
    ASSERT_EXIT(set_value(&m,3,0,8),::testing::ExitedWithCode(EXIT_FAILURE),"In function set_value.Dimension error.Index out of bounds\n");
}
// Invalid Column
TEST(SET_VALUE_TEST,invCol){
    double at[]={
        4,78,
        2,12,
        98,16,
    };
    matrix m=array_to_matrix(3,2,at);
     ASSERT_EXIT(set_value(&m,3,0,8),::testing::ExitedWithCode(EXIT_FAILURE),"In function set_value.Dimension error.Index out of bounds\n");
}
// Testing on lower bounds
TEST(SET_VALUE_TEST,lowerBoundindex){
    double at41[]={
        9,8,
        7,6,
        5,4
    };
    matrix mt41=array_to_matrix(3,2,at41);
    set_value(&mt41,0,0,1);
    double at42[]={
        1,8,
        7,6,
        5,4
    };
    matrix mt42=array_to_matrix(3,2,at42);
    ASSERT_TRUE(matrix_isequal(mt41,mt42));
}
// Testing on upper bounds
TEST(SET_VALUE_TEST,upperBoundindex){
    double at41[]={
        9,8,
        7,6,
        5,4
    };
    matrix mt41=array_to_matrix(3,2,at41);
    set_value(&mt41,2,1,7);
    double at42[]={
        9,8,
        7,6,
        5,7
    };
    matrix mt42=array_to_matrix(3,2,at42);
    ASSERT_TRUE(matrix_isequal(mt41,mt42));
}
// Testing on in between indexes
TEST(SET_VALUE_TEST,middleIndex){
    double arr1[]={
        9,8,
        7,6,
        5,4
    };
    matrix m1=array_to_matrix(3,2,arr1);
    set_value(&m1,1,1,23);
    double arr2[]={
        9,8,
        7,23,
        5,4
    };
    matrix m2=array_to_matrix(3,2,arr2);
    ASSERT_TRUE(matrix_isequal(m1,m2));
}
// Testing on double values
TEST(SET_VALUE_TEST,doubleValues){
    double arr1[]={
        6.98,7.12,
        1.23,5.14
    };
    matrix m1=array_to_matrix(2,2,arr1);
    set_value(&m1,0,1,8.86);
    double arr2[]={
        6.98,8.86,
        1.23,5.14
    };
    matrix m2=array_to_matrix(2,2,arr2);
    ASSERT_TRUE(matrix_isequal(m1,m2));
}
// ------------------------------------
// Main function
int main(int argc,char *argv[]){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}