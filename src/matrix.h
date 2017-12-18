/*
*	The MIT License (MIT)
*
*	Copyright (c) 2017 Hirad Asadi
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in
*	all copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*	THE SOFTWARE.
*
*/

#pragma once

#include <iostream>

namespace math
{
	// Class for storing a matrix
	template <typename T>
	class matrix
	{
	public:
		matrix<T>(std::size_t rows, std::size_t columns);
		matrix<T>(T** mat, std::size_t rows, std::size_t columns);
		matrix<T>(const matrix<T>& mat);
		virtual ~matrix<T>();

		matrix<T> operator+(const matrix<T>& n);
		matrix<T> operator-(const matrix<T>& n);
		matrix<T> operator*(const matrix<T>& n);
		T* operator[](std::size_t row);
		matrix<T>& operator=(const matrix<T>& mat);
		matrix<T> scalar(const T s);
		matrix<T> transpose();		
		T sum();
		double avg();
		std::size_t nr_of_rows() const;
		std::size_t nr_of_columns() const;
		void print();

	private:
		std::size_t _rows_size = 0;
		std::size_t _columns_size = 0;
		T** _matrix = NULL;

		void init();
		void clear();
		void copy_matrix(T** mat);
	};

	// Default constructor
	template<typename T> 
	matrix<T>::matrix(std::size_t rows, std::size_t columns) : 
		_rows_size(rows), _columns_size(columns)
	{
		if (_matrix != NULL)
			clear();

		init();
	}

	// Constructor
	template<typename T>
	matrix<T>::matrix(T ** mat, std::size_t rows, std::size_t columns) : 
		matrix<T>(rows, columns)
	{
		// Copy matrix to initialized memory
		copy_matrix(mat);
	}

	// Copy constructor
	template<typename T>
	matrix<T>::matrix(const matrix<T>& mat) : matrix<T>(mat._matrix, mat._rows_size, mat._columns_size)
	{			
	}

	// Destructor
	template<typename T>
	matrix<T>::~matrix()
	{
		clear();
	}

	// Copy internal matrix
	template<typename T>
	inline void matrix<T>::copy_matrix(T ** mat)
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");

