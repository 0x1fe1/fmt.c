#ifndef FMT_HEADER
#define FMT_HEADER

#include <stdio.h>
#include <stdarg.h>

extern int fmt(const char* format, ...);
extern int ffmt(FILE* output, const char* format, ...);

#ifdef FMT_IMPL

#define fmt(format, ...) ffmt(stdout, format, __VA_ARGS__)

// int fmt(const char* format, ...) {
//     int result;
//     va_list args;
//
//     va_start(args, format);
//     result = ffmt(stdout, format, args);
//     va_end(args);
//
//     return result;
// }

int ffmt(FILE* output, const char* str, ...) {
    int result = 0;
    va_list ptr;
    va_start(ptr, str);

    char token[1024];
    // index of where to store the characters of str in token
    int k = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        token[k++] = str[i];

        if (str[i+1] == '%' || str[i+1] == '\0') {
            token[k] = '\0';
            k = 0;
            if (token[0] != '%') {
                fprintf(output, "%s", token);
            } else {
                int j = 1;
                char ch1 = 0;
                char ch2 = 0;

                // this loop is required when printing formatted value like 0.2f, when ch1='f' loop ends
                while ((ch1 = token[j++]) <= '9') { }

                switch (ch1) {
                case 'i':
                case 'd':
                case 'u':
                case 'h':
                    fprintf(output, token, va_arg(ptr, int));
                    break;

                case 'c':
                    fprintf(output, token, va_arg(ptr, int));
                    break;

                case 'f':
                    fprintf(output, token, va_arg(ptr, double));
                    break;

                case 'l':
                    ch2 = token[2];
                    if (ch2 == 'u' || ch2 == 'd' || ch2 == 'i') {
                        fprintf(output, token, va_arg(ptr, long));
                    } else if (ch2 == 'f') {
                        fprintf(output, token, va_arg(ptr, double));
                    }
                    break;

                case 'L':
                    ch2 = token[2];
                    if (ch2 == 'u' || ch2 == 'd' || ch2 == 'i') {
                        fprintf(output, token, va_arg(ptr, long long));
                    } else if (ch2 == 'f') {
                        fprintf(output, token, va_arg(ptr, long double));
                    }
                    break;

                case 'b':
                    ch2 = token[2];

                    int bitwidth = 0;
                    int binary_value_int = 0;
                    long binary_value_long = 0;
                    long long binary_value_longlong = 0;

                    if (ch2 == 'i' || ch2 == '\n') {
                        bitwidth = 32;
                        int binary_value_int = va_arg(ptr, int);
                    } else {
                        break;
                    }


                    for (int bi = bitwidth-1; bi >= 0; bi--) {
                        int bit = (binary_value_int >> bi) & 1;
                        fprintf(output, "%d", bit);
                        if (bi % 4 == 0) fprintf(output, " ");
                    }

                    if (token[2] == '\n' || token[3] == '\n') fprintf(output, "\n");
                    break;

                case 's':
                    fprintf(output, token, va_arg(ptr, char*));
                    break;

                default:
                    fprintf(stderr, "ERROR: Unrecognized format: %s\n", token);
                    return 1;
                    break;
                }
            }
        }
    }

    va_end(ptr);
    return result;
}

#endif // FMT_IMPL
#endif // FMT_HEADER

/*

from https://alvinalexander.com/programming/printf-format-cheat-sheet

%c  character
%d  decimal (integer) number (base 10)
%e  exponential floating-point number
%f  floating-point number
%i  integer (base 10)
%o  octal number (base 8)
%s  a string of characters
%u  unsigned decimal (integer) number
%x  number in hexadecimal (base 16)
%%  print a percent sign
\%  print a percent sign

*/
