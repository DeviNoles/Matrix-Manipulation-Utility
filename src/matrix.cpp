/*****************************************************************************
 *                                                                           *
 *        File: matrix.cpp                                                   *
 * Descritpion: Function defintions for the Matrix class.                    *
 *      Author: COP3330-SPR18-PIYUSH                                         *
 *        Date: 2018-02-26                                                   *
 *                                                                           *
 * Assumptions/Notes:                                                        *
 *    - The OS must support the UTF-8 character encoding.                    *
 *    - Undefined behavior if element access is out of bounds.               *
 *                                                                           *
 *****************************************************************************/

#include <cmath>        /* abs                                 */
#include <iomanip>      /* setprecision, setw, showpoint       */
#include <iostream>     /* cout, endl, flush, ostream          */
#include <sstream>      /* stringstream                        */
#include <utility>      /* move, swap                          */
#include "matrix.hpp"
#include<fstream>

/*****************************************************************************
 * Function: (Default) Constructor
 * Description: Constructs an empty matrix.
 * Parameters: n/a
 * Return Value: n/a
 */
Matrix::Matrix()
{
   //std::cout << "Default Constructor... " << std::flush;

   m = nullptr;
   numRows = numColumns = 0;
   //std::cout << "Done" << std::endl;
}

/*****************************************************************************
 * Function: (Size) Constructor
 * Description: Constructs a zero matrix with the provided size.
 * Parameters:
 *    nR - Number of rows.
 *    nC - Number of columns.
 * Return Value: n/a
 */
Matrix::Matrix(const unsigned nR, const unsigned nC)
{
   //std::cout << "Size Constructor... " << std::flush;
   numRows = nR;
   numColumns = nC;

   if (numRows == 0 && numColumns == 0)
   {
      m = nullptr;
   }
   else
   {
      m = new double*[numRows];
      for (unsigned i = 0; i < numRows; ++i)
      {
         m[i] = new double[numColumns];
         for (unsigned j = 0; j < numColumns; ++j)
         {
            m[i][j] = 0;
         }
      }
   }
   //std::cout << "Done" << std::endl;
}
/*****************************************************************************
 * Function: (Size) Constructor (w/ pgm file input)
 * Description: Constructs a matrix according to pgm input.
 * Parameters:
 *    A  - String containing a pgm filename with ending "pgm"
 * Return Value: n/a
 */
Matrix::Matrix(const std::string& A)
{
   std::ifstream in;
   in.open(A);
   if (!in)
      std::cout << "Error opening input file." << std::endl;

   std::string dummystring="";
   std::string inputstring="";
   int colns, rws, maxGray;
   
   in >> dummystring;
   in.ignore();

   while(in.peek()=='#')
      getline(in, dummystring, '\n');

   in >> colns >> rws;
   in.ignore();

   while(in.peek()=='#')
      getline(in, dummystring, '\n');

   in >> maxGray;
   in.ignore();

   dummystring = "";
   for (int i=0; i<rws; ++i)
   {    
      if (in.peek()=='#')
      {
         getline(in, dummystring, '\n');
         i--;
      }
      else
      {
         getline(in, dummystring, '\n');
         inputstring = inputstring + " " + dummystring;
      }
      inputstring+='\n';
      
   }
   in.close();

   std::stringstream ss;

   numRows = rws;
   numColumns = colns;

   if (rws == 0 || colns == 0)
   {
      m = nullptr;
      numRows = numColumns = 0;
   }
   else
   {
      ss << inputstring;

      m = new double*[numRows];
      for (unsigned i = 0; i < numRows; ++i)
      {
         m[i] = new double[numColumns];
         for (unsigned j = 0; j < numColumns; ++j)
         {
            m[i][j]=0;
            ss >> m[i][j];
         }
      }
   }
   //std::cout << inputstring << std::endl;

}

/*****************************************************************************
 * Function: (Size) Constructor (w/ Elements)
 * Description: Constructs a matrix with the provided size and elements.
 * Parameters:
 *    nR - Number of rows.
 *    nC - Number of columns.
 *    A  - String containing matrix elements.
 * Return Value: n/a
 */
