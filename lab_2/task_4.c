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

void count_k_b(double zero_x, double zero_y, double one_x, double one_y, double* k, double* b, my_bool* x_is_const, my_bool* y_is_const);
error_state is_valid_point(double zero_x, double zero_y, double current_x, double current_y, double k, double b, my_bool x_is_const, my_bool y_is_const, my_bool* clockwise);


error_state fird_part(int count, int base, ...);

error_state to_decimal_num(char* word, long int* res, int base);
void recursive_print_num_in_base(long int init_number, int base);

//TODO: test first part of the task, please
int main(int argc, char* argv[]){
    my_bool result = INIT;
    switch(first_part( &result, 8, 1., 1., 2., 2., 5., 0., 3., -2., 1., 0.)){      //  (&result, counter, x1, y1, x2, y2, x3, y3, ...)
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

    //TODO: There will be second part of the task. Maybe.
    
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


error_state first_part(my_bool * result, int count, ...){
    if(!result){
        return NULL_PTR;
    }
    if (count < 6 || count % 2 != 0){
        return WRONG_PARAMETER; 
    }
    double zero_x = 0.0, zero_y = 0.0;
    double one_x = 0.0, one_y = 0.0;
    double current_x = 0.0, current_y = 0.0;
    my_bool clockwise = INIT, x_is_const = FALSE, y_is_const = FALSE;
    double k = 0.0, b = 0.0;        //y = kx + b
    double extra_x = 0.0, extra_y = 0.0;

    va_list item_ptr;
    va_start (item_ptr, count);
    
    zero_x = va_arg(item_ptr, double);
    extra_x = zero_x;
    zero_y = va_arg(item_ptr, double);
    extra_y = zero_y;
    one_x = va_arg(item_ptr, double);
    one_y = va_arg(item_ptr, double);
    current_x = va_arg(item_ptr, double);
    current_y = va_arg(item_ptr, double);
    count -= 3;

    count_k_b(zero_x, zero_y, one_x, one_y, &k, &b, &x_is_const, &y_is_const);
    
    error_state personal_errno = is_valid_point(zero_x, zero_y, current_x, current_y, k, b, x_is_const, y_is_const, &clockwise);
    
    if(personal_errno != COOL){
        va_end(item_ptr);
        return personal_errno;
    }
    
    while(count > 0){
        zero_x = one_x;
        zero_y = one_y;
        one_x = current_x;
        one_y = current_y;
        current_x = va_arg(item_ptr, double);
        current_y = va_arg(item_ptr, double);

        count_k_b(zero_x, zero_y, one_x, one_y, &k, &b, &x_is_const, &y_is_const);
        personal_errno = is_valid_point(zero_x, zero_y, current_x, current_y, k, b, x_is_const, y_is_const, &clockwise);
    
        if(personal_errno != COOL){
            va_end(item_ptr);
            return personal_errno;
        }
    }
    zero_x = one_x;
    zero_y = one_y;
    one_x = current_x;
    one_y = current_y;
    current_x = extra_x;
    current_y = extra_y;

    count_k_b(zero_x, zero_y, one_x, one_y, &k, &b, &x_is_const, &y_is_const);
    personal_errno = is_valid_point(zero_x, zero_y, current_x, current_y, k, b, x_is_const, y_is_const, &clockwise);

    va_end(item_ptr);
    return personal_errno;
}

void count_k_b(double zero_x, double zero_y, double one_x, double one_y, double* k, double* b, my_bool* x_is_const, my_bool* y_is_const){
    if(fabs(one_x - zero_x) < EPSILON){
        *k = -1;
        *b = 0;
        *x_is_const = TRUE;
    }
    if(fabs(one_y - zero_y) < EPSILON){
        *k = -1;
        *b = 0;
        *y_is_const = TRUE;
    }
    if(!*x_is_const || !*y_is_const){
        if(one_x > zero_x){
            *k = (one_y - zero_y) / (one_x - zero_x);
            *b = zero_y - *k * zero_x;
            *x_is_const = FALSE;
            *y_is_const = FALSE;
        }
        else if (one_x < zero_x){
            *k = (zero_y - one_y) / (zero_x - one_x);
            *b = one_y - *k * one_x;
            *x_is_const = FALSE;
            *y_is_const = FALSE;
        }
    }
}

error_state is_valid_point(double zero_x, double zero_y, double current_x, double current_y, double k, double b, my_bool x_is_const, my_bool y_is_const, my_bool* clockwise){
    if(!clockwise){
        return NULL_PTR;
    }

    if(x_is_const && fabs(current_x - zero_x) < EPSILON){      //more than two points on 1 line (x == const) 
        return WRONG_PARAMETER;
    }

    if(y_is_const && fabs(current_y - zero_y) < EPSILON){      //more than two points on 1 line ( y == const)
        return WRONG_PARAMETER;
    }

    if (fabs((k * current_x + b) - current_y) < EPSILON){        //more than two points on 1 line (y = kx + b)
        return WRONG_PARAMETER;
    }
    ///////
    if(*clockwise == INIT){
        if((k * current_x + b) > current_y){
            *clockwise = TRUE;
            return COOL;
        }
        else {
            *clockwise = FALSE;
            return COOL;
        }
    }
    else{
        if(*clockwise && (k * current_x + b) > current_y || !*clockwise && (k * current_x + b) < current_y)
            return COOL;
        else   
            return FUCKUP;
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

    while(*ptr == ' ' || *ptr == '\n' || *ptr == '\t' || *ptr == '\v' || *ptr == '\r' || *ptr == '\b'){     //space skip
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