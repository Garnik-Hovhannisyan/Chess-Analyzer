#include "Matrix.h"
#include "ChessFigures.h"
#include <stdexcept>

Matrix::Matrix() : m_rows(0), m_cols(0) {}

Matrix::Matrix(int rows, int cols) : m_rows(rows), m_cols(cols)
{
	m_matrix.resize(m_rows);
	for (int i = 0; i < m_rows; ++i) m_matrix[i].resize(m_cols);
}

void Matrix::setAt(int r, int c, std::unique_ptr<ChessFigures> value)
{
	if (r < 0 || r >= m_rows || c < 0 || c >= m_cols)
	{
		throw std::out_of_range("Matrix::setAt() - index out of range");
	}

	m_matrix[r][c] = std::move(value);
}

ChessFigures* Matrix::getAt(int r, int c) const
{
	if (r < 0 || r >= m_rows || c < 0 || c >= m_cols)
	{
		throw std::out_of_range("Matrix::getAt() - index out of range");
	}

	return m_matrix[r][c].get();
}