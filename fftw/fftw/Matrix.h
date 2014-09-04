// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__D58D0A47_68B4_11D6_AD90_00B0D0652E95__INCLUDED_)
#define AFX_MATRIX_H__D58D0A47_68B4_11D6_AD90_00B0D0652E95__INCLUDED_

#pragma once

#include "Sigdefine.h"
#include <string>
using namespace std;
// 
// Class : CMatrix
// Written by : R.I.Allen
// 
// This is an encapsulation of a matrix class that allows all standard operations to be
// performed on it. It also includes some odd procedures for importing and exporting matrices.
// 
// Data is stored for the matrix in a flat allocation with 1 additional entry at the end which is
// used as a reference counter for how many objects are using that data section. When one matrix
// differs from another when referencing the same memory, a copy is made at that point, and they
// go their separate ways. This is very useful when returing CMatrix objects which then need to be
// assigned as it avoid additional memory allocation/deallocation calls.
//

class CLASS_DLL_DSE_SigMath CMatrix 
{
public:
	// construction and destruction
					CMatrix() ;							// default constructor
					CMatrix(const CMatrix &other) ;		// copy constructor
					CMatrix(int nCols, int nRows) ;		// constructs an empty matrix of this size
					CMatrix(int size, bool set_diagonal = true) ;	// creates a square matrix
	virtual			~CMatrix();							// destructor
//	void			Serialize(CArchive& archive) ;		// serialization
    
	// matrix mathematical operations
	CMatrix&		operator=(const CMatrix &other) ;
	CMatrix			operator+(const CMatrix &other) const ;
	CMatrix			operator-(const CMatrix &other) const ;
	CMatrix			operator*(const CMatrix &other) const ;
	void			operator+=(const CMatrix &other) ;
	void			operator-=(const CMatrix &other) ;
	void			operator*=(const CMatrix &other) ;
	void			operator*=(double value) ;
	friend CMatrix	operator*(const CMatrix &other, double value) ;
	bool			operator==(const CMatrix &other) const ;
	const double*	operator[](int nRow) const ;			// reading version
	double*     	operator[](int nRow) ;					// writing version
	double&         operator()(const unsigned int& nRow, const unsigned int& nCol);

	// element access
	bool			SetElement(int nCol, int nRow, double value) ;
	double			GetElement(int nCol, int nRow) const ;
	inline int		GetColumns() const { return m_NumColumns ; } ;
	inline int		GetRows() const { return m_NumRows ; } ;
	double			SumColumn(int col) const ;
	double			SumRow(int row) const ;
	double			SumColumnSquared(int col) const ;
	double			SumRowSquared(int row) const ;
	double			GetRowMin(int row) const ;
	double			GetRowMax(int row) const ;
	double			GetColumnMin(int col) const ;
	double			GetColumnMax(int col) const ;
	///得到矩阵行列式。该矩阵必须是方阵
	double          GetDeterminant();
	// matrix transposition
	CMatrix			GetTransposed() const ;
	void			Transpose() ;
	// matrix inversion
	CMatrix			GetInverted() const ;
	void			Invert() ;
	// covariant (A' * A)
	CMatrix			Covariant() const ;
	// normalisation
	CMatrix			GetNormalised(double min, double max) const ;
	void			Normalise(double min, double max) ;
	// ranges functions
	void			GetNumericRange(double &min, double &max) const ;
	// matrix concatenation
	CMatrix			GetConcatinatedColumns(const CMatrix& other) const ;
	void			ConcatinateColumns(const CMatrix &other) ;
	CMatrix			GetConcatinatedRows(const CMatrix& other) const ;
	void			ConcatinateRows(const CMatrix &other) ;
	// adds an new row / column to the matrix
	void			AddColumn(const double *pData) ;
	void			AddRow(const double *pData) ;
	// sub matrix extraction, setting
	CMatrix			ExtractSubMatrix(int col_start, int row_start, int col_size, int row_size) const ;
	void			SetSubMatrix(int col_start, int row_start, const CMatrix &other) ;
	CMatrix			ExtractDiagonal() const ;
	// squaring the matrix functions
	CMatrix			GetSquareMatrix() const ;
	void			MakeSquare() ;

	//// KRON(X,Y) is the Kronecker tensor product of X and Y.
	//   The result is a large matrix formed by taking all possible
	//   products between the elements of X and those of Y. For
	//   example, if X is 2 by 3, then KRON(X,Y) is
	//      [ X(1,1)*Y  X(1,2)*Y  X(1,3)*Y
	//        X(2,1)*Y  X(2,2)*Y  X(2,3)*Y ]
	CMatrix         Kron(const CMatrix& mX_, const CMatrix& mY_);
	// helper functions
	string   		GetRowAsText(int row) const ;
private:
	// internal variables
	int				m_NumColumns ;			// number of columns in matrix
	int				m_NumRows ;				// number of rows in matrix
	double*			m_pData ;				// pointer to data, may be shared among objects
private:
	// private internal functions
	double*			AllocateMemory(int nCols, int nROws) ;
	// reference counting functions
	void			IncrementReferenceCount() ;	// increments the m_pData reference count
	void			DecrementReferenceCount() ;	// decrements the m_pData reference count
	void			DecrementAndRelease() ;		// decrements the count and releases the memory if required
	int				GetReferenceCount() const ;	// returns the m_pData's reference count
	///矩阵转换为数组
	void MatrixToArray(const CMatrix& a_, double * pdata_);
	///矩阵必须提前分配好
	void ArrayToMatrix(const double * pdata_, CMatrix& a_);

};


#endif // !defined(AFX_MATRIX_H__D58D0A47_68B4_11D6_AD90_00B0D0652E95__INCLUDED_)
