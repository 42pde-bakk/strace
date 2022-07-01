//
// Created by Peer de Bakker on 7/1/22.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include "strace.h"
#include <errno.h>
#define MAX_VARS 5

static size_t	array_length(char **arr) {
	size_t	i = 0;

	if (!arr)
		return (0);
	while (arr[i]) {
		++i;
	}
	return (i);
}

static int	count_escapable_chars(const char *haystack) {
	int	count = 0;
	int i = 0;

	while (haystack[i]) {
		if (haystack[i] >= '\a' && haystack[i] <= '\r') {
			// '\a', '\b', '\t', '\n', '\v', '\f', '\r'
			count++;
		}
		haystack++;
	}
	return (count);
}

char	*buffering(char *str) {
	size_t	escapableCharsCount = count_escapable_chars(str);
	size_t	len = strlen(str) + escapableCharsCount + 1;
	size_t	i = 0,
			i2 = 0;
	char	*buffer;

	buffer = calloc(len, sizeof(char));
	while (str[i]) {
		if (str[i] >= '\a' && str[i] <= '\r') {
			buffer[i2] = '\\';
			switch (str[i]) {
				case '\a':
					buffer[i2 + 1] = 'a';
					break ;
				case '\b':
					buffer[i2 + 1] = 'b';
					break ;
				case '\t':
					buffer[i2 + 1] = 't';
					break ;
				case '\n':
					buffer[i2 + 1] = 'n';
					break ;
				case '\v':
					buffer[i2 + 1] = 'v';
					break ;
				case '\f':
					buffer[i2 + 1] = 'f';
					break ;
				case '\r':
					buffer[i2 + 1] = 'r';
					break ;
				default:
					break ;
			}
			i2 += 2;
			i += 1;
		} else {
			buffer[i2] = str[i];
			i2++;
			i++;
		}
	}
	free(str);
	return (buffer);
}

char	*read_string(unsigned long long regval, bool *dots, size_t read_amount) {
	char		buf[1024];
	char		*out;
	long int	ret;
	size_t		i = 0;

	memset(buf, 0, sizeof(buf));
	while (i < read_amount) {
		ret = ptrace(PTRACE_PEEKDATA, g_childpid, regval + (i * sizeof(ret)), 0);
		if (errno) {
			break ;
		}
		memcpy(buf + (i * sizeof(ret)), &ret, sizeof(ret));
		if (memchr(&ret, 0, sizeof(ret)) != NULL)
			break ;
		i++;
	}
	if (i == read_amount) {
		*dots = true;
	}
	out = calloc(strlen(buf) + 1, sizeof(char));
	strcpy(out, buf);
	return (out);
}

void	read_string_array(unsigned long long regval) {
	size_t	len,
			i = 0;
	char	**string_array;

	string_array = (char **)regval;
	len = array_length(string_array);
	if (len > MAX_VARS) {
		fprintf(stderr, "%p /* %zu vars */", string_array, len);
		return ;
	}
	fprintf(stderr, "[");
	while (string_array[i]) {
		bool dots = false;
		if (i)
			fprintf(stderr, ", ");
		char	*str = read_string((unsigned long long)string_array[i], &dots, 5);
		fprintf(stderr, "\"%s\"", str);
		if (dots)
			fprintf(stderr, "...");
		free(str);
		++i;
	}
	fprintf(stderr, "]");
}