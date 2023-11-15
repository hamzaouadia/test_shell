#include "shell.h"

/**
 * ft_hsh - main shell loop
 * @inf: the parameter & return inf struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int ft_hsh(inf_t *inf, char **av)
{
	ssize_t ret = 0;
	int builtin_ret = 0;

	while (ret != -1 && builtin_ret != -2)
	{
		ft_clear_inf(inf);
		if (ft_interactive(inf))
			_puts("$ ");
		ft_eputchar(BUF_FLUSH);
		ret = ft_get_input(inf);
		if (ret != -1)
		{
			ft_set_inf(inf, av);
			builtin_ret = ft_find_builtin(inf);
			if (builtin_ret == -1)
				ft_find_cmd(inf);
		}
		else if (ft_interactive(inf))
			_putchar('\n');
		ft_free_inf(inf, 0);
	}
	ft_write_history(inf);
	ft_free_inf(inf, 1);
	if (!ft_interactive(inf) && inf->status)
		exit(inf->status);
	if (builtin_ret == -2)
	{
		if (inf->err_num == -1)
			exit(inf->status);
		exit(inf->err_num);
	}
	return (builtin_ret);
}

/**
 * ft_find_builtin - finds a builtin command
 * @inf: the parameter & return inf struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int ft_find_builtin(inf_t *inf)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", ft_myexit},
		{"env", ft_myenv},
		{"help", ft_myhelp},
		{"history", ft_myhistory},
		{"setenv", ft__mysetenv},
		{"unsetenv", ft_myunsetenv},
		{"cd", ft_mycd},
		{"alias", ft_myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(inf->argv[0], builtintbl[i].type) == 0)
		{
			inf->line_count++;
			built_in_ret = builtintbl[i].func(inf);
			break;
		}
	return (built_in_ret);
}

/**
 * ft_find_cmd - finds a command in PATH
 * @inf: the parameter & return inf struct
 *
 * Return: void
 */
void ft_find_cmd(inf_t *inf)
{
	char *path = NULL;
	int i, k;

	inf->path = inf->argv[0];
	if (inf->line_count_flag == 1)
	{
		inf->line_count++;
		inf->line_count_flag = 0;
	}
	for (i = 0, k = 0; inf->arg[i]; i++)
		if (!ft_is_delim(inf->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = ft_find_path(inf, ft_getenv(inf, "PATH="), inf->argv[0]);
	if (path)
	{
		inf->path = path;
		ft_fork_cmd(inf);
	}
	else
	{
		if ((ft_interactive(inf) || ft_getenv(inf, "PATH=")
			|| inf->argv[0][0] == '/') && ft_is_cmd(inf, inf->argv[0]))
			ft_fork_cmd(inf);
		else if (*(inf->arg) != '\n')
		{
			inf->status = 127;
			print_error(inf, "not found\n");
		}
	}
}

/**
 * ft_fork_cmd - forks a an exec thread to run cmd
 * @inf: the parameter & return inf struct
 *
 * Return: void
 */
void ft_fork_cmd(inf_t *inf)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(inf->path, inf->argv, get_environ(inf)) == -1)
		{
			ft_free_inf(inf, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(inf->status));
		if (WIFEXITED(inf->status))
		{
			inf->status = WEXITSTATUS(inf->status);
			if (inf->status == 126)
				print_error(inf, "Permission denied\n");
		}
	}
}
