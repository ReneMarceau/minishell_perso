/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rene <rene@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 22:21:04 by rene              #+#    #+#             */
/*   Updated: 2023/10/19 01:18:38 by rene             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "global.h"

void display_cmd_table(t_cmd *cmd_table);
t_cmd   *parsing_test(char *input, char *status);

#endif