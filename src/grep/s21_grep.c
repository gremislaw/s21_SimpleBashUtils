#include "s21_grep.h"

int main(int argc, char **argv) {
  struct opts options = {0};
  char search_string[MAXLEN] = {0};
  parse(argc, argv, &options, search_string);
  grep_priority_process(argc, argv, options, search_string);
  return 0;
}

void parse(int argc, char **argv, struct opts *options, char *search_string) {
  char arg;
  while ((arg = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL))) {
    if (arg == -1) break;
    switch (arg) {
      case 'e':
        options->e = 1;
        strcat(search_string, optarg);
        strcat(search_string, "|");
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        f_process(search_string, optarg);
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        fprintf(stderr, "use flags eivclnhsfo");
        break;
    }
  }
  if (options->o && (options->l || options->v || options->c)) options->o = 0;
  if (!options->e && !options->f) {
    options->e = 1;
    strcat(search_string, argv[optind++]);
    strcat(search_string, "|");
  }
  search_string[strlen(search_string) - 1] = '\0';
}

void f_process(char *search_string, const char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  char f_line[MAXLEN] = {0};
  if (file != NULL) {
    while (fgets(f_line, MAXLEN, file) != NULL) {
      if (f_line[strlen(f_line) - 1] == '\n') {
        f_line[strlen(f_line) - 1] = '\0';
      }
      if (strlen(f_line) == 0) {
        f_line[0] = '.';
      }
      strcat(search_string, f_line);
      strcat(search_string, "|");
    }
    fclose(file);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
  }
}

void grep_priority_process(int argc, char **argv, struct opts options,
                           char *search_string) {
  int files_cnt = argc - optind;

  while (optind < argc && argv[optind]) {
    int matches = 0;
    FILE *f = fopen(argv[optind], "r");
    if (!f) {
      if (!options.s) {
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
      }
    } else {
      grep_process(options, f, argv[optind], search_string, &matches,
                   files_cnt);
    }
    optind++;
  }
}

void grep_process(struct opts options, FILE *file, char *filename,
                  char *search_string, int *matches, int files_cnt) {
  int i = 1;
  char str[MAXLEN] = {0};
  while (fgets(str, MAXLEN, file) != NULL) {
    if (str[strlen(str) - 1] != '\n') {
      str[strlen(str) + 1] = '\0';
      str[strlen(str)] = '\n';
    }
    regex_t regex;
    if (!regcomp(&regex, search_string, options.i ? REG_ICASE : REG_EXTENDED)) {
      if (regexec(&regex, str, 0, NULL, 0) == options.v) {
        (*matches)++;
        if (!(options.c || options.l)) {
          if (!options.o) {
            if (!options.h && files_cnt > 1) printf("%s:", filename);
            if (options.n) printf("%d:", i);
            printf("%s", str);
          } else
            o_process(options, filename, files_cnt, str, regex, i);
        }
      }
      regfree(&regex);
    }
    i++;
  }
  aux_process(options, filename, files_cnt, *matches);
}

void o_process(struct opts options, const char *filename, int files_cnt,
               char *str, regex_t regex, int i) {
  while (!regexec(&regex, str, 0, NULL, 0)) {
    char buff[MAXLEN] = {0};
    strcpy(buff, str);
    int left = 0;
    int right = strlen(buff);
    while (!regexec(&regex, buff + left, 0, NULL, 0)) {
      buff[--right] = '\0';
    }
    buff[right] = str[right];
    right++;
    while (!regexec(&regex, buff + left, 0, NULL, 0)) left++;
    left--;
    if (!options.h && files_cnt > 1) printf("%s:", filename);
    if (options.n) printf("%d:", i);
    if (!options.v) printf("%s\n", buff + left);
    int len = right - left + 1;
    for (int _ = 0; _ < len; _++) {
      str[left + _] = 127;
    }
  }
  if (options.v) {
    printf("%s", str);
  }
}

void aux_process(struct opts options, const char *filename, int files_cnt,
                 int matches) {
  if (options.c) {
    if (options.l) {
      if (files_cnt > 1 && !options.h) printf("%s:", filename);
      if (matches > 0)
        printf("1\n");
      else
        printf("0\n");
    } else {
      if (files_cnt > 1 && !options.h) printf("%s:", filename);
      printf("%d\n", matches);
    }
  }
  if (options.l && matches) printf("%s\n", filename);
}