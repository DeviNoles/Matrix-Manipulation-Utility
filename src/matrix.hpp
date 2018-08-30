#ifndef MATRIX_H
#define MATRIX_H

#include <ostream>   /* ostream */
#include <string>    /* string  */

class Matrix
{
   public:

      // Constructors
      Matrix();
      Matrix(const unsigned r, const unsigned c);
      Matrix(const unsigned r, const unsigned c, const std::string& e);
      Matrix(const std::string& e);
      Matrix(const Matrix& A); //Copy Constructor - Given
      Matrix(Matrix&& A); //Move Constructor - Given

      ~Matrix();

      void Save(std::string&);
      Matrix& operator=(const Matrix& A);

      // General Matrix Information
      unsigned NumRows() const;
      unsigned NumColumns() const;
      std::string Size() const;

      // Element Access
      double& operator()(const unsigned i, const unsigned j) const;

      // General Matrix Operations
      friend Matrix operator+(const Matrix& A, const Matrix& B);
      friend Matrix operator*(const double c, const Matrix& A);
      friend Matrix Rotate90Clockwise(const Matrix& A);
      friend Matrix Blur(const Matrix& A, const int k);
      void AddRow(const unsigned rI, const unsigned rJ, const double c);
	void MultRow(const unsigned rI, const double c);

      // Printing
      friend std::ostream& operator<<(std::ostream&, const Matrix& A);

   private:

      double **m;
      unsigned numRows;
      unsigned numColumns;
};

#endif
