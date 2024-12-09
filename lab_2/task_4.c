#include <stdio.h>
#include <math.h>
#include <stdarg.h>     //va_list, va_arg, etc
#include <limits.h>
#include <ctype.h>      //toupper, isdigit
#include <stdlib.h>     //labs, 

#define EPSILON 0.00000000000001
#define BUFFER_SIZE 75


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

error_state first_part(my_bool * result, int count, ...);

my_bool is_positive_pseudo_skalyar_product(double one_proj_x, double one_proj_y, double two_proj_x, double two_proj_y);

error_state second_part(double* result, long int amount, double x_point, long int power, ...);
void fast_pow (double number, long int power, double* result);


error_state fird_part(int count, int base, ...);

error_state to_decimal_num(char* word, long int* res, int base);
void recursive_print_num_in_base(long int init_number, int base);



//TOD: second subtask ASAP
int main(void){
    my_bool result = INIT;
    switch(first_part( &result, 6, 0., 2., 5., 0., 3., -3.)){      //  (&result, counter, x1, y1, x2, y2, x3, y3, ...)
        case NULL_PTR:
            printf("WP\n");
            break;
        case WRONG_PARAMETER:
            printf("WP\n");
            break;
        case COOL:
            printf("TRUE\n");
            break;
        case FUCKUP:
            printf("FALSE\n");
            break;
        default:
            break;
    }

    result = INIT;
    switch(first_part( &result, 10, 1., 1., 2., 2., 5., 0., 3., -2., 1., 0.)){      //  (&result, counter, x1, y1, x2, y2, x3, y3, ...)
        case NULL_PTR:
            printf("WP\n");
            break;
        case WRONG_PARAMETER:
            printf("WP\n");
            break;
        case COOL:
            printf("TRUE\n");
            break;
        case FUCKUP:
            printf("FALSE\n");
            break;
        default:
            break;
    }

    result = INIT;
    switch(first_part( &result, 10, 1., 1., 2., 2., 5., 0., 1., -4., 1., 0.)){      //  (&result, counter, x1, y1, x2, y2, x3, y3, ...)
        case NULL_PTR:
            printf("WP\n"); 
            break;
        case WRONG_PARAMETER:
            printf("WP\n");
            break;
        case COOL:
            printf("TRUE\n");
            break;
        case FUCKUP:
            printf("FALSE\n");
            break;
        default:
            break;
    }

    //////////////////////////////////////////////////////

    double second_result = 0.0;
    switch(second_part( &second_result, 4, 5., 2., 4., 3., 2., 1.)){       //117,2
        case NULL_PTR:
            printf("NULL_PTR found\n");
            break;
        case COOL:
            printf("second function:  %lf\n", second_result);
            break;
        default:
            break;
    }

    switch(second_part( &second_result, 5, 3., 7., -10., 3., 4., -1., 6.)){       //-18 630
        case NULL_PTR:
            printf("NULL_PTR found\n");
            break;
        case COOL:
            printf("second function:  %lf\n", second_result);
            break;
        default:
            break;
    }



    /////////////////////////////////////////////////////

    switch(fird_part(4, 12, "9", "56", "66", "AA")){        //base = 12, true:   56, 66,           
        case NULL_PTR:
            printf("shit found\n");
            break;
        case COOL:
            break;
        default:
            break;
    }

    switch(fird_part(6, 16, "1234", "67FDFD", "AB", "CD", "CCC", "444")){        //base = 16, true: AB, CD, CCC,
        case NULL_PTR:
            printf("shit found\n");
            break;
        case COOL:
            break;
        default:
            break;
    }

    switch(fird_part(6, 10, "297", "67342", "99", "DFSG", "1", "0")){        //base = 10, TRUE: 297, 99, 1
        case NULL_PTR:
            printf("shit found\n");
            break;
        case COOL:
            break;
        default:
            break;
    }


    return COOL;
}


error_state  first_part(my_bool * result, int count, ...){
    if(!result){
        return NULL_PTR;
    }
    if (count < 6 || count % 2 != 0){
        return WRONG_PARAMETER; 
    }

    double a_x = 0.0, a_y = 0.0;
    double b_x = 0.0, b_y = 0.0;
    double c_x = 0.0, c_y = 0.0;
    double init_x = 0.0, init_y = 0.0;
    my_bool positive_sign = INIT, tmp_sign = INIT;

    va_list(item_ptr);
    va_start(item_ptr, count);

    a_x = va_arg(item_ptr, double);
    a_y = va_arg(item_ptr, double);
    b_x = va_arg(item_ptr, double);
    b_y = va_arg(item_ptr, double);
    c_x = va_arg(item_ptr, double);
    c_y = va_arg(item_ptr, double);
    init_x = a_x;
    init_y = a_y;
    
    count -=  6;
    positive_sign = is_positive_pseudo_skalyar_product((b_x - a_x), (b_y - a_y), (c_x - b_x), (c_y - b_y));

    while(count > 0){
        a_x = b_x;
        a_y = b_y;
        b_x = c_x;
        b_y = c_y;
        c_x = va_arg(item_ptr, double);
        c_y = va_arg(item_ptr, double);
        tmp_sign = is_positive_pseudo_skalyar_product((b_x - a_x), (b_y - a_y), (c_x - b_x), (c_y - b_y));

        if (positive_sign == INIT){
            positive_sign = tmp_sign;
        }
        else{
            if (tmp_sign != INIT && positive_sign != tmp_sign){
                return FUCKUP; 
            }
        }
        count -= 2;
    }
    
    a_x = b_x;
    a_y = b_y;
    b_x = c_x;
    b_y = c_y;
    tmp_sign = is_positive_pseudo_skalyar_product((b_x - a_x), (b_y - a_y), (init_x - b_x), (init_y - b_y));
    if (positive_sign != tmp_sign){
        return FUCKUP; 
    }

    return COOL;
}

