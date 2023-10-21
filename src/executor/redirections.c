/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rene <rene@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:20:01 by rene              #+#    #+#             */
/*   Updated: 2023/10/21 17:54:12 by rene             ###   ########.fr       */
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

static bool    handle_input_redir(t_shell *shell, t_cmd *cmd) {
    if (cmd->heredoc_delimiter) {
        if (!exec_heredoc(cmd->heredoc_delimiter))
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
        shell->input_fd = open(cmd->input_file, O_RDONLY);
        if (shell->input_fd == -1)
            return (print_error(ERR_OPEN, cmd->input_file), false);
        if (dup2(shell->input_fd, STDIN_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->input_fd) == -1)
            return (print_error(ERR_CLOSE, cmd->input_file), false);
    }
    return (true);
}

static bool    handle_output_redir(t_shell *shell, t_cmd *cmd) {
    if (cmd->output_file) {
        shell->output_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (shell->output_fd == -1)
            return (print_error(ERR_OPEN, cmd->output_file), false);
        if (dup2(shell->output_fd, STDOUT_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->output_fd) == -1)
            return (print_error(ERR_CLOSE, cmd->output_file), false);
    }
    else if (cmd->append_file) {
        shell->output_fd = open(cmd->append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (shell->output_fd == -1)
            return (print_error(ERR_OPEN, cmd->append_file), false);
        if (dup2(shell->output_fd, STDOUT_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->output_fd) == -1)
            return (print_error(ERR_CLOSE, cmd->append_file), false);
    }
    return (true);
}

static bool    handle_pipe_redir(t_shell *shell, t_cmd *cmd) {
    if (cmd->index > 0) {
        if (dup2(shell->pipes_fd[cmd->index - 1][READ_END], STDIN_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        //shell->input_fd = shell->pipes_fd[cmd->index - 1][READ_END];
        if (close(shell->pipes_fd[cmd->index - 1][READ_END]) == -1)
            return (print_error(ERR_CLOSE, NULL), false);
    }
    if (cmd->next && !cmd->output_file && !cmd->append_file) {
        if (dup2(shell->pipes_fd[cmd->index][WRITE_END], STDOUT_FILENO) == -1)
            return (print_error(ERR_DUP2, NULL), false);
        if (close(shell->pipes_fd[cmd->index][WRITE_END]) == -1)
            return (print_error(ERR_CLOSE, NULL), false);
    }
    return (true);
}

bool    handle_redirections(t_shell *shell, t_cmd *cmd) {
    if (cmd->input_file) {
        if (!handle_input_redir(shell, cmd))
            return (false);
    }
    // else {
    //     if (dup2(shell->input_fd, STDIN_FILENO) == -1)
    //         return (print_error(ERR_DUP2, NULL), false);
    // }
    if (cmd->output_file || cmd->append_file) {
        if (!handle_output_redir(shell, cmd))
            return (false);
    }
    // else {
    //     if (dup2(shell->output_fd, STDOUT_FILENO) == -1)
    //         return (print_error(ERR_DUP2, NULL), false);
    // }
    if (!handle_pipe_redir(shell, cmd))
        return (false);
    return (true);
}