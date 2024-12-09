#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

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

long int my_strlen(char* str);
void line_reverse(char*);

error_state is_valid_line(char *line, int base);
error_state summ_all(int base, char** final_res, int amount_of_numbers, ...);
error_state summ_2_lines(int base, char* one, char* two);

int main(void){
    char* res;
    error_state status = COOL;

    printf("test 1\n123 + FFFFFFFFFF + ABCFF; base = 16;\nexpected result: 100000ABE21");
    status = summ_all(16, &res, 3, "123", "FFFFFFffFF", "abcff");
    switch(status){
        case COOL:
            printf("\t result: %s\n", res);
            free(res);
            break;
        case WRONG_PARAMETER:
            printf("base mistake\n");
            break;
        case NULL_PTR:
            printf("Null_ptr was fount\n");
            break;
        case MEMORY_FUCKUP:
            printf("Memory error\n");
            break;
        case FUCKUP:
            printf("Spoiled line\n");
            break;
        default:
            break;
    }

    printf("test 2\nAHAHAHAHA + LOloLO + DOTA2; base = 36;\nexpected result: AHB3CRSD0");
    status = summ_all(36, &res, 3, "AHAHAHAHA", "LOloLO", "DOTA2");
    switch(status){
        case COOL:
            printf("\t result: %s\n", res);
            free(res);
            break;
        case WRONG_PARAMETER:
            printf("base mistake\n");
            break;
        case NULL_PTR:
            printf("Null_ptr was fount\n");
            break;
        case MEMORY_FUCKUP:
            printf("Memory error\n");
            break;
        case FUCKUP:
            printf("Spoiled line\n");
            break;
        default:
            break;
    }

    printf("test 3\n5 + abc; base = 10;\nexpected result: Spoiled line     ");
    status = summ_all(10, &res, 2, "5", "abc");
    switch(status){
        case COOL:
            printf("\t result: %s\n", res);
            free(res);
            break;
        case WRONG_PARAMETER:
            printf("base mistake\n");
            break;
        case NULL_PTR:
            printf("Null_ptr was fount\n");
            break;
        case MEMORY_FUCKUP:
            printf("Memory error\n");
            break;
        case FUCKUP:
            printf("Spoiled line\n");
            break;
        default:
            break;
    }

    printf("test 4\n999999998 + 0 + 0 + 2 + 1; base = 10;\nexpected result: 1000000001");
    status = summ_all(10, &res, 5, "999999998", "0", "0", "2", "1");
    switch(status){
        case COOL:
            printf("\t result: %s\n", res);
            free(res);
            break;
        case WRONG_PARAMETER:
            printf("base mistake\n");
            break;
        case NULL_PTR:
            printf("Null_ptr was fount\n");
            break;
        case MEMORY_FUCKUP:
            printf("Memory error\n");
            break;
        case FUCKUP:
            printf("Spoiled line\n");
            break;
        default:
            break;
    }

}

