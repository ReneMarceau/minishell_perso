/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:20:01 by rene              #+#    #+#             */
/*   Updated: 2023/10/25 12:34:36 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"
#include "../../includes/error.h"
#include "../../includes/parsing.h"

extern int g_exit_code;

static bool    exec_heredoc(char *delimiter) {
    int     fd;
    char    *line;

    fd = open(HEREDOC_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (print_error(ERR_OPEN, HEREDOC_FILE), false);
    while (true) {
        line = readline("> ");
        line = substitute_env_vars(line);
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) {
            free(line);
            break ;
        }
        ft_putendl_fd(line, fd);
        free(line);
    }
    close(fd);
    return (true);
}

static bool    handle_input_redir(t_shell *shell, t_rdir *rdir) {
    if (rdir->type == HEREDOC) {
        if (!exec_heredoc(rdir->value))
            return (false);
        shell->input_fd = open(HEREDOC_FILE, O_RDONLY);
        if (shell->input_fd == -1)
            return (print_error(ERR_OPEN, HEREDOC_FILE), false);
        if (dup2(shell->input_fd, STDIN_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->input_fd) == -1)
            return (print_error(ERR_CLOSE, HEREDOC_FILE), false);
        if (unlink(HEREDOC_FILE) == -1)
            return (print_error(ERR_UNLINK, HEREDOC_FILE), false);
    }
    else {
        shell->input_fd = open(rdir->value, O_RDONLY);
        if (shell->input_fd == -1)
            return (print_error(ERR_OPEN, rdir->value), false);
        if (dup2(shell->input_fd, STDIN_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->input_fd) == -1)
            return (print_error(ERR_CLOSE, rdir->value), false);
    }
    return (true);
}

static bool    handle_output_redir(t_shell *shell, t_rdir *rdir) {
    if (rdir->type == OUTPUT) {
        shell->output_fd = open(rdir->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (shell->output_fd == -1)
            return (print_error(ERR_OPEN, rdir->value), false);
        if (dup2(shell->output_fd, STDOUT_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->output_fd) == -1)
            return (print_error(ERR_CLOSE, rdir->value), false);
    }
    else if (rdir->type == APPEND) {
        shell->output_fd = open(rdir->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (shell->output_fd == -1)
            return (print_error(ERR_OPEN, rdir->value), false);
        if (dup2(shell->output_fd, STDOUT_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->output_fd) == -1)
            return (print_error(ERR_CLOSE, rdir->value), false);
    }
    return (true);
}

bool    handle_pipe_redir(t_shell *shell, t_cmd *cmd) {
    if (cmd->index > 0 && !has_redirection(cmd->rdir, INPUT)) {
        if (dup2(shell->pipes_fd[cmd->index - 1][READ_END], STDIN_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        //shell->input_fd = shell->pipes_fd[cmd->index - 1][READ_END];
    }
    if (cmd->next && !has_redirection(cmd->rdir, OUTPUT) && !has_redirection(cmd->rdir, APPEND)) {
        if (dup2(shell->pipes_fd[cmd->index][WRITE_END], STDOUT_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
    }
    return (true);
}

bool    handle_redirections(t_shell *shell, t_rdir *rdir) {
    if (rdir->type == INPUT || rdir->type == HEREDOC) {
        if (!handle_input_redir(shell, rdir))
            return (false);
    }
    // else {
    //     if (dup2(shell->input_fd, STDIN_FILENO) == -1)
    //         return (print_error(ERR_DUP2, NULL), false);
    // }
    if (rdir->type == OUTPUT || rdir->type == APPEND) {
        if (!handle_output_redir(shell, rdir))
            return (false);
    }
    else {
        if (dup2(shell->output_fd, STDOUT_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
    }
    close_pipes(shell);
    return (true);
}