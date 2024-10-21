#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define FIRST_PART_ARRAY_SIZE 16
#define MIN_CAP_SECOND_PART_ARRAY 10
#define MAX_CAP_SECOND_PART_ARRAY 10000
#define ABS_MAX_RAND_NUMBER 1000
typedef enum my_bool{
    FALSE,
    TRUE 
}  my_bool;

typedef enum error_state{
    DONE,
    INVALID_INPUT,
    WRONG_PARAMETERS,
    WRONG_PARAMETERS_NUMBER,
    OVERFLOW,
    NULL_PTR,
    INVALID_MEMORY
} error_state;

error_state str_to_num(char* word, long int* res, int base);
my_bool convert_check(error_state condition);

error_state first_part_func (long int** array, long int const * lb, long int const * ub);
error_state second_part_func (int** first, int** second, int** res);
int cmp(void const* a, void const* b);
int closest_element(int aim, int*array, int const* ar_size);

int main(int argc, char* argv[]) {
    long int* first_part_array = NULL;
    long int first_part_lower_bound = 0, first_part_upper_bound = 0;

    int* second_part_first_array = NULL;
    int* second_part_second_array = NULL;
    int* second_part_result_array = NULL; 


    if (argc != 3){
        printf("Wrong nubmer of console arguments\n");
        return WRONG_PARAMETERS_NUMBER;
    }

    if(!convert_check(str_to_num(argv[1], &first_part_lower_bound, 10)) || !convert_check(str_to_num(argv[2], &first_part_upper_bound, 10))){
        return WRONG_PARAMETERS;
    }

    if (first_part_lower_bound > first_part_upper_bound){
        long int tmp = first_part_lower_bound;
        first_part_lower_bound = first_part_upper_bound;
        first_part_upper_bound = tmp;
    }

    switch (first_part_func(&first_part_array, &first_part_lower_bound, &first_part_upper_bound)){
        case NULL_PTR:
            printf("NULL_PTR was found in first_part_function\n");
            return NULL_PTR;
        case INVALID_MEMORY:
            printf("Malloc didn't found spare space\n");
            return INVALID_MEMORY;
        case DONE:
            free(first_part_array);
            break;             
        default: break; 
    }

    switch(second_part_func(&second_part_first_array, &second_part_second_array, &second_part_result_array)){
        case NULL_PTR:
            printf("NULL_PTR was found in second_part_function\n");
            return NULL_PTR;
        case INVALID_MEMORY:
            printf("Malloc didn't found spare space\n");
            return INVALID_MEMORY;
        case DONE:
            free(second_part_first_array);
            free(second_part_second_array);
            free(second_part_result_array);
            break;
        default: break;
    }
    return DONE;
}


error_state str_to_num(char* word, long int* res, int base){
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

my_bool convert_check(error_state condition){
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

error_state first_part_func (long int** array, long int const * lb, long int const * ub){\
    long int curr_min_index = 0, curr_max_index = 0, tmp = 0;

    if(!array || !lb || !ub)
        return NULL_PTR;

    *array = (long int*) malloc(sizeof(long int) * FIRST_PART_ARRAY_SIZE);
    if (*array == NULL)
        return INVALID_MEMORY;

    srand(*lb);

    for(long int i = 0; i < FIRST_PART_ARRAY_SIZE; ++i)
        (*array)[i] = rand() % (*ub - *lb + 1) + *lb;
    
    for (long int i = 0; i < FIRST_PART_ARRAY_SIZE; ++i){     //TODO:  comment this block
        printf("%ld ", (*array)[i]);
        if (i == FIRST_PART_ARRAY_SIZE - 1)
            printf("\n");
    }

    for(long int i = 0; i < FIRST_PART_ARRAY_SIZE; ++i){
        if ((*array)[i] > (*array)[curr_max_index])
            curr_max_index = i;
        if ((*array)[i] < (*array)[curr_min_index])
            curr_min_index = i;
    }

    tmp = (*array)[curr_min_index];
    (*array)[curr_min_index] = (*array)[curr_max_index];
    (*array)[curr_max_index] = tmp;
    
    for (long int i = 0; i < FIRST_PART_ARRAY_SIZE; ++i){     //TODO:  comment this block
        printf("%ld ", (*array)[i]);
        if (i == FIRST_PART_ARRAY_SIZE - 1)
            printf("\n");
    }

    return DONE;
}

error_state second_part_func (int** first, int** second, int** res){
    if (!first || !second || !res)
        return NULL_PTR;

    srand(time(NULL));       // pointer - int, can we do this?
    
    int first_size = (rand() % (MAX_CAP_SECOND_PART_ARRAY - MIN_CAP_SECOND_PART_ARRAY + 1)) + MIN_CAP_SECOND_PART_ARRAY;
    int second_size = (rand() % (MAX_CAP_SECOND_PART_ARRAY - MIN_CAP_SECOND_PART_ARRAY + 1)) + MIN_CAP_SECOND_PART_ARRAY;

    *first = (int*) malloc(sizeof(int) * first_size);
    if (!(*first))
        return INVALID_MEMORY;
    *second = (int*) malloc(sizeof(int) * second_size);
    if(!(*second)){
        free(*first);
        return INVALID_MEMORY;
    }
    *res = (int*) malloc(sizeof(int) * first_size);
    if(!(*res)){
        free(*first);
        free(*second);
        return INVALID_MEMORY;
    }

    //printf("%d %d\n", first_size, second_size);
    for(int i = 0; i < first_size; ++i){
        (*first)[i] = (rand() % (ABS_MAX_RAND_NUMBER * 2  + 1)) - ABS_MAX_RAND_NUMBER;
    }
    for(int i = 0; i < second_size; ++i){
        (*second)[i] = (rand() % (ABS_MAX_RAND_NUMBER * 2  + 1)) - ABS_MAX_RAND_NUMBER;
    }

    qsort(*second, second_size, sizeof(int), cmp);

    /*for (int i = 0; i < first_size; ++i){     //TODO:  comment this block
        printf("%d ", (*first)[i]);
        if (i == first_size - 1)
            printf("\n");
    }
    for (int i = 0; i < second_size; ++i){     //TODO:  comment this block
        printf("%d ", (*second)[i]);
        if (i == second_size - 1)
            printf("\n");
    }*/

    for(int i = 0; i < first_size; ++i){
        (*res)[i] = (*first)[i] + closest_element((*first)[i], *second, &first_size);
    }

    /*for (int i = 0; i < first_size; ++i){     //TODO:  comment this block
        printf("%d ", (*res)[i]);
        if (i == first_size - 1)
            printf("\n");
    }   */
    return DONE;
}

int cmp(void const* a, void const* b){
        return (*(int*)a - *(int*)b);
}

int closest_element(int aim, int*array, int const* ar_size){
    int lb = -1, ub = *ar_size - 1;
    int current_index = 0;
    while(ub - lb != 1){
        current_index = (ub + lb) / 2;
        if(array[current_index] < aim)
            lb = current_index;
        else
            ub = current_index;        
    }

    if (array[ub] == aim)
        return aim;
    else{
        if (abs(aim - array[lb]) < abs(aim - array[ub]))
            return array[lb];
        else
            return array[ub];
    }
}