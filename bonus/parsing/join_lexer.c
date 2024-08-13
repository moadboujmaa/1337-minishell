/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:04:55 by mboujama          #+#    #+#             */
/*   Updated: 2024/08/05 10:59:19 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_arg(t_lex **lex)
{
	char	*str;
	char	*str2;

	str = ft_strdup("");
	if (!str)
		return (NULL);
	str2 = NULL;
	while ((*lex) && (*lex)->type != W_SPACE && (*lex)->type != PIPELINE
		&& !is_redirection((*lex)))
	{
		if ((*lex)->type == QUOTE || (*lex)->type == DOUBLE_QUOTE)
		{
			str2 = get_str(lex);
			if (!str2)
				return (NULL);
			str = ft_strjoin(str, str2);
			ft_free((void **)&str2);
		}
		else
			str = ft_strjoin(str, (*lex)->string);
		if (!str)
			return (NULL);
		(*lex) = (*lex)->next;
	}
	return (str);
}

static int	handle_env(t_cmd_utils *utils, t_lex **lex)
{
	char	**strs;
	int		i;

	i = 0;
	strs = ft_split((*lex)->string, ' ');
	if (!strs)
		return (0);
	while (strs[i])
	{
		utils->tmp_args = utils->args;
		utils->args = insert_to2d_array(utils->args, strs[i]);
		if (!utils->args)
			return (0);
		ft_free_strs(utils->tmp_args);
		utils->tmp_args = NULL;
		i++;
	}
	ft_free_strs(strs);
	return (1);
}

static int	add_arg(t_cmd_utils *utils, t_lex **lex)
{
	char	*str;

	if ((*lex)->type == ENV && (*lex)->string == NULL)
	{
		(*lex) = (*lex)->next;
		return (1);
	}
	if ((*lex)->type == ENV && (*lex)->string == NULL)
	{
		(*lex) = (*lex)->next;
		return (1);
	}
	if ((*lex)->type == ENV && ft_strchr((*lex)->string, ' '))
	{
		if (!handle_env(utils, lex))
			return (0);
		return (1);
	}
	str = get_arg(lex);
	if (!str)
		return (0);
	if (!add_arg_helper(utils, &str))
		return (0);
	return (1);
}

static t_cmd	*get_cmd(t_lex **lex, t_data *data)
{
	t_cmd_utils	utils;

	ft_bzero(&utils, sizeof(t_cmd_utils));
	while ((*lex) && (*lex)->type != PIPELINE && (*lex)->status == GENERAL)
	{
		if ((*lex)->type == W_SPACE)
		{
			*lex = (*lex)->next;
			continue ;
		}
		else if (is_redirection(*lex))
		{
			if (!fill_redirect(lex, &utils))
				return (NULL);
		}
		else
			if (!add_arg(&utils, lex))
				return (NULL);
		ft_free_strs(utils.tmp_args);
		utils.tmp_args = NULL;
		if ((*lex)->type != PIPELINE && !is_redirection(*lex))
			*lex = (*lex)->next;
	}
	utils.cmd = cmd_create(data, &utils);
	return (utils.cmd);
}

int	join_lexer(t_data *data)
{
	t_cmd	*cmd;
	t_lex	*tmp;

	tmp = data->lexer;
	search_rep_asterisk(data->lexer);
	while (tmp)
	{
		if (tmp->type == ENV && tmp->string == NULL)
			tmp = tmp->next;
		if (tmp->type == W_SPACE && tmp->status == GENERAL)
		{
			tmp = tmp->next;
			continue ;
		}
		cmd = get_cmd(&tmp, data);
		if (!cmd)
			return (0);
		mark_last(cmd->redire);
		red_sort(cmd->redire);
		cmd_addback(&data->command, cmd);
		if (tmp)
			tmp = tmp->next;
	}
	return (1);
}
