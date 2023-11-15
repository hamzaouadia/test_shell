#include "shell.h"

/**
 * ft_clear_inf - initializes inf_t struct
 * @inf: struct address
 */
void ft_clear_inf(inf_t *inf)
{
	inf->arg = NULL;
	inf->argv = NULL;
	inf->path = NULL;
	inf->argc = 0;
}

/**
 * ft_set_inf - initializes inf_t struct
 * @inf: struct address
 * @av: argument vector
 */
void ft_set_inf(inf_t *inf, char **av)
{
	int i = 0;

	inf->fname = av[0];
	if (inf->arg)
	{
		inf->argv = strtow(inf->arg, " \t");
		if (!inf->argv)
		{

			inf->argv = malloc(sizeof(char *) * 2);
			if (inf->argv)
			{
				inf->argv[0] = _strdup(inf->arg);
				inf->argv[1] = NULL;
			}
		}
		for (i = 0; inf->argv && inf->argv[i]; i++)
			;
		inf->argc = i;

		ft_replace_alias(inf);
		ft_replace_vars(inf);
	}
}

/**
 * ft_free_inf - frees inf_t struct fields
 * @inf: struct address
 * @all: true if freeing all fields
 */
void ft_free_inf(inf_t *inf, int all)
{
	ffree(inf->argv);
	inf->argv = NULL;
	inf->path = NULL;
	if (all)
	{
		if (!inf->cmd_buf)
			free(inf->arg);
		if (inf->env)
			ft_free_list(&(inf->env));
		if (inf->history)
			ft_free_list(&(inf->history));
		if (inf->alias)
			ft_free_list(&(inf->alias));
		ffree(inf->environ);
			inf->environ = NULL;
		ft_bfree((void **)inf->cmd_buf);
		if (inf->readfd > 2)
			close(inf->readfd);
		_putchar(BUF_FLUSH);
	}
}
