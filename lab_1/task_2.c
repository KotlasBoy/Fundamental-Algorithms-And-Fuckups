#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

typedef enum {
    COOL,
    FUCKUP,
    NULL_PTR,
    WRONG_PARAMETER,
    OVERFLOW,
    MEMORY_FUCKUP,
    FILE_FUCKUP
} error_state;

typedef enum my_bool{
    FALSE,
    TRUE
} my_bool;




void fast_pow (double number, long int power, double* result);
error_state string_to_double(char* word, double* res);
my_bool convert_check( error_state condition);
my_bool is_prime (long int number);

error_state e_lim (const double* epsilon, double* res);
error_state e_row (const double* epsilon,  double* res);
error_state e_ln (const double* epsilon,  double* res);

error_state pi_lim (const double* epsilon, double* res);
error_state pi_row (const double* epsilon,  double* res);
error_state pi_ln (const double* epsilon,  double* res);

error_state ln_2_lim (const double * epsilon, double* res);
error_state ln_2_row (const double* epsilon,  double* res);
error_state ln_2_ln (const double* epsilon,  double* res);

error_state sqrt_2_lim (const double * epsilon, double* res);
error_state sqrt_2_row (const double* epsilon,  double* res);
error_state sqrt_2_ln (const double* epsilon,  double* res);

error_state euler_mascheroni_lim (const double * epsilon, double* res);
error_state euler_mascheroni_row (const double* epsilon,  double* res);
error_state euler_mascheroni_ln  (const double* epsilon,  double* res);


int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Wrong amount of parameters\n");
        return WRONG_PARAMETER;
    }

    double epsilon = 0.0, current_number = 0.0;
    if(convert_check(string_to_double(argv[1], &epsilon)) != TRUE){
        return 0;
    }

    if (epsilon <= 0. || epsilon >= 1.){
        printf("Wrong parameters\n");
        return WRONG_PARAMETER;
    }

                                                        //find e
    if (e_lim(&epsilon, &current_number) != COOL){
        printf("FUCKUP in e_lim\n");
        return FUCKUP;
    }
    printf("e_lim = %.10lf\n", current_number);
    current_number = 0.0;

    if (e_row(&epsilon, &current_number) != COOL){
        printf("FUCKUP in e_row\n");
        return FUCKUP;
    }
    printf("e_row = %.10lf\n", current_number);
    current_number = 0.0;

    if (e_ln(&epsilon, &current_number) != COOL){
        printf("FUCKUP in e_ln\n");
        return FUCKUP;
    }
    printf("e_ln = %.10lf\n", current_number);
    current_number = 0.0;

                                                        //find pi
    if (pi_lim(&epsilon, &current_number) != COOL){
        printf("FUCKUP in pi_lim\n");
        return FUCKUP;
    }
    printf("pi_lim = %.10lf\n", current_number);
    current_number = 0.0;

    if (pi_row(&epsilon, &current_number) != COOL){
        printf("FUCKUP in pi_row\n");
        return FUCKUP;
    }
    printf("pi_row = %.10lf\n", current_number);
    current_number = 0.0;

    if (pi_ln(&epsilon, &current_number) != COOL){
        printf("FUCKUP in pi_ln\n");
        return FUCKUP;
    }
    printf("pi_ln = %.10lf\n", current_number);
    current_number = 0.0;

                                                        //find ln 2
    if (ln_2_lim(&epsilon, &current_number) != COOL){
        printf("FUCKUP in ln_2_lim\n");
        return FUCKUP;
    }
    printf("ln_2_lim = %.10lf\n", current_number);
    current_number = 0.0;

    if (ln_2_row(&epsilon, &current_number) != COOL){
        printf("FUCKUP in ln_2_row\n");
        return FUCKUP;
    }
    printf("ln_2_row = %.10lf\n", current_number);
    current_number = 0.0;

    if (ln_2_ln(&epsilon, &current_number) != COOL){
        printf("FUCKUP in ln_2_ln\n");
        return FUCKUP;
    }
    printf("ln_2_ln  = %.10lf\n", current_number);
    current_number = 0.0;
    
                                                        //find sqrt 2
   if (sqrt_2_lim(&epsilon, &current_number) != COOL){
        printf("FUCKUP in sqrt_2_lim\n");
        return FUCKUP;
    }
    printf("sqrt_2_lim = %.10lf\n", current_number);
    current_number = 0.0;

    if (sqrt_2_row(&epsilon, &current_number) != COOL){
        printf("FUCKUP in sqrt_2_row\n");
        return FUCKUP;
    }
    printf("sqrt_2_row = %.10lf\n", current_number);
    current_number = 0.0;

    if (sqrt_2_ln(&epsilon, &current_number) != COOL){
        printf("FUCKUP in sqrt_2_ln\n");
        return FUCKUP;
    }
    printf("sqrt_2_ln  = %.10lf\n", current_number);
    current_number = 0.0;
    
                                                        // find gamma
   if (euler_mascheroni_lim(&epsilon, &current_number) != COOL){
        printf("FUCKUP in euler_mascheroni_lim\n");
        return FUCKUP;
    }
    printf("euler_mascheroni_lim = %.10lf\n", current_number);
    current_number = 0.0;

    if (euler_mascheroni_row(&epsilon, &current_number) != COOL){
        printf("FUCKUP in euler_mascheroni_row\n");
        return FUCKUP;
    }
    printf("euler_mascheroni_row = %.10lf\n", current_number);
    current_number = 0.0;

    if (euler_mascheroni_ln(&epsilon, &current_number) != COOL){
        printf("FUCKUP in euler_mascheroni_ln\n");
        return FUCKUP;
    }
    printf("euler_mascheroni_ln  = %.10lf\n", current_number);
    current_number = 0.0;
}

