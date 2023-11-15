#include "shell.h"

/**
 *	ft_eputs - prints an input string
 *	@str: the string to be printed
 *	Return: Nothing
 */
void ft_eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		ft_eputchar(str[i]);
		i++;
	}
}

/**
 * ft_eputchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: integer
 */
int ft_eputchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * ft_putfd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 * Return: integer
 */
int ft_putfd(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 *ft_putsfd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int ft_putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += ft_putfd(*str++, fd);
	}
	return (i);
}
