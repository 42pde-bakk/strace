//
// Created by Peer de Bakker on 6/22/22.
//

#include <string.h>
#include <unistd.h>
#include "libft.h"
#include <stdio.h>

#define PATH_PREFIX "PATH="

static const char	*get_PATH(const char **envp) {
	size_t	i = 0;

	while (envp[i]) {
		if (ft_strncmp(envp[i], PATH_PREFIX, ft_strlen(PATH_PREFIX)) == 0) {
			return (envp[i]);
		}
		i++;
	}
	return (NULL);
}

static char	*ft_str3join(const char *a, const char *b, const char *c)
{
	char	*out;
	size_t	len_a = ft_strlen(a),
			len_b = ft_strlen(b),
			len_c = ft_strlen(c);
	size_t	total_length = len_a + len_b + len_c + 1;

	out = ft_calloc(total_length, sizeof(char));
	if (out == NULL)
		return (NULL);
	strcpy(out, a);
	strcpy(out + len_a, b);
	strcpy(out + len_a + len_b, c);
	return (out);
}

static void	free_array(char **arr) {
	size_t	i = 0;

	while (arr[i]) {
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

static char	*get_abspath(const char *PATH_with_prefix, const char *arg) {
	size_t	i = 0;
	char	*combined_path;
	char	*PATH = ft_substr(PATH_with_prefix, ft_strlen(PATH_PREFIX), ft_strlen(PATH_with_prefix) - ft_strlen(PATH_PREFIX));
	if (PATH == NULL)
		return (NULL);
	char	**path_split = ft_split(PATH, ':');

	if (path_split == NULL) {
		free(PATH);
		return (NULL);
	}
	while (path_split[i]) {
		combined_path = ft_str3join(path_split[i], "/", arg);
		if (combined_path && access(combined_path, F_OK | X_OK) == 0) {
			free_array(path_split);
			free(PATH);
			return (combined_path);
		}
		free(combined_path);
		combined_path = NULL;
		i++;
	}
	free_array(path_split);
	free(PATH);
	return (NULL);
}

char	*get_absolute_path(const char* arg, const char **envp) {
	const char	*PATH;

	if (access(arg, F_OK) == 0) {
		// Absolute path, we can execute that one!
		return (ft_strdup(arg));
	}
	PATH = get_PATH(envp);
	if (PATH == NULL)
		return (NULL);

	return (get_abspath(PATH, arg));
}