Matrix::Matrix(const unsigned nR, const unsigned nC, const std::string& A)
{
   //std::cout << "Size Constructor w/ Elements... " << std::flush;
   std::stringstream ss;

   numRows = nR;
   numColumns = nC;

   if (nR == 0 || nC == 0)
   {
      m = nullptr;
      numRows = numColumns = 0;
   }
   else
   {
      ss << A;

      m = new double*[numRows];
      for (unsigned i = 0; i < numRows; ++i)
      {
         m[i] = new double[nC];
         for (unsigned j = 0; j < numColumns; ++j)
         {  
            m[i][j]=0;
            ss >> m[i][j];
         }
      }
   }
}

/*****************************************************************************
 * Function: (Copy) Constructor
 * Description: Constructs a matrix based off the provided one.
 * Parameters:
 *    A - Matrix to copy.
 * Return Value: n/a
 */
Matrix::Matrix(const Matrix& A)
{
   //std::cout << "Copy Constructor... " << std::flush;
   numRows = A.numRows;
   numColumns = A.numColumns;

   m = new double*[numRows];
   for (unsigned i = 0; i < numRows; ++i)
   {
      m[i] = new double[numColumns];
      for (unsigned j = 0; j < numColumns; ++j)
      {
         m[i][j] = A.m[i][j];
      }
   }
   //std::cout << "Done" << std::endl;
}

/*****************************************************************************
 * Function: (Move) Constructor
 * Description: 'Constructs' a matrix based off the provided one.
 * Parameters:
 *    A - Matrix to steal members of.
 * Return Value: n/a
 */
Matrix::Matrix(Matrix&& A)
{
   //std::cout << "Move Constructor... " << std::flush;
   numRows = std::move(A.numRows);
   numColumns = std::move(A.numColumns);
   m = A.m;
   A.m = nullptr;
   //std::cout << "Done" << std::endl;
}

/*****************************************************************************
 * Function: Destructor
 * Description: Deallocates dynamic memory used by the Matrix.
 * Parameters: n/a
 * Return Value: n/a
 */
Matrix::~Matrix()
{
   //std::cout << "Destructor... " << std::flush;
   if (m != nullptr)
   {
      for (unsigned i = 0; i < numRows; ++i)
      {
         delete [] m[i];
         m[i] = nullptr;
      }
      delete [] m;
      m = nullptr;
   }
   //std::cout << "Done" << std::endl;
}

/*****************************************************************************
 * Function: operator= ((Copy) Assignment)
 * Description: 
 * Parameters:
 *    A - The Matrix.
 * Return Value: Reference to the Matrix just constructed.
 */
Matrix& Matrix::operator=(const Matrix& A)
{
   Matrix T(A);

   std::swap((*this).numRows, T.numRows);
   std::swap((*this).numColumns, T.numColumns);
   std::swap((*this).m, T.m);

   return *this;
}

/*****************************************************************************
 * Function: NumRows
 * Description: Returns the number of rows of the Matrix.
 * Parameters: n/a
 * Return Value: The number of rows.
 */
unsigned Matrix::NumRows() const
{
   return numRows;
}

/*****************************************************************************
 * Function: NumColumns
 * Description: Returns the number of columns of the Matrix.
 * Parameters: n/a
 * Return Value: The number of columns.
 */
unsigned Matrix::NumColumns() const
{
   return numColumns;
}

/*****************************************************************************
 * Function: Size
 * Description: Returns the size of the matrix.
 *       Reference: en.wikipedia.org/wiki/Matrix_(mathematics)#Size
 * Parameters: n/a
 * Return Value: A string representing the size, of the form: <rows>x<cols>.
 */
std::string Matrix::Size() const
{
   return std::to_string(numRows) + "x" + std::to_string(numColumns);
}

/*****************************************************************************
 * Function: operator()
 * Description: Access an element of the Matrix. The element is mutable.
 *       Note that element subscripts start at 1, not 0, so 1 is taken
 *       away from the input indices to form the correct internal indices.
 * Parameters:
 *    i - Index of the row.
 *    j - Index of the column.
 * Return Value: Reference to the element.
 */
double& Matrix::operator()(const unsigned i, const unsigned j) const
{
   return m[i-1][j-1];
}

