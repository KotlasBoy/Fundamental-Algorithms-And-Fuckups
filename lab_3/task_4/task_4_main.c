#include "task_4.c"



int main(void) {
                                                    //создание двух строк
    Tim_string* a = create_string("hello\0");
    Tim_string* b = create_string(", World\0");

    printf("a->line is \"%s\", a->length is \"%lu\"\n", a->line, a->length);
    printf("b->line is \"%s\", b->length is \"%lu\"\n\n", b->line, b->length);


    printf("string_comparator a and b: \"%d\"\n\n", string_comparator(a, b));            //сравнение

    error_state personal_errno = concatenate(a, b);     //конкатенация
    if (personal_errno != COOL) {}
    printf("concatination(to a added b)\n");
    printf("a->line is \"%s\", a->length is \"%lu\"\n", a->line, a->length);
    printf("b->line is \"%s\", b->length is \"%lu\"\n\n", b->line, b->length);



    printf("is_equal a and b result is \"%d\"\n\n", is_equal(a, b));                     //проверка на равенство

    Tim_string *new;
    new = string_non_exist_cpy(a);                          //создание + копирование

    printf("a->line is \"%s\", a->length is \"%lu\"\n", a->line, a->length);
    printf("b->line is \"%s\", b->length is \"%lu\"\n", b->line, b->length);
    printf("new->line is \"%s\", length is %lu\n\n", new->line, new->length);


    personal_errno = string_exist_cpy(b, a);                //копирование в существующий
    if (personal_errno != COOL) {}
    printf("a->line is \"%s\", a->length is \"%lu\"\n", a->line, a->length);
    printf("b->line is \"%s\", b->length is \"%lu\"\n", b->line, b->length);
    printf("new->line is \"%s\", length is %lu\n\n", new->line, new->length);


    delete_inside_string(a);        // delete inside
    delete_inside_string(b);


    printf("a->line is \"%s\", a->length is \"%lu\"\n", a->line, a->length);
    printf("b->line is \"%s\", b->length is \"%lu\"\n", b->line, b->length);
    printf("new->line is \"%s\", length is %lu\n\n", new->line, new->length);

    terminator(a);                  // total delete
    terminator(b);
    terminator(new);
 
    ////////////////////////////// ВТОРОЙ ПУНКТ ТАСКИ
    
}

