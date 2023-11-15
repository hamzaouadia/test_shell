#include "shell.h"

/**
 * ft_input_buf - buffers chained commands
 * @inf: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t ft_input_buf(inf_t *inf, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*ft_bfree((void **)inf->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, ft_sigHandler);
#if USEft_getline
		r = getline(buf, &len_p, stdin);
#else
		r = ft_getline(inf, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			inf->line_count_flag = 1;
			ft_remove_comments(*buf);
			ft_build_history_list(inf, *buf, inf->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				inf->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * ft_get_input - gets a line minus the newline
 * @inf: parameter struct
 *
 * Return: bytes read
 */
ssize_t ft_get_input(inf_t *inf)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(inf->arg), *p;

	_putchar(BUF_FLUSH);
	r = ft_input_buf(inf, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		ft_check_chain(inf, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(inf, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and len */
			inf->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return len of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from ft_getline() */
	return (r); /* return len of buffer from ft_getline() */
}

/**
 * ft_read_buf - reads a buffer
 * @inf: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t ft_read_buf(inf_t *inf, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(inf->readfd, buf, read_buf_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * ft_getline - gets the next line of input from STDIN
 * @inf: parameter struct
 * @pointer: address of pointer to buffer, preallocated or NULL
 * @len: size of preallocated pointer buffer if not NULL
 *
 * Return: s
 */
int ft_getline(inf_t *inf, char **pointer, size_t *len)
{
	static char buf[read_buf_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *pointer;
	if (p && len)
		s = *len;
	if (i == len)
		i = len = 0;

	r = ft_read_buf(inf, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (len)
		*len = s;
	*pointer = p;
	return (s);
}

/**
 * ft_sigHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void ft_sigHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
