#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum error_state{
    COOL,
    MEMORY_FUCKUP,
    WRONG_PARAMETER,
    NULL_PTR
} error_state;


typedef struct Tim_string {
    char* line;
    unsigned long int length;
}   Tim_string;




Tim_string* create_string(const char* init_char) {

    Tim_string *new_string = (Tim_string*) malloc(sizeof(Tim_string));
    if (!new_string)
        return NULL;

    new_string->line = NULL;
    new_string->length = 0;
    
    if (!init_char)
        return new_string;

    new_string->line = (char*) malloc(sizeof(char) * (strlen(init_char) + 1));
    if (!new_string->line)
        return new_string;
    
    new_string->length = strlen(init_char) + 1;
    strcpy(new_string->line, init_char);

    return new_string;
}


void delete_inside_string(Tim_string *delete_me) {
    free(delete_me->line);
    delete_me->line = NULL;
    delete_me->length = 0;
}


int string_comparator (const Tim_string *left, const Tim_string *right) {
    if (!left || !right)
        return -1;

    if (left->length != right->length)
        return (left->length - right->length);  
    
    for (unsigned int i = 0; i < left->length; ++i) {
        if ((left->line)[i] != (right->line)[i])
            return (left->line)[i] - (right->line)[i];
    }
    return 0;   
}


int is_equal (const Tim_string *left, const Tim_string *right) {
    return (string_comparator(left, right) == 0) ? 1 : 0;
}


error_state string_exist_cpy (const Tim_string *what, Tim_string *where) {
    if (!what || ! where || !what->line || !where->line)
        return NULL_PTR;

    if (is_equal(where, what))
        return COOL;
    
    char* tmp =(char*) realloc(where->line, sizeof(char) * (what->length));
    if (!tmp)
        return MEMORY_FUCKUP;

    where->length = what->length;
    where->line = tmp;
    strcpy(where->line, what->line);

    tmp = NULL;
    return COOL;
}


Tim_string* string_non_exist_cpy (const Tim_string *what) {
    if (!what || !what->line)
        return NULL;
    
    return create_string(what->line);
}

error_state concatenate(Tim_string* one, const Tim_string* two) {
    if (!one || !two || !one->line || !two->line)
        return NULL_PTR;

    one->length += two->length - 1;

    char* tmp = (char*) realloc (one->line, sizeof(char) * (one->length));
    if (!tmp) {
        one->length -= two->length - 1;
        return MEMORY_FUCKUP;
    }

    one->line = tmp;
    strcat(one->line, two->line);

    tmp = NULL;
    return COOL;
}

void terminator (Tim_string* a) {
    delete_inside_string(a);
    free(a);
    a = NULL;
}











