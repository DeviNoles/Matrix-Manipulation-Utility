/**
 * File: matman.cpp
 * Description: Simple matrix manipulation utility.
 *    Provides an interactive interface in which users have a variety
 *    of commands that manipulate matrices. Nothing too complicated.
 * Author: Forgetful Employee
 * Date: 2017-02-21
 */

#include <iomanip>      /* setw        */
#include <iostream>     /* cin, cout   */
#include <utility>      /* pair        */
#include <string>       /* string      */
#include <fstream>
#include "matrix.hpp"

/* CONSTANTS & TYPEDEFS --------------------------------------------------- */

/* Menu options. */
const char ADD_MATRIX_TO_LIST = 'a';
const char REMOVE_MATRIX_FROM_LIST = 'r';
const char PGM_MATRIX_TO_LIST = 'G';
const char DISPLAY_MATRIX = 'd';
const char ADD_MATRICES = 'A';
const char MULTIPLY_MATRIX_BY_SCALAR = 'S';
const char ROTATE_MATRIX = 'R';
const char BLUR_MATRIX = 'B';
const char EXPORT_TO_FILE = 'x';
const char MULTIPLY_ROW_BY_SCALAR = 'M';
const char ADD_SCALAR_ROW_TO_ROW = 'c';
const char DISPLAY_INFORMATION = 'i';
const char PRINT_MENU = 'p';
const char PRINT_MATRIX_LIST = 'P';
const char QUIT = 'q';

const unsigned MAX_STRLEN = 100000;

using MList = std::pair<std::string, Matrix>;

/* PROTOTYPES ------------------------------------------------------------- */

/* Printing. */
void DisplayMatrixList(const MList*);
void DisplayMatrix(const std::string&, const MList *);
void DisplayMenu();
void DisplayMatrixInformation(const std::string&, const MList *);

/* List manipulation. */
void AddMatrixToList(const std::string&, const Matrix&, MList * const);
void RemoveMatrixFromList(const std::string&, MList *);

/* Utility. */
bool NameExists(const std::string&, const MList *);
std::string NewMatrixName(const MList *);
std::string GetMatrixName(const MList *);
int IndexOfMatrix(const std::string&, const MList *);
unsigned GetValidRow(const std::string&, const MList *);

/* MAIN ------------------------------------------------------------------- */

