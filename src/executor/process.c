/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:57:49 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/17 16:07:53 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"

extern int g_exit_code;

void wait_all(pid_t *pids, size_t nb_cmds) {
    size_t i;

    i = 0;
    while (i < nb_cmds) {
        waitpid(pids[i], &g_exit_code, 0);
        i++;
    }
}