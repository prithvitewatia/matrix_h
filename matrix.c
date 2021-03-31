#include "matrix.h"

//writes a message to standard error and exits the program
static void error(const char *func_name)
{
    fprintf(stderr,"In function %s.",func_name);
    if(errno=EDIM){
        fprintf(stderr,"Dimension error.Index out of bounds\n");
    }
    else if(errno==EOUTMEM){
        fprintf(stderr,"Out of memory\n");
    }
    else if(errno==EDIMMISMATCH){
        fprintf(stderr,"Dimensions do not match\n");
    }
    else if(errno==NOSOL){
        fprintf(stderr,"No solution exists\n");
    }
    else{
        fprintf(stderr,"%s\n",strerror(errno));
    }
    exit(errno);
}

//Returns the number of rows in the matrix
//Can also be accessed by m.row
int matrix_rowcount(matrix m)
{
    return m.row;
}

//Returns the number of columns in matrix.
//Can also be accessed by m.column
int matrix_columncount(matrix m)
{
    return m.column;
}

//Displays the matrix. Field width is 14 and precision is 4.
void show_matrix(matrix m)
{
    for(int i=0;i<m.row;i++)
    {
        for(int j=0;j<m.column;j++)
        {
            int pos=i*m.column+j;
            printf("%14.4f",m.element[pos]);
        }
        printf("\n");
    }
}

//Returns a NULL (zero) matrix of dimension row x column
matrix new_matrix(int row,int column)
{
    if((row<=0) || (column<=0)){
        errno=EDOM;
        error(__func__);
    }
    else
    {
        matrix m;
        m.row=row;
        m.column=column;
        m.element=malloc(sizeof(double)*row*column);
        if (m.element!=NULL)
        {
            for(int i=0;i<row;i++)
            {
                for(int j=0;j<column;j++)
                {
                    int pos=i*column+j;
                    m.element[pos]=0.0;
                }
            }
            return m;
        }
        else
        {
            errno=EOUTMEM;
            error(__func__);
        }
         
    }
}

//Retrieves the value from matrix from indexes specified in row and column
double get_value(matrix m,int row,int column)
{
    if((row<0)||(row>=m.row)||(column<0)||(column>m.column)){
        errno=EDIM;
        error(__func__);
    }
    else
    {
        int pos=row*m.column+column;
        double val=m.element[pos];
        return val;
    }
    
}

//Sets the value of a matrix at position specified by indexes row and column.
void set_value(matrix *m,int row,int column,double value)
{
    if((row<0)||(row>=m->row)||(column<0)||(column>=m->column)){
        errno=EDIM;
        error(__func__);
    }
    int pos=row*m->column+column;
    m->element[pos]=value;    
}

//Sets all the values of matrix to the specified value
void set_all_value(matrix *m,double value)
{
    for(int i=0;i<m->row;i++)
    {
        for(int j=0;j<m->column;j++)
        {
            m->element[i*m->column+j]=value;
        }
    }
}


//Deletes a matrix and frees its memory
void delete_matrix(matrix *m)
{
    free(m->element);
    m->element=NULL;   
}

//Converts an array to a matrix in row major form
//Ensure that number of values in the array matches row x column
matrix array_to_matrix(int row,int column,double *values)
{
    if((row<=0)||(column<=0)){
        errno=EDOM;
        error(__func__);
    }
    matrix m=new_matrix(row,column);
    for(int i=0;i<row*column;i++)
    {
        int r=i/column;
        int c=i%column;
        set_value(&m,r,c,values[i]);
    }
    return m;
}

//Returns a deep copy of a matrix
matrix copy_matrix(matrix m)
{
    matrix deep_copy=new_matrix(m.row,m.column);
    for(int i=0;i<m.row;i++)
    {
        for(int j=0;j<m.column;j++)
        {
            set_value(&deep_copy,i,j,get_value(m,i,j));
        }
    }
    return deep_copy;
}

/*Kahan summation algorithm also known as compensated summation
significantly reduces the numerical error in the total obtained 
by adding a sequence of finite precision floating point numbers
compared to the obvious approach*/ 
static double KahanSum(matrix m)
{
    double sum=0;
    double c=0;

    for(int i=0;i<m.column;i++)
    {
        double y=get_value(m,0,i)-c;
        double t=sum+y;
        c=(t-sum)-y;
        sum=t;
    }
    return sum;
}

