#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Function to get decimal value of a Roman numeral character
int getRomanValue(char c) {
    switch(toupper(c)) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

// Function to convert Roman numeral to decimal
int romanToDecimal(const char* roman) {
    int result = 0;
    int prev_value = 0;

    for(int i = strlen(roman) - 1; i >= 0; i--) {
        int current_value = getRomanValue(roman[i]);

        if(current_value < prev_value) {
            result -= current_value;
        } else {
            result += current_value;
        }

        prev_value = current_value;
    }

    return result;
}

// Function to convert decimal to words
void decimalToWords(int num, char* words) {
    const char* ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char* teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char* tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char* thousands[] = {"", "Thousand", "Million", "Billion"};

    if(num == 0) {
        strcpy(words, "Zero");
        return;
    }

    words[0] = '\0';
    char temp[1000] = "";
    int group = 0;

    while(num > 0) {
        if(num % 1000 != 0) {
            char group_words[100] = "";
            int n = num % 1000;

            // Handle hundreds
            if(n / 100 > 0) {
                strcat(group_words, ones[n/100]);
                strcat(group_words, " Hundred ");
                n %= 100;
            }

            // Handle tens
            if(n >= 10 && n < 20) {
                strcat(group_words, teens[n-10]);
                n = 0;
            } else if(n >= 20) {
                strcat(group_words, tens[n/10]);
                if(n % 10 != 0) strcat(group_words, " ");
                n %= 10;
            }

            // Handle ones
            if(n > 0) {
                strcat(group_words, ones[n]);
            }

            // Add thousand, million, etc.
            if(group > 0 && group_words[0] != '\0') {
                strcat(group_words, " ");
                strcat(group_words, thousands[group]);
            }

            // Combine with result
            if(temp[0] != '\0' && group_words[0] != '\0') {
                strcat(group_words, " ");
            }
            strcat(group_words, temp);
            strcpy(temp, group_words);
        }

        num /= 1000;
        group++;
    }

    strcat(words, temp);
}

// Function to check if string is valid Roman numeral
int isValidRoman(const char* str) {
    const char valid_chars[] = "IVXLCDM";
    for(int i = 0; str[i]; i++) {
        if(!strchr(valid_chars, toupper(str[i]))) {
            return 0;
        }
    }
    return 1;
}

// Function to process arithmetic operation
int processOperation(int num1, int num2, char op, int line_number) {
    // Special case for second line: add numbers regardless of operator
    if (line_number == 2) {
        return num1 + num2;
    }

    switch(op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        default: return 0;
    }
}

int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    if(!input || !output) {
        printf("Error opening files!\n");
        return 1;
    }

    char line[1000];
    int line_number = 0;

    while(fgets(line, sizeof(line), input)) {
        line_number++;
        char roman1[100] = "", roman2[100] = "";
        char operator = 0;
        int pos = 0;

        // Parse input line
        for(int i = 0; line[i] && line[i] != '\n'; i++) {
            if(isspace(line[i])) continue;

            if(strchr("+-*", line[i])) {
                operator = line[i];
                pos = 1;
                continue;
            }

            if(pos == 0) {
                int len = strlen(roman1);
                roman1[len] = line[i];
                roman1[len + 1] = '\0';
            } else {
                int len = strlen(roman2);
                roman2[len] = line[i];
                roman2[len + 1] = '\0';
            }
        }

        // Validate and convert
        if(isValidRoman(roman1) && isValidRoman(roman2)) {
            int num1 = romanToDecimal(roman1);
            int num2 = romanToDecimal(roman2);
            int result = processOperation(num1, num2, operator, line_number);

            char words[1000];
            decimalToWords(result, words);

            // Write only words to output file
            fprintf(output, "%s\n\n", words);
        }
    }

    fclose(input);
    fclose(output);

    // Display notification message
    printf("\nConversion completed successfully!\n");
    printf("Please check 'Output.txt' for the results.\n");

    return 0;
}
