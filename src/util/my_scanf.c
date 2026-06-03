
#include <stdio.h>
#include <stdarg.h>


static inline int minmea_isfield(char c) {
    return c != '|';
}

/*
 * parse @sentence as described in @format
 * ok	return 1;
 * fail return 0;
 * @sentence:	sentence to parse
 * @format:	the format describe here
 * ...
 */
int minmea_scan(const char *sentence, const char *format, ...)
{
    int result = 0;
    int optional = 0;

    if (sentence == NULL)
        return 0;

    va_list ap;
    va_start(ap, format);

    const char *field = sentence;
#define next_field() \
    do { \
        /* Progress to the next field. */ \
        while (minmea_isfield(*sentence)) \
            sentence++; \
        /* Make sure there is a field there. */ \
        if (*sentence == '|') { \
            sentence++; \
            field = sentence; \
        } else { \
            field = NULL; \
        } \
    } while (0)


    while (*format) {
        char type = *format++;

        if (type == ';') {
            // All further fields are optional.
            optional = 1;
            continue;
        }

        if (!field && !optional) {
            // Field requested but we ran out if input. Bail out.
            goto parse_error;
        }

        switch (type) {
            case 's': { // String value (char *).
                char *buf = va_arg(ap, char *);

                if (field) {
                    while (minmea_isfield(*field))
                        *buf++ = *field++;
                }

                *buf = '\0';
            } break;

            default: { // Unknown.
                goto parse_error;
            }
        }

        next_field();
    }

    result = 1;

parse_error:
    va_end(ap);
    return result;
}

