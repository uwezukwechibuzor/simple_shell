#include "shell.h"

/**
 * fetch_env - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **fetch_env(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings_(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _un_setenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _un_setenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with_haystack(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = rm_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _set_env - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _set_env(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(strlen_length(var) + strlen_length(value) + 2);
	if (!buf)
		return (1);
	copy_string(buf, var);
	concat_two_strings(buf, "=");
	concat_two_strings(buf, value);
	node = info->env;
	while (node)
	{
		p = starts_with_haystack(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end_list(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}

