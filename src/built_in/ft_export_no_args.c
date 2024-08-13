/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_no_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 14:25:09 by mboujama          #+#    #+#             */
/*   Updated: 2024/07/27 15:28:07 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	__ft_swap(char **a, char **b)
{
	char	*c;

	c = NULL;
	c = *a;
	*a = *b;
	*b = c;
}

static t_env	*sort_env(t_env *env)
{
	t_env	*top;
	t_env	*tmp;

	top = env;
	tmp = env;
	env = (env)->next;
	while (env)
	{
		while (tmp)
		{
			if (ft_strcmp(tmp->key, env->key) > 0)
			{
				__ft_swap(&(tmp)->key, &(env)->key);
				__ft_swap(&(tmp)->value, &(env)->value);
			}
			tmp = tmp->next;
		}
		tmp = top;
		env = (env)->next;
	}
	return (top);
}

static void	___handler(t_env *sorted_env, bool a)
{
	if (a)
	{
		while (sorted_env)
		{
			if (sorted_env->value)
				ft_printf("declare -x %s=\"%s\"\n",
					sorted_env->key, sorted_env->value);
			sorted_env = sorted_env->next;
		}
	}
	else
	{
		while (sorted_env)
		{
			if (!sorted_env->value)
				ft_printf("declare -x %s\n", sorted_env->key);
			sorted_env = sorted_env->next;
		}
	}
}

int	ft_export_no_args_tmp(t_env *env)
{
	t_env	*sorted_env;
	t_env	*new_env;
	char	**envp;

	if (!env)
		return (0);
	envp = env__to_2dchar(env);
	if (!envp)
		return (0);
	new_env = dup_env(envp);
	if (!new_env)
		return (0);
	ft_free_strs(envp);
	sorted_env = sort_env(new_env);
	if (!sorted_env)
		return (0);
	___handler(sorted_env, 1);
	___handler(sorted_env, 0);
	ls_clear_env(&new_env);
	return (1);
}

int	ft_export_no_args(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	if (!tmp)
		return (0);
	while (tmp)
	{
		if (tmp->value)
			ft_printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else if (tmp->key && !tmp->value)
			ft_printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
	return (1);
}
