/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:57:49 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/26 14:51:52 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"
#include "../../includes/error.h"

extern int g_exit_code;

bool init_pipes(t_shell *shell) {
    t_cmd *cmd;
    size_t i;

    cmd = shell->cmds;
    i = 0;
    if (cmd->next != NULL) {
        shell->pipes_fd = (int **)ft_calloc(sizeof(int *), (shell->nb_cmds - 1));
        if (!shell->pipes_fd)
            return (print_error(ERR_MALLOC, NULL), false);
        while (i < shell->nb_cmds - 1) {
            shell->pipes_fd[i] = (int *)ft_calloc(sizeof(int), 2);
            if (!shell->pipes_fd[i])
                return (print_error(ERR_MALLOC, NULL), false);
            if (pipe(shell->pipes_fd[i]) == -1)
                return (print_error(ERR_PIPE, NULL), false);
            cmd = cmd->next;
            i++;
        }
    }
    return (true);
}

bool init_processes(t_shell *shell) {
    t_cmd *cmd;
    size_t i;

    cmd = shell->cmds;
    i = 0;
    shell->pids = (pid_t *)ft_calloc(sizeof(pid_t), shell->nb_cmds);
    if (!shell->pids)
        return (print_error(ERR_MALLOC, NULL), false);
    while (i < shell->nb_cmds) {
        shell->pids[i] = fork();
        if (shell->pids[i] == -1)
            return (print_error(ERR_FORK, NULL), false);
        else if (shell->pids[i] == 0) {
            cmd->pid = shell->pids[i];
            return (true);
        }
        cmd->pid = shell->pids[i];
        cmd = cmd->next;
        i++;
    }
    return (true);
}

void wait_all(pid_t *pids, size_t nb_cmds) {
    size_t i;

    i = 0;
    while (i < nb_cmds) {
        waitpid(pids[i], &g_exit_code, 0);
        i++;
    }
}