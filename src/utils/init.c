/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:59 by rene              #+#    #+#             */
/*   Updated: 2023/10/25 11:13:51 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"
#include "../../includes/error.h"

t_shell *init_data(char **env) {
    t_shell *shell;

    shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
    if (!shell) {
        print_error(ERR_MALLOC, NULL);
        exit(EXIT_FAILURE);
    }
    shell->nb_cmds = 0;
    shell->cmds = NULL;
    shell->pids = NULL;
    shell->input_fd = STDIN_FILENO;
    shell->output_fd = STDOUT_FILENO;
    shell->env = env;
    return (shell);
}

size_t  count_cmds(t_cmd *cmds) {
    size_t i;

    i = 0;
    while (cmds) {
        i++;
        cmds = cmds->next;
    }
    return (i);
}

char	**get_envp(char **env) {
	char	**enviroment;
	char	*path_trimmed;
	char	*path;
	int		i;

	enviroment = NULL;
	path_trimmed = NULL;
	path = NULL;
	i = -1;
	while (env[++i])
	{
		path = ft_strnstr(env[i], "PATH=", 5);
		if (path)
		{
			path_trimmed = ft_strtrim(env[i], "PATH=");
			enviroment = ft_split(path_trimmed, ':');
			break ;
		}
		free(path);
	}
	free(path_trimmed);
	return (enviroment);
}

bool	has_redirection(t_rdir *rdir, int type) {
	while (rdir) {
		if (rdir->type == type)
			return (true);
		rdir = rdir->next;
	}
	return (false);
}