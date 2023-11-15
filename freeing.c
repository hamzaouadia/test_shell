#include "shell.h"

/**
 * ft_bfree - frees a pointer and NULLs the address
 * @pointer: address of the pointer to free
 *
 * Return: integer.
 */
int ft_bfree(void **pointer)
{
	if (pointer && *pointer)
	{
		free(*pointer);
		*pointer = NULL;
		return (1);
	}
	return (0);
}
