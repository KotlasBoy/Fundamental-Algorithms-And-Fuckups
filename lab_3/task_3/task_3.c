#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <ctype.h>

#define FILE_MAX 2048
#define MY_EPSILON 0.00001

typedef enum error_state {          
    COOL,
    WRONG_PARAMETER,
    MEMORY_FUCKUP,
    NULL_PTR,
    FILE_FUCKUP,
    FUCKUP,
    OVERFLOW
} error_state;



typedef struct Employee {
    unsigned long id;
    char *name, *surname;
    double salary;      //must be non negative
} Employee;


//error_state get_absolute_path(char* current_path, char* absolute_path);

error_state parse_input_data(Employee **array, FILE *input, unsigned int *e_array_length);
error_state validate_employee(Employee* a);

int comparator_a(const void* a, const void* b);
int comparator_d(const void* a, const void* b);

int main(int argc, char* argv[]) {

    //input checking
    if (argc != 4 || (argv[2][0] != '/' && argv[2][0] != '-') || strlen(argv[2]) != 2 || (argv[2][1] != 'a' && argv[2][1] != 'd')) {
        printf("Something wrong with initial parameters\n");
        return WRONG_PARAMETER;
    }
    
    char absolute_input[FILE_MAX], absolute_output[FILE_MAX];
    if (!realpath(argv[1], absolute_input) || !realpath(argv[3], absolute_output))
        return FILE_FUCKUP;
    if (!strcmp(absolute_input, absolute_output))
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
    
    
    Employee *e_array = NULL;
    unsigned int e_array_length = 0;

    parse_input_data(&e_array, input, &e_array_length);

    if (argv[2][1] == 'a')
        qsort(e_array, e_array_length, sizeof(Employee), comparator_a);
    else if (argv[2][1] == 'd')
        qsort(e_array, e_array_length, sizeof(Employee), comparator_d);

    for(unsigned int i = 0; i < e_array_length; ++i)
        fprintf(output, "%lu %s %s %lf\n", e_array[i].id, e_array[i].name, e_array[i].surname, e_array[i].salary);
    free(e_array);
    fclose(input);
    fclose(output);
    return COOL;
}


error_state parse_input_data(Employee **array, FILE *input, unsigned int *e_array_real_length) {
    
    if (!input | !e_array_real_length)
        return NULL_PTR;

    unsigned long capacity = 2, count_scanned_fields = 0;  
    *e_array_real_length = 0;

    *array = NULL;

    Employee* current_e = (Employee*) malloc(sizeof(Employee) * capacity);
    if (!current_e)
        return MEMORY_FUCKUP;

    //FIXME: НИ робит((((9))))
    fscanf(input, "%lu%s%s%lf\n", &(current_e[*e_array_real_length].id), current_e[*e_array_real_length].name, current_e[*e_array_real_length].surname, &(current_e[*e_array_real_length].salary));
    
    printf(" %lu\n%s\n%s\n%lf\n", current_e[*e_array_real_length].id, current_e[*e_array_real_length].name, current_e[*e_array_real_length].surname, current_e[*e_array_real_length].salary);



    while((count_scanned_fields = fscanf(input, "%lu%s%s%lf\n", &(current_e[*e_array_real_length].id), current_e[*e_array_real_length].name, current_e[*e_array_real_length].surname, &(current_e[*e_array_real_length].salary))) == 4) {
        if (validate_employee(current_e) != COOL) {
            free(current_e);
            return FUCKUP;
        }

        (*e_array_real_length)++;
        if (*e_array_real_length == capacity) {
            capacity *= 2;
            Employee *tmp = (Employee *) realloc(current_e, sizeof(Employee) * capacity);
            if (!tmp) {
                free(tmp);
                return MEMORY_FUCKUP;
            }
            current_e = tmp;
        }
    }
    if (count_scanned_fields < 4 && count_scanned_fields > 0) {
        free(current_e);
        return FUCKUP;
    }
    *array = current_e;
    return COOL;
}

int comparator_a(const void* a, const void* b) {
    const Employee *emp_a = (const Employee *)a;
    const Employee *emp_b = (const Employee *)b;
    int temp;

    if (emp_a->salary - emp_b->salary > MY_EPSILON) {
        return 1;
    }
    if (emp_b->salary - emp_a->salary > MY_EPSILON) {
        return -1;
    }

    temp = strcmp(emp_a->surname, emp_b->surname);
    if (temp)
        return temp;

    temp = strcmp(emp_a->name, emp_b->name);
    if (temp)
        return temp;

    return emp_a->id - emp_b->id;
}

int comparator_d(const void *a, const void *b) {
    return comparator_a(a, b) * -1;
}



error_state validate_employee(Employee* a) {
    if (!a || !a->name || !a->surname)
        return NULL_PTR;

    unsigned int name_length = strlen(a->name), surname_length = strlen(a->surname);

    for(unsigned int i = 0; i < name_length; ++i)
        if (!isalpha((a->name)[i]))
            return FUCKUP;

    for (unsigned int i = 0; i < surname_length; ++i)
        if(!isalpha((a->surname)[i]))
            return FUCKUP;
    
    if (a->salary < MY_EPSILON)
        return FUCKUP;
    
    return COOL;
}


