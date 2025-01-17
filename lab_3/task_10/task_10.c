#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


#define FILE_MAX 2000


typedef enum error_state{
    COOL,
    MEMORY_FUCKUP,
    FILE_FUCKUP,
    WRONG_PARAMETER,
    NULL_PTR,
    OVERFLOW,
    FUCKUP
} error_state;

typedef struct Node {
    char letter;
    struct Node *son, *brother, *parent; 
} Node;

error_state get_absolute_path(char* current_path, char* absolute_path);

void node_initialisation (Node *a);
void tree_terminate (Node *a);

error_state parse_file_lines(FILE *input_file, FILE *output_file);
void tree_file_print(Node *a, FILE *output_file, unsigned int shift);


    //console arguments:    ./a.out input_file.txt output_file.txt
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong amount of parameters\n");
        return WRONG_PARAMETER;
    }

    char absolute_input[FILE_MAX], absolute_output[FILE_MAX];
    error_state personal_errno = get_absolute_path(argv[1], absolute_input);
    if (personal_errno != COOL)
        return FILE_FUCKUP;
    
    personal_errno = get_absolute_path(argv[2], absolute_output);
    if (personal_errno != COOL)
        return FILE_FUCKUP;

    FILE* input = fopen(absolute_input, "r");
    if (!input) {
        printf("can't open file\n");
        return FILE_FUCKUP;
    }
    FILE* output = fopen(absolute_output, "w");
    if (!output) {
        printf("can't open file\n");
        fclose(input);
        return FILE_FUCKUP;
    }

    personal_errno = parse_file_lines(input, output);
    if (personal_errno != COOL) {
        printf("parse crushed\n");
        fclose(input);
        fclose(output);

        return FUCKUP;
    }
    
    fclose(input);
    fclose(output);

    return COOL;
}


error_state parse_file_lines(FILE *input_file, FILE *output_file) {
    if (!input_file || !output_file)
        return NULL_PTR;
    
    int level = 0;
    int letter = 1;
    int prev_is_bracket = 0;

    char expression[FILE_MAX];
    if (fgets(expression, FILE_MAX, input_file) == NULL)
        return FUCKUP;

    for(int i = 0; expression[i] != '\0'; ++i) {
        if (expression[i] == '(') {
            if (prev_is_bracket)
                return FUCKUP;
            ++letter;
            ++level;
            prev_is_bracket = 1;
        }
        else if (expression[i] == ')'){
            --level;
            if (prev_is_bracket)
                return FUCKUP;
        }
        else if (expression[i] == ',') {
            ++letter;
            if (prev_is_bracket)
                return FUCKUP;
        }

        else if (isalpha(expression[i])){
            prev_is_bracket = 0;
            --letter;
            if (letter < 0)
                return FUCKUP;

            for (int j = 0; j < level; ++j)
                fprintf(output_file, "\t");
            fprintf(output_file, "%c\n", expression[i]);
        }
    }
    

}

void tree_file_print(Node *a, FILE *output_file, unsigned int shift){
    //hope that a and output_file are not null_ptr;
    if(!a)
        return;

    for (unsigned int i = 0; i < shift; ++i)
        fputc('\t', output_file);

    fprintf(output_file, "%c\n", a->letter);

    if (a->son)
        tree_file_print(a->son, output_file, shift + 1);
    if (a->brother)
        tree_file_print(a->brother, output_file, shift);


}

void node_initialisation (Node *a) {
    a->letter = '-';
    a->son = NULL;
    a->brother = NULL;
    a->parent = NULL;
}

void tree_terminate (Node* a) {
    if (a->brother)
        tree_terminate(a->brother);
    
    if (a->son)
        tree_terminate(a->son);
    
    free(a);
}





error_state get_absolute_path(char* current_path, char* absolute_path){

    if (!current_path || !absolute_path){
        return NULL_PTR;
    }

    char current_dir[FILE_MAX + 1];                         // TOD:  #define FILE_MAX = number
    getcwd(current_dir, FILE_MAX);                          // TOD: include <stdio.h>
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
    if (current_path[0] == '/'){                    // already absolute for UNIX system
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

    if(valid_char_count + 1 > FILE_MAX){
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

