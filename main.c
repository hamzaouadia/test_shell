#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	inf_t inf[] = { inf_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				ft_eputs(av[0]);
				ft_eputs(": 0: Can't open ");
				ft_eputs(av[1]);
				ft_eputchar('\n');
				ft_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		inf->readfd = fd;
	}
	populate_env_list(inf);
	ft_read_history(inf);
	ft_hsh(inf, av);
	return (EXIT_SUCCESS);
}
