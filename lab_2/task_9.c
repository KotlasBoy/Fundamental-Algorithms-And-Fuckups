#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <limits.h>

#define MY_EPS 0.00000000001
#define RESULT_SIZE 10000

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



void my_swap(unsigned long int* a, unsigned long int* b);
unsigned long int my_gcd(unsigned long int a, unsigned long int b);
error_state parce_fractions(int* result_massive, unsigned long int amount_of_fractions, unsigned long base, ...);
error_state is_finite_fraction(unsigned long int base, double fraction);


int main(void){

    // int result_massive[RESULT_SIZE];                                   
    // unsigned long int amount_of_fractions = 1;      //   <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <--КОЛИЧЕСТВО ПАРАМЕТРОВ 
    // unsigned long int base = 1;                     // СС
    // error_state personal_errno = parce_fractions(result_massive, amount_of_fractions, base, 0.3);
    // switch(personal_errno){
    //     case WRONG_PARAMETER:
    //         printf("Wrong amount of fractions or incorrect base\n");
    //         break;
    //     case MEMORY_FUCKUP:
    //         printf("Too many fractions, error overcome\n");
    //         break;
    //     case COOL:
    //         for(unsigned int i = 0; i < amount_of_fractions; ++i){
    //             printf("Is fraction %-2d finite? Answer: %d\n", i + 1, result_massive[i]);
    //         }
    //         break;
    //     default: break;
    // }

    int result_massive[RESULT_SIZE];                                   
    unsigned long int amount_of_fractions = 1;      //   <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <--КОЛИЧЕСТВО ПАРАМЕТРОВ 
    unsigned long int base = 30;                     // СС
    error_state personal_errno = parce_fractions(result_massive, amount_of_fractions, base, 0.127);
    switch(personal_errno){
        case WRONG_PARAMETER:
            printf("Wrong amount of fractions or incorrect base\n");
            break;
        case MEMORY_FUCKUP:
            printf("Too many fractions, error overcome\n");
            break;
        case COOL:
            for(unsigned int i = 0; i < amount_of_fractions; ++i){
                printf("Is fraction %-2d finite? Answer: %d\n", i + 1, result_massive[i]);
            }
            break;
        default: break;
    }

    return COOL;
}

error_state parce_fractions(int* result_massive, unsigned long int amount_of_fractions, unsigned long base, ...){
    if (amount_of_fractions < 1 || base < 2 || base > 10000) {
        return WRONG_PARAMETER;
    }

    va_list(item);
    va_start(item, base);
    double current_fraction = 0.0;
    error_state personal_errno = COOL;


    for(unsigned int i = 0; i < amount_of_fractions; ++i){
        current_fraction = va_arg(item, double);

        if ((current_fraction > 1.0 - MY_EPS) || (current_fraction < MY_EPS)){
            if (i >= RESULT_SIZE)
                return MEMORY_FUCKUP;

            result_massive[i] = -1;
            continue;
        }

        personal_errno = is_finite_fraction(base, current_fraction);
        if (i >= RESULT_SIZE)
                return MEMORY_FUCKUP;

        result_massive[i] = (personal_errno == COOL) ? 1 : 0;
    }

    va_end(item);
    return COOL;
}


error_state is_finite_fraction(unsigned long int base, double fraction){        
    unsigned long int numerator = 1, denominator = 1;
    unsigned long int i = 2;

    while(fabs(fraction - floor(fraction)) > MY_EPS && denominator <= UINT_MAX / 10){
        fraction *= 10;
        denominator *= 10;
    }

    numerator = (unsigned long int) fraction;
    denominator /= my_gcd(numerator, denominator);

    if (((denominator & 1) == 0) && (base & 1))       // чётный знаменатель и нечётная сс
        return FUCKUP;

    while ((denominator & 1) == 0)            
        denominator /= 2;

    for (i = 3; i < base + 1; i += 2){                  // проверка простых чисел как множителей знаменателя и сс
        if ((denominator % i == 0) && (base % i != 0))       
            return FUCKUP;

        while (denominator % i == 0)
            denominator /= i;
    }

    if (denominator != 1)
        return FUCKUP;

    return COOL;

}


unsigned long int my_gcd(unsigned long int a, unsigned long int b){
    //unsigned int result = 0;
    if (b > a)
        my_swap(&a, &b);

    while(b > 0){
        a %= b; 

        if(b > a)
            my_swap(&a, &b);
    }

    return a;
}

void my_swap(unsigned long int* a, unsigned long int* b){
    unsigned long int tmp = *b;
    *b = *a;
    *a = tmp;
}



