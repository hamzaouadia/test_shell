#include "shell.h"

/**
 * ft_myexit - exits the shell
 * @inf: Structure containing potential arguments.
 * Return: exits with a given exit status
 *         (0) if inf.argv[0] != "exit"
 */

int ft_myexit(inf_t *inf)
{
	int exit_check;

	if (inf->argv[1])
	{
		exit_check = ft_erratoi(inf->argv[1]);
		if (exit_check == -1)
		{
			inf->status = 2;
			print_error(inf, "Illegal number: ");
			ft_eputs(inf->argv[1]);
			ft_eputchar('\n');
			return (1);
		}
		inf->err_num = ft_erratoi(inf->argv[1]);
		return (-2);
	}
	inf->err_num = -1;
	return (-2);
}

/**
 * ft_mycd - changes the current directory of the process
 * @inf: Structure containing potential arguments.
 *  Return: Always 0
 */
int ft_mycd(inf_t *inf)
{
	char *s, *dir, buffer[1024];
	int chdirret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!inf->argv[1])
	{
		dir = ft_getenv(inf, "HOME=");
		if (!dir)
			chdirret = /* TODO: what should this be? */
				chdir((dir = ft_getenv(inf, "PWD=")) ? dir : "/");
		else
			chdirret = chdir(dir);
	}
	else if (_strcmp(inf->argv[1], "-") == 0)
	{
		if (!ft_getenv(inf, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(ft_getenv(inf, "OLDPWD=")), _putchar('\n');
		chdirret = /* TODO: what should this be? */
			chdir((dir = ft_getenv(inf, "OLDPWD=")) ? dir : "/");
	}
	else
		chdirret = chdir(inf->argv[1]);
	if (chdirret == -1)
	{
		print_error(inf, "can't cd to ");
		ft_eputs(inf->argv[1]), ft_eputchar('\n');
	}
	else
	{
		ft_setenv(inf, "OLDPWD", ft_getenv(inf, "PWD="));
		ft_setenv(inf, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * ft_myhelp - changes the current directory of the process
 * @inf: Structure containing potential arguments.
 *  Return: Always 0
 */
int ft_myhelp(inf_t *inf)
{
	char **arg_array;

	arg_array = inf->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array);
	return (0);
}
