/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:44:19 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/18 16:13:57 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/global.h"
#include "../includes/error.h"

int main(int argc, char **argv, char **env) {
	t_shell *shell;
    char	*input;
	(void)argc;
	(void)argv;

	shell = init_data(env);
	while (1) {
		input = readline("minishell> ");
		if (input == NULL)
			break;
		if (input && *input) {
			add_history(input);
		}
		free(input);
	}
    return (EXIT_SUCCESS);
}
