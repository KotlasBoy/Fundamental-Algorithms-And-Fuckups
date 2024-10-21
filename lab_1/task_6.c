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

enum error_state sum_a(double* epsilon, double* res);
enum error_state sum_b(double* epsilon, double* res);
enum error_state sum_c(double* epsilon, double* res);
enum error_state sum_d(double* epsilon, double* res);

int main(int argc, char* argv[]) {
    if(argc!= 2){
        printf("Wrong amount of initial parameteres\n");
        return WRONG_PARAMETERS_NUMBER;
    }
    double epsilon = 0.0;
    if(!convert_check(to_double(argv[1], &epsilon))){
        return INVALID_INPUT;
    }
    if(!(epsilon > 0. && epsilon < 1.)){
        printf("Epsilon is to big\n");
        return WRONG_PARAMETERS;
    }

    double summary[4];

    if (sum_a(&epsilon, &summary[0]) == DONE){
        printf("case 'a' summary is %lf, WolframAlpha says 0.822467\n", summary[0]);
    }
    else{
        printf("Null-ptr in case 'a'\n");
        return NULL_PTR;
    }
    if (sum_b(&epsilon, &summary[1]) == DONE){
        printf("case 'b' summary is %lf, WolframAlpha says 0.890959\n", summary[1]);
    }
    else{
        printf("Null-ptr in case 'b'\n");
        return NULL_PTR;
    }
    if (sum_c(&epsilon, &summary[2]) == DONE){
        printf("case 'c' summary is %lf, WolframAlpha says 1.\n", summary[2]);
    }
    else{
        printf("Null-ptr in case 'c'\n");
        return NULL_PTR;
    }
    if (sum_d(&epsilon, &summary[3]) == DONE){
        printf("case 'd' summary is %lf, WolframAlpha says 0.783431\n", summary[3]);
    }
    else{
        printf("Null-ptr in case 'd'\n");
        return NULL_PTR;
    }

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

enum error_state sum_a(double* epsilon, double* res){
    if (epsilon == NULL || res == NULL){
        return NULL_PTR;
    }

    double partition = 0.5, prev_sum = 0.;
    double curr_sum = log(2), edge = 1.;

    while (fabs(prev_sum - curr_sum) > *epsilon){
        prev_sum = curr_sum;
        curr_sum = 0.;
        for(double x = partition; fabs(x - edge) >= *epsilon; x += partition){
            curr_sum += (log(1 + x) / x) * partition;
        }
        partition /= 2;
    }

    *res = curr_sum;
    return DONE;
}

enum error_state sum_b(double* epsilon, double* res){   // FIXME: something wrong with accuracy
    if (epsilon == NULL || res == NULL){
        return NULL_PTR;
    }

    double partition = 0.5, prev_sum = 0.;
    double curr_sum = 1., edge = 1.;

    while (fabs(prev_sum - curr_sum) > *epsilon){
        prev_sum = curr_sum;
        curr_sum = 0.;
        for(double x = 0; fabs(x - edge) >= *epsilon; x += partition){
            curr_sum += exp(-x * x * 0.5) * partition;     // лучше всегда умножать на 0.5
        }
        partition /= 2.;
    }
    
    *res = curr_sum;      
    return DONE; 

}

enum error_state sum_c(double* epsilon, double* res){
    if (epsilon == NULL || res == NULL){
        return NULL_PTR;
    }

    double partition = 0.5, prev_sum = -1.;
    double curr_sum = 0., edge = 1.;

    while (fabs(prev_sum - curr_sum) > *epsilon){
        prev_sum = curr_sum;
        curr_sum = 0.;
        for(double x = 0; fabs(x - edge) > *epsilon; x += partition){
            curr_sum += log(1. / (1. - x)) * partition;
        }
        partition /= 2;
    }
    *res = curr_sum;      
    return DONE;  
}

enum error_state sum_d(double* epsilon, double* res){
    if (epsilon == NULL || res == NULL){
        return NULL_PTR;
    }

    double partition = 0.5, prev_sum = 1.;
    double curr_sum = 0., edge = 1.;

    while (fabs(prev_sum - curr_sum) > *epsilon){
        prev_sum = curr_sum;
        curr_sum = 0.;
        for(double x = partition; fabs(x - edge) > *epsilon; x += partition){
            curr_sum += pow(x,x) * partition;
        }
        partition /= 2;
    }
    *res = curr_sum;      
    return DONE;  
}