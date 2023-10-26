/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 16:44:19 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/25 10:57:35 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/global.h"
#include "../includes/error.h"
#include "../includes/colors.h"
#include "../includes/parsing.h"
#include "../includes/executor.h"

int g_exit_code = 0;

bool   readlines(char **input, char **last_input) {
    *input = readline(READLINE_MSG);
    if (*input == NULL)
        return (print_error(ERR_MALLOC, NULL), false);
    if (*last_input == NULL) {
        *last_input = ft_strdup("");
		if (*last_input == NULL)
			return (print_error(ERR_MALLOC, NULL), false);
	}
    if (*input && **input && ft_strncmp(*input, *last_input, ft_strlen(*input)))
        add_history(*input);
    *last_input = ft_strdup(*input);
	if (*last_input == NULL)
		return (print_error(ERR_MALLOC, NULL), false);
    return (true);
}

void	shell_loop(t_shell *shell) {
	char	*input;
	char	*last_input;
	char	*exit_code;

	input = NULL;
	last_input = NULL;
	while (true) {
		if (!readlines(&input, &last_input))
			break ;
		if (input && *input) {
			exit_code = ft_itoa(WEXITSTATUS(g_exit_code));
			shell->cmds = parsing_test(input, exit_code);
			shell->nb_cmds = count_cmds(shell->cmds);
			display_cmd_table(shell->cmds);
			if (shell->cmds)
				executor(shell);
		}
		free(input);
	}
	free(last_input);
	free(exit_code);
}

int main(int argc, char **argv, char **env) {
	t_shell *shell;
	(void)argc;
	(void)argv;

	shell = init_data(env);
	printf("%s\n", WELCOME_MSG);
	shell_loop(shell);
	exit_program(shell, g_exit_code);
    return (EXIT_SUCCESS);
}
