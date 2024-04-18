#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct opts {
  int b, E, n, s, T, v;
} opts;

void parse(int argc, char **argv, struct opts *options);
void print_file(int argc, char **argv, struct opts options);
void write_sym(struct opts options, char *prev_prev, char *prev, int cur,
               int *i);

#endif