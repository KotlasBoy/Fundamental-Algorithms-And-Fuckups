#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>

#define BUF_SIZE 2048
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

unsigned long int my_strlen(unsigned char* str);
enum my_bool convert_check(enum error_state condition);
error_state to_decimal_num(char* word, long int* res, int base);

error_state case_n (char* initial_line, char* result_line);
error_state case_r (char* initial_line, char* result_line);
error_state case_u (char* initial_line, char* result_line);
error_state case_c(int argc, char** argv, char* result);



int main(int argc, char* argv[]){
    if (argc < 2 || argv[1][0] != '-')
        return WRONG_PARAMETER;

    switch (argv[1][1]){
        case 'l':

            if(argc != 3){
                printf("Wrong number of parameters\n");
                return FUCKUP;
            }
            printf("The length of line is %lu\n", my_strlen(argv[2]));
            break;

        case 'r':               //  ./a.out -r abcdefg
            
            if(argc != 3){
                printf("Wrong number of parameters\n");
                return FUCKUP;
            }

            char* reversed_line = (char*) malloc(sizeof(char) * (my_strlen(argv[2]) + 1));

            if (case_r(argv[2], reversed_line) == NULL_PTR){
                printf("Null_PTR found\n");
            }
            else {
                printf("Reversed line is %s\n", reversed_line);
            }

            free(reversed_line);
            break;

        case 'u':           // ./a.out -u abcdef
            
            if(argc != 3){
                printf("Wrong number of parameters\n");
                return FUCKUP;
            }
            char* ironic_line = (char*) malloc(sizeof(char) * (my_strlen(argv[2]) + 1));

            if (case_u(argv[2], ironic_line) == NULL_PTR){
                printf("Null_PTR found\n");
            }
            else {
                printf("Ironic line is %s\n", ironic_line);
            }

            free(ironic_line);
            break;

        case 'n':       // ./a.out -n a1b2c3d4e5f6g6h7i8j9k1l5m4n3o2p

            if(argc != 3){
                printf("Wrong number of parameters\n");
                return FUCKUP;
            }

            char* numbers_ahead = (char*) malloc(sizeof(char) * (my_strlen(argv[2]) + 1));

            if(case_n(argv[2], numbers_ahead) == NULL_PTR){
                printf("Null_ptr found\n");
            }
            else {
                printf("Numbers_ahead line is %s\n", numbers_ahead);
            }

            free(numbers_ahead);
            break;

        case 'c':       // ./a.out -c 5 j a  aaa bbb cc

            if(argc < 3){
                printf("Wrong number of parameters\n");
                return FUCKUP;
            }

            char* result = (char*) malloc(sizeof(char) * BUF_SIZE);
            if (!result){
            return MEMORY_FUCKUP;
            }

            if(case_c(argc, argv, result) != COOL){
                printf("FUCKUP\n");
            }
            else {
                printf("The line is %s\n", result);
            }

            free(result);
            break;

         default:
            printf("Wrong flag\n");
            return WRONG_PARAMETER;
    }
    return COOL;  
}


error_state case_c(int argc, char** argv, char* result){

    long int seed = 0;                                      //./a.out -c SEED str1 str2 str3 str4
    unsigned long int index = 0, ptr_to_the_end = 0;

    if(convert_check(to_decimal_num(argv[2], &seed, 10)) != TRUE)
        return FUCKUP;
    
    if (seed < 0)
        seed *= -1;
    
    srand(seed);

    /*if (seed == 0){
        for(unsigned long int i = 0; i < my_strlen(argv[2]); ++i)
            result[i] = argv[2][i];
        return COOL;
    }*/

    /*if (seed < argc - 2){
        printf("Wrong number of parameters\n");
        return FUCKUP;
    }*/
    
    
    my_bool* already_concatenated = (my_bool*) malloc(sizeof(my_bool) * (argc));
    if(already_concatenated == NULL){
        return MEMORY_FUCKUP;
    }
            
    for (int i = 0; i < argc; ++i){
        already_concatenated[i] = (i < 3 && i != 1) ? TRUE : FALSE;
    }

    index = rand() % (argc);
    for(int i = 0; i < argc - 2; ++i){
        while (already_concatenated[index] != FALSE){
            index = rand() % (argc);
        }        
        if ((BUF_SIZE - ptr_to_the_end) < my_strlen(argv[index])){
            //printf("The length overflow\n");
            free(result);
            free(already_concatenated);
            return MEMORY_FUCKUP;
        }
                
        for(unsigned long int j = 0; j < my_strlen(argv[index]); ++j, ++ptr_to_the_end){
            result[ptr_to_the_end] = argv[index][j];
        }
        already_concatenated[index] = TRUE;
        
    }

    return COOL;
}


error_state case_n (char* initial_line, char* result_line){

    if (initial_line == NULL || result_line == NULL)
        return NULL_PTR;

    unsigned long int n = my_strlen(initial_line), current_index = 0;

    for (unsigned long int i = 0; i < n + 1; ++i)
        if ((initial_line[i] >= '0') && (initial_line[i] <= '9')){
            result_line[current_index++] = initial_line[i];
        }
    
    for (unsigned long int i = 0; i < n + 1; ++i)
        if (isalpha(initial_line[i])){
            result_line[current_index++] = initial_line[i];
        }
        
    if (my_strlen(result_line) != n + 1){
        for (unsigned long int i = 0; i < n + 1; ++i)
            if (!isalnum(initial_line[i])){
                result_line[current_index++] = initial_line[i];
        }
    }

    return COOL;

}


error_state case_r (char* initial_line, char* result_line){
    if (initial_line == NULL || result_line == NULL)
        return NULL_PTR;

    unsigned long int n = my_strlen(initial_line);

    for (unsigned long int i = 0; i < n; ++ i){
        result_line[i] = initial_line[n - i - 1]; 
    }

    return COOL;
}


error_state case_u (char* initial_line, char* result_line){
    if (initial_line == NULL || result_line == NULL)
        return NULL_PTR;

    unsigned long int n = my_strlen(initial_line);

    for (unsigned long int i = 0; i < n + 1; ++i){
        result_line[i] = isalpha(initial_line[i]) && (i % 2) ?  initial_line[i] - 'a' + 'A' : initial_line[i];
    }

    return COOL;
}



unsigned long int my_strlen(unsigned char* str){
    unsigned char const* end = str;
	while (*end++);
	return end - str - 1;
}


enum my_bool convert_check(enum error_state condition){
    switch(condition){
        case NULL_PTR:
            printf("NULL-ptr found\n");
            return FALSE;
        case OVERFLOW:
            printf("Number overflow\n");
            return FALSE;
        case WRONG_PARAMETER:
            printf("Wrong parameter passed\n");
            return FALSE;    
        default:
            break;
    }
    return TRUE;
}

error_state to_decimal_num(char* word, long int* res, int base){                        // include <limits.h> for INT_MAX
    if (word == NULL || res == NULL){                                                   // TOO: include <stdio.h> for NULL
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
            if ((*ptr < '0' || *ptr > '9') && (*ptr < 'A' || *ptr >= 'A' + base - 10) && (*ptr < 'a' || *ptr >= 'a' + base - 10)){
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


