#include <stdio.h>
#include <stdarg.h>
#include <math.h>

typedef enum my_bool{
    INIT = -1,
    FALSE,
    TRUE
} my_bool;

typedef enum error_state{
    COOL,
    FUCKUP,
    NULL_PTR,
    WRONG_PARAMETER,
    OVERFLOW,
    MEMORY_FUCKUP,
    FILE_FUCKUP
} error_state;


error_state geometric_mean(double* result, int amount, ...);
void fast_pow_recursive(double, long int, double*);

int main(void){
    double res = 0.;
    if(geometric_mean(&res, 3, 2.0, 4.0, 8.0) != COOL)
        printf("FUCKUP\n");
    else
        printf("result is %lf\n", res);


    fast_pow_recursive(2000.56, 12, &res);

    printf("%lf ^ %d = %lf\n", 2000.56, 12, res);
    return COOL;
}
 

error_state geometric_mean(double* result, int amount, ...){
    if(result == NULL)
        return NULL_PTR;

    if (amount <= 1)
        return WRONG_PARAMETER;

    *result = 1.;
    double current_num = 0.;
    va_list item_ptr;
    va_start(item_ptr, amount);


    for (int i = 0; i < amount; ++i){
        current_num = va_arg(item_ptr, double);
        *result *= current_num;
    }

    *result = pow(*result, 1.0 / amount);
    
    va_end(item_ptr);
    
    return COOL;
}


void fast_pow_recursive(double number, long int power, double* result) {
    if (power == 0) {  
        *result = 1.0;
    }
    else if (power < 0) {  
        fast_pow_recursive(number, -power, result);
        *result = 1.0 / *result; 
        return;
    }

    else if (power % 2 == 0) {
        fast_pow_recursive(number * number, power / 2, result);
    } else {  
        double temp = 0.0;
        fast_pow_recursive(number * number, power / 2, &temp);
        *result = temp * number;
    }
}
