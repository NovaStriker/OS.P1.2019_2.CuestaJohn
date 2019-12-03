// Primera parte del Proyecto de Sistemas Operativos
// extras.c

// Nombre: John Aldo Cuesta Agila
// Paralelo: 2
// Fecha de entrega: 3 de Diciembre del 2019

// Importación propia
#include <extras.h>

// Importaciones adicionales necesarias
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

char *str_replace(char* string, const char* substr, const char* replacement) {
	char* tok = NULL;
	char* newstr = NULL;
	char* oldstr = NULL;
	int   oldstr_len = 0;
	int   substr_len = 0;
	int   replacement_len = 0;

	newstr = strdup(string);
	substr_len = strlen(substr);
	replacement_len = strlen(replacement);

	if (substr == NULL || replacement == NULL) {
		return newstr;
	}

	while ((tok = strstr(newstr, substr))) {
		oldstr = newstr;
		oldstr_len = strlen(oldstr);
		newstr = (char*)malloc(sizeof(char) * (oldstr_len - substr_len + replacement_len + 1));

		if (newstr == NULL) {
			free(oldstr);
			return NULL;
		}

		memcpy(newstr, oldstr, tok - oldstr);
		memcpy(newstr + (tok - oldstr), replacement, replacement_len);
		memcpy(newstr + (tok - oldstr) + replacement_len, tok + substr_len, oldstr_len - substr_len - (tok - oldstr));
		memset(newstr + oldstr_len - substr_len + replacement_len, 0, 1);

		free(oldstr);
	}

	return newstr;
}

char *str_sep(char **stringp, const char *delim) {
    char *begin, *end;
    begin = *stringp;
    if (begin == NULL)
        return NULL;

    // Se busca el final del token
    end = begin + strcspn (begin, delim);
    if (*end)
        {
            // Termina el token y se establece el puntero después del caracter nulo
            *end++ = '\0';
            *stringp = end;
        }
    else
        // No hay más delimitadores en el último token
        *stringp = NULL;
    return begin;
}

char *strstrip(char *s) {
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(*end))
        end--;
    *(end + 1) = '\0';

    while (*s && isspace(*s))
        s++;

    return s;
}

char **str_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    // Se contabilizan los elementos que se van a extraer
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Se agrega un espacio para el útilmo token
    count += last_comma < (a_str + strlen(a_str) - 1);

    // Se agrega un espacio para terminar la cadena nula, de esta manera
    // se sabe en dónde se termina la lista de cadenas devueltas.
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;

        char *copy;
        copy = malloc(sizeof(char*) * (strlen(a_str) + 1));
        strcpy(copy, a_str);

        char *token;

        while ((token = strtok_r(copy, &a_delim, &copy)) != NULL) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
        }

        *(result + idx) = 0;
    }

    return result;
}