#include <stdio.h>      
#include <stdarg.h>


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

#define BUFSIZE 200000

typedef struct result_indexes{
    int line_position;
    int char_position;
    char* file_name;
} result_indexes;


long int my_strlen(char* str);
error_state get_back(FILE* file, int* char_index, int* line_index, char* substr, int* substr_index);
error_state find_substr_in_many_files(char* substr, int file_number, int* res_length, result_indexes* res, ...);


int main(void){

    result_indexes coincidences[BUFSIZE];
    int res_length = -1;
                                                    //TOO: substr && FILENAME      <--<--<--<--<--<--<--<--<--<--<--<--
    error_state personal_errno = find_substr_in_many_files("333", 2, &res_length, coincidences, "TEXT_DOC_31.txt", "TEXT_DOC_32.txt");

    /*for (int i = 0; i < 5; ++i){
        printf("file: %s, line: %d, number: %d\n", coincidences[i].file_name, coincidences[i].line_position, coincidences[i].char_position);
    }*/

    switch (personal_errno){
        case NULL_PTR:
            printf("NULL-PTR FOUND\n");
            return NULL_PTR;
        case WRONG_PARAMETER:
            printf("wrong amount of files\n");
            return WRONG_PARAMETER;
        case FILE_FUCKUP:
            printf("COULDN'T OPEN FILE\n");
            return FILE_FUCKUP;
        case MEMORY_FUCKUP:
            printf("Too many substrings\n");
            return MEMORY_FUCKUP;
        case FUCKUP:
            printf("You spoiled protected function, idiot\n");
            return FUCKUP;
        case COOL:
            if (res_length == -1){
                printf("No substr was found\n");
            }
            for (int i = 0; i <= res_length; ++i){
                printf("file: %s, line: %d, number: %d\n", coincidences[i].file_name, coincidences[i].line_position, coincidences[i].char_position);
            }

            return COOL;
        default:
            return COOL;
    
    }


    

//TDO: call function

}

long int my_strlen(char* str){
    char const* end = str;
	while (*end++);
	return end - str - 1;
}

error_state find_substr_in_many_files(char* substr, int file_number, int* res_length,  result_indexes* res, ...){
    if (!substr || !res)
        return NULL_PTR;
    if (file_number < 1)
        return WRONG_PARAMETER;

    int substr_length = my_strlen(substr), char_index = 0, line_index = 1, substr_char_index = 0;
    int coincidence_char_index = 0, coincidence_line_index = 0;

    FILE* current_file;
    char* current_file_name;
    char current_c;
    va_list item;
    va_start(item, res);

    for(int i = 0; i < file_number; ++i){
        char_index = 0, line_index = 1, substr_char_index = 0;
        current_file_name = va_arg(item, char*);

        if (!(current_file = fopen(current_file_name, "r"))){
            va_end(item);
            return FILE_FUCKUP;
        }

        while((current_c = getc(current_file)) != EOF){
            ++char_index;
            if (current_c == '\n')
                ++line_index;   
            
            if (current_c == substr[substr_char_index]){ // есть совпадение
                ++substr_char_index;

                if(substr_char_index == 1){     //remember position
                    coincidence_char_index = char_index;
                    coincidence_line_index = line_index;
                }

                if (substr_char_index == substr_length){
                    
                    // remember coincidence
                    if (*res_length < BUFSIZ){
                        ++(*res_length);
                        res[*res_length].char_position = coincidence_char_index;
                        res[*res_length].line_position = coincidence_line_index;
                        res[*res_length].file_name = current_file_name;
                    }
                    else
                        return MEMORY_FUCKUP;
                
                    if (get_back(current_file, &char_index, &line_index, substr, &substr_char_index) != COOL)
                        return FUCKUP;
                        
                    if(current_c == '\n')
                        --line_index;
                }
            }
            else if (substr_char_index != 0){

                if (get_back(current_file, &char_index, &line_index, substr, &substr_char_index) != COOL)
                    return FUCKUP;

                if(current_c == '\n')
                    --line_index;
            }

            if (current_c == '\n')
                char_index = 0;
        }

        fclose(current_file);
    }

    va_end(item);
    return COOL;
}



//FIXME: rewrite this function
error_state get_back(FILE* file, int* char_index, int* line_index, char* substr, int* substr_index){
    if (!file || !char_index || !line_index || ! substr || !substr_index)
        return NULL_PTR;
    
    fseek(file, -(*substr_index - 1), SEEK_CUR);

    for(int i = 0; i < (*substr_index); ++i){
        if (substr[i] == '\n')
            --(*line_index);
    }

    *char_index -= (*substr_index - 1);
    *substr_index = 0;
    

    return COOL;
}

//FIXME: extra strings_positions. IDK why