my_bool is_positive_pseudo_skalyar_product(double one_proj_x, double one_proj_y, double two_proj_x, double two_proj_y){
    return (one_proj_x * two_proj_y) - (two_proj_x * one_proj_y) > 0 ? TRUE : ( ((one_proj_x * two_proj_y) - (two_proj_x * one_proj_y) < 0) ? FALSE : INIT);
}


error_state second_part(double* result, long int amount, double x_point, long int power, ...){
    if(!result)
        return NULL_PTR;

    va_list(item_ptr);
    va_start(item_ptr, power);
    double koef = va_arg(item_ptr, double);
    double exp_x = 0.;
    *result = 0.;
    fast_pow(x_point, power, &exp_x);

    while(amount--){
        *result += koef * exp_x;
        exp_x /= x_point;
        koef = va_arg(item_ptr, double);
    }

    va_end(item_ptr);
    return COOL;

}

void fast_pow (double number, long int power, double* result){
    char sign = power > 0 ? 1 : 0;
    *result = 1.0;
    power = abs(power);
        while (power > 0){
            if(power % 2){
                *result = sign == 1 ? *result * number : *result / number;
            }
            number *= number;
            power /= 2;
        }
}



error_state fird_part(int count, int base, ...){        //char* line
    if (count < 1 || base < 2 || base > 36 ){
        return WRONG_PARAMETER;
    }

    va_list item_ptr;
    va_start(item_ptr, base);

    long int init_number = 0, left_part = 0, right_part = 0;
    int remainder = 0, power = 1;
    char* current_line = NULL;

    while(count-- > 0){
        
        current_line = va_arg(item_ptr, char*);

        error_state personal_errno = to_decimal_num(current_line, &init_number, base);    
        if (personal_errno != COOL || init_number <= 0 || LONG_MAX / init_number < init_number){
            continue;
        }
        // x ^ 2
        left_part = init_number * init_number;
        right_part = 0;
        power = 1;
        while (left_part > 0){                      // idea: left & right parts we keep in decimal base. 
            remainder = left_part % base;           // if left_part(base = 10) + right_part(base = 10) == init_number (base = 10)
            left_part /= base;                      // then it's Kaprekar's number
            right_part += remainder * power;
            power *= base;
            if(right_part != 0 && left_part + right_part == init_number){
                printf("%s ^ 2 = ", current_line);
                recursive_print_num_in_base(init_number * init_number, base);
                printf(" ==> ");
                recursive_print_num_in_base(left_part, base);
                printf(" + ");
                recursive_print_num_in_base(right_part, base);
                printf("\n");
                break;
            }
        }

    }
    return COOL;
}

error_state to_decimal_num(char* word, long int* res, int base){
    if (word == NULL || res == NULL){
        return NULL_PTR;
    }
    char* ptr = word;
    *res = 0;
    my_bool minus = FALSE;
    my_bool only_nums_in_numerical_system = (base <= 10 ? TRUE : FALSE);

    while(*ptr == ' ' || *ptr == '\n' || *ptr == '\t' || *ptr == '\v' || *ptr == '\r' || *ptr == '\b'){
        ++ptr;
    }
    
    while (*ptr != '\0'){
        if(ptr == word && *ptr == '-'){
            minus = TRUE;
            ++ptr;
            continue;
        }
        if(ptr == word && *ptr == '+'){ 
            ++ptr;
            continue;
        }
        
        if(only_nums_in_numerical_system){
            if (*ptr < '0' || *ptr > '0' + base){
                *res = 0;
                return WRONG_PARAMETER;
            }

            if ((LONG_MAX - (*ptr - '0')) / base < *res){
                *res = 0;
                return OVERFLOW;
            }
        
            *res *= base;
            *res += *ptr - '0';
        }
        else{
            if (*ptr < '0' || *ptr > '9' && *ptr < 'A' || *ptr >= 'A' + base - 10 && *ptr < 'a' || *ptr >= 'a' + base - 10){
                *res = 0;
                return WRONG_PARAMETER;
            }

            if (isdigit(*ptr)){
                if ((LONG_MAX - (*ptr - '0')) / base < *res){
                    *res = 0;
                    return OVERFLOW;
                }
                *res *= base;
                *res += *ptr - '0';
            }
            else{
                if(toupper(*ptr) == *ptr){
                    if ((LONG_MAX - (*ptr - 'A' + 10)) / base < *res){
                        *res = 0;
                        return OVERFLOW;
                    }
                    *res *= base;
                    *res += *ptr - 'A' + 10;
                }
                else {
                    if ((LONG_MAX - (*ptr - 'a' + 10)) / base < *res){
                        *res = 0;
                        return OVERFLOW;
                    }
                    *res *= base;
                    *res += *ptr - 'a' + 10;
                }
            }
        }
        ++ptr;
    }

    if(minus)
        *res  = -*res; 
    return COOL;
}

void recursive_print_num_in_base(long int init_number, int base){
    if (init_number == 0);
    else if (labs(init_number) < base){
        if (init_number < 0)
            printf("-");

        if(labs(init_number) < 10)
            printf("%ld", labs(init_number));
        else 
            printf("%c", (char)('A' + labs(init_number) - 10));
    }
    else{
        int reminder = labs(init_number) % base;
        recursive_print_num_in_base(init_number / base, base);
        if(reminder < 10)
            printf("%d", reminder);
        else
            printf("%c", 'A' + reminder - 10);
    }
}