/*****************************************************************************
 * Function: operator* (Scalar multiplication)
 * Description: Multiplies each element of the matrix by the scalar.
 *       Reference: en.wikipedia.org/wiki/Scalar_multiplication
 * Parameters:
 *    c - The scalar.
 *    A - The Matrix.
 * Return Value: A Matrix that represents cA.
 */
Matrix operator*(const double c, const Matrix& A)
{
   Matrix C = A;

   for (unsigned i = 0; i < C.numRows; ++i)
      for (unsigned j = 0; j < C.numColumns; ++j)
         C.m[i][j] *= c;

   return C;
}

/*****************************************************************************
 * Function: operator<<
 * Description: Prints out the Matrix in the convential form.
 * Parameters:
 *    out - Output stream to insert Matrix in.
 *    A   - The Matrix.
 * Return Value: Reference to the output stream (for chaining).
 */
std::ostream& operator<<(std::ostream& out, const Matrix& A)
{
   /* Unicode symbols for the matrix boundary. */
   const std::string VERT_BAR = u8"\u2502";
   const std::string CORNER_TOP_LEFT = u8"\u250c";
   const std::string CORNER_TOP_RIGHT = u8"\u2510";
   const std::string CORNER_BOTTOM_LEFT = u8"\u2514";
   const std::string CORNER_BOTTOM_RIGHT = u8"\u2518";

   out << "\n";

   /* Prints the top of the matrix:
    *
    *     +--                     --+
    *     |                         |
    */
   out << CORNER_TOP_LEFT << std::setw(A.numColumns * 5 + 2) << " "
       << CORNER_TOP_RIGHT << "\n";

   /* Prints the contents of the matrix:
    *
    *     |  a_11  a_12  ...  a_1m  |
    *     |                         |
    *     |  a_21  a_22  ...  a_2m  |
    *     |  ...                    |
    *     |  a_n1  a_n2  ...  a_nm  |
    */
   for (unsigned i = 0; i < A.numRows; ++i)
   {
      out << VERT_BAR;
      for (unsigned j = 0; j < A.numColumns; ++j)
      {
         if (A.m[i][j] < 0)
            out << " -" << std::setw(3) << fabs(A.m[i][j]);
         else
            out << "  " << std::setw(3) << A.m[i][j];
      }
      out << "  " << VERT_BAR << "\n";

      if (i != A.numRows-1)
         out << VERT_BAR << std::setw(A.numColumns * 5 + 2) << " "
             << VERT_BAR << "\n";
   }

   /* Prints the bottom of the matrix:
    *
    *     |                         |
    *     +--                     --+
    */
   out << CORNER_BOTTOM_LEFT << std::setw(A.numColumns * 5 + 2) << " "
       << CORNER_BOTTOM_RIGHT << "\n";

   /* Ensure everything is displayed immediately. */
   out << std::flush;

   return out;
}

void Matrix::AddRow(const unsigned rI, const unsigned rJ, const double c)
{
	for (unsigned i = 0; i < numColumns; ++i)
		m[rI-1][i] += c * m[rJ-1][i];
}

void Matrix::MultRow(const unsigned rI, const double c)
{
	for (unsigned i = 0; i < numColumns; ++i)
		m[rI-1][i] *= c;
}

class DimensionMismatch{}; //Exception class

/*****************************************************************************
 * Function: operator+ (Matrix Addition)
 * Description: Add two matrices. Throws Dimension mismatch if applicable.
 *       Reference: en.wikipedia.org/wiki/Matrix_addition
 * Parameters:
 *    A - LHS Matrix.
 *    B - RHS Matrix.
 * Return Value: 
     A Matrix that represents A + B, if all pixel sums are <=255
       Otherwise the matrix will be same except for those pixels:
       any element whose pixel sum is >= 255 will be replace with sum/2
 */


Matrix operator+(const Matrix& A, const Matrix& B)
{  
   Matrix C = A;
   try{
      if (A.NumRows()!=B.NumRows() || A.NumColumns()!=B.NumColumns())
         throw DimensionMismatch();
      for (unsigned i = 0; i < C.numRows; ++i)
         for (unsigned j = 0; j < C.numColumns; ++j)
         {
            if ( (C.m[i][j] + B.m[i][j]) <= 255 )
               C.m[i][j] += B.m[i][j];
            else 
               C.m[i][j]= (int) ((C.m[i][j] + B.m[i][j])/2.0);
         }
   }
   catch(DimensionMismatch){
      std::cout << "The dimensions of the matrices to be added mismatch." << std::endl;
      exit(1);
   }

   return C;
}