error_state e_lim (const double * epsilon, double* res){
    if (!epsilon || !res)
        return NULL_PTR;
    double current_num = 0, previous_num = 0;
    int n = 1;
    do {
        previous_num = current_num;
        fast_pow(1 + (double)1 / n, n, &current_num);
        ++n;
    }
    while (fabs(current_num - previous_num) >= *epsilon);

    *res = current_num;
    return COOL; 
}

error_state e_row (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;
    double current_sum = 1.;
    int n = 0;
    while (fabs(current_sum) >= *epsilon){
        *res += current_sum;
        current_sum /= (double)(n + 1);
        ++n;
    }
    return COOL;
}
error_state e_ln (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;

    double x = 0.0, better_x = 3. , derivative = 0. , phi = 0.;

    while (fabs(x - better_x ) >= *epsilon){
        x = better_x;
        phi = log(x) - 1;
        derivative = 1 / x;
        better_x = x - (phi) / derivative;
    }
    *res = better_x;
    return COOL;
}


error_state pi_lim (const double * epsilon, double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    double current_num = 4, previous_num = 0;
    double tmp_pow = 0.;
    int n = 2;
    do {
        previous_num = current_num;
        //tmp_pow = pow(2 * n + 1, 2);
        fast_pow((2.0 * n - 1.0), 2, &tmp_pow);
        current_num *= (4.0 * (n - 1.0) * n) / tmp_pow;
        ++n;
    }
    while (fabs(current_num - previous_num) >= *epsilon);

    *res = current_num;
    return COOL; 
}

error_state pi_row (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;  
    double current_sum = 1.0;
    int n = 1, sign = 1;
    while (fabs(4 * current_sum) >= *epsilon){
        current_sum = sign / (2. * n - 1.);
        sign = -sign;
        *res += current_sum;
        ++n;
    }
    *res *= 4;
    return COOL;
}

error_state pi_ln (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;

    double x = 0.0, better_x = 5. , derivative = 0. , phi = 0.;

    while (fabs(x - better_x ) >= *epsilon){
        x = better_x;
        phi = cos(x) + 1;
        derivative = -sin(x);
        better_x = x - (phi) / derivative;
    }
    *res = better_x;
    return COOL;
}


error_state ln_2_lim (const double * epsilon, double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    double current_num = 1, previous_num = 0;
    double tmp_pow = 0.;
    int n = 2;
    do {
        previous_num = current_num;
        tmp_pow = pow(2.0, 1.0 / n);
        current_num = n * (tmp_pow - 1.);
        ++n;
    }
    while (fabs(current_num - previous_num) >= *epsilon);

    *res = current_num;
    return COOL; 
}

error_state ln_2_row (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;  
    double current_sum = 1.0;
    int n = 1, sign = 1;
    while (fabs(current_sum) >= *epsilon){
        current_sum = sign / (double) n;
        sign = -sign;
        *res += current_sum;
        ++n;
    }
    return COOL;
}