		for (std::size_t i = 0; i < _rows_size; i++)
		{
			for (std::size_t j = 0; j < _columns_size; j++)
			{
				_matrix[i][j] = mat[i][j];
			}
		}
	}

	// Initializes matrix
	template<typename T>
	inline void matrix<T>::init()
	{
		// Alloc and init memory
		_matrix = new T*[_rows_size];

		for (std::size_t r = 0; r < _rows_size; r++)
		{
			_matrix[r] = new T[_columns_size];
		}

		// Set default values
		for (std::size_t i = 0; i < _rows_size; i++)
		{
			for (std::size_t j = 0; j < _columns_size; j++)
			{
				_matrix[i][j] = T();
			}
		}
	}

	// Clears matrix memory
	template<typename T>
	inline void matrix<T>::clear()
	{
		if (_matrix == NULL)
			return;

		for (std::size_t i = 0; i < _rows_size; i++)
			if (_matrix[i])
				delete _matrix[i];
		if (_matrix)
			delete[] _matrix;

		_matrix = NULL;
		_rows_size = 0;
		_columns_size = 0;
	}

	// Matrix addition operator
	template<typename T>
	inline matrix<T> matrix<T>::operator+(const matrix<T>& n)
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");

		if (this->_rows_size != n._rows_size || this->_columns_size != n._columns_size)
			throw std::exception("Dimension error! Number of columns and rows don't match.");

		// Init new matrix
		matrix<T> res(_rows_size, _columns_size);

		// Perform operation
		for (size_t i = 0; i < _rows_size; i++)
		{
			for (size_t j = 0; j < _columns_size; j++)
			{
				res[i][j] = _matrix[i][j] + n._matrix[i][j];
			}
		}

		return res;
	}

	// Matrix substraction operator
	template<typename T>
	inline matrix<T> matrix<T>::operator-(const matrix<T>& n)
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");

		if (this->_rows_size != n._rows_size || this->_columns_size != n._columns_size)
			throw std::exception("Dimension error! Number of columns and rows don't match.");

		// Init new matrix
		matrix<T> res(_rows_size, _columns_size);

		// Perform operation
		for (size_t i = 0; i < _rows_size; i++)
		{
			for (size_t j = 0; j < _columns_size; j++)
			{
				res[i][j] = _matrix[i][j] - n._matrix[i][j];
			}
		}

		return res;
	}

	// Matrix dot product
	template<typename T>
	inline matrix<T> matrix<T>::operator*(const matrix<T>& n)
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");

		if (_columns_size != n._rows_size)
			throw std::exception("Dimension error! Number of columns in m-matrix don't match rows in n-matrix.");		

		// Init new matrix
		matrix<T> res(_rows_size, n._columns_size);

		// Perform operation
		for (std::size_t i = 0; i < _rows_size; i++)
		{
			for (std::size_t j = 0; j < n._columns_size; j++)
			{
				for (std::size_t inner = 0; inner < _rows_size - 1; inner++)
				{
					res[i][j] += _matrix[i][inner] * n._matrix[inner][j];
				}				
			}
		}

		return res;
	}

	// Matrix scalar operation
	template<typename T>
	inline matrix<T> matrix<T>::scalar(const T s)
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");

		matrix<T> res = *this;

		// Perform operation
		for (size_t i = 0; i < res._rows_size; i++)
		{
			for (size_t j = 0; j < res._columns_size; j++)
			{
				res[i][j] *= s;
			}
		}

		return res;
	}

	// Matrix transpose operation
	template<typename T>
	inline matrix<T> matrix<T>::transpose()
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");

		matrix<T> res(_columns_size, _rows_size);

		// Perform operation
		for (size_t i = 0; i < res._rows_size; i++)
		{
			for (size_t j = 0; j < res._columns_size; j++)
			{
				res[i][j] = _matrix[j][i];
			}
		}

		return res;
	}

	// Sum function
	template<typename T>
	inline T matrix<T>::sum()
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");
		
		T sum = 0;

		for (std::size_t i = 0; i < _rows_size; i++)
		{
			for (std::size_t j = 0; j < _columns_size; j++)
			{
				sum += _matrix[i][j];
			}
		}

		return sum;
	}

	// Average function
	template<typename T>
	inline double matrix<T>::avg()
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");
		
		std::size_t items = _rows_size * _columns_size;

		if (items == 0)
			throw std::exception("Division by zero.");

		return sum() / items;
	}

	// Matrix index operator overload
	template<typename T>
	inline T* matrix<T>::operator[](std::size_t row)
	{
		if (_matrix == NULL)
			throw std::exception("Matrix not initialized.");

		if (row < 0 || row >= _rows_size)
			throw std::exception("Index out of range!");

		return _matrix[row];
	}

	// Matrix assignment operator
	template<typename T>
	inline matrix<T>& matrix<T>::operator=(const matrix<T>& mat)
	{
		if (_matrix != NULL)
			clear();

		if (this == &mat)
			return *this;

		_rows_size = mat._rows_size;
		_columns_size = mat._columns_size;

		init();

		// Copy matrix to initialized memory
		copy_matrix(mat._matrix);

		return *this;
	}

	// Returns number of rows in matrix
	template<typename T>
	inline std::size_t matrix<T>::nr_of_rows() const
	{
		return _rows_size;
	}

	// Returns number of columns in matrix
	template<typename T>
	inline std::size_t matrix<T>::nr_of_columns() const
	{
		return _columns_size;
	}

	// Prints the matrix
	template<typename T>
	inline void matrix<T>::print()
	{
		if (_matrix == NULL)
			return;

		for (std::size_t i = 0; i < _rows_size; i++)
		{
			for (std::size_t j = 0; j < _columns_size; j++)
			{
				std::cout << _matrix[i][j] << " ";
			}

			std::cout << std::endl;
		}
	}
}

