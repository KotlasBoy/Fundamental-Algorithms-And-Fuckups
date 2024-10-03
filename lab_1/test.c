#include <stdio.h>
#include <stdlib.h>

    // TODO: highlight this
int main(int argc, char* argv[]){

    printf(" the amount of arfuments is: %d\n", argc);

    char* ptr = (char*) malloc(sizeof(char));

    *ptr = 'f';
    printf("hello, char is %c\n", *ptr);

    //free(ptr);
    return 0;

    // FIXME: repair this thing
}