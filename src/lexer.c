/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:56:16 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 15:25:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

int		ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

static int	handle_quotes(char c, bool *in_quote, char *quote_char)
{
	if (!*in_quote && (c == '\'' || c == '\"'))
	{
		*in_quote = true;
		*quote_char = c;
		return (1);
	}
	else if (*in_quote && c == *quote_char)
	{
		*in_quote = false;
		*quote_char = '\0';
		return (1);
	}
	return (0);
}

static int	add_token(char **tokens, int *count, char *start, char *end)
{
	if (*count >= MAX_TOKENS - 1)
		return (-1);
	tokens[*count] = ft_strndup(start, end - start);
	if (!tokens[*count])
		return (-1);
	(*count)++;
	return (0);
}

static int	process_special_char(char **tokens, int *count, char **input)
{
	if (is_special_char(**input))
	{
		if (add_token(tokens, count, *input, *input + 1) < 0)
			return (-1);
		(*input)++;
		return (1);
	}
	return (0);
}

char	**lexer(char *input)
{
	char		**tokens;
	t_lexer		lx;

	lx = (t_lexer){0};
	tokens = ft_calloc(MAX_TOKENS, sizeof(char *));
	if (!tokens || !input)
		return (free(tokens), NULL);
	lx.start = input;
	while (*input && lx.token_count < MAX_TOKENS - 1)
	{
		if (handle_quotes(*input, &lx.in_quote, &lx.quote_char))
			continue ;
		if (!lx.in_quote && (ft_isspace(*input) || is_special_char(*input)))
		{
			if (lx.start != input && add_token(tokens, &lx.token_count, 
				lx.start, input) < 0)
				return (free_tokens(tokens), NULL);
			if (process_special_char(tokens, &lx.token_count, &input) < 0)
				return (free_tokens(tokens), NULL);
			lx.start = input + 1;
		}
		if (*input) input++;
	}
	if (lx.start != input && add_token(tokens, &lx.token_count, lx.start, input) < 0)
		return (free_tokens(tokens), NULL);
	return (tokens);
}


// char	**lexer(char *input)
// {
// 	char **tokens;

// 	int token_count = 0;
// 	char *start = input;
// 	bool in_quote = false;
// 	char quote_char = '\0';
//     tokens = malloc(MAX_TOKENS * sizeof(char *));
// 	if (!tokens)
// 		return (NULL);
// 	while (*input)
// 	{
// 		if (*input == '\'' || *input == '\"')
// 		{
// 			if (!in_quote)
// 			{
// 				in_quote = true;
// 				quote_char = *input;
//             }
// 			else if (*input == quote_char)
// 				in_quote = false;
// 		}
// 		if (!in_quote && (isspace(*input) || is_special_char(*input)))
// 		{
// 			if (start != input)
// 			{
// 				tokens[token_count] = strndup(start, input - start);
// 				token_count++;
// 			}
// 			if (is_special_char(*input))
// 			{
// 				tokens[token_count] = strndup(input, 1);
// 				token_count++;
// 			}
// 			start = input + 1;
// 		}
// 		input++;
// 	}
// 	if (start != input)
// 	{
// 		tokens[token_count] = strndup(start, input - start);
// 		token_count++;
// 	}
// 	tokens[token_count] = NULL; // Null-terminate the array
// 	return (tokens);
// }
