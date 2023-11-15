#include "shell.h"

/**
 * ft_myenv - prints the current environment
 * @inf: Structure containing potential arguments.
 * Return: Always 0
 */
int ft_myenv(inf_t *inf)
{
	ft_ft_print_list_str(inf->env);
	return (0);
}

/**
 * ft_getenv - gets the value of an environ variable
 * @inf: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *ft_getenv(inf_t *inf, const char *name)
{
	list_t *node = inf->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * ft__mysetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @inf: Structure containing potential arguments.
 *  Return: Always 0
 */
int ft__mysetenv(inf_t *inf)
{
	if (inf->argc != 3)
	{
		ft_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (ft_setenv(inf, inf->argv[1], inf->argv[2]))
		return (0);
	return (1);
}

/**
 * ft_myunsetenv - Remove an environment variable
 * @inf: Structure containing potential arguments.
 *  Return: Always 0
 */
int ft_myunsetenv(inf_t *inf)
{
	int i;

	if (inf->argc == 1)
	{
		ft_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= inf->argc; i++)
		ft_unsetenv(inf, inf->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @inf: Structure containing potential arguments.
 * Return: Always 0
 */
int populate_env_list(inf_t *inf)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		ft_add_node_end(&node, environ[i], 0);
	inf->env = node;
	return (0);
}
