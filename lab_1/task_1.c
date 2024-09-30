#include <stdio.h>
#include <stdlib.h>  //strtol()
#include <limits.h>  //LONG_MAX, LONG_MIN
#include <string.h>  //strlen

#define ABS(x) ((x) >=0 ? (x) : -1*(x))

enum my_bool {FALSE, TRUE};

enum error_state {
    DONE,
    WRONG_PARAMETRES,
    DOUBTFUL_PARAMETRES,
    OVERFLOW
};


enum error_state to_num(char* word, long int* res, int base);
enum error_state flag_h(long int* num);
enum my_bool flag_p(long int* num);
enum error_state flag_f(long int* num);
enum error_state flag_a(long int* num);
void flag_e(long int* num);
void flag_s(long int num);
long int my_pow(int a, int b);
 

int main(int argc, char* const argv[]){
    if (argc != 3 || strlen(argv[1]) != 2 || argc == 3 && (argv[1][0] != '-' && argv[1][0] != '/')){
        printf("Wrong amount of parametres or no flag found\n");
        return WRONG_PARAMETRES;
    }

    long int number = 0;
    char flag = *(argv[1] + 1);
    if (to_num(argv[2], &number, 10)){
        printf("couldn't cast second parameter to lu\n");
        return WRONG_PARAMETRES;
    }
    
    //printf("number is %ld, flag is %c.\n", number, flag);

    switch(flag){
        case 'h':
            if(flag_h(&number)){
                printf("no suitable parametres\n");
                return DOUBTFUL_PARAMETRES;
            }
            break;

        case 'p':
            if (number < 0){
                printf("%ld doesn't belong to natural numbers\n", number);
                return DOUBTFUL_PARAMETRES;
            }
            else{
                printf(flag_p(&number) ? "%ld is prime\n" : "%ld is NOT prime\n", number);
                break;
            }

        case 'f':
            if (number < 0){
                printf("couldn't find factorial of negative number\n");
                return DOUBTFUL_PARAMETRES;
            }

            if(flag_f(&number)){
                printf("factorial overflow\n");
                return OVERFLOW;
            }
            break;

        case 'a':
            if(number < 1){
                printf("0\n");
                return DOUBTFUL_PARAMETRES;
            }
            if(flag_a(&number)){
                printf("accumulation is overflowed\n");
                return OVERFLOW;
            }
            break;

        case 'e':
            if(number > 10 || number < 1){
                printf("Broken condition\n");
                return WRONG_PARAMETRES;
            }
            else{
                flag_e(&number);
            }
            break;

        case 's':
            flag_s(number);
            printf("\n");
            break;

        default:
            printf("Unknown flag\n");
            return WRONG_PARAMETRES;
     }

    return DONE;
}


void flag_s(long int num){
    if (num < 0){
        printf("- ");
        num = ABS(num);
    }
    int btw = 0;
    if(num > 0){
        btw = num % 16;
        num /= 16;
        flag_s(num);
        if(btw < 10)
            printf("%d ", btw);
        else
            printf("%c ", 'A' + btw - 10);
    }
}


enum error_state flag_a(long int* num){
    if (*num < 1){
        printf("sum is 0\n");
        return DOUBTFUL_PARAMETRES;
    }
    unsigned long int res = 0;
    for (int i = 1; i < *num + 1; ++i){
        if (LONG_MAX - i < res){
            return OVERFLOW;
        }
        else{
            res += (unsigned long) i;
        }
    }

    printf("summ of 1 to %ld == %lu\n", *num, res);
    
    return DONE;
}


enum error_state flag_f(long int* num){
    if (*num == 0 || *num == 1){
        printf("1\n");
        return DONE;
    }

    unsigned long int result = 1;
    for (int i = 2; i < *num + 1; ++i){
        if (ULONG_MAX / i < result){
            return OVERFLOW;
        }
        else{
            result = (unsigned long) i * result;
        }
    }

    printf("factorial(%ld) is %lu\n", *num, result);


    return DONE;
}



void flag_e(long int* num){
    for(int i = 1; (i < 10 + 1) && (i < *num + 1); ++i){
        for(int j = 1; j < 5 + 1; ++j){
            printf("%d ^ %-2d = %-10ld  ", j, i, my_pow(j, i));         // TODO дописать своё возведение в степень(((
        }
        printf("\n");
    }
    printf("\n\n");

    for(int i = 1; (i < 10 + 1) && (i < *num + 1); ++i){
        for(int j = 6; (j < 10 + 1); ++j){
            printf("%d ^ %-2d = %-10ld  ", j, i, my_pow(j, i));        
        }
        printf("\n");
    }

}


long int my_pow(int base, int exp){
    long int res = base;
    for(int i = 1; i < exp; ++i)
        res *= (long int) base;    
    return res;
}


enum my_bool flag_p(long int* num){
    char prime = TRUE;
    if(*num != 2 && *num % 2 == 0 || *num == 1)
        prime = FALSE;
    else {
        for (int i = 3; i * i < *num + 1; i += 2){
            if (*num % i == 0){
                prime = FALSE;
                break;
            }
        }
    }
    return prime;
}

enum error_state flag_h(long int* num){
    if (ABS(*num) > 100 || *num == 0)
        return DOUBTFUL_PARAMETRES;
    
    for(int i = ABS(*num); i <= 100; ++i){
        printf(i % *num == 0 ? "%i " : "", i);
    }
    printf("\n");

    return DONE;
    

}

enum error_state to_num(char* word, long int* res, int base){

    char* ptr = word;
    enum my_bool minus = FALSE;
    while (*ptr != '\0'){
        if(ptr == word && *ptr == '-'){
            minus = TRUE;
            ++ptr;
            continue;
        }
        if (ptr != word && (*ptr < '0' || *ptr > '9')){
            printf("Invalid input\n");
            return WRONG_PARAMETRES;
        }
        if (LONG_MAX / 10 < *res){
            printf("Overflow\n");
            return OVERFLOW;
        }
        *res *= 10;
        *res += *ptr - '0';
        ++ptr;
    }
    if(minus)
        *res  = -*res; 
    return DONE;
}

//line for git?