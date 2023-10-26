/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:59:25 by rene              #+#    #+#             */
/*   Updated: 2023/10/25 12:44:01 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"
#include "../../includes/executor.h"
#include "../../includes/error.h"
#include "../../includes/parsing.h"

extern int g_exit_code;

char    *get_cmd_fullpath(char *cmd_name, char *path) {
    char *cmd_path;
    char *cmd_file;

    cmd_file = ft_strjoin("/", cmd_name);
    if (!cmd_file)
        return (print_error(ERR_MALLOC, NULL), NULL);
    cmd_path = ft_strjoin(path, cmd_file);
    if (!cmd_path)
        return (print_error(ERR_MALLOC, NULL), NULL);
    free(cmd_file);
    return (cmd_path);
}

bool    exec_cmd(t_shell *shell, t_cmd *cmd) {
    char *cmd_path;
    char **envp;
    int i;

    if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.') {
        if (access(cmd->args[0], F_OK | X_OK) != -1)
            execve(cmd->args[0], cmd->args, shell->env);
        return (print_error(ERR_CMD_NF, cmd->args[0]), false);
    }
    envp = get_envp(shell->env);
    if (!envp)
        return (false);
    i = -1;
    while (envp[++i]) {
        cmd_path = get_cmd_fullpath(cmd->args[0], envp[i]);
        if (!cmd_path)
            return (false);
        if (access(cmd_path, F_OK | X_OK) != -1)
            execve(cmd_path, cmd->args, shell->env);
    }
    return (print_error(ERR_CMD_NF, cmd->args[0]), false);
}

bool    executor(t_shell *shell) {
    t_cmd *cmd;
    size_t i;

    cmd = shell->cmds;
    if (!init_pipes(shell))
        return (false);
    if (!init_processes(shell))
        return (false);
    i = 0;
    while (i < shell->nb_cmds) {
        if (cmd->pid == 0) {
            while (cmd->rdir) {
                if (!handle_redirections(shell, cmd->rdir))
                    exit_program(shell, g_exit_code);
                cmd->rdir = cmd->rdir->next;
            }
            if (!handle_pipe_redir(shell, cmd))
                return (false);
            if (!exec_cmd(shell, cmd))
                exit_program(shell, g_exit_code);
        }
        i++;
        cmd = cmd->next;
    }
    close_pipes(shell);
    wait_all(shell->pids, shell->nb_cmds);
    printf("All commands executed\n");
    return (true);
}