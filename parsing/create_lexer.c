/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochouati <ochouati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:33:09 by mboujama          #+#    #+#             */
/*   Updated: 2024/08/02 18:58:00 by ochouati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	other_cases_2(t_lex_helper *lex, char **line)
{
	if (ft_isspace(**line))
	{
		lex->lex = lex_create(char_to_str(**line),
				W_SPACE, 1, _status(*lex));
		if (!lex->lex)
			return ;
		lex_add_back(&lex->lexer, lex->lex);
	}
	else if (**line == '<')
		lex_red_in(lex, line);
	else if (**line == '>')
		lex_red_out(lex, line);
	else if (**line == '$')
		lex_env(lex, line);
	else
		lex_word(lex, line);
}

static void	other_cases(t_lex_helper *lex, char **line)
{
	if (**line == '\"')
	{
		if (lex->in_s_quote)
			lex->lex = lex_create(char_to_str('\"'),
					DOUBLE_QUOTE, 1, IN_D_QUOTE);
		else
		{
			lex->lex = lex_create(char_to_str('\"'), DOUBLE_QUOTE, 1, GENERAL);
			lex->in_d_quote = !lex->in_d_quote;
		}
		lex_add_back(&lex->lexer, lex->lex);
	}
	else if (**line == '|')
	{
		lex->lex = lex_create(char_to_str('|'), PIPELINE, 1, _status(*lex));
		if (!lex->lex)
			return ;
		lex_add_back(&lex->lexer, lex->lex);
	}
	else
		other_cases_2(lex, line);
}

t_lex	*create_lexer(char *line)
{
	t_lex_helper	lex;

	ft_bzero(&lex, sizeof(t_lex_helper));
	lex.lexer = NULL;
	while (*line)
	{
		if (*line == '\'')
		{
			if (lex.in_d_quote)
				lex.lex = lex_create(char_to_str('\''), QUOTE, 1, IN_D_QUOTE);
			else
			{
				lex.lex = lex_create(char_to_str('\''), QUOTE, 1, GENERAL);
				lex.in_s_quote = !lex.in_s_quote;
			}
			lex_add_back(&lex.lexer, lex.lex);
		}
		else
			other_cases(&lex, &line);
		line++;
	}
	lex_add_back(&lex.lexer, lex_create(ft_strdup(" "), W_SPACE, 1, END));
	return (lex.lexer);
}
