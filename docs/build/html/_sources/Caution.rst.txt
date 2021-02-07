Caution
=========

*   If x and y are two matrices do not use statements such as x=y to copy the contents of y into x. 
    Rather use x=copy_matrix(y) to copy the contents. 
    This is because statement x=y will make the references of x and y same and hence further changes in y will lead to changes in x.
*   Also since floating point numbers are not infinitely precise therefore ieee has two representation of number 0 ie 0 and -0 .
    This is not a bug. This is just a way of representation in which no information is lost.