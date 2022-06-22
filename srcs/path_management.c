//
// Created by Peer de Bakker on 6/22/22.
//

#include <string.h>
#include <unistd.h>
#include "libft.h"

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

	out = ft_calloc(len_a + len_b + len_b + 1, sizeof(char));
	if (out == NULL)
		return (NULL);
	ft_strlcpy(out, a, len_a + 1);
	ft_strlcpy(out + len_a, b, len_b + 1);
	ft_strlcpy(out + len_a + len_b, c, len_c + 1);
	return (out);
}

static void	free_array(char **arr) {
	size_t	i = 0;

	while (arr[i]) {
		free(arr[i]);
		arr[i] = NULL;
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

	if (path_split == NULL)
		return (NULL);
	while (path_split[i]) {
		combined_path = ft_str3join(path_split[i], "/", arg);
		if (combined_path && access(combined_path, F_OK | X_OK) == 0) {
			free_array(path_split);
			return (combined_path);
		}
		free(combined_path);
		i++;
	}
	free_array(path_split);
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