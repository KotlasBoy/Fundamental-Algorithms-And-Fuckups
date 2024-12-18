#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum error_state{
    COOL,
    FUCKUP,
    NULL_PTR,
    WRONG_PARAMETER,
    OVERFLOW,
    MEMORY_FUCKUP,
    FILE_FUCKUP
} error_state;


double count_polynomial_in_a (double a, double* coeffs, unsigned long int max_power);
error_state parce_coefs(double a, double** result_coef, long int power, ...);

int main(void) {
    double *result;
    error_state personal_errno = COOL;

    unsigned long int power = 4;            // <== <== <== <== <== <== <== <== <== <==FIXME:   

    personal_errno = parce_coefs(2.0, &result, power, 3.0, 3.0, 3.0, 3.0, 3.0);
    switch (personal_errno) {           
        case MEMORY_FUCKUP:
            printf("bad alloc\n");
            break;
        case WRONG_PARAMETER:
            printf("wrong power\n");
            break;
        case COOL:
            for (unsigned int i = 0; i <= power; ++i)
                printf("%lf ", result[i]);
            printf("\n");
            break;
        default: break;
    }

    free(result);
    return COOL;

}


error_state parce_coefs(double a, double** result_coef, long int power, ...) {

    double current_coefficient = 0.0;

    if (power < 0)
        return WRONG_PARAMETER;

    *result_coef = (double*) malloc(sizeof(double) * (power + 1));        
    if (!(*result_coef))
        return MEMORY_FUCKUP;
    
    double *initial_coef = (double*) malloc(sizeof(double) * (power + 1));
    if (!initial_coef) {
        free(result_coef);                                        // free(res)
        return MEMORY_FUCKUP;
    }
    
    va_list(item);
    va_start(item, power);

    for (unsigned int i = 0; i <= power; ++i){
        current_coefficient = va_arg(item, double);
        initial_coef[i] = current_coefficient;
    }

    va_end(item);

    unsigned long int i = 0;
    double factorial_coef = 1.0;
    while(power >= 0) {
        (*result_coef)[i] = count_polynomial_in_a(a, initial_coef, power);

        if (i > 1) {
            factorial_coef *= i;
            (*result_coef)[i] /= factorial_coef;
        }

        --power;
        for(long int j = 0; j <= power; ++j) {
            double init_k = initial_coef[j + 1];
            initial_coef[j] = init_k * (j + 1);
        }

        ++i;

    }

    free(initial_coef);
    return COOL;
}

double count_polynomial_in_a (double a, double* coeffs, unsigned long int max_power) {
    double result = 0.0, current_function_value = 1.0;

    for(unsigned long int i = 0; i <= max_power; ++i){ 
        result += coeffs[i] * current_function_value;
        current_function_value *= a;
    }

    return result;
}