//Checks that all the values of both the matrices lie close to each other as specified by the tolerance.
static int check_tolerance(matrix m1,matrix m2,double tol)
{
    for(int i=0;i<m1.column;i++)
    {
        double a=get_value(m1,0,i);
        double b=get_value(m2,0,i);
        if(fabs(a-b)>tol*fabs(b))
        {
            return 0;
        }
    }
    return 1;
}

//returns a dot product of two vectors
double matrix_dot(matrix m1,matrix m2)
{
    if((m1.column!=1)||(m2.column!=1)||(m1.column!=m2.column)){
        errno=EDIMMISMATCH;
        error(__func__);
    }
    matrix temp_product=new_matrix(1,m1.column);
    for(int i=0;i<m1.column;i++)
    {
        double tp=get_value(m1,0,i)*get_value(m2,0,i);
        set_value(&temp_product,0,i,tp);
    }
    double dot_product=KahanSum(temp_product);
    delete_matrix(&temp_product);
    return dot_product;
}

//Checks if two matrices are equal
//returns 1 for true and 0 for false
int matrix_isequal(matrix m1, matrix m2)
{
    if(m1.row!=m2.row||m1.column!=m2.column)
    {
        return 0;
    }
    for(int i=0;i<m1.row;i++)
    {
        for (int j = 0; j < m1.column; j++)
        {
            if(fabs(get_value(m1,i,j)-get_value(m2,i,j))>1e-7)
                return 0;
        }
    }
    return 1;
}

//Returns a submatrix of m with dimension from row start to row end 
//and column start to column end (both inclusive)
//Equivalent of python's numpy M[row_start:row_end+1,column_start:column_end+1]
matrix submatrix(matrix m,int row_start,int column_start,int row_end,int column_end)
{
    if((row_start<0)||(column_start<0)
        ||(row_end>=m.row)||(column_end>=m.column)
        ||(row_end-row_start+1)<=0
        ||(column_end-column_start+1)<=0){
            errno=EDIM;
            error(__func__);
    }
    else
    {
        int row_count=row_end-row_start+1;
        int col_count=column_end-column_start+1;
        matrix mini_mat=new_matrix(row_count,col_count);
        for(int i=0;i<row_count;i++)
        {
            for(int j=0;j<col_count;j++)
            {
                int mini_pos=i*mini_mat.column+j;
                int m_pos=(row_start+i)*m.column+(column_start+j);
                mini_mat.element[mini_pos]=m.element[m_pos];
            }
        }
        return mini_mat;
    }
}

//Adds two matrices of same dimensions and returns the matrix.
matrix add_matrix(matrix m1,matrix m2)
{
    if((m1.row!=m2.row)||(m1.column!=m2.column)){
        errno=EDIMMISMATCH;
        error(__func__);
    }
    else
    {

        matrix result=new_matrix(m1.row,m1.column);
        for(int i=0;i<m1.row;i++)
        {
            for(int j=0;j<m1.column;j++)
            {
                int pos=i*m1.column+j;
                result.element[pos]=m1.element[pos]+m2.element[pos];

            }
        }
        return result;
    }
    
}

//Adds all the matrices in the arguments.
//First argument is the number of matrices >1
//This is followed by the matrices.
matrix sum_matrix(int args_count,matrix m,...)
{
    if(args_count<1){
        errno=EINVAL;
        error(__func__);
    }
    else
    {
        va_list ap;
        va_start(ap,m);

        matrix sum_res=new_matrix(m.row,m.column);
        sum_res=add_matrix(sum_res,m);

        for(int i=0;i<args_count-1;i++)
        {
           
            matrix next_matrix=va_arg(ap,matrix);
            sum_res=add_matrix(sum_res,next_matrix);
        }
        va_end(ap);
        return sum_res;
    }
}

//Multiplies two matrices of appropriate dimensions.
matrix multiply_matrix(matrix m1,matrix m2)
{
    if(m1.column!=m2.row){
        errno=EDIMMISMATCH;
        error(__func__);
    }
    else
    {
        matrix mul_res=new_matrix(m1.row,m2.column);
        for(int i=0;i<m1.row;i++)
        {
            for(int j=0;j<m2.column;j++)
            {
                double sigma=0;
                for(int k=0;k<m1.column;k++)
                {
                    sigma+=(m1.element[i*m1.column+k]*m2.element[k*m2.column+j]);
                }
                mul_res.element[i*mul_res.column+j]=sigma;
            }
        }
        return mul_res;
    }
}

