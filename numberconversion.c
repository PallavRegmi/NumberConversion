/*Pallav Regmi*/
/*CS241-Lab05*/
/*Number Conversion*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

void print_binary(unsigned long number, int size) {
    int i;
    for (i = size - 1; i >= 0; i--) {
        printf("%lu", (number >> i) & 1);
        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

void print_decimal(unsigned long number, int size) {
    char buffer[32];
    int length, i, count, field_width;

    length = snprintf(buffer, sizeof(buffer), "%lu", number);
    field_width = (size == 8) ? 3 : 6;

    for (i = 0; i < field_width - length; i++) {
        printf(" ");
    }

    count = 1;
    for (i = 0; i < length; i++) {
        printf("%c", buffer[i]);
        if (count == 3 && i != length - 1) {
            printf(",");
            count = 0;
        }
        count++;
    }
    printf("\n");
}

void print_hexadecimal(unsigned long number, int size) {
    int i, num_bytes;
    num_bytes = size / 8;
    for (i = num_bytes - 1; i >= 0; i--) {
        printf("%02X", (unsigned int)((number >> (i * 8)) & 0xFF));
        if (i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char command[100];
    char output_file[] = "testconvert.out";
        bool input_is_binary, output_is_binary, output_is_decimal, output_is_hexadecimal;
    int size;
    char *endptr;
    unsigned long number, max_value;

    fp = fopen("testconvert.sh", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    while (fgets(command, sizeof(command), fp) != NULL) {
        command[strcspn(command, "\n")] = 0;
        char *args[5];
        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL && i < 5) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        if (i != 5) {
            fprintf(stderr, "Error: Invalid number of arguments\n");
            printf("usage:\nconvert IN OUT SIZE NUMBER\n");
            continue;
        }

        input_is_binary = strcmp(args[1], "-inB") == 0;

        output_is_binary = strcmp(args[2], "-outB") == 0;
        output_is_decimal = strcmp(args[2], "-outD") == 0;
        output_is_hexadecimal = strcmp(args[2], "-outH") == 0;

        size = atoi(args[3] + 1);

        number = strtoul(args[4], &endptr, input_is_binary ? 2 : 10);

        max_value = (size == 8) ? UCHAR_MAX : (size == 16) ? USHRT_MAX : (size == 32) ? UINT_MAX : ULONG_MAX;
        number &= max_value;

        if (output_is_binary) {
            print_binary(number, size);
        } else if (output_is_decimal) {
            print_decimal(number, size);
        } else if (output_is_hexadecimal) {
            print_hexadecimal(number, size);
        }

        /*Append output to output file*/
        freopen(output_file, "a", stdout);
    }

    fclose(fp);

    return 0;
}