error_state summ_all(int base, char** final_res, int amount_of_numbers, ...){

    if (base < 2 || base > 36)
        return WRONG_PARAMETER;

    if (!final_res)
        return NULL_PTR;

    va_list(item);
    va_start(item, amount_of_numbers);

    char *initial_line, *current_line, *current_result;
    unsigned int current_line_length = 0, current_result_length = 0;
    error_state personal_errno = COOL;

    for(int i = 0; i < amount_of_numbers; ++i){
        // dublicate of string-number
        initial_line = va_arg(item, char*);
        current_line_length = my_strlen(initial_line);
        current_line = (char*) malloc(sizeof(char) * (current_line_length + 1));
        if(!current_line){
            va_end(item);
            return MEMORY_FUCKUP;
        }
        strcpy(current_line, initial_line);
        line_reverse(current_line);

        personal_errno = is_valid_line(current_line, base);
        if (personal_errno != COOL){
            va_end(item);
            free(current_line);
            if (i != 0)
                free(current_result);
            
        }
        switch(personal_errno){
            case NULL_PTR:     
                return NULL_PTR;
            case FUCKUP:        // spoiled line
                return FUCKUP;
            case COOL:
                break;
            default:
                break;
        }
    
        if (i == 0){
            current_result = (char*) malloc(sizeof(char) * (current_line_length + 1) * 2);
            if (!current_result){
                va_end(item);
                free(current_line);
            }
            
            current_result_length = (current_line_length + 1) * 2;
            strcpy(current_result, current_line);
        }
        else{

            if (current_line_length + 2 > current_result_length){
                current_result_length = (current_line_length + 1) * 2;
                char* temporary_realloc = (char*) realloc(current_result, sizeof(char) * current_result_length);
                if (!temporary_realloc){
                    va_end(item);
                    free(current_line);
                    // FIXME: Do we need to free(current_result) ???
                }
                current_result = temporary_realloc;
                    
                
            }

            personal_errno = summ_2_lines(base, current_result, current_line);
            if (personal_errno != COOL)
                return FUCKUP;
            
        } 

        free(current_line);
    }

    line_reverse(current_result);
    *final_res = current_result;

    //free(current_result);
    va_end(item);
    return COOL;
}

error_state summ_2_lines(int base, char* one, char* two){
    unsigned int length_1 = my_strlen(one), length_2 = my_strlen(two);
    unsigned int digit_1 = 0, digit_2 = 0, shift = 0, tmp_sum = 0, i = 0;

    for ( ; i < length_1 && i < length_2; ++i){
        digit_1 = isdigit(one[i]) ? (one[i] - '0') : (one[i] - 'A' + 10);
        digit_2 = isdigit(two[i]) ? (two[i] - '0') : (two[i] - 'A' + 10);
        tmp_sum = digit_1 + digit_2 + shift;

        one[i] = (tmp_sum % base < 10) ? ('0' + tmp_sum % base) : ('A' + tmp_sum % base- 10);
        shift = tmp_sum / base;
    }

    for( ; i < length_1; ++i){
        digit_1 = isdigit(one[i]) ? (one[i] - '0') : (one[i] - 'A' + 10);
        tmp_sum = digit_1 + shift;
        one[i] = (tmp_sum % base < 10) ? ('0' + tmp_sum % base) : ('A' + tmp_sum % base - 10);
        shift = tmp_sum / base;
    }

    for( ; i < length_2; ++i){
        digit_2 = isdigit(two[i]) ? (two[i] - '0') : (two[i] - 'A' + 10);
        tmp_sum = digit_2 + shift;
        one[i] = (tmp_sum % base < 10) ? ('0' + tmp_sum % base) : ('A' + tmp_sum % base - 10);
        shift = tmp_sum / base;
    }

    if(shift){
        tmp_sum = shift;
        one[i++] = (tmp_sum % base < 10) ? ('0' + tmp_sum % base) : ('A' + tmp_sum % base - 10);
    }
    one[i] = '\0';
    return COOL;
}



error_state is_valid_line(char *line, int base){
    if (!line || !my_strlen(line))
        return NULL_PTR;
    
    long int n = my_strlen(line);
    if (base < 10){
        for (int i = 0; i < n; ++i){
            if (line[i] < '0' || line[i] > '0' + base - 1)
                return FUCKUP;
        }    
    }
    else {
        for (int i = 0; i < n; ++i){
            line[i] = toupper(line[i]);
            if (line[i] < '0' || (line[i] > '9' && line[i] < 'A') || line[i] > 'A' + base - 11)
                return FUCKUP;
        }
    }
    return COOL;
}

void line_reverse(char* line){
    int n = my_strlen(line);
    for(int i = 0; i < n / 2; ++ i){
        char tmp = line[i];
        line[i] = line[n - i - 1];
        line[n - i - 1] = tmp; 
    }
}

long int my_strlen(char* line){
    char const* end = line;
	while (*end++);
	return end - line - 1;
}