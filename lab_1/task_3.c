#include <stdio.h>
#include <string.h>     //strlen
#include <limits.h>     //type_limits
#include <math.h>       //HUGE_VAL; fabs  gcc fird_task.c -lm
#include <stdlib.h>     //strtod
#include <float.h>      //DBL_MAX

#define ABS(x) ((x) >=0 ? (x) : -1*(x))

enum my_bool{FALSE, TRUE};

enum error_state{
    DONE,
    INVALID_INPUT,
    WRONG_PARAMETERS,
    WRONG_PARAMETERS_NUMBER,
    OVERFLOW,
    NULL_PTR
};

enum amount_of_solutions{
    ZERO,
    ONE,
    TWO
};

typedef struct{
    double first;
    double second;
} my_Pair;

enum error_state to_long_int(char* word, long long int*  res);
enum error_state to_double(char* word, double* res);
enum error_state double_triangle_inequality(double* epsilon, double* one, double* two, double* three, enum my_bool* result);

//int permutations(double* a, double* b, double* c, double* epsilon);
enum my_bool convert_check(enum error_state condition);

enum amount_of_solutions find_root(double* epsilon, double* a, double* b, double* c,my_Pair* placemment);

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("Wrong nubmer of parameters\n");
        return WRONG_PARAMETERS_NUMBER;
    }

    if (strlen(argv[1]) != 2 || argv[1][0] != '-' && argv[1][0] != '/'){
        printf("Wrong flag parameter\n");
        return WRONG_PARAMETERS;
    }

    switch (argv[1][1]){
        case 'q':
            if (argc != 6){
                printf("Wrong number of parameters\n");
                return WRONG_PARAMETERS_NUMBER;
            }
            
            double q_epsilon = 0., a = 0., b = 0., c = 0.;

            if(!(convert_check(to_double(argv[2], &q_epsilon)) && convert_check(to_double(argv[3], &a)) &&
            convert_check(to_double(argv[4], &b)) && convert_check(to_double(argv[5], &c)))){
                //printf("Invalid input\n");
                return INVALID_INPUT;
            }

            if(!(q_epsilon > (double)0 && q_epsilon < (double)1)){
                printf("WRONG_EPSILON\n");
                return WRONG_PARAMETERS;
            }
            
            const int perm_cnt = 6;
            my_Pair results[6];
            int valid_num[6];
            char curr_index = 0;

            for(int i = 0; i < perm_cnt; ++i){
                valid_num[i] = find_root(&q_epsilon, &a, &b, &c, &(results[i]));
                if(i % 2 == 0){
                    double btw = b;     //swap коэффициентов
                    b = c;
                    c = btw;
                }
                else{
                    double btw = b;     //swap коэффициентов
                    b = a;
                    a = btw;
                }
            }

            for(int i = 0; i < perm_cnt; ++i){
                if(valid_num[i] == ONE)
                    printf("%lf\n", results[i].first);
                else if(valid_num[i] == TWO)
                    printf("%lf %lf\n", results[i].first, results[i].second);
            }
            break;
        
        case 'm':
            if (argc != 4){
                printf("Wrong nubmer of parameters\n");
                return WRONG_PARAMETERS_NUMBER;
            }

            long long int first = 0, second = 0;

            if(! (convert_check(to_long_int(argv[2], &first)) && convert_check(to_long_int(argv[3], &second)))){
                return INVALID_INPUT;
            }
            if (labs(second) == 0){
                printf("CAn't divide on zero\n");
            }
            else if (first % second == 0){
                printf("True, %lld is divided by %lld\n", first, second);
            }
            else{
                printf("False, %lld is NOT divided by %lld\n", first, second);
            }
            return DONE;
        
        case 't':
            if (argc != 6){
                printf("Wrong number of parameters\n");
                return WRONG_PARAMETERS_NUMBER;
            }

            double epsilon = 0., one = 0., two = 0., three = 0.;
        
            if(! (convert_check(to_double(argv[2], &epsilon)) && convert_check(to_double(argv[3], &one)) &&
            convert_check(to_double(argv[4], &two)) && convert_check(to_double(argv[5], &three)))){
                return INVALID_INPUT;
            }

            enum my_bool result = FALSE;
            switch (double_triangle_inequality(&epsilon, &one, &two, &three, &result)){
                case DONE:
                    if (result)
                        printf("Right triangle exists\n");
                    else
                        printf("Right triangle doesn't exist\n");
                    return DONE;

                case OVERFLOW:
                    printf("Overflov\n");
                    return OVERFLOW;

                case WRONG_PARAMETERS:
                    printf("Wrong  parameters\n");
                    return WRONG_PARAMETERS;
                    
                default:
                    break;
            }
            
            break;
        
        default:
            printf("Unknown flag\n");
    }

}

