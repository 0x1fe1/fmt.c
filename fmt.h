#ifndef FMT_HEADER
#define FMT_HEADER

#include <stdio.h>
#include <stdarg.h>
extern void fmt(const char* format, ...);

#ifdef FMT_IMPL

void fmt(const char* str, ...) {
    va_list ptr;
    va_start(ptr, str);

    char token[1000];
    // index of where to store the characters of str in token
    int k = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        token[k++] = str[i];

        if (str[i+1] == '%' || str[i+1] == '\0') {
            token[k] = '\0';
            k = 0;
            if (token[0] != '%') {
                fprintf(stdout, "%s", token);
            } else {
                int j = 1;
                char ch1 = 0;

                // this loop is required when printing formatted value like 0.2f, when ch1='f' loop ends
                while ((ch1 = token[j++]) < '9') { }

                switch (ch1) {
                    // for integers
                    case 'i':
                    case 'd':
                    case 'u':
                    case 'h':
                        fprintf(stdout, token, va_arg(ptr, int));
                        break;

                    // for characters
                    case 'c':
                        fprintf(stdout, token, va_arg(ptr, int));
                        break;

                    case 'f':
                        // for float values
                        fprintf(stdout, token, va_arg(ptr, double));
                        break;

                    case 'l':
                        char ch2 = token[2];

                        // for long int
                        if (ch2 == 'u' || ch2 == 'd' || ch2 == 'i') {
                            fprintf(stdout, token, va_arg(ptr, long));
                        }

                        // for double
                        else if (ch2 == 'f') {
                            fprintf(stdout, token, va_arg(ptr, double));
                        }
                        break;

                    case 'L':
                        char ch2 = token[2];

                        // for long long int
                        if (ch2 == 'u' || ch2 == 'd' || ch2 == 'i') {
                            fprintf(stdout, token, va_arg(ptr, long long));
                        }

                        // for long double
                        else if (ch2 == 'f') {
                            fprintf(stdout, token, va_arg(ptr, long double));
                        }
                        break;

                    // for strings
                    case 's':
                        fprintf(stdout, token, va_arg(ptr, char*));
                        break;

                    // print the whole token
                    // if no case is matched
                    default:
                        fprintf(stderr, "Unrecognized token: [%s]\n", token);
                        fprintf(stdout, "%s", token);
                        break;
                }
            }
        }
    }

    va_end(ptr);
}

#endif // FMT_IMPL
#endif // FMT_HEADER
