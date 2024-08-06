
#ifndef C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_
#define C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_
#define size 512

#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool error_f;
} grep_flagi;

int flagi(int argc, char **argv, grep_flagi *info, char *pat[],
          char *filepIndex[]);
void file_open(char *fname, int *countp, char *pat[], char *filepIndex[]);
char *mystrdup(char *src);
void file_solve(int argc, char **argv, grep_flagi info, char *pattern[],
                int pcount);
void grep_cl(FILE *mf, grep_flagi info, regex_t *preg, char *buff,
             char *filename);
void reg(char *pattern, FILE *mf, grep_flagi info, char *filename, int fcount);
void grep(FILE *mf, grep_flagi info, regex_t preg, char *buff, char *prefix,
          char *filename, int compare);
void usage(void);

#endif  // C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_