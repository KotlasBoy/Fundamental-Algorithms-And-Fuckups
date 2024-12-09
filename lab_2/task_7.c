#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846 
#endif

#define TRUE 1
#define FALSE 0
typedef enum error_state{
    COOL,
    FUCKUP,
    NULL_PTR,
    WRONG_PARAMETER,
    OVERFLOW,
    MEMORY_FUCKUP,
    FILE_FUCKUP
} error_state;

double (*func_ptr)(double);
double first_func (double);         // -64.125    5.6313
double second_func (double);        // stupid function, no roots    
double fird_func (double);          // - 0.26    1.26
double fourth_func (double);        //подаю сегмент с 2-мя корнями
double fifth_func (double);

error_state dichotomy_method (double left_bound, double right_bound, double const epsilon, double(*func_ptr)(double), double* result);
int errno_parcer(error_state my_errno);

int main() {
    double result = 0;

    error_state personal_errno = dichotomy_method(-4, 4, 0.000001, first_func, &result);
    printf("1) ");
    if(errno_parcer(personal_errno))
        printf("Root is %lf\n", result);

    personal_errno = dichotomy_method(-30, 20, 0.000001, second_func, &result);
    printf("2) ");
    if(errno_parcer(personal_errno))
        printf("Root is %lf\n", result);

    personal_errno = dichotomy_method(1, 2, 0.000001, fird_func, &result);
    printf("3) ");
    if(errno_parcer(personal_errno))
        printf("Root is %lf\n", result);

    personal_errno = dichotomy_method(-15, 15, 0.000001, fourth_func, &result);
    printf("4) ");
    if(errno_parcer(personal_errno))
        printf("Root is %lf\n", result);

    personal_errno = dichotomy_method(1, 3, 0.000001, fird_func, &result);
    printf("5) ");
    if(errno_parcer(personal_errno))
        printf("Root is %lf\n", result);


}

error_state dichotomy_method (double left_bound, double right_bound, double const epsilon, double(*func_ptr)(double), double* result){    
    double middle_point = 0.;

    if(epsilon <= 0 || epsilon >= 1)
        return WRONG_PARAMETER;
    
    if(func_ptr(left_bound) * func_ptr(right_bound) > 0)
        return FUCKUP; 

    do {
        middle_point = (left_bound + right_bound) / 2.;

        if (func_ptr(middle_point) * func_ptr(left_bound) < 0){
            right_bound = middle_point;
        }
        else{
            left_bound = middle_point;
        }
    
    } while(fabs(func_ptr(middle_point)) >= epsilon);

    *result = middle_point;
    return COOL;
}

int errno_parcer(error_state my_errno){
    switch(my_errno){
        case WRONG_PARAMETER:
            printf("Wrong epsilon was found\n");
            return FALSE;
        case FUCKUP:
            printf("NO root can exist on this segment\n");
            return FALSE;
        case COOL:
            //printf("Root is %lf\n", result);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

double first_func (double x ){
    return (x - 1) * (x - 1);
}
double second_func (double x){
    return(sin(x) + 14.2);
}
double fird_func (double x){
    return ( -x * x + x + 1./3.);
}
double fourth_func (double x){
    return (x * x - 42);
}
double fifth_func (double x){
    return (cos(M_PI / x) + 0.75);
}