/*****************************************************************************
 * Function: Rotate90Clockwise 
 * Description: Rotates a matrix 90 degrees clockwise
 * Parameters:
 *    A - The Matrix.
 * Return Value: The rotated matrix
 */
Matrix Rotate90Clockwise(const Matrix& A)
{
   Matrix C = Matrix(A.NumColumns(), A.NumRows());
   for (int i = 0; i < (int) C.NumRows(); ++i)
      for (int j = 0; j < (int) C.NumColumns(); ++j)
         C(i+1,j+1) = A( ((int)A.NumRows())-j, i+1);
   return C;
}

/*****************************************************************************
 * Function: Blur 
 * Description: Applies a Gaussian Blur to matrix A and returns the result
 * Parameters:
 *    A - The Matrix.
 *    k - Dimension of the Gaussian kernl
 * Return Value: The Blurred matrix
 */
Matrix Blur(const Matrix& A, const int k)
{
   Matrix A_Blur(A);

   //Initialize a set of kernels for each A-entry
   Matrix** kernels = new Matrix*[(int) A.NumRows()];
   for (int i = 0; i < (int) A.NumRows(); ++i)
   {
      kernels[i] = new Matrix[(int) A.NumColumns()];
      for (int j = 0; j < (int) A.NumColumns(); ++j)
         kernels[i][j] = Matrix(k, k);
   }

   //Determine each kernel
   for (int i = 0; i < (int) A.NumRows(); ++i)
      for (int j = 0; j < (int) A.NumColumns(); ++j)
      {
         //Determine the kernel for entry ij
         for (int r = 0; r < k; ++r)
            for (int c = 0; c < k; ++c)
            {
               if ((r+i-((k-1)/2)) < 0  || (c+j-((k-1)/2)) < 0 ||
                   (r+i-((k-1)/2)) >= (int) A.NumRows() ||
                   (c+j-((k-1)/2)) >= (int) A.NumColumns())
                  kernels[i][j](r+1,c+1) = 0;
               else
                  kernels[i][j](r+1,c+1) = A(r+i-((k-1)/2)+1, c+j-((k-1)/2)+1);
            }

      }

   //Calculate A_Blur by Applying the Gaussian Blur to each entry
   for (int i = 0; i < (int) A_Blur.NumRows(); ++i)
      for (int j = 0; j < (int) A_Blur.NumColumns(); ++j)
      {
         A_Blur(i+1,j+1) = 0;
         for (int r = 0; r < k; ++r)
            for (int c = 0; c < k; ++c)
            {
               A_Blur(i+1,j+1) += 
                  kernels[i][j](r+1,c+1) *
                  pow(M_E, -(pow(r-((k-1)/2),2)+pow(c-((k-1)/2),2)));
            }
         A_Blur(i+1,j+1)= (int) A_Blur(i+1,j+1);
         if (A_Blur(i+1,j+1)>=255)
            A_Blur(i+1,j+1)=255;
      }

   return A_Blur;
}

/*****************************************************************************
 * Function: Save
 * Description: Accepts an output filename as argument and saves the matrix 
 * content to the output file in PGM format.
 * Parameters:
 *    filename - the outfile filename
 * Return Value: none
 */
void Matrix::Save(std::string& filename)
{
   std::ofstream out;

   out.open(filename);

   out << "P2\n";
   out << this->numColumns << " " << this->numRows << "\n";

   //Do a loop to determine the number largest value
   int largest = 0;
   for (unsigned i = 0; i < this->numRows; ++i)
      for (unsigned j = 0; j < this->numColumns; ++j)
            if (this->m[i][j] > largest)
               largest = this->m[i][j];

   out << largest << "\n";

   for (unsigned i = 0; i < this->numRows; ++i)
   {
      for (unsigned j = 0; j < this->numColumns; ++j)
      {
            out << this->m[i][j];
            if (j!=this->numColumns-1) 
               out << " ";
      }
      out << "\n";
   }
   out.close();
}







