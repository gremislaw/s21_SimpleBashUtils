#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 99999

typedef struct opts {
  int e, i, v, c, l, n, h, s, f, o;
} opts;

void parse(int argc, char **argv, struct opts *options, char *search_string);
void grep_priority_process(int argc, char **argv, struct opts options,
                           char *search_string);
void f_process(char *search_string, const char *filename);
void grep_process(struct opts options, FILE *file, char *filename,
                  char *search_string, int *matches, int files_cnt);
void o_process(struct opts options, const char *filename, int files_cnt,
               char *str, regex_t regex, int i);
void aux_process(struct opts options, const char *filename, int files_cnt,
                 int matches);

#endif