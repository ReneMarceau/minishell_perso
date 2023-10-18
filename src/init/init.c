/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 11:58:31 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/18 16:04:00 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"
#include "../../includes/error.h"

t_shell *init_data(char **env) {
    t_shell *shell;

    shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
    if (!shell) {
        print_error(ERR_MALLOC, TAG);
        exit(EXIT_FAILURE);
    }
    shell->nb_cmds = 0;
    shell->cmds = NULL;
    shell->pids = NULL;
    shell->input_fd = STDIN_FILENO;
    shell->output_fd = STDOUT_FILENO;
    shell->env = env;
    return (shell);
}