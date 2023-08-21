#include "shell.h"

/**
 * _displayhistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _displayhistory(info_t *info)
{
	my_print_list(info->history);
	return (0);
}

/**
 * unset_alias_string - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias_string(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = find_char_in_string(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = rm_node_at_index(&(info->alias),
						   get_node_at_index(info->alias, node_starts_with_prefix(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias_string - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias_string(info_t *info, char *str)
{
	char *p;

	p = find_char_in_string(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias_string(info, str));

	unset_alias_string(info, str);
	return (add_node_end_list(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias_string - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias_string(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = find_char_in_string(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		prints_input_string(p + 1);
		prints_input_string("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias_builtin - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myalias_builtin(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias_string(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = find_char_in_string(info->argv[i], '=');
		if (p)
			set_alias_string(info, info->argv[i]);
		else
			print_alias_string(node_starts_with_prefix(info->alias, info->argv[i], '='));
	}

	return (0);
}

