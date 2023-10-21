/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rene <rene@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 14:06:25 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/21 18:35:45 by rene             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "global.h"

/* *************** ***************           *************** *************** */
/*                                  Prototypes                               */
/* *************** ***************           *************** *************** */
bool    init_pipes(t_shell *shell);
bool    init_processes(t_shell *shell);
void    wait_all(pid_t *pids, size_t nb_cmds);
bool    handle_redirections(t_shell *shell, t_cmd *cmd);
bool    executor(t_shell *shell);
#endif