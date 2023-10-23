/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 17:11:12 by rene              #+#    #+#             */
/*   Updated: 2023/10/23 14:31:22 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"
#include "../../includes/error.h"

bool    close_pipes(t_shell *shell) {
    size_t i;

    i = 0;
    while (i < shell->nb_cmds - 1) {
        if (close(shell->pipes_fd[i][READ_END]) == -1)
            return (print_error(ERR_CLOSE, NULL), false);
        if (close(shell->pipes_fd[i][WRITE_END]) == -1)
            return (print_error(ERR_CLOSE, NULL), false);
        i++;
    }
    return (true);
}

static void    free_cmds(t_cmd *cmds) {
    t_cmd *tmp;

    while (cmds) {
        tmp = cmds->next;
        free_2d_array(cmds->args);
        free(cmds->input_file);
        free(cmds->output_file);
        free(cmds->append_file);
        free(cmds->heredoc_delimiter);
        free(cmds);
        cmds = tmp;
    }
}

static void free_pipes(t_shell *shell) {
    size_t i;

    i = 0;
    while (i < shell->nb_cmds - 1) {
        free(shell->pipes_fd[i]);
        i++;
    }
    free(shell->pipes_fd);
}

void    exit_program(t_shell *shell ,int exit_code) {
    close(shell->input_fd);
    close(shell->output_fd);
    close_pipes(shell);
    if (shell->cmds)
        free_cmds(shell->cmds);
    if (shell->pipes_fd)
        free_pipes(shell);
    free(shell);
    exit(exit_code);
}