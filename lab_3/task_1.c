#include <stdio.h>
#include <math.h>
#include <stdlib.h>


typedef enum error_state {          
    COOL,
    WRONG_PAPAMETER,
    MEMORY_FUCKUP,
    NULL_PTR
} error_state;


long int add(long int a, long int b);
long int negative_number_convertion(long int a);

error_state  convert_from_dec_to_2_base(long int initial_number, long int parameter_r, char* result_line,  long int size_of_result_line);

int main(void) {

    long int initial_number = -237;          
     short int parameter_r = 4; //<-- <--  <-- <--  <-- <--  <-- <--  <-- <--  <-- <--  <-- <--

    long int size_of_result_line = 0;
    if (initial_number == 0)
        size_of_result_line = 2;
    else
        size_of_result_line = (long int) (log(labs(initial_number)) / log(2)) + 2;


    char * result_line = (char*) malloc(sizeof(char) * size_of_result_line);
    if (!result_line){
        printf("bad alloc\n");
    }

    error_state personal_errno = convert_from_dec_to_2_base(initial_number, parameter_r, result_line, size_of_result_line);
    switch (personal_errno){
        case COOL:
            printf("Result is: ");
            for(long int i = add(size_of_result_line, -1); i > 0; i = add(i, -1)){
                putchar(result_line[i]);
            }
            putchar('\n');
            break;
        case WRONG_PAPAMETER:
            printf("WP\n");
            break;
        case NULL_PTR:
            printf("Nptr\n");
            break;
        default: break;
    }
    free(result_line);

    return COOL;
}


error_state  convert_from_dec_to_2_base(long int initial_number, long int parameter_r, char* result_line,  long int size_of_result_line){        
    
    char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    long int base_mask = add(1 << parameter_r, -1), current_digit = 0, negative_sign = 0, pointer_to_the_end = 0;
    result_line[pointer_to_the_end] = '\0';
    pointer_to_the_end = add(pointer_to_the_end, 1);

    if (parameter_r < 1 || parameter_r > 5 || size_of_result_line < 1)
        return WRONG_PAPAMETER;

    if(!result_line)
        return NULL_PTR;

    if (initial_number < 0) {
        initial_number = negative_number_convertion(initial_number);
        negative_sign = 1;
    }

    do {
        current_digit = (initial_number & base_mask);
        result_line[pointer_to_the_end] = alphabet[current_digit];

        pointer_to_the_end = add(pointer_to_the_end, 1);
        initial_number >>= parameter_r;
    } while (initial_number);

    if (negative_sign) {
        result_line[pointer_to_the_end] = '-';
        pointer_to_the_end = add(pointer_to_the_end, 1);
    }

    return COOL;
}



long int add(long int a, long int b) {
    long int collision = (a & b);
    long int bitwise_summary = (a ^ b);

    if (collision)
        bitwise_summary = add(bitwise_summary, collision << 1);

    return bitwise_summary;
}

long int negative_number_convertion(long int a) {
    return add(~a, 1);
    }

