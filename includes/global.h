/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 16:26:05 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/18 16:27:26 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H
# define GLOBAL_H

/* *************** ***************           *************** *************** */
/*                                  INCLUDES                                 */
/* *************** ***************           *************** *************** */
/* Standard Input/Output functions, used for reading/writing to the terminal */
// printf, strerror, perror
# include <stdio.h>

/* Defines the bool type and constants */
// ----> true, false
# include <stdbool.h>

/* POSIX operating system API, provides access to various
   low-level operations like process management and file handling */
// ----> write, read, close, pipe, fork, dup, dup2, access, execve,
// ----> getcwd, chdir, unlink, isatty, ttyname, ttyslot
# include <unistd.h>

/* Various data types used in system calls */
# include <sys/types.h>

/* Functions and constants related to process control and wait operations */
// ----> wait, wait3, wait4, waitpid
# include <sys/wait.h>

/* Signal handling, used for managing process signals */
// ----> signal, sigaction, sigemptyset, sigaddset, kill
# include <signal.h>

/* File control options and functions for file I/O. */
// ----> open
# include <fcntl.h>

/* Data types, structures and functions related to the file system */
// ----> stat, lstat, fstat
# include <sys/stat.h>

/* I/O control operations, often used for terminal-related functionality */
// ----> ioctl
# include <sys/ioctl.h>

/* Functions and data types for terminal I/O, allowing control over terminal settings */
// ----> tcsetattr, tcgetattr
# include <termios.h>

/* Functions for Terminal-based User Interface */
// ---->  tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <curses.h>
# include <term.h>

/* Readline library for command line input handling */
// ----> readline, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/readline.h>

/* Functionality to manage command history when using the Readline library */
// ----> add_history, rl_clear_history
# include <readline/history.h>

/* Libft library for custom functions */
# include "../libs/libft/inc/libft.h"

/* *************** ***************           *************** *************** */
/*                                  Macros                                   */
/* *************** ***************           *************** *************** */
# define MAX_INPUT 1024
# define MAX_ARGS 100
# define HERE_DOC_FILE "/tmp/.heredoc"

# define READ_END 0
# define WRITE_END 1

/* *************** ***************           *************** *************** */
/*                                  Structures                               */
/* *************** ***************           *************** *************** */

int g_exit_code;

typedef struct s_cmd {
    char **args;
    char *input_file;
    char *output_file;
    char *append_file;
    char *heredoc_delimiter;
    struct s_cmd *next;
}   t_cmd;

typedef struct s_shell {
    t_cmd *cmds;
    size_t nb_cmds;
    pid_t *pids;
    int *pipes_fd[2];
    int input_fd;
    int output_fd;
    char **env;
}   t_shell;

/* *************** ***************           *************** *************** */
/*                                  Prototypes                               */
/* *************** ***************           *************** *************** */
t_shell *init_data(char **env);

#endif
