#include "shell.h"

/**
 * ft_is_cmd - determines if a file is an executable command
 * @inf: the inf struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int ft_is_cmd(inf_t *inf, char *path)
{
	struct stat st;

	(void)inf;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * ft_dup_chars - duplicates characters
 * @pathstring: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *ft_dup_chars(char *pathstring, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstring[i] != ':')
			buf[k++] = pathstring[i];
	buf[k] = 0;
	return (buf);
}

/**
 * ft_find_path - finds this cmd in the PATH string
 * @inf: the inf struct
 * @pathstring: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *ft_find_path(inf_t *inf, char *pathstring, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstring)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (ft_is_cmd(inf, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstring[i] || pathstring[i] == ':')
		{
			path = ft_dup_chars(pathstring, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (ft_is_cmd(inf, path))
				return (path);
			if (!pathstring[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