int main () {

   char choice;
   int scalar;
   int m1loc, m2loc;
   Matrix resMatrix;
   MList mlist[10];
   std::string name, elements, rowElements, m1, m2;
   unsigned rows, cols, r1, r2;

   std::cout << "\n";
   DisplayMenu();  

   /* Get user's choice. */
   std::cout << "\n> ";
   std::cin >> choice;
   std::cin.ignore(MAX_STRLEN, '\n');
   
   while (true)
   {
      std::cout << "\n";

      switch (choice)
      {
         case ADD_MATRIX_TO_LIST: /* -------------------------------------- */
            
            /* Get the matrix name. */
            std::cout << "new matrix name: ";
            name = NewMatrixName(mlist);

            /* Get the number of rows of the matrix. */
            std::cout << "number of rows: ";
            std::cin >> rows;
            std::cin.ignore(MAX_STRLEN, '\n');

            /* Get the number of columns of the matrix. */
            std::cout << "number of cols: ";
            std::cin >> cols;
            std::cin.ignore(MAX_STRLEN, '\n');

            for (unsigned i = 0; i < rows; ++i)
            {
               /* Get the elements of the matrix. */
               std::cout << "elements of row " << i+1 << ": ";
               std::getline(std::cin, rowElements, '\n');
               elements += " " + rowElements;
            }

            AddMatrixToList(name, Matrix(rows, cols, elements), mlist);
            break;

         case REMOVE_MATRIX_FROM_LIST: /* --------------------------------- */

            /* Get the matrix name. */
            std::cout << "matrix name: ";
            name = GetMatrixName(mlist);

            RemoveMatrixFromList(name, mlist);
            break;

         case DISPLAY_MATRIX: /* ------------------------------------------ */

            /* Get the matrix name. */
            std::cout << "matrix name: ";
            name = GetMatrixName(mlist);

            DisplayMatrix(name, mlist);
            break;

         case ADD_MATRICES: /* -------------------------------------------- */
            // Get name for matrix denoting A + B. 
            std::cout << "new matrix name: ";
            name = NewMatrixName(mlist);

            // Matrix A. 
            std::cout << "m1: ";
            m1 = GetMatrixName(mlist);

            // Matrix B. 
            std::cout << "m2: ";
            m2 = GetMatrixName(mlist);

            m1loc = IndexOfMatrix(m1, mlist);
            m2loc = IndexOfMatrix(m2, mlist);
            resMatrix = mlist[m1loc].second + mlist[m2loc].second;

            AddMatrixToList(name, resMatrix, mlist);
            break;

         case ROTATE_MATRIX: /* -------------------------------------------- */

            /* Get name for rotated matrix. */
            std::cout << "new matrix name: ";
            name = NewMatrixName(mlist);

            /* Matrix A. */
            std::cout << "name of matrix to rotate: ";
            m1 = GetMatrixName(mlist);

            m1loc = IndexOfMatrix(m1, mlist);
            resMatrix = Rotate90Clockwise(mlist[m1loc].second);

            AddMatrixToList(name, resMatrix, mlist);
            break;


         case BLUR_MATRIX: /* -------------------------------------------- */
            {
            /* Get name for blurred matrix. */
            std::cout << "new matrix name: ";
            name = NewMatrixName(mlist);

            /* Matrix A. */
            std::cout << "name of matrix to blur: ";
            m1 = GetMatrixName(mlist);

            m1loc = IndexOfMatrix(m1, mlist);

            //k-value
            int k;
            std::cout << "enter odd k-value: ";
            std::cin >> k;

            resMatrix = Blur(mlist[m1loc].second, k);

            AddMatrixToList(name, resMatrix, mlist);
            }
            break;

         case PGM_MATRIX_TO_LIST: /* -------------------------------------------- */
            {

            /* Get name for new matrix. */
            std::cout << "new matrix name: ";
            name = NewMatrixName(mlist);

            //Get filename
            std::string filename="";
            std::cout << "name of pgm file (include extension): ";
            std::cin >> filename;

            Matrix m1(filename);

            AddMatrixToList(name, m1, mlist);
            }
            break;

         case EXPORT_TO_FILE: /* -------------------------------------------- */
            {
            std::string matrixName;
            std::cout << "name of matix to export: ";
            std::cin >> matrixName;

            //Get filename
            std::string filename;
            std::cout << "name of export file (include extension): ";
            std::cin >> filename;

            m1loc = IndexOfMatrix(matrixName, mlist);

            (mlist[m1loc].second).Save(filename);

            std::cout << "Export complete." << "\n";
            }
            break;

         case MULTIPLY_MATRIX_BY_SCALAR: /* -------------------------------------------- */

            /* Get name for matrix denoting c * A. */
            std::cout << "new matrix name: ";
            name = NewMatrixName(mlist);

            /* Matrix A. */
            std::cout << "m1: ";
            m1 = GetMatrixName(mlist);

            /* Scalar c. */
            std::cout << "c: ";
            std::cin >> scalar;
            std::cin.ignore(MAX_STRLEN, '\n');

            m1loc = IndexOfMatrix(m1, mlist);
            resMatrix = scalar * mlist[m1loc].second;

            AddMatrixToList(name, resMatrix, mlist);
            break;

         
         case MULTIPLY_ROW_BY_SCALAR: /* -------------------------------------------- */

            /* Get name of matrix. */
            std::cout << "matrix name: ";
            name = GetMatrixName(mlist);

            /* R. */
            std::cout << "r1: ";
            r1 = GetValidRow(name, mlist);

            /* Scalar. */
            std::cout << "c: ";
            std::cin >> scalar;

            m1loc = IndexOfMatrix(name, mlist);
            mlist[m1loc].second.MultRow(r1, scalar);
            break;

         case ADD_SCALAR_ROW_TO_ROW: /* -------------------------------------------- */

            /* Get name of matrix. */
            std::cout << "matrix name: ";
            name = GetMatrixName(mlist);

            /* Original row. */
            std::cout << "r1: ";
            r1 = GetValidRow(name, mlist);

            /* Multiple row. */
            std::cout << "r2: ";
            r2 = GetValidRow(name, mlist);
            while (r1 == r2)
            {
               std::cout << "invalid row index. cannot equal r1.\n";
               std::cout << "re-enter: ";
               r2 = GetValidRow(name, mlist);
            }

            /* Scalar. */
            std::cout << "c: ";
            std::cin >> scalar;

            m1loc = IndexOfMatrix(name, mlist);
            mlist[m1loc].second.AddRow(r1, r2, scalar);
            break;

         case DISPLAY_INFORMATION: /* -------------------------------------------- */

            /* Get name of matrix. */
            std::cout << "matrix name: ";
            name = GetMatrixName(mlist);

            DisplayMatrixInformation(name, mlist);
            break;

         case PRINT_MENU: /* -------------------------------------------- */

            DisplayMenu();
            break;

         case PRINT_MATRIX_LIST: /* -------------------------------------------- */

            DisplayMatrixList(mlist);
            break;

         case QUIT: /* -------------------------------------------- */

            break;

         default: /* -------------------------------------------- */

            std::cout << "invalid menu choice.\n";
            break;
      }

      if (choice == QUIT)
         break;

      /* Get user's choice. */
      std::cout << "\n> ";
      std::cin >> choice;
      std::cin.ignore(MAX_STRLEN, '\n');
   }

   return 0;
}

