#include <stdio.h>
#include <stdlib.h>     //malloc()
#include <string.h>     //strcmp()
#include <limits.h>
#include <ctype.h>      //isdigit()

#define BUFFER_SIZE 100

enum my_bool{FALSE, TRUE};

enum error_state{
    DONE,
    INVALID_INPUT,
    WRONG_PARAMETERS,
    WRONG_PARAMETERS_NUMBER,
    OVERFLOW,
    NULL_PTR,
    INVALID_MEMORY
};

enum my_bool convert_check(enum error_state condition);
enum error_state to_decimal_num(char* word, long int* res, int base);
void recursive_print_num_in_base(long int init_number, int base);

int main(int argc, char* argv[]){
    int base = 0;
    if (!scanf("%d", &base) || base < 2 || base > 36){
        printf("wrong numeral system\n");
        return WRONG_PARAMETERS_NUMBER;
    }

    long int curr_number = 0, max_decimal_number = 0;
    enum my_bool valid_nums_cnt = FALSE; 
    char * input_buffer = NULL;
    input_buffer = (char*) malloc(sizeof(char) * (BUFFER_SIZE + 1));      //100 + 1
    if (input_buffer == NULL){
        printf("Memory wasn't allocated\n");
        return INVALID_MEMORY;
    }

    while(scanf("%100s", input_buffer)){                //очищать буффер
        if(strcmp("Stop", input_buffer) == 0){
            if (valid_nums_cnt){
                printf("%ld\n", max_decimal_number);        //FIXME: REMOVE THIS LINE
                recursive_print_num_in_base(max_decimal_number, base);
                printf("\n");
                recursive_print_num_in_base(max_decimal_number, 9);
                printf("\n");
                recursive_print_num_in_base(max_decimal_number, 18);
                printf("\n"); 
                recursive_print_num_in_base(max_decimal_number, 27);
                printf("\n");           
                recursive_print_num_in_base(max_decimal_number, 36);
                printf("\n");
            }
            free(input_buffer);
            return DONE;
        }
        
        if (convert_check(to_decimal_num(input_buffer, &curr_number, base))){
            if (labs(curr_number) > labs(max_decimal_number)){
                max_decimal_number = curr_number;
            }
            valid_nums_cnt = TRUE;
        }
        if(strlen(input_buffer) == BUFFER_SIZE){                //Buffer_clean added
            int curr_char = 0;
            while((curr_char = getchar() != '\n')){
            }
        }

    }

    free(input_buffer);
    return DONE;
}

enum my_bool convert_check(enum error_state condition){
    switch(condition){
        case NULL_PTR:
            printf("NULL-ptr found\n");
            return FALSE;
        case OVERFLOW:
            printf("Number overflow\n");
            return FALSE;
        case WRONG_PARAMETERS:
            printf("Wrong parameter passed\n");
            return FALSE;    
        default:
            break;
    }
    return TRUE;
}

enum error_state to_decimal_num(char* word, long int* res, int base){
    if (word == NULL || res == NULL){
        return NULL_PTR;
    }
    char* ptr = word;
    *res = 0;
    enum my_bool minus = FALSE;
    enum my_bool only_nums_in_numerical_system = (base <= 10 ? TRUE : FALSE);

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
                return WRONG_PARAMETERS;
            }

            if ((LONG_MAX - (*ptr - '0')) / base < *res){
                *res = 0;
                return OVERFLOW;
            }
        
            *res *= base;
            *res += *ptr - '0';
        }
        else{
            if (*ptr < '0' || *ptr > '9' && *ptr < 'A' || *ptr >= 'A' + base - 10){
                *res = 0;
                return WRONG_PARAMETERS;
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
                if ((LONG_MAX - (*ptr - 'A' + 10)) / base < *res){
                    *res = 0;
                    return OVERFLOW;
                }
                *res *= base;
                *res += *ptr - 'A' + 10;
            }
        }
        ++ptr;
    }

    if(minus)
        *res  = -*res; 
    return DONE;
}

void recursive_print_num_in_base(long int init_number, int base){
    if (init_number == 0);
    else if (labs(init_number) < base){
        if (init_number < 0)
            printf("-");

        if(labs(init_number) < 10)
            printf("%ld", labs(init_number));
        else 
            printf("%c", 'A' + labs(init_number) - 10);
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

