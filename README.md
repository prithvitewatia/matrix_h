# Libmatrix

Libmatrix is a fast, efficient and easy to use **C linear algerbra library**.  
Documentation: [![Documentation Status](https://readthedocs.org/projects/matrix-h/badge/?version=latest)](https://matrix-h.readthedocs.io/en/latest/?badge=latest)  
License: [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## MOTIVATION

This project was started to develop a linear algebra library written in the C programming language
that is simple and easy to use without reducing the performance of the code

## KEY FEATURES

- Simple and easy to use
- Fast and memory efficient
- No explicit pointers
- **Error detection**

## INSTALLATION

Clone the repository into your computer by executing the following command in the terminal 
```
git clone https://github.com/prithvitewatia/matrix_h
```

cd in the the repository
```
cd matrix_h
```

Run the following command
```
make libmatrix.a
```

If everything runs fine you will see files libmatrix.a and matrix.o generated in the directory.
If not please make sure that you have **gcc** and **make** installed and set on the **PATH**.
Link to  
**gcc**: [gcc](https://sourceforge.net/projects/mingw-w64/)  
**make**: [make](https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download?use_mirror=excellmedia&download=)

Congrats your library is now generated

## Usage
- Copy the include folder to your project directory.
- Create a lib folder and move the **libmatrix.a** into the lib folder
- Include the library in your C file by using the include directive
```C
#include<matrix.h>
```
- To build your C file execute the command
```
gcc yourCfile.c -I ./include -L ./lib -lmatrix -o yourOutputfile
```
- Run your file by
```
./yourOutputfile
```

Happy coding!

## EXAMPLES

A program to add to matrices and check if the result is equal to third matrix.
![example](https://github.com/prithvitewatia/matrix_h/blob/master/doc_images/matrix_add_example_code.png?raw=true)

The output of the above program
![example](https://github.com/prithvitewatia/matrix_h/blob/master/doc_images/matrix_add_output.png?raw=true)

Here is another example in which we calculate rank and determinant.
![example](https://github.com/prithvitewatia/matrix_h/blob/master/doc_images/rank_det_example.png?raw=true)

Here is its output
![example](https://github.com/prithvitewatia/matrix_h/blob/master/doc_images/rank_det_output.png?raw=true)

For official documentation visit [matrix-h documentation](https://matrix-h.readthedocs.io/en/latest/?badge=latest)

### CAUTION
- If x and y are two matrices do not use statements such as x=y to copy the contents of y into x.
Rather use x=copy_matrix(y) to copy the contents. This is because statement x=y will make the references of
x and y same and hence further changes in y will lead to changes in x.
- Also since floating point numbers are not infinitely precise therefore ieee has two representation of 
number 0 ie 0 and -0 . This is not a bug. This is just a way of representation in which no information is lost.

### FEEDBACK AND SUGGESTIONS
This library is still under development .
Write your queries to prithvisinghtewatia@gmail.com