//Returns the transpose of the provided matrix.
matrix transpose_matrix(matrix m)
{
    matrix t_matrix=new_matrix(m.column,m.row);
    for(int i=0;i<t_matrix.row;i++)
    {
        for(int j=0;j<t_matrix.column;j++)
        {
            t_matrix.element[i*t_matrix.column+j]=m.element[j*m.column+i];
        }
    }
    return t_matrix;
}

//Raises the provided matrix to the given power
//Arguments: A square matrix, An integer >= 0
matrix power_matrix(matrix m,int pow)
{
    if(pow<0){
        errno=EINVAL;
        error(__func__);
    }
    if(m.row!=m.column){
        errno=EDIMMISMATCH;
        error(__func__);
    }
    if(pow==0)
    {
        matrix identity_matrix=new_matrix(m.row,m.column);
        for(int i=0;i<m.row;i++)
        {
            for(int j=0;j<m.column;j++)
            {
                if(i==j)
                {
                    set_value(&identity_matrix,i,j,1);
                }
            }
        }
        return identity_matrix;
    }
    matrix temp_res=power_matrix(m,pow/2);
    if(pow%2)
    {
        return multiply_matrix(m,multiply_matrix(temp_res,temp_res));
    }
    else
    {
        return multiply_matrix(temp_res,temp_res);
    }
}

//Converts a matrix to upper triangular matrix
static matrix upp_triangular(matrix m)
{

    matrix up_mat=copy_matrix(m);
    for(int k=0;k<up_mat.row-1;k++)
    {
        int z=k;
        for(int j=k+1;j<up_mat.row;j++)
        {
            if(fabs(get_value(up_mat,z,k))<fabs(get_value(up_mat,j,k)))
                z=j;
        }
        if(z!=k)
        {
            for (int  i = 0; i < up_mat.column; i++)
            {   
                double t=get_value(up_mat,k,i);
                set_value(&up_mat,k,i,get_value(up_mat,z,i));
                set_value(&up_mat,z,i,t);
            }
        }
        for(int j=k+1;j<up_mat.row;j++)
        {
            double mult_fact=get_value(up_mat,j,k)/get_value(up_mat,k,k);
            for(int p=k;p<up_mat.column;p++)
            {
                double new_val=get_value(up_mat,j,p)-mult_fact*get_value(up_mat,k,p);
                set_value(&up_mat,j,p,new_val);
            }
        }
    }
    return up_mat;
}

//Returns the determinant of matrix
//Argument must be a square matrix
double determinant(matrix m)
{
    if(m.row!=m.column)
    {
        errno=EDIMMISMATCH;
        error(__func__);
    }
    matrix m_copy=copy_matrix(m);
    int row_swap_count=0;
    for(int k=0;k<m_copy.row-1;k++)
    {
        int z=k;
        for(int j=k+1;j<m_copy.row;j++)
        {
            if(fabs(get_value(m_copy,z,k))<fabs(get_value(m_copy,j,k)))
                z=j;
        }
        if(fabs(get_value(m_copy,z,k))<1e-7)
        {
            return 0;
        }
        else
        {
            //Swapping of rows z and k
            row_swap_count++;
            for(int  i = 0; i < m_copy.column; i++)
            {   
                double t=get_value(m_copy,k,i);
                set_value(&m_copy,k,i,get_value(m_copy,z,i));
                set_value(&m_copy,z,i,t);
            } 
        }
        if(fabs(get_value(m_copy,m_copy.row-1,m_copy.row-1))<1e-7)
        {   
            return 0;
        }
        else
        {
            for (int  j = k+1; j < m_copy.row; j++)
            {
                double mult_fact=get_value(m_copy,j,k)/get_value(m_copy,k,k);
                for(int p=k;p<m_copy.row;p++)
                {
                    double new_val=get_value(m_copy,j,p)-mult_fact*get_value(m_copy,k,p);
                    set_value(&m_copy,j,p,new_val);
                }
            }
        }
        
    }
    double det=1.0;
    for(int i=0;i<m_copy.row;i++)
    {
        double entry=get_value(m_copy,i,i);
        det*=entry;
    }
    delete_matrix(&m_copy);
    if(row_swap_count&1)
        return -det;
    else
        return det;
}

