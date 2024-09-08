/*
USAGE:
    int result = fmt("format string", ...); // Print to STDOUT
    int result = ffmt(FILE* stream, "format string", ...); // Print to `stream`
    int result = vffmt(FILE* stream, "format string", va_list); // Print to `stream`, uses va_list

SUPPORTED FORMATS:
    %i  %d  %u  %h - integers.
    %l? %L?        - long integer (? is either one of [idu]).
    %f  %lf %Lf    - float, double and long double.
    %c  %s         - char and string.
    %b  %o  %x  %X - binary, octal and hex. These also support # for alternative notation.
                       (e.g. %#b -> 0b1000101)

EXAMPLE:
  main.c:
    #define FMT_IMPL // ONLY IN _ONE_ FILE!
    #include "fmt.h"

    int main() {
        fmt("%s\n", "Hello, Sailor!");

        int b = 420;
        fmt("%i =\n= %x = %X = %#x = %#X =\n= %o = %#o =\n= %b = %#b\n", b,b,b,b,b,b,b,b,b);
        return 0;
    }

  compile and run:
  $ gcc main.c -o out && ./out
*/


#ifndef FMT_HEADER
#define FMT_HEADER

#include <stdio.h>
#include <stdarg.h>

extern int fmt(const char* format, ...);
extern int ffmt(FILE* output, const char* format, ...);
extern int vffmt(FILE* output, const char* format, va_list args);

#ifdef FMT_IMPL

int fmt(const char* format, ...) {
    int result;
    va_list args;

    va_start(args, format);
    result = vffmt(stdout, format, args);
    va_end(args);

    return result;
}

int ffmt(FILE* output, const char* format, ...) {
    int result;
    va_list args;

    va_start(args, format);
    result = vffmt(output, format, args);
    va_end(args);

    return result;
}

int vffmt(FILE* stream, const char* format, va_list args) {
    int result = 0;
    char token[1024];
    // index of where to store the characters of format in token
    int k = 0;

    for (int i = 0; format[i] != '\0'; i++) {
        token[k++] = format[i];

        if (format[i+1] == '%' || format[i+1] == '\0') {
            token[k] = '\0';
            k = 0;
            if (token[0] != '%') {
                fprintf(stream, "%s", token);
                continue;
            }
            int j = 1;
            int val_32 = 0;
            char ch1 = 0;
            char ch2 = 0;

            // this loop is required when printing formatted value like 0.2f, when ch1='f' loop ends
            while ((ch1 = token[j++]) <= '9') { }
            // printf("\nDEBUG: |%s|\n", token);

            switch (ch1) {
            case 'i': case 'd': case 'u': case 'h':
                fprintf(stream, token, va_arg(args, int));
                break;

            case 'c':
                fprintf(stream, token, va_arg(args, int));
                break;

            case 'f':
                fprintf(stream, token, va_arg(args, double));
                break;

            case 'l':
                switch (token[2]) {
                case 'u': case 'd': case 'i':
                    fprintf(stream, token, va_arg(args, long));
                    break;
                case 'f':
                    fprintf(stream, token, va_arg(args, double));
                    break;
                }
                break;

            case 'L':
                switch (token[2]) {
                case 'u': case 'd': case 'i':
                    fprintf(stream, token, va_arg(args, long long));
                    break;
                case 'f':
                    fprintf(stream, token, va_arg(args, long double));
                    break;
                }
                break;

            case 'b':
                int bitwidth = 32;
                val_32 = va_arg(args, int);

                char flag_start = 0;
                for (int nibble_index = 0; nibble_index < bitwidth/4; nibble_index++) {
                    char nibble = (val_32 >> (bitwidth - nibble_index*4 - 4)) & 0xF;
                    if (flag_start == 0 && nibble == 0) { continue; } else { flag_start = 1; }
                    fprintf(stream, "%i", (nibble >> 3) & 1);
                    fprintf(stream, "%i", (nibble >> 2) & 1);
                    fprintf(stream, "%i", (nibble >> 1) & 1);
                    fprintf(stream, "%i", (nibble >> 0) & 1);
                    if (token[1] == '#' && nibble_index < bitwidth/4 - 1) fprintf(stream, "_");
                }

                fprintf(stream, "%s", token + j);
                break;

            case 'o':
                val_32 = va_arg(args, int);
                if (token[1] == '#') fprintf(stream, "%#o", val_32);
                else fprintf(stream, "%o", val_32);

                fprintf(stream, "%s", token + j);
                break;

            case 'x':
                val_32 = va_arg(args, int);
                if (token[1] == '#') fprintf(stream, "%#x", val_32);
                else fprintf(stream, "%x", val_32);

                fprintf(stream, "%s", token + j);
                break;
            case 'X':
                val_32 = va_arg(args, int);
                if (token[1] == '#') fprintf(stream, "%#X", val_32);
                else fprintf(stream, "%X", val_32);

                fprintf(stream, "%s", token + j);
                break;

            case 's':
                fprintf(stream, token, va_arg(args, char*));
                break;

            default:
                // unable to fallback to default printf because of type ambiguity
                fprintf(stderr, "ERROR: Unrecognized format: %s\n", token);
                fprintf(stream, "%s", token);
                break;
            }
        }
    }

    return result;
}

#endif // FMT_IMPL
#endif // FMT_HEADER

/* DO NOT LOOK HERE!

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
