#include "s21_string.h"

int exp_num_error(const char *mass, const char **c) {
    int k = 0;
    if (mass[0] == '-' || mass[0] == '+') {
        k += 1;
    }
    while (mass[k] != '\0') {
        if (*c[0] == '.' && mass[k] == '.') {
            k = -1;
            break;
        }
        if ((*c[0] == '+' || *c[0] == '-') && (mass[k] == '+' || mass[k] == '-')) {
            c[0] -= 1;
            c[0] -= 1;
            k = -1;
            break;
        }
        if ((*c[0] == 'e' || *c[0] == 'E') && (mass[k] == 'e' || mass[k] == 'E')) {
            c[0] -= 1;
            k = -1;
            break;
        }
        k++;
    }
    return k;
}

void exp_num(char *mass, const char* format, ...) {
    for (int i = 0; i < (int)s21_strlen(mass); i++) {
        if ((mass[i] == 'e' || mass[i] == 'E') && (mass[i + 1] == '-' ||
                                                   mass[i + 1] == '+') && numspace(mass[i + 2], '0', '9')) {
            char sign = mass[++i];
            i += 1;
            for (int j = 0; j < (int)s21_strlen(mass); j++) {
                if (numspace(mass[i], '0', '9')) {
                    mass[j] = mass[i++];
                } else {
                    mass[j] = '\0';
                }
            }
            va_list args;
            va_start(args, format);
            if (s21_strcmp(format, "double") == 0) {
                double *val = va_arg(args, double*);
                if (sign == '+') {
                    *val *= pow(10, s21_atoi(mass));
                } else if (sign == '-') {
                    *val /= pow(10, s21_atoi(mass));
                }
            } else if (s21_strcmp(format, "long double") == 0) {
                long double *val = va_arg(args, long double*);
                if (sign == '+') {
                    *val *= pow(10, s21_atoi(mass));
                } else if (sign == '-') {
                    *val /= pow(10, s21_atoi(mass));
                }
            } else if (s21_strcmp(format, "float") == 0) {
                float *val = va_arg(args, float*);
                if (sign == '+') {
                    *val *= pow(10, s21_atoi(mass));
                } else if (sign == '-') {
                    *val /= pow(10, s21_atoi(mass));
                }
            }
            va_end(args);
            break;
        }
    }
}

char *str_to_mass(const char **str, char* mass, int *n, const char* format, size_t i, int *error, int width) {
    while (*str[0] == ' ' || *str[0] == format[i]) {
        str[0]++;
        (*n)++;
    }
    int j = 0;
    if (mass[0] == '-') {
        j += 1;
    }
    int flag = 0;
    if (width != 0) {
        flag = 1;
    }
    if ((*str[0] == '+' || *str[0] == '-') && numspace(*(str[0] + 1), '0', '9')) {
        if (flag == -1) {
        } else if (flag == 1 && width != 0) {
            if (*str[0] == '+') {
                width--;
                *(n) += 1;
                str[0] += 1;
            } else if (*str[0] == '-') {
                mass[j++] = *str[0]++;
                *(n) += 1;
                width--;
            }

        } else if (flag == 0) {
            mass[j++] = *str[0]++;
            *(n) += 1;
        }
    }
    if (mass[1] == '-') {
        mass[0] = '-';
        mass[1] = '0';
        j += 1;
    }
    // if (*str[0] == '.' && numspace(*(str[0] + 1), '0', '9')) {
    //     mass[0] = '0';
    //     mass[1] = *str[0]++;
    //     *(n) += 1;
    //     j = 2;
    //     if (width > 0) {
    //         width--;
    //     }
    // }
    if (flag == 1 && width == 0) {
        flag = -1;
    }
    if (numspace(*str[0], '0', '9')) {
        if ((format[i] == 'f' || format[i + 1] == 'f' || format[i + 1] == 'g' ||
             format[i + 1] == 'G' || format[i + 1] == 'e' || format[i + 1] == 'E')
            && (numspace(*str[0], '0', '9'))) {
            for ( ; *str[0] != ' '; j++) {
                if (*str[0] == '.' || *str[0] == 'E' || *str[0] == 'e' || *str[0] == '+'
                    || *str[0] == '-' || numspace(*str[0], '0', '9')) {
                    if ((*str[0] == '+' || *str[0] == '-') && (!(mass[j - 1] == 'e' || mass[j - 1] == 'E'))) {
                        break;
                    }
                    if ((*str[0] == 'e' || *str[0] == 'E') && (!numspace(mass[j - 1], '0', '9'))) {
                        break;
                    }
                    if (flag == -1)  {
                        mass[0] = '\0';
                        *error = -1;
                        break;
                    }
                    if (*str[0] == '.' && !numspace(*(str[0] + 1), '0', '9')) {
                        str[0]++;
                        (*n)++;
                        break;
                    }
                    if (exp_num_error(mass, &str[0]) == -1) {
                        break;
                    }
                    if (flag == 1 && width != 0) {
                        mass[j] = *str[0]++;
                        (*n)++;
                        width--;
                    } else if (flag == 0) {
                        mass[j] = *str[0]++;
                        (*n)++;
                    } else if (flag == 1 && width == 0) {
                        break;
                    }
                } else {
                    break;
                }
            }
        } else if (format[i] == 'd' || format[i + 1] == 'd' || format[i] == 'u' || format[i + 1] == 'u'
                   || format[i] == 'i' || format[i + 1] == 'i') {
            for ( ; *str[0] != ' ' && numspace(*str[0], '0', '9'); j++) {
                if (flag == -1)  {
                    mass[0] = '\0';
                    *error = -1;
                    break;
                }
                if (flag == 1 && width != 0) {
                    mass[j] = *str[0]++;
                    (*n)++;
                    width--;
                } else if (flag == 0) {
                    mass[j] = *str[0]++;
                    (*n)++;
                } else if (flag == 1 && width == 0) {
                    break;
                }
            }
        }
    } else {
        *error = -1;
    }
    return mass;
}

