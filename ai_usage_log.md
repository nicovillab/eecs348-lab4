# AI Usage Log

- **Course:** EECS 348, Lab 8 (C Programming)
- **AI assistant:** Claude (Anthropic)
- **Files involved:** `task1.c`, `task2.c`, `Makefile`, assignment description PDF

## Interaction 1

### User

I want you to act as a code reviewer for my EECS 348 C programming lab.

I am going to give you:

1. the assignment description
2. my current `task1.c`
3. my current `task2.c`
4. my current `Makefile`

Do NOT rewrite the programs from scratch.

First, carefully read the entire assignment and compare it against my existing implementation.

Check specifically for:

- whether Task 1 meets every requirement
- whether Task 2 meets every requirement
- whether the Makefile correctly supports `make all` and `make clean`
- input validation
- invalid and malformed inputs
- boundary cases
- output formatting
- possible compiler warnings
- anything that could fail hidden grading tests
- anything I implemented that is not actually required by the assignment and could cause problems

For each file, tell me whether it is correct and explain any actual issue you find.

If something is already correct, say so and do not suggest changing it just for style.

Do not generate replacement code yet.

### Assistant Summary

The assistant read the assignment PDF and the student's three existing files, then compiled them and ran them against the sample and a set of edge cases. It did not rewrite any code.

- **Task 1:** the output for a score of 25 matched all 38 sample lines exactly. Invalid inputs (`abc`, `25abc`, negatives) and end of input were handled correctly. The one real risk found was the student's `MAX_SCORE 100` limit: the assignment sets no upper limit, so a hidden test above 100 would be rejected.
- **Task 2:** the sample conversion and every category boundary were correct. The assistant confirmed two of the student's decisions as correct: categorizing by the Celsius value (the sample requires it) and using 25 as the start of "Hot" (the spec's `[20, 35)` is a typo). The one real validation gap was that `strtod` accepts `inf`, `nan` and hex values. It also flagged that the absolute-zero re-prompt loop, which the assignment does not require, behaves oddly if an automated test ends right after an impossible value. A few minor floating-point edge cases were listed.
- **Makefile:** correct, with no changes needed. No compiler warnings under the course flags.

## Interaction 2

### User

Thanks. Before changing anything, I want you to separate actual assignment-risk issues from extra defensive-programming concerns.

For each issue you identified, classify it as one of:

1. Must fix before submission
2. Worth fixing, but unlikely to matter for the grader
3. Not necessary for this assignment

Use only the assignment requirements and realistic hidden tests for an undergraduate C lab.

In particular, evaluate:

- the `MAX_SCORE 100` restriction in Task 1
- accepting `inf`, `nan`, and hexadecimal floating-point forms in Task 2
- the absolute-zero validation loop
- the minor floating-point edge cases you mentioned

Do not modify the code yet.

At the end, tell me the minimum changes you would make before running the full test suite.

### Assistant Summary

The assistant sorted each issue by assignment risk. It did not change any code.

- **Must fix:** only the `MAX_SCORE 100` limit, because it is the only case where valid input gets rejected against the spec.
- **Worth fixing but unlikely to matter:** rejecting `inf` and `nan`.
- **Not necessary:** rejecting hex input, removing the absolute-zero loop, the floating-point edge cases, the unreachable "No combinations" branch, and clang-only warnings.

The recommended minimum change was removing the score limit. Adding an `isfinite()` check was offered as the one optional extra.

## Interaction 3

### User

Make only the minimum changes you just recommended.

For `task1.c`:
- remove the `MAX_SCORE` definition
- remove the check that rejects scores greater than 100
- do not change any other logic, prompts, formatting, comments, loop order, or input handling

For `task2.c`:
- add the optional `isfinite()` check so `nan` and `inf` are rejected as invalid numeric input
- include whatever standard header is required
- do not change any other behavior

Do not rewrite either file from scratch.

After making those changes:
1. show me exactly what changed
2. give me the full updated `task1.c`
3. give me the full updated `task2.c`
4. do not run tests yet

### Assistant Summary

The assistant made targeted edits to the student's existing files and showed a diff plus both full updated files. It did not run tests.

- **`task1.c`:** removed the `MAX_SCORE` define, the comment describing it, and the `score > MAX_SCORE` check.
- **`task2.c`:** added `#include <math.h>` and an `isfinite(*value)` condition to the existing temperature-validation check.

The assistant also disclosed that it had added a one-line comment above the new check, which the student had not asked for.

## Interaction 4

### User

Now compile and test the updated files.

Use:

gcc -Wall -Wextra -std=c11 -pedantic

Also verify:

make clean
make all

For Task 1, test:
- 25
- 2
- 3
- 0
- -5
- abc
- 25abc
- 1
- 120

For Task 2, test:
- 30 C -> F
- 32 F -> C
- 273.15 K -> C
- -273.15 C -> K
- 0 C -> F
- 10 C -> F
- 25 C -> F
- 35 C -> F
- lowercase scales
- invalid scale X
- abc as the temperature
- nan
- inf
- a value below absolute zero
- C -> C
- F -> F
- K -> K

For every test, report:
- input
- observed output
- pass or fail

Also report any compiler warnings or errors.

Do not modify the code during testing.

### Assistant Summary

The assistant compiled both files with the requested flags and ran `make clean` and `make all`. There were no warnings or errors. It ran every requested test without modifying the code and reported the input, observed output and pass/fail for each.

- **Task 1:** all 9 tests passed. The output for 25 matched the sample exactly. The output for 120 (6,463 combinations) was checked against a separate Python calculation and matched line for line.
- **Task 2:** all requested tests passed. `nan`, `inf`, `-inf`, `INFINITY` and `NaN` are now rejected as invalid input.
- **Below-absolute-zero case:** the assistant pointed out that it still re-prompts, and exits with code 1 if input ends right after the impossible value. This is the behavior chosen in Interaction 2.

## Interaction 5

### User

Uploaded the course's Gen-AI Log Guide PDF.

### Assistant Summary

The assistant read the guide and generated this `ai_usage_log.md` from the conversation, following the structure shown in the guide. The course's `AI_USAGE_LOG_INSTRUCTIONS.md` file was not provided in this chat, so any extra fields it requires were not included.
