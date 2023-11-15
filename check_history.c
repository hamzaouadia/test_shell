#include "shell.h"

/**
 * get_history_file - gets the history file
 * @inf: parameter struct
 * Return: string.
 */

char *get_history_file(inf_t *inf)
{
	char *buf, *dir;

	dir = ft_getenv(inf, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * ft_write_history - creates a file, or appends to an existing file
 * @inf: the parameter struct
 * Return: integer.
 */
int ft_write_history(inf_t *inf)
{
	ssize_t fd;
	char *filename = get_history_file(inf);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = inf->history; node; node = node->next)
	{
		ft_putsfd(node->str, fd);
		ft_putfd('\n', fd);
	}
	ft_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * ft_read_history - reads history from file
 * @inf: the parameter struct
 * Return: integer.
 */
int ft_read_history(inf_t *inf)
{
	int i, last = 0, line_count = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(inf);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			ft_build_history_list(inf, buf + last, line_count++);
			last = i + 1;
		}
	if (last != i)
		ft_build_history_list(inf, buf + last, line_count++);
	free(buf);
	inf->histcount = line_count;
	while (inf->histcount-- >= HIST_MAX)
		node_at_index(&(inf->history), 0);
	ft_renumber_history(inf);
	return (inf->histcount);
}

/**
 * ft_build_history_list - adds entry to a history linked list
 * @inf: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @line_count: the history line_count, histcount
 *
 * Return: Always 0
 */
int ft_build_history_list(inf_t *inf, char *buf, int line_count)
{
	list_t *node = NULL;

	if (inf->history)
		node = inf->history;
	ft_add_node_end(&node, buf, line_count);

	if (!inf->history)
		inf->history = node;
	return (0);
}

/**
 * ft_renumber_history - renumbers the history linked list after changes
 * @inf: Structure containing potential arguments. Used to maintain
 * Return: integer.
 */
int ft_renumber_history(inf_t *inf)
{
	list_t *node = inf->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (inf->histcount = i);
}
