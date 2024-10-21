#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

typedef enum error_state{
    COOL,
    FUCKUP,
    NULL_PTR,
    WRONG_PARAMETER,
    OVERFLOW,
    MEMORY_FUCKUP,
    FILE_FUCKUP
} error_state;

typedef enum my_bool{
    FALSE,
    TRUE
} my_bool;

error_state get_absolute_path(char* current_path, char* absolute_path);
error_state what_flag(char* input_flag, char* result);
void recursive_print_num_in_base(long int init_number, FILE* output_file, int base);

error_state flag_r(FILE* one_input, FILE* two_input, FILE* output);
error_state flag_a(FILE* input_file, FILE* output_file);

int main(int argc, char*argv[]){
    if(argc < 4){
        printf("Wrong amount of parameters\n");
        return WRONG_PARAMETER;
    }

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    char* absolute_input = malloc(sizeof(char) * FILENAME_MAX);
    if(!absolute_input)
        return NULL_PTR;
    
    char* absolute_output = malloc(sizeof(char) * FILENAME_MAX);
    if(!absolute_output){
        free(absolute_input);
        return NULL_PTR;
    }
    
    char flag = ' ';
    switch(what_flag(argv[1], &flag)){
        case NULL_PTR:
            printf("NULL_PTR found in flag function\n");
            return NULL_PTR;
        case WRONG_PARAMETER:
            printf("WRONG PARAMETER found in flag function\n");
            return WRONG_PARAMETER;
        case COOL:
            break;
        default:
            break;
    }
    
    if(flag == 'r' && argc < 5 || flag == 'a' && argc < 4){
        printf("Wrong amount of parameters\n");
        return WRONG_PARAMETER;
    }
        //GET ABSOLUTE PATH BLOCK
    int personal_errno = get_absolute_path(argv[2], absolute_input);
    if(personal_errno != COOL){
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
    personal_errno = get_absolute_path(argv[argc - 1], absolute_output);
    if(personal_errno != COOL){
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
    
    if (flag == 'a'){
        input_file = fopen(absolute_input, "r");
        if(!input_file){
            free(absolute_input);
            free(absolute_output);
            printf("FILE open fuckup\n");
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
        
        switch(flag_a(input_file, output_file)){
            case NULL_PTR:
                printf("NULL-PTR was found in flag_a funciton\n");
                free(absolute_input);
                free(absolute_output);
                fclose(input_file);
                fclose(output_file);
                return NULL_PTR;
            case COOL:
                free(absolute_input);
                free(absolute_output);
                fclose(input_file);
                fclose(output_file);
                break;
            default: break;
        }
    }
    else{   //flag == r
        char* absolute_second_input = malloc(sizeof(char) * FILENAME_MAX);
        if(!absolute_second_input){
            free(absolute_input);
            free(absolute_output);
            fclose(input_file);
            fclose(output_file);
            return NULL_PTR;
        }
        
        personal_errno = get_absolute_path(argv[3], absolute_second_input);
        if(personal_errno != COOL){
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

        if(strcmp(absolute_input, absolute_output) == 0 || strcmp(absolute_input, absolute_second_input) == 0){
            free(absolute_input);
            free(absolute_output);
            free(absolute_second_input);
            printf("the same file was opened 2 times\n");
            return WRONG_PARAMETER;
        }

        input_file = fopen(absolute_input, "r");
        if(!input_file){
            free(absolute_input);
            free(absolute_output);
            free(absolute_second_input);
            printf("FILE open fuckup\n");
            return FILE_FUCKUP;
        }
        
        output_file = fopen(absolute_output, "w");
        if(!output_file){
            free(absolute_input);
            free(absolute_output);
            free(absolute_second_input);
            fclose(input_file);
            printf("Error during opening file\n");
            return NULL_PTR;
        }
        
        FILE* second_input_file = NULL;
        second_input_file = fopen(absolute_second_input, "r");
        if(!second_input_file){
            free(absolute_input);
            free(absolute_output);
            free(absolute_second_input);
            fclose(input_file);
            fclose(output_file);
        }

        if(flag_r(input_file, second_input_file, output_file) == NULL_PTR){
            printf("NULL_PTR found in flag_r\n");
            free(absolute_input);
            free(absolute_output);
            free(absolute_second_input);
            fclose(input_file);
            fclose(output_file);
            fclose(second_input_file);
            return NULL_PTR;
        }
        else{
            free(absolute_input);
            free(absolute_output);
            free(absolute_second_input);
            fclose(input_file);
            fclose(output_file);
            fclose(second_input_file);
            return COOL;
        }
    }

    return COOL;
}

error_state flag_a(FILE* input_file, FILE* output_file){
    if(!input_file || !output_file)
        return NULL_PTR;
    
    unsigned long int counter = 1;
    char curr_char = getc(input_file);

    while(curr_char != EOF){
        while((curr_char == ' ' || curr_char == '\n' || curr_char == '\t') && curr_char != EOF){
            curr_char = getc(input_file);
        }
        if (counter % 10 == 0){
            while(curr_char != ' ' && curr_char != '\n' && curr_char != '\t' && curr_char != EOF){
                curr_char = tolower(curr_char);
                recursive_print_num_in_base(curr_char, output_file, 4);
                putc('|', output_file);
                curr_char = getc(input_file);
            }
            putc('\n', output_file);
        }
        else if (counter % 2 == 0){
            while(curr_char != ' ' && curr_char != '\n' && curr_char != '\t' && curr_char != EOF){
                curr_char = tolower(curr_char);
                putc(curr_char, output_file);
                curr_char = getc(input_file);
            }
            putc('\n', output_file);
        }
        else if(counter % 5 == 0){
            while(curr_char != ' ' && curr_char != '\n' && curr_char != '\t' && curr_char != EOF){
                recursive_print_num_in_base(curr_char, output_file, 8);
                putc('|', output_file);
                curr_char = getc(input_file);
            }
            putc('\n', output_file);
        }
        else{
            while(curr_char != ' ' && curr_char != '\n' && curr_char != '\t' && curr_char != EOF){
                putc(curr_char, output_file);
                curr_char = getc(input_file);
            }
            putc('\n', output_file);
        }
        ++counter;
    }
    return COOL;
}


error_state flag_r(FILE* one_input, FILE* two_input, FILE* output){
    if(!one_input || !two_input || !output){
        return NULL_PTR;
    }

    char one_char = getc(one_input), two_char = getc(two_input);
    my_bool who_prints = FALSE;

    while(one_char != EOF && two_char != EOF){
        if(who_prints == FALSE){
            while((one_char == ' ' || one_char == '\n' || one_char == '\t') && one_char != EOF)
                one_char = getc(one_input);
            while(one_char != ' ' && one_char != '\t' && one_char != '\n' && one_char != EOF){
                putc(one_char, output);
                one_char = getc(one_input);
            }
            putc(' ', output);
            who_prints = TRUE;
        }
        else{
            while((two_char == ' ' || two_char == '\n' || two_char == '\t') && two_char != EOF)
                two_char = getc(two_input);
            while(two_char != ' ' && two_char != '\t' && two_char != '\n' && two_char != EOF){
                putc(two_char, output);
                two_char = getc(two_input);
            }
            putc(' ', output);
            who_prints = FALSE;
        }
    }

    while(one_char != EOF){
        while((one_char == ' ' || one_char == '\n' || one_char == '\t') && one_char != EOF)
            one_char = getc(one_input);
        while(one_char != ' ' && one_char != '\t' && one_char != '\n' && one_char != EOF){
            putc(one_char, output);
            one_char = getc(one_input);
        }
        putc(' ', output);
    }

    while(two_char != EOF){
        while((two_char == ' ' || two_char == '\n' || two_char == '\t') && two_char != EOF)
                two_char == getc(two_input);
        while(two_char != ' ' && two_char != '\t' && two_char != '\n' && two_char != EOF){
            putc(two_char, output);
            two_char = getc(two_input);
        }
        putc(' ', output);

    }
    
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

error_state what_flag(char* input_flag, char* result){
    if(!input_flag || !result)
        return NULL_PTR;
    if (strlen(input_flag) != 2 || !(input_flag[0] == '-' || input_flag[0] == '/')){
        return WRONG_PARAMETER;
    }

    switch(input_flag[1]){
        case 'a':
            *result = 'a';
            break;
        case 'r':
            *result = 'r';
            break;
        default:
            *result = '\0';
            return WRONG_PARAMETER;
    }
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
            putc('a'  + labs(init_number) - 10, output_file);
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
            putc('a' + reminder - 10, output_file);
            //printf("%c", 'A' + reminder - 10);
        }
    }
}