long double s21_atof(const char *str) {
    int i = 0;
    long double res = 0;
    int sign = 1;
    while (str[i] == '0' || str[i] == ' ') i++;
    if (str[i] == '-') {
        sign = -1;
        i++;
    }
    if (str[i] == '+') {
        i++;
    }
    while ('0' <= str[i] && str[i] <= '9') {
        res = res * 10 + ((int)str[i] - '0');
        i++;
    }
    short precision = 0;
    if (str[i] == '.') {
        i++;
        while ('0' <= str[i] && str[i] <= '9') {
            res = res * 10 + ((int)str[i] - '0');
            i++;
            precision++;
        }
    }
    // printf("precision = %d\n", precision);
    res /= pow(10, precision);
    return res * sign;
}

int oct_pars(char** mass, const char** str, int *n, const char *format, size_t i, int *error, int width) {
    int flag = 0;
    int j = 0;
    while (*str[0] == ' ' || *str[0] == format[i]) {
        str[0]++;
        (*n)++;
    }
    int wi_flag = 0;
    if (width != 0) {
        wi_flag = 1;
    }
    if ((*str[0] == '+' || *str[0] == '-') && numspace(*(str[0] + 1), '0', '9')) {
        if (wi_flag == 1 && width != 0) {
            if (*str[0] == '+') {
                str[0] += 1;
                (*n)++;
                width--;
            }
            if (*str[0] == '-') {
                mass[0][j++] = '-';
                str[0] += 1;
                *(n) += 1;
                width--;
            }
        } else if (wi_flag == 0) {
            mass[0][j++] = *str[0]++;
            *(n) += 1;
        }
    }
    if (wi_flag == 1 && width == 0) {
        wi_flag = -1;
    }
    mass[0][j++] = '0';
    if (numspace(*str[0], '0', '7')) {
        if ((*str[0] == '0' && numspace(*(str[0] + 1), '0', '7')) || ((format[i + 1] == 'o'))) {
            for ( ; numspace(*str[0], '0', '7'); j++) {
                if (wi_flag == -1)  {
                    mass[0][0] = '\0';
                    *error = -1;
                    break;
                }
                if (*str[0] == ' ') {
                    break;
                }
                if (wi_flag == 1 && width != 0) {
                    mass[0][j] = *str[0]++;
                    (*n)++;
                    width--;
                } else if (wi_flag == 0) {
                    mass[0][j] = *str[0]++;
                    (*n)++;
                }  else if (wi_flag == 1 && width == 0) {
                    break;
                }
            }
        } else {
            flag = -1;
        }
    } else {
        *error = -1;
    }
    return flag;
}