enum my_bool convert_check(enum error_state condition){
    switch(condition){
        case NULL_PTR:
            printf("NULL-ptr found\n");
            return FALSE;
        case OVERFLOW:
            printf("Number overflow\n");
            return FALSE;
        case WRONG_PARAMETERS:
            printf("Wrong parameter passed\n");
            return FALSE;    
        default:
            break;
    }
    return TRUE;
}

enum error_state double_triangle_inequality (double* epsilon, double* one, double* two, double* three, enum my_bool* result){
    if (epsilon == NULL || one == NULL || two == NULL || three == NULL){
        *result = FALSE;
        return NULL_PTR;
    }
    
    if(!(*epsilon > (double)0 && *epsilon < (double)1)){
        return WRONG_PARAMETERS;
    }

    if(*one <-*epsilon || *two < -*epsilon || *three < -*epsilon){       // < 0
        return WRONG_PARAMETERS;
    }

    //*one == min; *two == middle; *three == max        //primitive sort
    if (*one - *two > *epsilon){     // *one > *two
        double btw = *two;
        *two = *one;
        *one = btw;
    }
    if (*one - *three > *epsilon){  // *one > *three
        double btw = *one;
        *one = *three;
        *three = btw;
    }
    if (*two - *three > *epsilon){  // *two > *three
        double btw = *two;
        *two = *three;
        *three = btw;
    }
    
    /*if (fabs(*three - *two) < *epsilon){
        *result = FALSE;
    }

    else if (*three > DBL_MAX / *three || *one * *one > DBL_MAX - *two * *two) {
        return OVERFLOW;
    }*/

    if (fabs(*three * *three - *one * *one - *two * *two) < *epsilon){
        *result = TRUE;
    }
    else   
        *result = FALSE;
    return DONE;
}


enum error_state to_long_int(char* word, long long int* res){

    if (word == NULL || res == NULL){       //NULL check
        return NULL_PTR;
    }

    char* ptr = word;
    *res = 0;
    enum my_bool minus = FALSE;

    while(*ptr == ' ' || *ptr == '\n' || *ptr == '\t' || *ptr == '\v' || *ptr == '\r' || *ptr == '\b'){     //space skip
        ++ptr;
    }

    while (*ptr != '\0'){
        if(ptr == word && *ptr == '-'){     //minus check
            minus = TRUE;
            ++ptr;
            continue;
        }

        if(ptr == word && *ptr == '+'){     //plus check
            ++ptr;
            continue;
        }

        if (*ptr < '0' || *ptr > '9'){      //is digit check
            return WRONG_PARAMETERS;
        }

        if ((LONG_MAX - (*ptr - '0')) / 10 < *res){     //overflow check
            return OVERFLOW;
        }

        *res *= 10;
        *res += (*ptr - '0');
        ++ptr;
    }

    if(minus)       //add minus
        *res  = -*res; 
    return DONE;
}   

enum error_state to_double(char* word, double* res){
    if(word == NULL || res == NULL){
        return NULL_PTR;
    }

    char* endptr = NULL;
    *res = strtod(word, &endptr);

    if (*res == HUGE_VAL || *res == -HUGE_VAL){
        return OVERFLOW;
    }

    if((*res == 0 && endptr == word) || *endptr != '\0'){
        return WRONG_PARAMETERS;
    }
    
    return DONE;
}

enum amount_of_solutions find_root(double* epsilon, double* a, double* b, double* c, my_Pair* placement){

    if(fabs(*a) <= *epsilon && fabs(*b) <= *epsilon){
        placement->first = placement->first = -1.;
        return ZERO;
    }

    if(fabs(*a) < *epsilon){
        if (fabs(*c) < *epsilon)
            placement->first = placement->first = 0.;
        else
            placement->first = placement->first = -(*c / *b);
        return ONE;
    }

    if(fabs(*b) < *epsilon){
        placement->first = sqrt(-*c / *a);
        placement->second = -1 * sqrt(-*c / *a);
        return TWO;
    }
    else {
        double discriminant = (*b * *b) - 4.0 * *a * *c;
        if (discriminant < -*epsilon){
            placement->first = placement->second = -1;
            return ZERO;
        }
        else {
            if(fabs(discriminant) <= *epsilon){

                placement->first = -*b / (2. * *a);
                placement->second = -*b  / (2. * *a);
                return ONE;
            }
            discriminant = sqrt(discriminant);
            placement->first = (-*b + discriminant) / (2. * *a);
            placement->second = (-*b - discriminant) / (2. * *a);
            return TWO;
        }
    }
}