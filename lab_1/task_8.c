#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

#define NUM_LEN 100

typedef enum error_state{
    COOL,
    FUCKUP,
    NULL_PTR,
    WRONG_PARAMETER,
    OVERFLOW,
    MEMORY_FUCKUP,
    FILE_FUCKUP
} error_state;

typedef enum my_bool {FALSE, TRUE} my_bool;

error_state get_absolute_path(char* current_path, char* absolute_path);
error_state to_decimal_num(char* word, long int* res, int base);
error_state task_func(FILE* input_file, FILE* output_file);

void recursive_fprint_num_in_base(long int init_number, FILE* output_file, int base);


int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Wrong amount of arguments\n");
        return WRONG_PARAMETER;
    }

    int personal_errno = 0;

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    char* absolute_input = (char*) malloc(sizeof(char) * (FILENAME_MAX + 1));
    if(!absolute_input)
        return MEMORY_FUCKUP;
    char* absolute_output = (char*) malloc(sizeof(char) * (FILENAME_MAX + 1));
    if(!absolute_output){
        free(absolute_input);
        return MEMORY_FUCKUP;
    }
    personal_errno = get_absolute_path(argv[1], absolute_input);

    if (personal_errno != COOL){
        free(absolute_input);
        free(absolute_output);
    } 
    switch(personal_errno){
        case NULL_PTR:
            printf("Null-ptr was found in calculating first absolute path\n");
            return NULL_PTR;
        case OVERFLOW:
            printf("Overflow in calculating first absolute path\n");
            return OVERFLOW;
        case WRONG_PARAMETER:
            printf("Wrong path in calculating first absolute path\n");
            return WRONG_PARAMETER;
        case COOL:
            break;
        default:
            break;
    }

    personal_errno = get_absolute_path(argv[2], absolute_output);
    if (personal_errno != COOL){
        free(absolute_input);
        free(absolute_output);
    } 
    switch(personal_errno){
        case NULL_PTR:
            printf("Null-ptr was found in calculating second absolute path\n");
            return NULL_PTR;
        case OVERFLOW:
            printf("Overflow in calculating second absolute path\n");
            return OVERFLOW;
        case WRONG_PARAMETER:
            printf("Wrong path in calculating second absolute path\n");            
            return WRONG_PARAMETER;
        case COOL:
            break;
        default:
            break;
    }
    
    if(strcmp(absolute_input, absolute_output) == 0){
        free(absolute_input);
        free(absolute_output);
        printf("Same i/o file\n");
        return WRONG_PARAMETER;
    }
    
    input_file = fopen(absolute_input, "r");
    if(!input_file){
        free(absolute_input);
        free(absolute_output);
        printf("FILE OPEN fuckup\n");
        return FILE_FUCKUP;
    }
    output_file = fopen(absolute_output, "w");
    if(!output_file){
        free(absolute_input);
        free(absolute_output);
        fclose(input_file);
        printf("Error during opening file\n");
        return NULL_PTR;
    }

    switch(task_func(input_file, output_file)){         //TODO: make switch error_states
        case NULL_PTR:
            printf("NULL-PTR found in task_func\n");
            free(absolute_input);
            free(absolute_output);
            fclose(input_file);
            fclose(output_file);
            return NULL_PTR;
        case MEMORY_FUCKUP:
            printf("malloc hasn't done it in task_func\n");
            free(absolute_input);
            free(absolute_output);
            fclose(input_file);
            fclose(output_file);
            return MEMORY_FUCKUP;
        case COOL:
            break;
        default:
            break;
    }
    free(absolute_input);
    free(absolute_output);
    fclose(input_file);
    fclose(output_file);
    return COOL;
}


