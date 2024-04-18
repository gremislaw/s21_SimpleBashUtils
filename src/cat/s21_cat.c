#include "s21_cat.h"

int main(int argc, char **argv) {
  struct opts options = {0};
  parse(argc, argv, &options);
  print_file(argc, argv, options);
  return 0;
}

void parse(int argc, char **argv, struct opts *options) {
  struct option adv_opts[] = {{"number-nonblank", 0, 0, 'b'},
                              {"number", 0, 0, 'n'},
                              {"squeeze-blank", 0, 0, 's'}};
  char arg;
  while ((arg = getopt_long(argc, argv, "+beEnstvT", adv_opts, NULL))) {
    if (arg == -1) break;
    switch (arg) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->E = 1;
        options->v = 1;
        break;
      case 'E':
        options->E = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->T = 1;
        options->v = 1;
        break;
      case 'T':
        options->T = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      default:
        fprintf(stderr, "use flags beEnstTv");
        exit(1);
    }
  }
}

void print_file(int argc, char **argv, struct opts options) {
  while (argc != 0 && argv[optind]) {
    FILE *f = fopen(argv[optind], "r");
    if (!f) {
      fprintf(stderr, "No such file");
      exit(1);
    } else {
      char prev_prev = ' ';
      char prev = '\n';
      char c = fgetc(f);
      int i = 1;
      while (!feof(f)) {
        write_sym(options, &prev_prev, &prev, c, &i);
        c = fgetc(f);
      }
    }
    optind++;
  }
}

void write_sym(struct opts options, char *prev_prev, char *prev, int cur,
               int *i) {
  int access_print = 1;
  char res[10] = {cur, '\0'};
  if (options.E && cur == '\n') {
    res[0] = '$';
    res[1] = '\n';
    res[2] = '\0';
  }
  if (options.T && cur == '\t') {
    res[0] = '^';
    res[1] = 'I';
    res[2] = '\0';
  }
  if (options.v && cur != '\n' && cur != '\t') {
    if (cur <= 31) {
      cur += 64;
      res[0] = '^';
      res[1] = cur;
    } else if (cur == 127) {
      res[0] = '^';
      res[1] = '?';
    } else if (cur >= 128 && cur < 160) {
      cur -= 64;
      res[0] = 'M';
      res[1] = '-';
      res[2] = '^';
      res[3] = cur;
    }
  } else if (options.s && *prev_prev == '\n' && *prev == '\n' && cur == '\n')
    access_print = 0;
  if (access_print) {
    if ((options.b && *prev == '\n' && cur != '\n') ||
        (!options.b && options.n && *prev == '\n')) {
      printf("%6d\t", *i);
      (*i)++;
    }
    if (options.b && *prev == '\n' && cur == '\n' && options.E) {
      printf("      \t");
    }
    printf("%s", res);
  }
  *prev_prev = *prev;
  *prev = cur;
}