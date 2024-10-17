#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define PREFIX "out_"

typedef enum error_state{
    COOL,
    FUCKUP,
    NULL_PTR,
    WRONG_PARAMETER,
    OVERFLOW,
    MEMORY_FUCKUP,
    FILE_FUCKUP
} error_state;

error_state get_absolute_path(char* current_path, char* absolute_path);
error_state is_valid_flag (char* flag, int* has_n, char* character);
error_state add_prefix(char* path, char* prefix);

void remove_arabic_figures(FILE* inp_file, FILE* out_file, char character, unsigned int counter);
void letters_in_string_counter(FILE* inp_file, FILE* out_file, char character, unsigned int counter);
void different_symbols_counter(FILE* inp_file,  FILE* out_file, char character, unsigned int counter);
void hex_symbols(FILE* inp_file, FILE* out_file, char character, unsigned int counter);

void file_and_memory_clean (char* a, char* b, FILE* c, FILE* d);
error_state flag_function(FILE* input, FILE* output, unsigned char func_num);

void(*flag_ptr[])(FILE*, FILE*, char, unsigned int) = {remove_arabic_figures, letters_in_string_counter, different_symbols_counter, hex_symbols}; //FIXME: global ?

int main(int argc, char* argv[]){
    if (argc < 3 || argc > 4){
        printf("Wrong amount of parameters\n");
        return FUCKUP;
    }
                                                                                                                               
    int has_n = FALSE;
    int personal_errno = 0;
    char flag = ' ';

    personal_errno = is_valid_flag(argv[1], &has_n, &flag);
    switch(personal_errno){
        case NULL_PTR:
            printf("Null-ptr in flag validation\n");
            return NULL_PTR;
        case WRONG_PARAMETER:
            printf("Wrong parameter in flag validation\n");
            return NULL_PTR;
        case COOL:
            break;
        default:
            break;
    }

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
    personal_errno = get_absolute_path(argv[2], absolute_input);
    if (personal_errno != COOL){
        free(absolute_input);
        free(absolute_output);
    } 
    switch(personal_errno){
        case NULL_PTR:
            printf("Null-ptr was found in calculating first absolute path\n");
            free(absolute_input);
            free(absolute_output);
            return NULL_PTR;
        case OVERFLOW:
            printf("Overflow in calculating first absolute path\n");
            free(absolute_input);
            free(absolute_output);
            return OVERFLOW;
        case WRONG_PARAMETER:
            printf("Wrong path in calculating first absolute path\n");
            free(absolute_input);
            free(absolute_output);
            return WRONG_PARAMETER;
        case COOL:
            break;
        default:
            break;
    }

    if (has_n){
        personal_errno = get_absolute_path(argv[3], absolute_output);
        if (personal_errno != COOL){
            free(absolute_input);
            free(absolute_output);
        }    
        switch(personal_errno){
            case NULL_PTR:
                printf("Null-ptr was found during calculating second absolute path\n");
                return NULL_PTR;
            case OVERFLOW:
                printf("Overflow during calculating second absolute path\n");
                return OVERFLOW;
            case WRONG_PARAMETER:
                printf("Wrong path during calculating second absolute path\n");
                return WRONG_PARAMETER;
            case COOL:
                break;
            default:
                break;
        }
    }
    else {
        strcpy(absolute_output, absolute_input);
        personal_errno = add_prefix(absolute_output, PREFIX);
        if (personal_errno != COOL){
            free(absolute_input);
            free(absolute_output);
        } 
        switch(personal_errno){
            case OVERFLOW:
                printf("OVERFLOW durint adding prefix\n");
                return OVERFLOW;
            case NULL_PTR:
                printf("Null-ptr during adding prefix\n");
                return NULL_PTR;
            case COOL:
                break;
            default:
                break;
        }   
    }

    //printf("%s\n%s\n", absolute_input, absolute_output);        //FIXME: remove this line

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

    switch(flag){
        case 'd':
            if (flag_function(input_file, output_file, 0) == NULL_PTR){
                file_and_memory_clean(absolute_input, absolute_output, input_file, output_file);
                printf("Null-ptr found during flag d\n");
                return NULL_PTR;
            }
            break;  
        case 'i':
            if (flag_function(input_file, output_file, 1) == NULL_PTR){
                file_and_memory_clean(absolute_input, absolute_output, input_file, output_file);
                printf("Null-ptr found during flag d\n");
                return NULL_PTR;
            }
            break;  
        case 's':
            if (flag_function(input_file, output_file, 2) == NULL_PTR){
                file_and_memory_clean(absolute_input, absolute_output, input_file, output_file);
                printf("Null-ptr found during flag d\n");
                return NULL_PTR;
            }
            break;
        case 'a':
            if (flag_function(input_file, output_file, 3) == NULL_PTR){
                file_and_memory_clean(absolute_input, absolute_output, input_file, output_file);
                printf("Null-ptr found during flag d\n");
                return NULL_PTR;
            }
            break;
        default:
            break;
    }                   
    file_and_memory_clean(absolute_input, absolute_output, input_file, output_file);
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



error_state is_valid_flag(char* flag, int* has_n, char* character){
    if (!flag){
        return NULL_PTR;
    }

    if (strlen(flag) < 2 || strlen(flag) > 3 || flag[0] != '-' && flag[0] != '/'){
        return WRONG_PARAMETER; 
    }
    int last = strlen(flag) - 1;
    
    if (!(flag[last] == 'd' || flag[last] == 'i' || flag[last] == 's' || flag[last] == 'a')){
        return WRONG_PARAMETER;
    }

    if (strlen(flag) == 2){
        *has_n = FALSE;
    }
    else if (strlen(flag) == 3) {
        if(flag[1] != 'n'){
            return WRONG_PARAMETER;
        }
        *has_n = TRUE;
    }
    *character = flag[last];
    return COOL;
}

error_state add_prefix(char* path, char* prefix){
    if (!path || !prefix){
        return NULL_PTR;
    }
    if (strlen(path) + strlen(prefix) > FILENAME_MAX){
        return OVERFLOW;
    }
    
    char* tmp = (char*) malloc(sizeof(char) * FILENAME_MAX);
    int last_slash = strlen(path);

    while(path[--last_slash] != '/'){}
    
    strcpy(tmp, path + (++last_slash));

    for(last_slash; path[last_slash] != '\0'; ++last_slash)
        path[last_slash] = '\0';

    strcat(path, prefix);
    strcat(path, tmp);

    free(tmp);
    return COOL;
}

//0
void remove_arabic_figures(FILE* inp_file, FILE* out_file, char character, unsigned int counter){
    while((character = getc(inp_file)) != EOF){
        if (character >= '0' && character <= '9')
            continue;
        else{
            putc(character, out_file);
        }
    }
}

// 1
void letters_in_string_counter(FILE* inp_file, FILE* out_file, char character, unsigned int counter){
    do {
        character = getc(inp_file);
        if (character == '\n' || character == EOF){
            fprintf(out_file, "%d\n", counter);
            counter = 0;
            continue;
        }
        if (isalpha(character))
            ++counter;
    } while(character != EOF);
}

//2
void different_symbols_counter(FILE* inp_file, FILE* out_file, char character, unsigned int counter){
    do {
        character = getc(inp_file);
        if (character == '\n' || character == EOF){
            fprintf(out_file, "%d\n", counter);
            counter = 0;
            continue;
        }
        if (!isalnum(character) && character != ' ')
            ++counter;
    } while(character != EOF);
}

// 3
void hex_symbols(FILE* inp_file, FILE* out_file, char character, unsigned int counter){
    do {
        character = getc(inp_file);
        if(isdigit(character) || character == '\n'){
            putc(character, out_file);
        }
        else if(character != '\n' || character != EOF){
            fprintf(out_file, "%d|", character);
            continue;
        }
    } while(character != EOF);
}

error_state flag_function(FILE* input, FILE* output, unsigned char func_num){
    if (!input || !output)
        return NULL_PTR;
    char character = ' ';
    unsigned int counter = 0;
    (*flag_ptr[func_num])(input, output, character, counter);  
    return  COOL;
}

void file_and_memory_clean (char* a, char* b, FILE* c, FILE* d){
    free(a);
    free(b);
    fclose(c);
    fclose(d);
}