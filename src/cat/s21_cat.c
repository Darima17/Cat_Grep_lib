#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  bool number_nonblank;
  bool show_ends;
  bool number;
  bool squeez_blank;
  bool show_tabs;
  bool nonprint;
  bool error_f;
} cat_flagi;

static struct option const long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"show-ends", no_argument, NULL, 'e'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-tabs", no_argument, NULL, 't'},
    {"T", no_argument, NULL, 'T'},
    {"E", no_argument, NULL, 'E'},
    {NULL, 0, NULL, 0}};

void flagi(int argc, char **argv, cat_flagi *info);
void file_solve(int argc, char **argv, cat_flagi info);
void cat_bnse(FILE *mf, cat_flagi info);

int main(int argc, char **argv) {
  cat_flagi info = {0, 0, 0, 0, 0, 0, 0};
  flagi(argc, argv, &info);
  if (info.error_f == 0) {
    file_solve(argc, argv, info);
  }
  return 0;
}

void flagi(int argc, char **argv, cat_flagi *info) {
  int opt, opt_index;
  while ((opt = getopt_long(argc, argv, "benstvTE", long_options,
                            &opt_index)) != -1) {
    switch (opt) {
      case 'b':
        info->number_nonblank = 1;
        break;
      case 'e':
        info->show_ends = info->nonprint = 1;
        break;
      case 'E':
        info->show_ends = 1;
        break;
      case 'n':
        info->number = 1;
        break;
      case 's':
        info->squeez_blank = 1;
        break;
      case 't':
        info->show_tabs = info->nonprint = 1;
        break;
      case 'T':
        info->show_tabs = 1;
        break;
      case 'v':
        info->nonprint = 1;
        break;
      default:
        info->error_f = 1;
        printf("usage: cat [-benstvTE] [file ...]\n");
    }
  }
}

void file_solve(int argc, char **argv, cat_flagi info) {
  int currfile = optind;
  while (currfile < argc) {
    FILE *mf;
    if (currfile != argc && (mf = fopen(argv[currfile], "rb")) == NULL) {
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
              argv[currfile]);
    } else {
      cat_bnse(mf, info);
      fclose(mf);
    }
    currfile++;
  };
}

void cat_bnse(FILE *mf, cat_flagi info) {
  int cur_s = '\n', next_s = 0, str_count = 0, fl = 0, squeeze = 0;
  while ((next_s = fgetc(mf)) != EOF) {
    if (cur_s == '\n') {
      if (info.squeez_blank && next_s == '\n') {
        if (squeeze) {
          continue;
        }
        squeeze = 1;
      } else {
        squeeze = 0;
      }
      if (info.number_nonblank) {
        if (next_s != '\n') {
          fprintf(stdout, "%6d\t", (++str_count));
        }
      } else if (info.number) {
        fprintf(stdout, "%6d\t", (++str_count));
      }
    }
    if (next_s == '\n' && info.show_ends) {
      printf("$");
      fl = 1;
    } else {
      fl = 0;
    }
    if (next_s == '\t' && info.show_tabs) {
      printf("^I");
      fl = 1;
    } else {
      fl = 0;
    }
    if (info.nonprint && next_s != '\n' && next_s != '\t') {
      if (next_s > 127) {
        printf("M-");
        next_s = 127;
        fl = 1;
      } else
        fl = 0;
      if (next_s < 32) {
        printf("^%c", next_s + 64);
        fl = 1;
      } else if (next_s == 127) {
        printf("^?");
        fl = 1;
      } else
        fl = 0;
    }
    cur_s = next_s;
    if (fl != 1) {
      printf("%c", cur_s);
    }
  }
}