/*****************************************************************************
 * DisplayMatrixList - Prints out the list of matrices. Just the names,
 *       not the entire matrices.
 *
 *    Assumptions: No element of the matrix has 6 digits to the left of the
 *          decimal point.
 */
void DisplayMatrixList (const MList* mlist)
{
   std::cout << std::left;

   /* Unicode symbols for the nice boundaries. */
   const std::string HORI_BAR = u8"\u2500";
   const std::string VERT_BAR = u8"\u2502";
   const std::string TOP_CROSS_BAR = u8"\u252c";
   const std::string BOTTOM_CROSS_BAR = u8"\u2534";
   const std::string CORNER_TOP_LEFT = u8"\u250c";
   const std::string CORNER_TOP_RIGHT = u8"\u2510";
   const std::string CORNER_BOTTOM_LEFT = u8"\u2514";
   const std::string CORNER_BOTTOM_RIGHT = u8"\u2518";

   /* Print the top border. */
   std::cout << CORNER_TOP_LEFT;
   for (unsigned i = 0; i < 10; ++i)
   {
      std::cout << HORI_BAR << TOP_CROSS_BAR << HORI_BAR << HORI_BAR
                << HORI_BAR << HORI_BAR;
      if (i != 9)
         std::cout << TOP_CROSS_BAR;
   }
   std::cout << CORNER_TOP_RIGHT << "\n";

   /* Print the list of matrices. */
   std::cout << VERT_BAR;
   for (unsigned i = 0; i < 10; ++i)
   {
      std::cout << i << VERT_BAR << " " << std::setw(2)
                << mlist[i].first << " " << VERT_BAR;
   }

   /* Print the bottom border. */
   std::cout << "\n" << CORNER_BOTTOM_LEFT;
   for (unsigned i = 0; i < 10; ++i)
   {
      std::cout << HORI_BAR << BOTTOM_CROSS_BAR << HORI_BAR << HORI_BAR
                << HORI_BAR << HORI_BAR;
      if (i != 9)
         std::cout << BOTTOM_CROSS_BAR;
   }
   std::cout << CORNER_BOTTOM_RIGHT << "\n";
}

/*****************************************************************************
 * DisplayMenu - Prints out the program menu.
 */
void DisplayMenu()
{
   std::cout << ADD_MATRIX_TO_LIST << ": add matrix to list (manually)\n"
             << PGM_MATRIX_TO_LIST << ": add matrix to list (from pgm file)\n"
             << REMOVE_MATRIX_FROM_LIST << ": remove matrix from list\n"
             << DISPLAY_MATRIX << ": display matrix\n"
             << ADD_MATRICES << ": add matrices\n"
             << MULTIPLY_MATRIX_BY_SCALAR << ": multiply matrix by a scalar\n"
             << ROTATE_MATRIX << ": rotate a matrix 90 degrees clockwise\n"
             << BLUR_MATRIX << ": blur a matrix\n"
             << MULTIPLY_ROW_BY_SCALAR << ": multiply a row of a matrix by a "
                     << "scalar\n"
             << ADD_SCALAR_ROW_TO_ROW << ": add a scalar multiple of a row to "
                     << "another row of a matrix\n"
             << DISPLAY_INFORMATION << ": matrix information\n"
             << PRINT_MENU << ": print this menu\n"
             << PRINT_MATRIX_LIST << ": print matrix list\n"
             << EXPORT_TO_FILE << ": export matrix to file\n"
             << QUIT << ": quit\n";
}