error_state get_absolute_path(char* current_path, char* absolute_path){

    if (!current_path || !absolute_path){
        return NULL_PTR;
    }

    char current_dir[FILENAME_MAX + 1];
    getcwd(current_dir, FILENAME_MAX);
    if (current_dir == NULL){
        return NULL_PTR;
    }

    int curr_dir_len = strlen(current_dir);     //without \n
    int curr_path_len = strlen(current_path);
    int curr_elem = 0, valid_char_count = 0;
    int delete_amount = 0;

    if(current_path[curr_path_len - 1] == '/'){     //file can't end with '/'
        return WRONG_PARAMETER;
    }    
    if (current_path[0] == '/'){                    // already absolute
        strcpy(absolute_path, current_path);
        return COOL;
    }

    char* dirty_abs_path = (char*) malloc(sizeof(char) * (curr_dir_len + curr_path_len + 2));       // + / + \n
    if(!dirty_abs_path)
        return MEMORY_FUCKUP;

    strcpy(dirty_abs_path, current_dir);           //get dirty absolute path with /../ or /./
    strcat(dirty_abs_path, "/");                                                                                                           
    strcat(dirty_abs_path, current_path);

    curr_elem = curr_dir_len + curr_path_len - 1; //index of the last elem 
    while(curr_elem > 0){
        //  dir1/../dir2/  case
        if(dirty_abs_path[curr_elem] == '.' &&  dirty_abs_path[curr_elem - 1] == '.'){
            if(dirty_abs_path[curr_elem - 2] != '/'){
                free(dirty_abs_path);
                return WRONG_PARAMETER;
            }
            
            ++delete_amount;
            dirty_abs_path[curr_elem] = dirty_abs_path[curr_elem - 1] = dirty_abs_path[curr_elem - 2] = '\0';
            curr_elem -= 3;
        }
        //  /./ case
        else if (dirty_abs_path[curr_elem] == '.' && dirty_abs_path[curr_elem - 1] == '/'){
            dirty_abs_path[curr_elem--] = '\0';
            dirty_abs_path[curr_elem--] = '\0';
        }
        //    dir//dir  case
        else if(dirty_abs_path[curr_elem] == '/' && dirty_abs_path[curr_elem - 1] == '/'){
            dirty_abs_path[curr_elem--] = '\0';
        }
        else{
            if(delete_amount > 0){
                dirty_abs_path[curr_elem--] = '\0'; 
                while(dirty_abs_path[curr_elem] != '/'){
                    dirty_abs_path[curr_elem--] = '\0';
                }
                dirty_abs_path[curr_elem--] = '\0';
                --delete_amount;
            }
            else{
                ++valid_char_count;
                --curr_elem;
            }
        }
    }

    if(valid_char_count + 1 > FILENAME_MAX){      
        free(dirty_abs_path);
        return OVERFLOW;
    }

    curr_elem = 0;
    for(int i = 0; i <= (curr_dir_len + curr_path_len); ++i){
        if (dirty_abs_path[i] != '\0'){
            absolute_path[curr_elem] = dirty_abs_path[i];
            ++curr_elem;
        }
    }

    free(dirty_abs_path);
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

void recursive_print_num_in_base(long int init_number, FILE* output_file, int base){
    if (init_number == 0);
    else if (labs(init_number) < base){
        if (init_number < 0)
            putc('-', output_file);

        if(labs(init_number) < 10){
            putc('0' + labs(init_number), output_file);
            //printf("%ld", labs(init_number));
        }
        else{
            putc('A'  + labs(init_number) - 10, output_file);
            //printf("%c", 'A' + labs(init_number) - 10);
        }
    }
    else{
        int reminder = labs(init_number) % base;
        recursive_print_num_in_base(init_number / base, output_file, base);
        if(reminder < 10){
            putc('0' + reminder, output_file);
            //printf("%d", reminder);
        }
        else{
            putc('A' + reminder - 10, output_file);
            //printf("%c", 'A' + reminder - 10);
        }
    }
}

error_state task_func(FILE* input_file, FILE* output_file){
    if (!input_file || !output_file)
        return NULL_PTR;
    
    char* str_buffer = (char*) malloc(sizeof(char) * (NUM_LEN + 1));
    if(!str_buffer)
        return MEMORY_FUCKUP;

    long int decimal_number = 0;
    char current_input_char = ' ', biggest_char = '1';
    int curr_buffer_ptr = 0;
    my_bool should_skip = FALSE;

    while((current_input_char = getc(input_file)) != EOF){
        current_input_char = toupper(current_input_char);

         //невалидный ввод, скип пока не пробел
        if(should_skip && !(current_input_char == ' ' || current_input_char == '\n' || current_input_char == '\t'))    
            continue;
        else
            should_skip = FALSE;    //конец невалидного ввода, парсим новое число

        if(current_input_char == ' ' || current_input_char == '\n' || current_input_char == '\t'){  //пробельный символ. Закончилось валидное/ невалидное число
            if(curr_buffer_ptr == 0){
                fprintf(output_file, "---\n");
                continue; 
            }    //пустой буффер
            //error_state convert_state = COOL;
            if(biggest_char >= '1' && biggest_char <= '9')
                biggest_char = biggest_char - '0' + 1;
            else
                biggest_char = biggest_char - 'A' + 11;
            
            str_buffer[curr_buffer_ptr] = '\0';

            if(to_decimal_num(str_buffer, &decimal_number, biggest_char) == COOL){
                if(decimal_number == 0)
                    fprintf(output_file, "0 2 0\n");
                else{
                    recursive_print_num_in_base(decimal_number, output_file, biggest_char);                   
                    fprintf(output_file, " %d %ld\n", biggest_char, decimal_number);
                }
            }
            else{
                fprintf(output_file, "---\n");
            }
            curr_buffer_ptr = 0;
            biggest_char = '1';
        }

        // невалидное число, есть сторонние символы. Simply skip the input of this number
        if(!isalnum(current_input_char) && current_input_char != ' ' && current_input_char != '\n' && current_input_char != '\t'){      
            curr_buffer_ptr = 0;
            biggest_char = 0;
            should_skip = TRUE; //скипаем до конца строки
            continue;
        }

        if(isalnum(current_input_char)){     //валидный символ
            if (curr_buffer_ptr >= NUM_LEN){  //переполнен буффер, точно переполнение
                curr_buffer_ptr = 0;
                should_skip = TRUE;
            }
            else{
                str_buffer[curr_buffer_ptr] = current_input_char;
                ++curr_buffer_ptr;

                if(current_input_char > biggest_char){      //наибольший символ
                    biggest_char = current_input_char;
                }
            }
        }
    }
    free(str_buffer);
    return COOL;
}