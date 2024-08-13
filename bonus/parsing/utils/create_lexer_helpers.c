/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lexer_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboujama <mboujama@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:41:58 by mboujama          #+#    #+#             */
/*   Updated: 2024/08/03 10:50:21 by mboujama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_status	_status(t_lex_helper lex)
{
	if (lex.in_d_quote)
		return (IN_D_QUOTE);
	else if (lex.in_s_quote)
		return (IN_S_QUOTE);
	else
		return (GENERAL);
}

void	lex_red_in(t_lex_helper *lex, char **line)
{
	if (*(*line + 1) == '<')
	{
		lex->lex = lex_create(ft_strdup("<<"), HEREDOC, 2, _status(*lex));
		if (!lex->lex)
			return ;
		lex_add_back(&lex->lexer, lex->lex);
		(*line)++;
	}
	else
	{
		lex->lex = lex_create(char_to_str('<'), REDIR_IN, 1, _status(*lex));
		if (!lex->lex)
			return ;
		lex_add_back(&lex->lexer, lex->lex);
	}
}

void	lex_red_out(t_lex_helper *lex, char **line)
{
	if (*(*line + 1) == '>')
	{
		lex->lex = lex_create(ft_strdup(">>"), APPEND, 2, _status(*lex));
		if (!lex->lex)
			return ;
		lex_add_back(&lex->lexer, lex->lex);
		(*line)++;
	}
	else
	{
		lex->lex = lex_create(char_to_str('>'), REDIR_OUT, 1, _status(*lex));
		if (!lex->lex)
			return ;
		lex_add_back(&lex->lexer, lex->lex);
	}
}

// e_h ==> env_helper
void	lex_env(t_lex_helper *lex, char **line)
{
	t_lex_env	e_h;

	ft_bzero((void *) &e_h, sizeof(t_lex_env));
	e_h.ch = char_to_str(**line);
	e_h.str = ft_strjoin(e_h.str, e_h.ch);
	if (!e_h.str)
		return ;
	(*line)++;
	ft_free((void **) &e_h.ch);
	if (!ft_strncmp(*line, "?", 1))
	{
		ft_free((void **) &e_h.str);
		e_h.str = ft_strdup("$?");
		if (!e_h.str)
			return ;
	}
	else
		lex_env_helper(&e_h, line);
	if (lex->in_s_quote)
		lex->lex = lex_create(e_h.str, WORD, ft_strlen(e_h.str), _status(*lex));
	else
		lex->lex = lex_create(e_h.str, ENV, ft_strlen(e_h.str), _status(*lex));
	lex_add_back(&lex->lexer, lex->lex);
	ft_free((void **) &e_h.ch);
}

void	lex_word(t_lex_helper *lex, char **line)
{
	char	*str;
	char	*ch;

	str = ft_strdup("");
	if (!str)
		return ;
	while (**line && !ft_isspace(**line) && **line != '|' && **line != '$'
		&& !(**line == QUOTE || **line == DOUBLE_QUOTE) && **line != REDIR_IN 
		&& **line != REDIR_OUT)
	{
		ch = char_to_str(**line);
		if (!ch)
			return ;
		str = ft_strjoin(str, ch);
		if (!str)
			return ;
		ft_free((void **) &ch);
		(*line)++;
	}
	(*line)--;
	lex->lex = lex_create(str, WORD, ft_strlen(str), _status(*lex));
	lex_add_back(&lex->lexer, lex->lex);
}
