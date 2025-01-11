#define S21_OK 0
#define S21_INVALID_MATRIX 1
#define S21_CALCULATION_ERROR 2

#define SUCCESS 1
#define FAILURE 0
#define S21_MATRIX_EQ SUCCESS
#define S21_MATRIX_NOT_EQ FAILURE 

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t* result);
int s21_create_matrix_fill(int rows, int columns, matrix_t* result, 
    double value);
void s21_remove_matrix(matrix_t* A);
int s21_is_matrix_valid(matrix_t* A);

int s21_eq_matrix(matrix_t* A, matrix_t* B);

typedef int (*s21_op_element)(double a, double b, double* result);
int s21_add_element(double a, double b, double* result);
int s21_sub_element(double a, double b, double* result);

int s21_op_matrix(matrix_t* A, matrix_t* B, matrix_t* result, 
    s21_op_element op);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
