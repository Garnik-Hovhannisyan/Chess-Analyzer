#ifndef MATRIX_H
#define MATRIX_H

#include <memory>
#include <vector>

class ChessFigures;

class Matrix
{
protected:
	int m_rows;
	int m_cols;
    std::vector<std::vector<std::unique_ptr<ChessFigures>>> m_matrix;

public:
    Matrix();
    Matrix(int rows, int cols);

    Matrix(const Matrix& other) = delete;
    Matrix& operator=(const Matrix& other) = delete;

    Matrix(Matrix&& other) = default;
    Matrix& operator=(Matrix&& other) = default;

    virtual ~Matrix() = default;

public:
    ChessFigures* getAt(int r, int c) const;
    void setAt(int r, int c, std::unique_ptr<ChessFigures> value);

	virtual void reset() = 0;
	virtual void print() = 0;
};

#endif // MATRIX_H