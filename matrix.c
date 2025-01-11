#include "matrix.h"

#include <stdlib.h>

int s21_create_matrix(int rows, int columns, matrix_t* result) {
  if (rows <= 0 || columns <= 0 || !result) {
    // TODO: is this the right return code?
    return S21_INVALID_MATRIX;
  }

  int error_code = S21_OK;

  result->rows = rows;
  result->columns = columns;
  result->matrix = calloc(rows, sizeof(double*));
  for (int i = 0; error_code == S21_OK && i < rows; ++i) {
    result->matrix[i] = calloc(columns, sizeof(double));
    if (!result) {
      error_code = S21_CALCULATION_ERROR;
    }
  }

  return error_code;
}

int s21_create_matrix_fill(int rows, int columns, matrix_t* result, 
    double value) {
  int error_code = s21_create_matrix(rows, columns, result);

  for (int i = 0; error_code == S21_OK && i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      result->matrix[i][j] = value;
    }
  }

  return error_code;
}

void s21_remove_matrix(matrix_t* A) {
  for (int i = 0; i < A->rows; ++i) {
    free(A->matrix[i]);
  }
  free(A->matrix);
  A->matrix = NULL;
}

int s21_is_matrix_valid(matrix_t* A) {
  if (!A || A->rows <= 0 || A->columns <= 0) {
    return S21_INVALID_MATRIX;
  }
  
  return S21_OK;
}

int s21_eq_matrix(matrix_t* A, matrix_t* B) {
  if (s21_is_matrix_valid(A) == S21_INVALID_MATRIX 
      || s21_is_matrix_valid(B) == S21_INVALID_MATRIX) {
    return S21_INVALID_MATRIX;
  }

  int is_eq = S21_MATRIX_EQ;
  if (A->rows != B->rows || A->columns != B->columns) {
    is_eq = S21_MATRIX_NOT_EQ;
  }

  for (int i = 0; is_eq == S21_MATRIX_EQ && i < A->rows; ++i) {
    for (int j = 0; is_eq == S21_MATRIX_EQ && j < A->columns; ++j) {
      if (A->matrix[i][j] != B->matrix[i][j]) {
        is_eq = S21_MATRIX_NOT_EQ;
      }
    }
  }

  return is_eq;
}

int s21_add_element(double a, double b, double* result) {
  if (!result) {
    return S21_CALCULATION_ERROR;
  }
  *result = a + b;
  return S21_OK;
}

int s21_sub_element(double a, double b, double* result) {
  return s21_add_element(a, -b, result);
}

int s21_op_matrix(matrix_t* A, matrix_t* B, matrix_t* result, 
    s21_op_element op) {
  if (s21_is_matrix_valid(A) == S21_INVALID_MATRIX 
      || s21_is_matrix_valid(B) == S21_INVALID_MATRIX) {
    return S21_INVALID_MATRIX;
  }

  int error_code = S21_OK;
  
  if (A->rows != B->rows || A->columns != B->columns) {
    error_code = S21_CALCULATION_ERROR;
  }

  if (error_code == S21_OK) {
    error_code = s21_create_matrix(A->rows, A->columns, result);
  }

  for (int i = 0; error_code == S21_OK && i < A->rows; ++i) {
    for (int j = 0; error_code == S21_OK && j < A->columns; ++j) {
      error_code = op(A->matrix[i][j], B->matrix[i][j], &result->matrix[i][j]);
    }
  }

  return error_code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_op_matrix(A, B, result, s21_add_element);
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_op_matrix(A, B, result, s21_sub_element);
}
