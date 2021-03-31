/*Description of the project matrix.h
Author: Prithvi

BUGS: 
none known at present

Note:  Some functions have copies of matrices . This is because even though structures are passed which 
        is pass by value still the operations take place at original memeory location as one of the element 
        of the structure is a pointer (hence the address is also copied resulting in the operations at 
        original memory locations.)


#TODO:
write tests for all the functions

*/
//----------------------------------------------------------
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<math.h>
#include<errno.h>
#include<string.h>


#define EDIM 9001     //Dimension error
#define EOUTMEM 9002  //Out of memory
#define EDIMMISMATCH 9003  //Dimensions mismatch
#define NOSOL 9004 //No solution exists

typedef struct 
{
    double *element;
    int row;
    int column;
}matrix;

static void error(const char *msg);
int matrix_rowcount(matrix m);
int matrix_columncount(matrix m);
void show_matrix(matrix m);
matrix new_matrix(int row,int column);
double get_value(matrix m,int row,int column);
void set_value(matrix *m,int row,int column,double value);
void set_all_value(matrix *m,double value);
void delete_matrix(matrix *m);
matrix array_to_matrix(int row,int column,double *values);
matrix copy_matrix(matrix m);
static double KahanSum(matrix m);
static int check_tolerance(matrix m1,matrix m2,double tol);
double matrix_dot(matrix m1,matrix m2);
int matrix_isequal(matrix m1, matrix m2);
matrix submatrix(matrix m,int row_start,int column_start,int row_end,int column_end);
matrix add_matrix(matrix m1,matrix m2);
matrix sum_matrix(int args_count,matrix m,...);
matrix multiply_matrix(matrix m1,matrix m2);
matrix transpose_matrix(matrix m);
matrix power_matrix(matrix m,int pow);
static matrix upp_triangular(matrix m);
double determinant(matrix m);
matrix concat_side(matrix m1, matrix m2);
matrix concat_down(matrix m1,matrix m2);
int rank(matrix m);
matrix inverse(matrix m);
matrix solvematrix(matrix coeff_mat,matrix val_mat);
matrix gauss_siedel(matrix coeff,matrix val_mat,double tol,int iterations);
