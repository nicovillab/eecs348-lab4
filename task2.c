/*
 * EECS 348 Lab 8 - Task 2: Temperature Conversion
 *
 * Converts a temperature between Celsius, Fahrenheit, and Kelvin,
 * then categorizes it and prints a weather advisory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define LINE_SIZE 256

/* Lowest physically possible temperature in each scale */
#define ABS_ZERO_C  -273.15
#define ABS_ZERO_F  -459.67
#define ABS_ZERO_K     0.0

/* Small tolerance so values like 283.15 K -> 9.9999999 C still count as 10 C */
#define EPSILON 1e-9

/*
 * Reads one line of input into buffer.
 * Returns 1 on success, 0 if the line was too long, -1 on EOF.
 */
int read_line(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) == NULL) {
        return -1;
    }

    /* Line too long: discard the rest of it and report it as invalid */
    if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard */
        }
        return 0;
    }

    return 1;
}

/*
 * Keeps asking for a temperature value until a valid number is entered.
 * Returns 1 on success, 0 on EOF.
 */
int get_temperature(double *value)
{
    char line[LINE_SIZE];
    char *end;
    int status;

    while (1) {
        printf("Enter the temperature value: ");
        fflush(stdout);

        status = read_line(line, LINE_SIZE);
        if (status == -1) {
            return 0;
        }

        if (status == 1) {
            errno = 0;
            *value = strtod(line, &end);

            if (end != line && errno != ERANGE) {
                /* Only trailing whitespace is allowed after the number */
                while (isspace((unsigned char)*end)) {
                    end++;
                }
                if (*end == '\0') {
                    return 1;
                }
            }
        }

        printf("Invalid input. Please enter a numeric temperature.\n");
    }
}

/*
 * Keeps asking for a scale letter (C, F, or K) until a valid one is entered.
 * Lowercase letters are accepted. Returns 1 on success, 0 on EOF.
 */
int get_scale(const char *prompt, char *scale)
{
    char line[LINE_SIZE];
    char letter = '\0';
    int status;
    int i;
    int letters_found;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        status = read_line(line, LINE_SIZE);
        if (status == -1) {
            return 0;
        }

        /* The line must contain exactly one non-space character */
        letters_found = 0;
        if (status == 1) {
            for (i = 0; line[i] != '\0'; i++) {
                if (!isspace((unsigned char)line[i])) {
                    letter = (char)toupper((unsigned char)line[i]);
                    letters_found++;
                }
            }
        }

        if (letters_found == 1 && (letter == 'C' || letter == 'F' || letter == 'K')) {
            *scale = letter;
            return 1;
        }

        printf("Invalid scale. Please enter C, F, or K.\n");
    }
}

/* Returns 1 if the value is at or above absolute zero for its scale */
int is_physically_possible(double value, char scale)
{
    switch (scale) {
    case 'C': return value >= ABS_ZERO_C;
    case 'F': return value >= ABS_ZERO_F;
    case 'K': return value >= ABS_ZERO_K;
    default:  return 0;
    }
}

/* Converts a value in the given scale to Celsius */
double to_celsius(double value, char scale)
{
    switch (scale) {
    case 'F': return (value - 32.0) * 5.0 / 9.0;
    case 'K': return value - 273.15;
    default:  return value; /* already Celsius */
    }
}

/* Converts a Celsius value to the given scale */
double from_celsius(double celsius, char scale)
{
    switch (scale) {
    case 'F': return celsius * 9.0 / 5.0 + 32.0;
    case 'K': return celsius + 273.15;
    default:  return celsius; /* Celsius */
    }
}

/* Picks the temperature category based on the Celsius value */
const char *get_category(double celsius)
{
    double c = celsius + EPSILON;

    if (c < 0.0) {
        return "Freezing";
    } else if (c < 10.0) {
        return "Cold";
    } else if (c < 25.0) {
        return "Comfortable";
    } else if (c < 35.0) {
        return "Hot";
    } else {
        return "Extreme Heat";
    }
}

/* Prints a short advisory that matches the category */
void print_advisory(const char *category)
{
    const char *advice;

    if (strcmp(category, "Freezing") == 0) {
        advice = "Stay indoors and bundle up!";
    } else if (strcmp(category, "Cold") == 0) {
        advice = "Wear a jacket.";
    } else if (strcmp(category, "Comfortable") == 0) {
        advice = "Enjoy the nice weather!";
    } else if (strcmp(category, "Hot") == 0) {
        advice = "Drink lots of water!";
    } else {
        advice = "Stay indoors and avoid the heat!";
    }

    printf("Weather advisory: %s\n", advice);
}

int main(void)
{
    double value;
    double celsius;
    double converted;
    char original_scale;
    char target_scale;
    const char *category;

    /* Keep asking until we get a temperature that is physically possible */
    while (1) {
        if (!get_temperature(&value)) {
            printf("\nNo input received. Exiting.\n");
            return 1;
        }

        if (!get_scale("Enter the original scale (C, F, or K): ", &original_scale)) {
            printf("\nNo input received. Exiting.\n");
            return 1;
        }

        if (is_physically_possible(value, original_scale)) {
            break;
        }

        printf("Invalid temperature: %.2f %c is below absolute zero. Please try again.\n",
               value, original_scale);
    }

    if (!get_scale("Enter the scale to convert to (C, F, or K): ", &target_scale)) {
        printf("\nNo input received. Exiting.\n");
        return 1;
    }

    /* Convert through Celsius, which also gives us the value to categorize */
    celsius = to_celsius(value, original_scale);
    converted = from_celsius(celsius, target_scale);

    /* Avoid printing "-0.00" when floating-point error leaves a tiny negative value */
    if (converted > -EPSILON && converted < EPSILON) {
        converted = 0.0;
    }

    printf("Converted temperature: %.2f %c\n", converted, target_scale);

    category = get_category(celsius);
    printf("Temperature category: %s\n", category);
    print_advisory(category);

    return 0;
}
