/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 14:58:38 by rmarceau          #+#    #+#             */
/*   Updated: 2023/10/18 16:08:48 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"
#include "../../includes/error.h"
#include "../../includes/colors.h"

void    print_error(char *error_message, char *arg) {
    ft_putstr_fd(RED, STDERR_FILENO);
    ft_putstr_fd(TAG, STDERR_FILENO);
    if (arg) {
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
    }
    ft_putstr_fd(error_message, STDERR_FILENO);
    ft_putendl_fd(RESET, STDERR_FILENO);
}