long oct_to_dec(char* mass) {
    short sign = 1;
    long int oct = 0;
    int j = 0;
    if (mass[j++] == '-') {
        sign *= -1;
    }
    for (int i = (int)s21_strlen(mass) - 2; i >= 0; i--) {
        oct += (mass[j++] - '0') * pow(8, i);
    } return oct * sign;
}

int hex_pars(char** mass, const char** str, int *n, const char *format, size_t i, int *error, int width) {
    int flag = 0;
    int j = 0;
    while (*str[0] == ' ' || *str[0] == format[i]) {
        str[0]++;
        (*n)++;
    }
    int wi_flag = 0;
    if (width != 0) {
        wi_flag = 1;
    }
    if (mass[0][0] == '-') {
        mass[0][1] = '-';
    }
    if ((*str[0] == '+' || *str[0] == '-') && numspace(*(str[0] + 1), '0', '9') && format[i + 1] != 'p') {
        if (wi_flag == 1 && width != 0) {
            if (*str[0] == '+') {
                str[0] += 1;
                (*n)++;
                width--;
            }
            if (*str[0] == '-') {
                mass[0][j++] = '-';
                str[0] += 1;
                *(n) += 1;
                width--;
            }
        } else if (wi_flag == 0) {
            mass[0][j++] = *str[0]++;
            *(n) += 1;
        }
    }
    if (wi_flag == 1 && width == 0) {
        wi_flag = -1;
    }
    mass[0][j++] = '0';
    if (numspace(*str[0], '0', '9') || numspace(*str[0], 'a', 'f') || numspace(*str[0], 'F', 'F')) {
        if ((*str[0] == '0' && (*(str[0] + 1) == 'x' || *(str[0] + 1) == 'X') &&
             ((numspace(*(str[0] + 2), '0', '9') || numspace(*(str[0] + 2), 'A', 'F') ||
               numspace(*(str[0] + 2), 'a', 'f')))) ||
            (format[i + 1] == 'x' || (format[i + 1] == 'X') || (format[i + 1] == 'p'))) {
            if (*(str[0] - 1) == '-') {
                mass[0][0] = '-';
                mass[0][1] = '0';
            }
            if (format[i + 1] == 'p') {
                if (numspace(*str[0], '0', '9')) {
                    mass[0][j++] = '0';
                }
            }
            if (format[i + 1] == 'x' || format[i + 1] == 'X' || format[i + 1] == 'i') {
                if (*(str[0] - 1) == '-' && mass[0][0] != '-') {
                    mass[0][j++] = *(str[0]++ - 1);
                    str[0]++;
                    (*n)++;
                } else {
                    str[0] += 2;
                    mass[0][j++] = '0';
                }
                if (format[i + 1] == 'x' || format[i + 1] == 'X') {
                    str[0] -= 2;
                }
            }
            for ( ; numspace(*str[0], '0', '9') || numspace(*str[0], 'A', 'F') ||
                    numspace(*str[0], 'a', 'f') || *str[0] == 'x' || *str[0] == 'X'; j++) {
                if (wi_flag == -1)  {
                    mass[0][0] = '\0';
                    *error = -1;
                    break;
                }
                if (*str[0] == ' ') {
                    break;
                }
                if (wi_flag == 1 && width != 0) {
                    mass[0][j] = *str[0]++;
                    (*n)++;
                    width--;
                } else if (wi_flag == 0) {
                    mass[0][j] = *str[0]++;
                    (*n)++;
                }  else if (wi_flag == 1 && width == 0) {
                    break;
                }
            }
        } else {
            flag = -1;
        }
    } else {
        *error = -1;
    }
    return flag;
}

