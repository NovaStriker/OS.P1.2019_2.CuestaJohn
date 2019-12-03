#define MAXIMO_COMANDO 8096

char *str_replace(char* string, const char* substr, const char* replacement);
char *str_sep(char **stringp, const char *delim);
char *strstrip(char *s);
char **str_split(char* a_str, const char a_delim);