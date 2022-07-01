//
// Created by peerdb on 29-6-22.
//

#include <time.h>
#include <stdlib.h>
#include <string.h>

double	measure_elapsed_time(const clock_t start) {
	clock_t	now = clock();

	return ((double)(now - start) / CLOCKS_PER_SEC);
}

char	*ft_str3join(const char *a, const char *b, const char *c) {
	char	*out;
	size_t	len_a = strlen(a),
			len_b = strlen(b),
			len_c = strlen(c);
	size_t	total_length = len_a + len_b + len_c + 1;

	out = calloc(total_length, sizeof(char));
	if (out == NULL)
		return (NULL);
	strcpy(out, a);
	strcpy(out + len_a, b);
	strcpy(out + len_a + len_b, c);
	return (out);
}

void	free_array(char **arr) {
	size_t	i = 0;

	while (arr[i]) {
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}