long hex_to_dec(char* mass) {
    int sign = 1;
    long int hex = 0;
    int j = 0;
    if (mass[j++] == '-') {
        sign *= -1;
    }
    for (int i = (int)s21_strlen(mass) - 2; i >= 0; i--) {
        int tmp = 0;
        if (mass[j] <= '9') {
            tmp = mass[j++] - '0';
        } else if (mass[j] > '9') {
            switch (mass[j++]) {
                case 'a':
                case 'A': {
                    tmp = 10;
                    break;
                }
                case 'b':
                case 'B': {
                    tmp = 11;
                    break;
                }
                case 'c':
                case 'C': {
                    tmp = 12;
                    break;
                }
                case 'd':
                case 'D': {
                    tmp = 13;
                    break;
                }
                case 'e':
                case 'E': {
                    tmp = 14;
                    break;
                }
                case 'f':
                case 'F': {
                    tmp = 15;
                    break;
                }
            }
        }
        hex += tmp * pow(16, i);
    }
    return hex * sign;
}

int s21_sscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int count = 0;
    int n = 0;
    int error = 0;
    int width = 0;
    char tmp_width[3] = {'\0'};
    for (size_t i = 0; i < s21_strlen(format); i++) {
        if (error == -1) break;
        if (format[i] == '%' && format[i + 1] != '*') {
            if (format[i] == '%' && numspace(format[i + 1], '0', '9')) {
                for (int x = 0; numspace(format[i + 1], '0', '9'); x++) {
                    tmp_width[x] = format[i + 1];
                    i++;
                }
                width = s21_atoi(tmp_width);
                for (int y = 0; tmp_width[y] != '\0'; y++) {
                    tmp_width[y] = '\0';
                }
            }
            if (format[i + 1] == 'l' || format[i + 1] == 'L' || format[i + 1] == 'h') i += 1;
            if (format[i + 1] == 'c' || format[i + 1] == 'd' || format[i + 1] == 'i' || format[i + 1] == 'f'
            || format[i + 1] == 's' || format[i + 1] == 'u' || format[i + 1] == '%' || format[i + 1] == 'o'
            || format[i + 1] == 'x' || format[i + 1] == 'X' || format[i + 1] == 'p' || format[i + 1] == 'n'
            || format[i + 1] == 'g' || format[i + 1] == 'G' || format[i + 1] == 'e' || format[i + 1] == 'E') {
                switch (format[i + 1]) {
                    case 'c': {
                        char *p = va_arg(args, char*);
                        if (count != 0) {
                            while ((*str == ' ' || *str == format[i]) && (format[i - 1] != '%')) {
                                str++;
                                n++;
                            }
                        }
                        *p = *str++;
                        if (error != -1) count++;
                        n++;
                        break;
                    }
                    case 'd': {
                        char mass[20] = {0};
                        if (format[i] == 'h') {
                            short *p = va_arg(args, short*);
                            *p = s21_atoi(str_to_mass(&str, mass, &n, format, i, &error, width));
                        } else if (format[i] == 'l') {
                            long *p = va_arg(args, long*);
                            *p = s21_atoi(str_to_mass(&str, mass, &n, format, i, &error, width));
                        } else {
                            int *p = va_arg(args, int*);
                            *p = s21_atoi(str_to_mass(&str, mass, &n, format, i, &error, width));
                        }
                        if (error != -1) count++;
                        break;
                    }
                    case 'i': {
                        char* mass = (char*)calloc(20, sizeof(char));;
                        if (format[i] == 'h') {
                            short *p = va_arg(args, short*);
                            if (oct_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = oct_to_dec(mass);
                            } else if (hex_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = hex_to_dec(mass);
                            } else {
                                *p = s21_atoi(str_to_mass(&str, mass, &n, format, i, &error, width));
                            }
                        } else if (format[i] == 'l') {
                            long *p = va_arg(args, long*);
                            if (oct_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = oct_to_dec(mass);
                            } else if (hex_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = hex_to_dec(mass);
                            } else {
                                *p = s21_atoi(str_to_mass(&str, mass, &n, format, i, &error, width));
                            }
                        } else {
                            int *p = va_arg(args, int*);
                            if (oct_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = oct_to_dec(mass);
                            } else if (hex_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = hex_to_dec(mass);
                            } else {
                                *p = s21_atoi(str_to_mass(&str, mass, &n, format, i, &error, width));
                            }
                        }
                        free(mass);
                        if (error != -1) count++;
                        break;
                    }
                    case 'E':
                    case 'e':
                    case 'G':
                    case 'g':
                    case 'f': {
                        char mass[50] = {0};
                        if (format[i] == 'l') {
                            double *p = va_arg(args, double*);
                            double val = 0.0;
                            val = s21_atof(str_to_mass(&str, mass, &n, format, i, &error, width));
                            exp_num(mass, "double", &val);
                            *p = val;
                        } else if (format[i] == 'L') {
                            long double *p = va_arg(args, long double*);
                            long double val = 0.0;
                            val = s21_atof(str_to_mass(&str, mass, &n, format, i, &error, width));
                            exp_num(mass, "long double", &val);
                            *p = val;
                        } else {
                            float *p = va_arg(args, float*);
                            float val = 0.0;
                            val = s21_atof(str_to_mass(&str, mass, &n, format, i, &error, width));
                            exp_num(mass, "float", &val);
                            *p = val;
                        }
                        if (error != -1) count++;
                        break;
                    }
                    case 's': {
                        char *p = va_arg(args, char*);
                        int j = 0;
                        while ((*str == ' ' || *str == format[i]) && (format[i - 1] != '%')) {
                            str++;
                            n++;
                        }
                        for ( ; *str != ' '; j++) {
                            if (width > 0) {
                                while (width != 0) {
                                    if (*str == ' ') {
                                        break;
                                    }
                                    *(p + j++) = *str++;
                                    n++;
                                    width -= 1;
                                }
                                if (width == 0) break;
                            } else {
                                *(p + j) = *str++;
                                n++;
                            }
                        }
                        if (*str == ' ') {
                            *(p + j) = '\0';
                        }
                        if (error != -1) count++;
                        break;
                    }
                    case 'u': {
                        char mass[20] = {0};
                        if (format[i] == 'h') {
                            unsigned short *p = va_arg(args, unsigned short*);
                            *p = (unsigned short)s21_atoi
                                    (str_to_mass(&str, mass, &n, format, i, &error, width));
                        } else if (format[i] == 'l') {
                            unsigned long *p = va_arg(args, unsigned long*);
                            *p = (unsigned long)s21_atoi
                                    (str_to_mass(&str, mass, &n, format, i, &error, width));
                        } else {
                            unsigned int *p = va_arg(args, unsigned int*);
                            *p = (unsigned int)s21_atoi
                                    (str_to_mass(&str, mass, &n, format, i, &error, width));
                        }
                        if (error != -1) count++;
                        break;
                    }
                    case '%': {
                        break;
                    }
                    case 'o': {
                        char* mass = (char*)calloc(20, sizeof(char));
                        if (format[i] == 'h') {
                            short *p = va_arg(args, short*);
                            if (oct_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = oct_to_dec(mass);
                            }
                        } else if (format[i] == 'l') {
                            long *p = va_arg(args, long*);
                            if (oct_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = oct_to_dec(mass);
                            }
                        } else {
                            int *p = va_arg(args, int*);
                            if (oct_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = oct_to_dec(mass);
                            }
                        }
                        free(mass);
                        if (error != -1) count++;
                        break;
                    }
                    case 'p':
                    case 'x':
                    case 'X': {
                        char* mass = (char*)calloc(20, sizeof(char));
                        if (format[i] == 'h') {
                            short *p = va_arg(args, short*);
                            if (hex_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = hex_to_dec(mass);
                            }
                        } else if (format[i] == 'l') {
                            long *p = va_arg(args, long*);
                            if (hex_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = hex_to_dec(mass);
                            }
                        } else {
                            int *p = va_arg(args, int*);
                            if (hex_pars(&mass, &str, &n, format, i, &error, width) == 0) {
                                *p = hex_to_dec(mass);
                            }
                        }
                        free(mass);
                        if (error != -1) count++;
                        break;
                    }
                    case 'n': {
                        int *p = va_arg(args, int*);
                        *p = n;
                    }
                    default:
                        break;
                }
            }
        } else if (format[i] == *str) {
            n++;
            str += 1;
        }
        width = 0;
    }
    va_end(args);
    return count;
}