//Returns a new matrix in which second parameter in concatenated to the right of first parameter
matrix concat_side(matrix m1, matrix m2)
{
    if(m1.row!=m2.row)
    {
        errno=EDIMMISMATCH;
        error(__func__);
    }
    matrix concat_smat=new_matrix(m1.row,m1.column+m2.column);
    for(int i=0;i<m1.row;i++)
    {
        for(int j=0;j<m1.column;j++)
        {
            set_value(&concat_smat,i,j,get_value(m1,i,j));
        }
    }
    for(int i=0;i<m1.row;i++)
    {
        for(int k=0;k<m2.column;k++)
        {
            set_value(&concat_smat,i,m1.column+k,get_value(m2,i,k));
        }
    }
    return concat_smat;
}

//Returns a new matrix in which second matrix is concatenated below the first matrix 
matrix concat_down(matrix m1,matrix m2)
{
    if(m1.column!=m2.column)
    {
        errno=EDIMMISMATCH;
        error(__func__);
    }
    matrix concat_dmat=new_matrix(m1.row+m2.row,m1.column);
    for(int i=0;i<m1.row;i++)
    {
        for(int j=0;j<m1.column;j++)
        {
            set_value(&concat_dmat,i,j,get_value(m1,i,j));
        }
    }
    for(int i=0;i<m2.row;i++)
    {
        for(int j=0;j<m2.column;j++)
        {
            set_value(&concat_dmat,m1.row+i,j,get_value(m2,i,j));
        }
    }
    return concat_dmat;
}

//warning calculating requires comparison of a floating point number with zero
int rank(matrix m)
{
    matrix upp=upp_triangular(m);
    int rank_value=upp.row;
    for(int i=upp.row-1;i>=0;i--)
    {
        for(int j=0;j<upp.column;j++)
        {
            //since floating point numbers are not infinitely precise we 
            //may assume that a number is zero if it lies within the range 1e-7 inclusive
            if(fabs(get_value(upp,i,j))>1e-7)
            {
                //number is not zero hence we found a non zero row
                return rank_value;
            }
        }
        rank_value--;
    }
    delete_matrix(&upp);
    return rank_value;
}

matrix inverse(matrix m)
{
    if(m.row!=m.column)
    {
        errno=EDIMMISMATCH;
        error(__func__);
    }
    matrix I=new_matrix(m.row,m.column);
    for(int i=0;i<m.row;i++)
    {
        set_value(&I,i,i,1);
    }
    matrix upp=concat_side(m,I);
    if(rank(upp)<m.row)
    {
        errno=NOSOL;
        error(__func__);
    }
    for(int i=upp.row-1;i>0;i--)
    {
        if(get_value(upp,i-1,0)<get_value(upp,i,0))
        {
            for(int p=0;p<upp.column;p++)
            {
                double val=get_value(upp,i,p);
                set_value(&upp,i,p,get_value(upp,i-1,p));
                set_value(&upp,i-1,p,val);
            }
        }
    }
    for(int i=0;i<m.row;i++)
    {
        for(int j=0;j<m.row;j++)
        {
            if(j!=i)
            {
                double mult_fact=get_value(upp,j,i)/get_value(upp,i,i);
                for(int k=0;k<upp.column;k++)
                {
                    double new_val=get_value(upp,j,k)-mult_fact*get_value(upp,i,k);
                    set_value(&upp,j,k,new_val);
                }
            }
        }
    }

    for(int i=0;i<m.row;i++)
    {
        double diag=get_value(upp,i,i);
        for(int j=0;j<upp.column;j++)
        {
            double new_val=get_value(upp,i,j)/diag;
            set_value(&upp,i,j,new_val);
        }
    }
    matrix inv=submatrix(upp,0,m.column,m.row-1,2*m.column-1);
    delete_matrix(&upp);
    return inv;
}

