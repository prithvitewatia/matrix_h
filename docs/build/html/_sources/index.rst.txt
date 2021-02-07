.. Matrix_h documentation master file, created by
   sphinx-quickstart on Thu Jan 14 18:53:36 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.


#####################################
Welcome to Matrix_h's documentation!
#####################################

Matrix_h
==========

Matrix_h hosts a fast, efficient and easy to use C linear algebra library by the name libmatrix.

Key Features
=============

* Simple and easy to use
* Fast and memory efficient
* No explicit pointers
* Error detection 

Installation
=============

Clone the repository into your computer by executing the following command in the terminal.
::
  git clone https://github.com/prithvitewatia/matrix_h

cd in the the repository
::
  cd matrix_h

Run the following command 
::
  make libmatrix.a

If everything runs fine you will see files libmatrix.a and matrix.o generated in the directory. If not please make sure that
you have gcc and make installed and set on **PATH**. Links to
gcc: `gcc <https://sourceforge.net/projects/mingw-w64/>`_
make: `make <https://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download?use_mirror=excellmedia&download=>`_

Congrats your library is now generated.

Usage
=======

* Copy the include folder into your project directory
* Create a lib folder and move the **libmatrix.a** into lib folder
* Include the library in your C file by using the include directive
  ::
    #include<matrix.h>
* To build your C file execute the command
  ::
    gcc yourCfile.c -I ./include -L ./lib -lmatrix -o yourOutputfile

* Run your file by
  ::
    ./yourOutputfile

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   func_examples
   Caution
   Feedback
   



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
