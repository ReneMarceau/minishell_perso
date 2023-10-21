/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rene <rene@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 22:22:56 by rene              #+#    #+#             */
/*   Updated: 2023/10/20 18:44:08 by rene             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/global.h"

void display_cmd_table(t_cmd *cmd_table) {
    t_cmd *tmp_table = cmd_table;
    int i = 0;
    int j;

    while (tmp_table) {
        j = 0;
        printf("cmd[%d]->index = %d\n", i, tmp_table->index);
        if (tmp_table->args[j] == NULL)
            printf("cmd[%d]->args[%d] = NULL\n", i, j);
        else {
            while (tmp_table->args[j]) {
                printf("cmd[%d]->args[%d] = %s\n", i, j, tmp_table->args[j]);
                j++;
            }
        }
        printf("cmd[%d]->nb_args = %ld\n", i, tmp_table->nb_args);
        printf("cmd[%d]->input_file = %s\n", i, tmp_table->input_file);
        printf("cmd[%d]->output_file = %s\n", i, tmp_table->output_file);
        printf("cmd[%d]->append_file = %s\n", i, tmp_table->append_file);
        printf("cmd[%d]->heredoc = %s\n", i, tmp_table->heredoc_delimiter);
        printf("cmd[%d]->pid = %d\n", i, tmp_table->pid);
        printf("cmd[%d]->next = %p\n", i, tmp_table->next);
        tmp_table = tmp_table->next;
        i++;
        printf("\n");
    }
}

static t_cmd   *create_node(void) {
    t_cmd   *node;

    node = (t_cmd *)malloc(sizeof(t_cmd));
    if (!node)
        return (NULL);
    node->index = -1;
    node->nb_args = 0;
    node->input_file = NULL;
    node->output_file = NULL;
    node->append_file = NULL;
    node->heredoc_delimiter = NULL;
    node->pid = 69;
    node->args = (char **)malloc(sizeof(char *) * (MAX_ARGS + 1));
    if (!node->args)
        return (NULL);
    node->next = NULL;
    return (node);
}

static void add_node(t_cmd **head, t_cmd *newnode) {
    t_cmd   *tmp;

    tmp = *head;
    if (*head) {
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = newnode;
    }
    else
        *head = newnode;
}

char *substitute_env_vars(char* input) {
    char *start = ft_strchr(input, '$');  // Find the first $
    if (!start) {
        return input;
    }
    while (start) {
        char *end = start + 1;
        while (*end && (ft_isalnum(*end) || *end == '_')) {
            end++;
        }

        char var_name[256];
        ft_strlcpy(var_name, start + 1, end - start);
        var_name[end - start - 1] = '\0';

        // printf("Input: %s\n", input);
        // printf("Start: %s\n", start);
        // printf("start - input: %ld\n", start - input);
        // printf("End: %s\n", end);
        // printf("dst size: %ld\n", end - start - 1);
        // printf("Found variable name: %s\n", var_name);


        char *var_value = getenv(var_name);
        if (!var_value) {
            var_value = "";
        }

        // printf("Variable value: %s\n", var_value);

        size_t new_len = strlen(input) + strlen(var_value) - (end - start);
        char *new_input = (char *)malloc(new_len + 1);
        if (!new_input) {
            return NULL;
        }
        ft_strlcpy(new_input, input, (start - input) + 1);
        new_input[start - input] = '\0';
        ft_strlcat(new_input, var_value, new_len + 1);
        ft_strlcat(new_input, end, new_len + 1);

        free(input);
        input = new_input;

        // printf("New length: %ld\n", new_len);
        // printf("Updated input: %s\n", input);

        start = ft_strchr(input, '$');  // Find the next $
    }

    return input;
}

static char *substitute_status(char *input, char *status) {
    char *start = ft_strnstr(input, "$?", ft_strlen(input)); // Find the first occurrence of "$?"
    if (!start) {
        return input;
    }

    while (start) {
        // Calculate the length of the prefix before "$?"
        size_t prefix_len = start - input;
        //printf("Prefix length: %ld\n", prefix_len);

        // Calculate the new length of the result string with space
        size_t new_len = prefix_len + ft_strlen(status) + ft_strlen(start + 2);
        //printf("New length: %ld\n", new_len);

        // Allocate memory for the new input string
        char *new_input = (char *)malloc(new_len + 1);
        if (!new_input) {
            return NULL;
        }

        // Copy the prefix before "$?"
        ft_strlcpy(new_input, input, prefix_len + 1);
        //printf("New input: %s\n", new_input);

        // Append the exit status code with a space
        ft_strlcat(new_input, status, new_len + 1);
        //printf("New input: %s\n", new_input);
        //ft_strlcat(new_input, " ", new_len + 1);

        // Append the part after "$?"
        ft_strlcat(new_input, start + 2, new_len + 1);
        // printf("New input: %s\n", new_input);

        //free(input); // Free the old input string
        input = new_input; // Update the input pointer

        // Find the next occurrence of "$?"
        start = ft_strnstr(input, "$?", ft_strlen(input));
    }

    return input;
}

t_cmd   *parsing_test(char *input, char *status) {
    t_cmd   **cmd_table = NULL;
    t_cmd   *tmp_table = NULL;
    char    **tokens;
    int     cmd_count;
    int     i;

    // Expand environment variables and status code
    input = substitute_status(input, status);
    input = substitute_env_vars(input);

    // Split the input into tokens
    tokens = ft_split(input, ' ');
    if (tokens[0] == NULL)
        return (NULL);

    // Count the number of commands
    i = -1;
    cmd_count = 0;
    while (tokens[++i]) {
        if (!ft_strncmp(tokens[i], "|", ft_strlen(tokens[i]) )) {
            cmd_count++;
        }
    }
    cmd_count++;
    // printf("cmd_count = %d\n", cmd_count);

    // Fill the cmd_table with empty nodes
    i = -1;
    while (++i < cmd_count) {
        if (i == 0) {
            cmd_table = (t_cmd **)malloc(sizeof(t_cmd *));
            if (!cmd_table)
                return (NULL);
            *cmd_table = create_node();
        }
        else
            add_node(cmd_table, create_node());
    }

    // Fill the cmd_table with data
    tmp_table = *cmd_table;
    int cmd_index = 0;
    int arg_index = 0;
    i = 0;
    tmp_table->index = cmd_index;
    while (tokens[i]) {
        if (!strncmp(tokens[i], "|", ft_strlen(tokens[i]))) {
            tmp_table->args[arg_index] = NULL;
            tmp_table = tmp_table->next;
            tmp_table->index = ++cmd_index;
            arg_index = 0;
        }
        else if (!strncmp(tokens[i], "<", ft_strlen(tokens[i]))) {
            i++;
            tmp_table->input_file = ft_strdup(tokens[i]);
            if (tmp_table->input_file == NULL)
                return (NULL);
        }
        else if (!strncmp(tokens[i], "<<", ft_strlen(tokens[i]))) {
            i++;
            tmp_table->input_file = ft_strdup(HEREDOC_FILE);
            if (tmp_table->input_file == NULL)
                return (NULL);
            tmp_table->heredoc_delimiter = ft_strdup(tokens[i]);
            if (tmp_table->heredoc_delimiter == NULL)
                return (NULL);
        }
        else if (!strncmp(tokens[i], ">", ft_strlen(tokens[i]))) {
            i++;
            tmp_table->output_file = ft_strdup(tokens[i]);
            if (tmp_table->output_file == NULL)
                return (NULL);
        }
        else if (!strncmp(tokens[i], ">>", ft_strlen(tokens[i]))) {
            i++;
            tmp_table->append_file = ft_strdup(tokens[i]);
            if (tmp_table->append_file == NULL)
                return (NULL);
        }
        else {
            tmp_table->args[arg_index] = ft_strdup(tokens[i]);
            if (tmp_table->args[arg_index] == NULL)
                return (NULL);
            arg_index++;
            tmp_table->nb_args++;
        }
        i++;
    }
    tmp_table->args[arg_index] = NULL;
    //*cmd_table = tmp_table;
    return (*cmd_table);

}
