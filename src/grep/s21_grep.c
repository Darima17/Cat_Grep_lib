#include "s21_grep.h"

int main(int argc, char **argv) {
  char *pat[size] = {0};
  char *filepIndex[size] = {0};
  grep_flagi info = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int pcount = 0;
  pcount = flagi(argc, argv, &info, pat, filepIndex);
  if (info.error_f == 0 && pcount != -1) {
    file_solve(argc, argv, info, pat, pcount);
  }
  for (int i = 0; i < pcount; i++) {
    if (filepIndex[i] != NULL) {
      free(pat[i]);
    }
  }
  return 0;
}

int flagi(int argc, char **argv, grep_flagi *info, char *pat[],
          char *filepIndex[]) {
  int opt;
  int countp = 0;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        info->e = 1;
        pat[countp] = optarg;
        countp++;
        break;
      case 'i':
        info->i = 1;
        break;
      case 'v':
        info->v = 1;
        break;
      case 'c':
        info->c = 1;
        break;
      case 'l':
        info->l = 1;
        break;
      case 'n':
        info->n = 1;
        break;
      case 'h':
        info->h = 1;
        break;
      case 's':
        info->s = 1;
        break;
      case 'f':
        info->f = 1;
        file_open(optarg, &countp, pat, filepIndex);
        break;
      default:
        info->error_f = 1;
        usage();
    }
  }
  return countp;
}

void file_open(char *fname, int *countp, char *pat[], char *filepIndex[]) {
  FILE *mf = NULL;
  mf = fopen(fname, "r");
  if (mf != NULL) {
    char buff[size];
    while (fgets(buff, size, mf) != NULL) {
      if (buff[strlen(buff) - 1] == '\n') {
        buff[strlen(buff) - 1] = '\0';
      }
      pat[(*countp)] = mystrdup(buff);
      filepIndex[(*countp)] = "1";
      (*countp)++;
    }
    fclose(mf);
  } else {
    fprintf(stderr, "%s: No such file or directory\n", fname);
    (*countp) = -1;
  }
}

char *mystrdup(char *src) {
  char *str, *p;
  int len = 0;
  while (src[len]) len++;
  str = malloc(len + 1);
  p = str;
  while (*src) *p++ = *src++;
  *p = '\0';
  return str;
}

void file_solve(int argc, char **argv, grep_flagi info, char *pattern[],
                int pcount) {
  char pline[size] = "";
  FILE *mf = NULL;
  int fcount = 0, f = 0, n = 0;
  if (info.e || info.f) {
    fcount = argc - optind;
    if (pcount > 0) {
      for (int i = 0; i < pcount; i++) {
        strcat(pline, pattern[i]);
        if (i + 1 < pcount) {
          strcat(pline, "|");
        }
      }
    } else {
      fcount = 0;
    }
  } else {
    fcount = argc - optind - 1;
    f = 1;
    strcat(pline, argv[argc - fcount - f]);
  }
  while (n < fcount) {
    mf = fopen(argv[optind + n + f], "r");
    if (mf == NULL) {
      if (!info.s) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind + n + f]);
      }
    } else {
      reg(pline, mf, info, argv[optind + n + f], fcount);
      fclose(mf);
    }
    n++;
  };
}

void reg(char *pattern, FILE *mf, grep_flagi info, char *filename, int fcount) {
  int cflags = 0, compare = 0;
  regex_t preg;
  char buff[size], prefix[100] = "";
  if (fcount > 1 && !info.h) {
    strcat(prefix, filename);
    strcat(prefix, ":");
  }
  if (info.v) {
    compare = REG_NOMATCH;
  }
  if (info.i) {
    cflags = REG_ICASE;
  }
  if (info.e || info.f) {
    if (info.i) {
      cflags = REG_ICASE | REG_EXTENDED;
    } else {
      cflags = REG_EXTENDED;
    }
  }
  regcomp(&preg, pattern, cflags);
  grep(mf, info, preg, buff, prefix, filename, compare);
}

void grep(FILE *mf, grep_flagi info, regex_t preg, char *buff, char *prefix,
          char *filename, int compare) {
  regmatch_t match;
  int numline = 0, stop = 0, linecount = 0;
  char finallinenum[100] = "";
  while ((fgets(buff, size, mf)) && stop != 1) {
    int line, resultreg;
    line = strlen(buff);
    if (buff[line - 1] == '\n') {
      buff[line - 1] = '\0';
    }
    numline++;
    resultreg = regexec(&preg, buff, 1, &match, 0);
    if (resultreg == compare) {
      linecount++;
      if (!info.c) {
        if (info.l) {
          printf("%s\n", filename);
          stop = 1;
        } else {
          if (info.n) {
            sprintf(finallinenum, "%d:", numline);
          }
          printf("%s%s%s\n", prefix, finallinenum, buff);
        }
      }
    }
  }
  regfree(&preg);
  if (info.c) {
    if (!info.l) {
      printf("%s%d\n", prefix, linecount);
    }
  }
}

void usage(void) {
  printf("usage: s21_grep [-eivclnhsf] [-e pattern] [-f file]\n");
}