#include <stdio.h>

#define MAX_NAME_SIZE 100

typedef enum error_state{
    COOL,
    MEMORY_FUCKUP,
    WRONG_PARAMETER,
    FILE_FUCKUP,
    NULL_PTR
} error_state;


typedef struct Student {
    unsigned long id;
    char name[MAX_NAME_SIZE];
    char surname[MAX_NAME_SIZE];
    char group[MAX_NAME_SIZE];
    unsigned char* marks_array;
} Student;

error_state create_Student (Student *res, unsigned long s_id, char *s_name, char *s_surname, char *s_group, unsigned char *s_array);

void terminate_Student(Student *a);


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong amount of parameters\n");
        return WRONG_PARAMETER;
    }

    char *absolute_input, *absolute_output;


    //TODO: parse input && output files, get their absolute paths

    FILE* input = fopen(absolute_input, "r");
    if (!input)
        return FILE_FUCKUP;
    FILE* output = fopen(absolute_output, "w");
    if (!output) {
        fclose(input);
        return FILE_FUCKUP;
    }


}




error_state create_Student (Student *res, unsigned long s_id, char *s_name, char *s_surname, char *s_group, unsigned char *s_array) {
    if (!res || !s_name || !s_surname || !s_group || !s_array) {
        return NULL_PTR;
    }
    unsigned int name_length = strlen(s_name), surname_length = strlen(s_surname);

    if (!name_length || !surname_length || !strlen(s_group) || !strlen(s_array)) {
        return WRONG_PARAMETER;
    }

    for(unsigned int i = 0; i < name_length; ++i) {
        if (!isalpha(s_name[i]))
            return WRONG_PARAMETER;
    }

    for(unsigned int i = 0; i < surname_length; ++i) {
        if (!isalpha(s_surname[i]))
            return WRONG_PARAMETER;
    }

    res = (Student*) malloc(sizeof(Student));
    if (!res)
        return MEMORY_FUCKUP;

    res->marks_array = (unsigned char*) malloc(sizeof(unsigned char) * 6);
    if (!res->marks_array){
        free(res);
        return MEMORY_FUCKUP;
    }
    
    res->id = s_id;
    strncpy(res->name, s_name, MAX_NAME_SIZE);
    strncpy(res->surname, s_surname, MAX_NAME_SIZE);
    strncpy(res->group, s_group, MAX_NAME_SIZE);
    strncpy(res->marks_array, s_array, 6);             //FIXME: strncpy

    return COOL;
}



void terminate_Student(Student *a) {
    free(a->marks_array);
    free(a);
}




