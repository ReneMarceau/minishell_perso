/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rene <rene@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:26:19 by rene              #+#    #+#             */
/*   Updated: 2023/10/21 17:14:36 by rene             ###   ########.fr       */
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

void free_2d_array(char **array) {
    size_t i;

    i = 0;
    while (array[i]) {
        free(array[i]);
        i++;
    }
    free(array);
}





