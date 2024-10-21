#include <stdio.h>
#include <math.h>
#include <stdlib.h>

enum my_bool{FALSE, TRUE};
enum error_state{
    DONE,
    INVALID_INPUT,
    WRONG_PARAMETERS,
    WRONG_PARAMETERS_NUMBER,
    OVERFLOW,
    NULL_PTR
};

enum error_state to_double(char* word, double* res);
enum my_bool convert_check(enum error_state condition);

enum error_state raw_a(double* epsilon, double* x, double* res);
enum error_state raw_b(double* epsilon, double* x, double* res);
enum error_state raw_c(const double* epsilon, double* x, double* res);
enum error_state raw_d(double* epsilon, double* x, double* res);

int main(int argc, char* argv[]){
    if(argc!= 3){
        printf("Wrong amount of initial parameteres\n");
        return WRONG_PARAMETERS_NUMBER;
    }
    double epsilon = 0.0, var = 0.0;
    if(!(convert_check(to_double(argv[1], &epsilon)) && convert_check(to_double(argv[2], &var)))){
        return INVALID_INPUT;
    }
    if(!(epsilon > 0. && epsilon < 1.)){
        printf("Epsilon is to big\n");
        return WRONG_PARAMETERS;
    }

    double summary[4];

    if(raw_a(&epsilon, &var, &summary[0]) == DONE){
        printf("The summary of raw 'a' is %lf, WolphramAlpha says it's e^%lf, i.e. %lf\n", summary[0], var, exp(var));
    }
    else{
        printf("Null-ptr in raw 'a'\n");
        return NULL_PTR;
    }

    if(raw_b(&epsilon, &var, &summary[1]) == DONE){
        printf("The summary of raw 'b' is %lf, WolphramAlpha says it's cos(%lf), i.e. %lf\n", summary[1], var, cos(var));
    }
    else{
        printf("Null-ptr in raw 'b'\n");
        return NULL_PTR;
    }

    if(raw_c(&epsilon, &var, &summary[2]) == DONE){
        printf("The summary of raw 'c' is %lf, WolphramAlpha says it's divergent if abs(x) big enough\n", summary[2]);
    }
    else{
        printf("Null-ptr in raw 'c'\n");
        return NULL_PTR;
    }

    if(raw_d(&epsilon, &var, &summary[3]) == DONE){
        printf("The summary of raw 'd' is %lf, WolphramAlpha says  it's divergent if abs(x) big enough\n", summary[3]);       //FIXME: Check what WolframAlpha says it
    }
    else{
        printf("Null-ptr in raw 'd'\n");
        return NULL_PTR;
    }
    

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

    if((*res == 0. && endptr == word) || *endptr != '\0'){
        return WRONG_PARAMETERS;
    }
    
    return DONE;
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


enum error_state raw_a(double* epsilon, double* x, double* result){
    if(epsilon == NULL || result == NULL || x == NULL){
        *result = 0.;
        return NULL_PTR;  
    }

    if(fabs(*x) < *epsilon){       // x +- == 0.
        *result = 1.;
    }
    else{
        double curr_sum = 1., n = 1;
        *result = 0;
        while(fabs(curr_sum) > *epsilon && isfinite(*result)){
            *result += curr_sum;
            curr_sum *= *x / (n + 1);
            ++n;
        }
    }

    return DONE;
}

enum error_state raw_b(double* epsilon, double* x, double* result){
    if(epsilon == NULL || result == NULL || x == NULL){
        *result = 0.;
        return NULL_PTR;  
    }

    if(fabs(*x) < *epsilon){       // x +- == 0.
        *result = 1.;
    }
    else{
        double curr_sum = 1., curr_n = 0;
        *result = 0;
        while(fabs(curr_sum) > *epsilon && isfinite(*result)){
            *result += curr_sum;
            curr_sum *= -(*x * *x / ((2 * curr_n + 1) * (2 * curr_n + 2)));
            ++curr_n;
        }
    }
    return DONE;
}

enum error_state raw_c(const double* epsilon, double* x, double* result){
    if(epsilon == NULL || result == NULL || x == NULL){
        *result = 0.;
        return NULL_PTR;  
    }

    if(fabs(*x) < *epsilon){       // x +- == 0.
        *result = 1.;
    }
    else{
        double curr_sum = 1., curr_n = 0;
        *result = 0.;
        while(fabs(curr_sum) > *epsilon && isfinite(*result)){
            *result += curr_sum;
            curr_sum *= (9 * (curr_n + 1) * (curr_n + 1) * *x * *x) / ((3 * curr_n + 1) *(3 * curr_n + 2));
            ++curr_n;
        }
    }
    return DONE;
}

enum error_state raw_d(double* epsilon, double* x, double* result){
    if(epsilon == NULL || result == NULL || x == NULL){
        *result = 0.;
        return NULL_PTR;  
    }
    if (fabs(*x) >= 1){
        *result = INFINITY;
    }
    else{
        if(fabs(*x) < *epsilon){       // x +- == 0.
            *result = 1.;
        }
        else{
            double curr_sum = -*x * *x / 2.0, curr_n = 1;
            *result = 0;
            while(fabs(curr_sum) > *epsilon && isfinite(*result)){
                *result += curr_sum;
                curr_sum *= -((2 * curr_n + 1) * *x * *x) / (2* curr_n + 2);
                ++curr_n;
            }
        }
    }
    return DONE;
}