/*
 * EECS 348 Lab 8 - Task 1: Football Score Possibilities
 *
 * Repeatedly asks the user for an NFL score and prints every combination
 * of scoring plays that adds up to that score. Entering 1 stops the program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/* Point values for each type of scoring play */
#define TD_2PT_POINTS  8   /* touchdown + 2-point conversion */
#define TD_FG_POINTS   7   /* touchdown + 1-point field goal */
#define TD_POINTS      6   /* touchdown */
#define FG_POINTS      3   /* 3-point field goal */
#define SAFETY_POINTS  2   /* safety */

#define LINE_SIZE 256

/*
 * Reads one line of input and tries to turn it into an integer.
 * Returns  1 if a valid integer was read (stored in *value),
 *          0 if the line was not a valid integer,
 *         -1 if there is no more input (EOF).
 */
int read_integer(int *value)
{
    char line[LINE_SIZE];
    char *end;
    long number;

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }

    /* If the line was too long to fit, throw away the rest of it */
    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* discard */
        }
        return 0;
    }

    errno = 0;
    number = strtol(line, &end, 10);

    /* No digits were found at all */
    if (end == line) {
        return 0;
    }

    /* Only whitespace is allowed after the number (e.g. "25abc" is rejected) */
    while (*end != '\0') {
        if (!isspace((unsigned char)*end)) {
            return 0;
        }
        end++;
    }

    /* Number did not fit in an int */
    if (errno == ERANGE || number > 1000000000L || number < -1000000000L) {
        return 0;
    }

    *value = (int)number;
    return 1;
}

/*
 * Prints every combination of scoring plays that adds up to score.
 * The loops go from the most valuable play to the least valuable one,
 * and the number of safeties is whatever is left over (if it divides evenly).
 */
void print_combinations(int score)
{
    int td2, tdfg, td, fg;
    int count = 0;

    printf("Possible combinations of scoring plays if a team's score is %d:\n", score);

    for (td2 = 0; td2 * TD_2PT_POINTS <= score; td2++) {
        for (tdfg = 0; td2 * TD_2PT_POINTS + tdfg * TD_FG_POINTS <= score; tdfg++) {
            for (td = 0; td2 * TD_2PT_POINTS + tdfg * TD_FG_POINTS
                         + td * TD_POINTS <= score; td++) {
                for (fg = 0; td2 * TD_2PT_POINTS + tdfg * TD_FG_POINTS
                             + td * TD_POINTS + fg * FG_POINTS <= score; fg++) {

                    int remaining = score - (td2 * TD_2PT_POINTS + tdfg * TD_FG_POINTS
                                             + td * TD_POINTS + fg * FG_POINTS);

                    /* The rest has to be made up entirely of safeties */
                    if (remaining % SAFETY_POINTS == 0) {
                        int safety = remaining / SAFETY_POINTS;
                        printf("%d TD + 2pt, %d TD + FG, %d TD, %d 3pt FG, %d Safety\n",
                               td2, tdfg, td, fg, safety);
                        count++;
                    }
                }
            }
        }
    }

    if (count == 0) {
        printf("No combinations of scoring plays can produce a score of %d.\n", score);
    }
}

int main(void)
{
    int score;
    int status;

    while (1) {
        printf("Enter the NFL score (Enter 1 to stop): ");
        fflush(stdout);

        status = read_integer(&score);

        if (status == -1) {
            /* End of input: stop cleanly instead of looping forever */
            printf("\n");
            break;
        }

        if (status == 0) {
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }

        if (score == 1) {
            break;
        }

        if (score < 0) {
            printf("Invalid score: a score cannot be negative.\n");
            continue;
        }


        print_combinations(score);
    }

    return 0;
}