error_state ln_2_ln (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;

    double x = 0.0, better_x = 5. , derivative = 0. , phi = 0.;

    while (fabs(x - better_x ) >= *epsilon){
        x = better_x;
        phi = exp(x) - 2.;
        derivative = exp(x);
        better_x = x - (phi) / derivative;
    }
    *res = better_x;
    return COOL;
}


error_state sqrt_2_lim (const double * epsilon, double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    double current_num = -0.5, previous_num = 0;
    do {
        previous_num = current_num;
        current_num = previous_num - pow(previous_num, 2) / 2.0 + 1;
    }
    while (fabs(current_num - previous_num) >= *epsilon);

    *res = current_num;
    return COOL; 
}

error_state sqrt_2_row (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = pow(2, 0.25);  
    double prev_mult = 5.0, curr_part = *res;
    while (fabs(prev_mult - *res) >= *epsilon){
        prev_mult = *res;
        curr_part = pow(curr_part, 0.5);
        *res *= curr_part;
    }
    return COOL;
}

error_state sqrt_2_ln (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;

    double x = 0.0, better_x = 5. , derivative = 0. , phi = 0.;

    while (fabs(x - better_x ) >= *epsilon){
        x = better_x;
        phi = pow(x, 2) - 2.;
        derivative = 2. * x;
        better_x = x - (phi) / derivative;
    }
    *res = better_x;
    return COOL;
}


error_state euler_mascheroni_lim (const double * epsilon, double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 1.;
    double previous_num = 0., harmonic_row_sum = 1.;
    int n = 2;

    do {
        previous_num = *res;
        harmonic_row_sum += 1. / n;
        *res = harmonic_row_sum - log(n);
        ++n;
    }
    while (fabs(*res - previous_num) >= *epsilon);

    return COOL; 
}

error_state euler_mascheroni_row (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.5;
    double prev_sum = 0., tmp_sqrt = 0.;
    int k = 2;

    while (fabs(prev_sum - *res) >= *epsilon){
        ++k;
        prev_sum = *res;
        tmp_sqrt = sqrt(k);
        if ((int)tmp_sqrt * (int)tmp_sqrt == k){
            ++k;
            tmp_sqrt = (int)pow(k, 0.5);
        }
        *res += 1.0 / pow((int)tmp_sqrt, 2) - 1.0 / k;
    }
    *res -= pow(M_PI, 2) / 6;
    return COOL;
}

error_state euler_mascheroni_ln (const double* epsilon,  double* res){
    if (!epsilon || !res)
        return NULL_PTR;

    *res = 0.;

    double curr_mult = 0.5;
    double prev_lim = 0., curr_lim = log(2) * curr_mult;
    int p = 3;

    while (fabs(curr_lim - prev_lim) >= *epsilon){
        prev_lim = curr_lim;
    
        while(!is_prime(p)){
            ++p;
        }

        curr_mult *= (double) (p - 1.0) / p;
        curr_lim = log(p) * curr_mult;
        ++p;
    }
    *res = -(log(curr_lim));
    return COOL;
}

void fast_pow (double number, long int power, double* result){
    char sign = power > 0 ? 1 : 0;
    *result = 1.0;
    power = labs(power);
    while (power > 0){
        if(power % 2){
            *result = sign == 1 ? *result * number : *result / number;
        }
        number *= number;
        power /= 2;
    }
}

error_state string_to_double (char* word, double* res){
    if(word == NULL || res == NULL){                        
        return NULL_PTR;                                    
    }

    char* endptr = NULL;
    *res = strtod(word, &endptr);

    if (*res == HUGE_VAL || *res == -HUGE_VAL){
        return OVERFLOW;
    }

    if((*res == 0 && endptr == word) || *endptr != '\0'){
        return WRONG_PARAMETER;
    }
    
    return COOL;
}

my_bool convert_check (error_state condition){
    switch(condition){
        case NULL_PTR:
            printf("NULL-ptr found\n");
            return FALSE;
        case OVERFLOW:
            printf("Number overflow\n");
            return FALSE;
        case WRONG_PARAMETER:
            printf("Wrong parameter passed\n");
            return FALSE;    
        default:
            break;
    }
    return TRUE;
}

my_bool is_prime (long int number){
    if (number == 2)
        return TRUE;

    if (number == 0 || number == 1 || number % 2 == 0)
        return FALSE;
    

    for (int i = 3; i * i < number + 1; i += 2){
        if (number % i == 0)
            return FALSE;
    }
    
    return TRUE;
}

