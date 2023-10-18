/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 13:48:52 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/18 16:07:44 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

/* *************** ***************           *************** *************** */
/*                                  INCLUDES                                 */
/* *************** ***************           *************** *************** */
/* Standard Library functions, provides memory allocation and other functions */
// ----> malloc, free, exit, getenv
# include <stdlib.h>

/* *************** ***************           *************** *************** */
/*                                  Macros                                   */
/* *************** ***************           *************** *************** */
// Minishell Tag
# define TAG		        "minishell: "

// Memory Allocation Errors
# define ERR_MALLOC		    "Out of memory"

// Command Execution Errors
# define ERR_CMD_NF         "command not found"
# define ERR_PERM_DENIED    "Permission denied"
# define ERR_CMD_NOT_EXEC   "command not executable"
# define ERR_TOO_MANY_ARGS  "Argument list too long"
# define ERR_INVALID_OPT    "Invalid option"

// Variable and String Errors
#define ERR_VAR_NOT_SET     "Variable not set"
#define ERR_UNTERM_STR      "unterminated quoted string"

// Process and System Errors
#define ERR_JOB_CONTROL     "Job control not active"
#define ERR_BAD_FD          "Bad file descriptor"
#define ERR_CMD_INT         "Command interrupted"
#define ERR_NUM_ARG_REQ     "Numeric argument required"

// File Handling Errors
# define ERR_NO_SUCH_FD     "No such file or directory"
# define ERR_ISDIR          "Is a directory"
# define ERR_NODIR		    "Not a directory"
# define ERR_PERM		    "Permission denied"
# define ERR_CLOSE		    "closing file failed"
# define ERR_OPEN		    "opening file failed"
# define ERR_VAR            "Variable not set"

// Syntax and Parsing Errors
# define ERR_SYNTAX		    "syntax error near unexpected token"
# define ERR_AMBREDIR	    "ambiguous redirect"

/* *************** ***************           *************** *************** */
/*                                  Prototypes                               */
/* *************** ***************           *************** *************** */
void    print_error(char *error_message, char *arg);

#endif