//Solves the system of linear equations Ax=B 
//Returns a solution as a row vector if it exists or writes to stderr that no unique solution exists
//Arguments : A coeff_mat, B: val_mat
matrix solvematrix(matrix coeff_mat,matrix val_mat)
{
    if(coeff_mat.row!=coeff_mat.column)
    {
        errno=EDIMMISMATCH;
        error(__func__);
    }
    if (val_mat.row!=coeff_mat.row)
    {
        errno=EDIMMISMATCH;
        error(__func__);
    }
    if(val_mat.column>1)
    {
        errno=EDIMMISMATCH;
        error(__func__);
    }
    matrix aug_matrix=concat_side(coeff_mat,val_mat);
    for(int k=0;k<aug_matrix.row-1;k++)
    {
        int z=k;
        for(int j=k+1;j<aug_matrix.row;j++)
        {
            if(fabs(get_value(aug_matrix,z,k)<fabs(get_value(aug_matrix,j,k))))
                z=j;
        }
        if(fabs(get_value(aug_matrix,z,k))<1e-7){
            errno=NOSOL;
            error(__func__);
        }
        else
        {        
            //swapping rows k and z
            for(int i=0;i<aug_matrix.column;i++)
            {
                double val=get_value(aug_matrix,k,i);
                set_value(&aug_matrix,k,i,get_value(aug_matrix,z,i));
                set_value(&aug_matrix,z,i,val);
            }
        }
        if(fabs(get_value(aug_matrix,aug_matrix.row-1,aug_matrix.row-1))<1e-7)
        {
            errno=NOSOL;
            error(__func__);
        }
        else
        {
            for(int j=k+1;j<aug_matrix.row;j++)
            {
                double mult_fact=get_value(aug_matrix,j,k)/get_value(aug_matrix,k,k);
                for(int p=k;p<aug_matrix.column;p++)
                {
                    double new_val=get_value(aug_matrix,j,p)-mult_fact*get_value(aug_matrix,k,p);
                    set_value(&aug_matrix,j,p,new_val);
                }
            }
        }
    }
    int n=aug_matrix.row;
    matrix sol_vector=new_matrix(1,n);

    double xn=get_value(aug_matrix,n-1,n)/get_value(aug_matrix,n-1,n-1);
    set_value(&sol_vector,0,sol_vector.column-1,xn);
    
    for(int i=n-2;i>=0;i--)
    {
        double diff_val=0;
        for(int j=i+1;j<n;j++)
        {
            diff_val+=get_value(aug_matrix,i,j)*get_value(sol_vector,0,j);
        }
        double x_sol=(get_value(aug_matrix,i,n)-diff_val)/get_value(aug_matrix,i,i);
        set_value(&sol_vector,0,i,x_sol);
    }
    delete_matrix(&aug_matrix);
    return sol_vector;
}

//Solves the system of linear equations using gauss-siedel algorithm
//Alert: procedure fails if the any diagonal entry of coefficient matrix is zero
matrix gauss_siedel(matrix coeff,matrix val_mat,double tol,int iterations)
{
    for(int i=0;i<coeff.row;i++)
    {
        if(fabs(get_value(coeff,i,i))<1e-7)
        {
            // gauss-siedel algorithm works only if
            // diagonal entries are non zero
            errno=EINVAL;
            error(__func__);
        }
    }
    matrix x=new_matrix(1,val_mat.row);
    matrix x_new=new_matrix(1,val_mat.row);
    double s1,s2;
    s1=s2=0;

    for(int iter=0;iter<iterations;iter++)
    {
        set_all_value(&x_new,0);
        for(int i=0;i<coeff.row;i++)
        {
            if(i==0)
            {
                s1=0;
                matrix sub_coeff2=submatrix(coeff,i,i+1,i,coeff.column-1);
                matrix sub_x=submatrix(x,0,i+1,0,x.column-1);
                s2=matrix_dot(sub_coeff2,sub_x);
            }
            else if(i==coeff.row-1)
            {
                matrix sub_coeff1=submatrix(coeff,i,0,i,i-1);
                matrix sub_x_new=submatrix(x_new,0,0,0,i-1);
                s1=matrix_dot(sub_coeff1,sub_x_new);
                s2=0;
            }
            else
            {
                matrix sub_coeff1=submatrix(coeff,i,0,i,i-1);
                matrix sub_x_new=submatrix(x_new,0,0,0,i-1);
                matrix sub_coeff2=submatrix(coeff,i,i+1,i,coeff.column-1);
                matrix sub_x=submatrix(x,0,i+1,0,x.column-1);
                s1=matrix_dot(sub_coeff1,sub_x_new);
                s2=matrix_dot(sub_coeff2,sub_x);
            }
            double x_new_val=(get_value(val_mat,i,0)-s1-s2)/get_value(coeff,i,i);
            set_value(&x_new,0,i,x_new_val);
        }
        if(check_tolerance(x_new,x,tol)==1)
            break;
        x=copy_matrix(x_new);
    }
    return x;
}