/*****************************************************************************
 * AddMatrixToList - Adds a matrix to the list of matrices.
 *       Puts the matrix in the first empty spot in the list.
 *       If there is no empty spot, nothing happens.
 */
void AddMatrixToList(const std::string& name, const Matrix& M,
      MList * const mlist)
{
   for (unsigned i = 0; i < 10; ++i)
   {
      if (mlist[i].first == "")
      {
         mlist[i].first = name;
         mlist[i].second = M;
         break;
      }
   }
}

/*****************************************************************************
 * NameExists - Checks to see if a matrix with the name 'name' exists in
 *       the list of matrices.
 */
bool NameExists(const std::string& name, const MList * mlist)
{
   int mloc = IndexOfMatrix(name, mlist);

   if (mloc != -1)
      return true;

   return false;
}

/*****************************************************************************
 * RemoveMatrixFromList - Looks through the list of matrices for a matrix
 *       with name 'name' and removes it from the list.
 *       If the 'name' isn't found, nothing happens.
 */
void RemoveMatrixFromList(const std::string& name, MList * const mlist)
{
   int mloc = IndexOfMatrix(name, mlist);

   mlist[mloc].first = "";
   mlist[mloc].second = Matrix();
}

/*****************************************************************************
 * DisplayMatrix - Prints out the matrix with name 'name' from the list
 *       of matrices.
 */
void DisplayMatrix(const std::string& name, const MList * mlist)
{
   int mloc = IndexOfMatrix(name, mlist);

   std::cout << mlist[mloc].second;
}

/*****************************************************************************
 * NewMatrixName - Generates a new matrix name that can be put into the list
 *       of matrices without causing any name conflicts.
 */
std::string NewMatrixName(const MList * mlist)
{
   std::string name;

   /* Get the desired matrix name. */
   std::cin >> name;
   std::cin.ignore(MAX_STRLEN, '\n');

   /* Ensure a matrix with said name does not exist. */
   while ( (name.length() < 1) || (name.length() > 2)
         || NameExists(name, mlist) )
   {
      std::cout << "invalid name. name cannot exist and can only "
                << "have 1 or 2 characters.\n"
                << "re-enter: ";
      std::cin >> name;
      std::cin.ignore(MAX_STRLEN, '\n');
   }

   return name;
}

/*****************************************************************************
 * GetMatrixName - Returns the name of a matrix in the list.
 */
std::string GetMatrixName(const MList * mlist)
{
   std::string name;

   /* Get the desired matrix name. */
   std::cin >> name;
   std::cin.ignore(MAX_STRLEN, '\n');

   /* Ensure a matrix with said name exists. */
   while ( !NameExists(name, mlist) )
   {
      std::cout << "invalid name. name must exist.\n";
      std::cout << "re-enter: ";
      std::cin >> name;
      std::cin.ignore(MAX_STRLEN, '\n');
   }

   return name;
}

/*****************************************************************************
 * GetValidRow - Ensures that only valid indices of matrix with name 'name'
 *       are returned.
 */
unsigned GetValidRow(const std::string& name, const MList * mlist)
{
   int mloc = IndexOfMatrix(name, mlist);
   unsigned rowIndex;

   /* Get the desired row index. */
   std::cin >> rowIndex;
   std::cin.ignore(MAX_STRLEN, '\n');

   /* Ensure the row is valid for the given matrix. */
   while (rowIndex < 1 || rowIndex > mlist[mloc].second.NumRows())
   {
      std::cout << "invalid row index. index must exist.\n";
      std::cout << "re-enter: ";
      std::cin >> rowIndex;
      std::cin.ignore(MAX_STRLEN, '\n');
   }

   return rowIndex;
}

/*****************************************************************************
 * DisplayMatrixInformation - Prints out useful information about the  matrix
 *       with name 'name'.
 */
void DisplayMatrixInformation(const std::string& name, const MList * mlist)
{
   int mloc = IndexOfMatrix(name, mlist);

   std::cout << "Size: " << mlist[mloc].second.Size() << "\n";
}

/*****************************************************************************
 * IndexOfMatrix - Returns the index of the matrix with name 'name' in the
 *       list of matrices.
 */
int IndexOfMatrix(const std::string& name, const MList * mlist)
{
   int mloc = -1;

   for (unsigned i = 0; i < 10; ++i)
      if (name == mlist[i].first)
         mloc = i;

   return mloc;
}

