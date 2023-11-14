# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 12:55:26 by ppfiel            #+#    #+#              #
#    Updated: 2023/10/13 09:27:09 by ppfiel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRC_DIR =	./srcs/
INC_DIR =	./includes/
LIBFT_DIR =	./libft/

SRC_FILES =		main.c \
				env_utils.c env_create.c \
				builtins/builtin_env.c \
				builtins/builtin_pwd.c \
				builtins/builtin_echo.c \
				builtins/builtin_unset.c \
				builtins/builtin_exit.c \
				builtins/builtin_cd.c builtins/builtin_cd_utils.c \
				builtins/builtin_export.c builtins/builtin_export_utils.c builtins/builtin_export_validating.c \
				exec/exec.c \
				exec/exec_redirection.c \
				exec/exec_pipe.c \
				exec/exec_heredoc.c \
				exec/exec_handle_single_command.c \
				exec/exec_handle_multiple_commands.c \
				exec/exec_handle_parent_process.c \
				exec/exec_handle_child_process.c \
				exec/exec_execve.c \
				exec/exec_error.c \
				exec/exec_waiting.c \
				exec/exec_builtins.c \
				parsing/parsing_error.c parsing/parsing_finish.c \
				parsing/parsing_free.c parsing/parsing_init.c parsing/parsing_list_utils.c \
				parsing/parsing_redirections.c parsing/parsing_tokens_2.c parsing/parsing_tokens.c \
				parsing/parsing_utils.c parsing/parsing.c parsing/utils.c \
				parsing/parsing_utils_2.c parsing/parsing_vars.c parsing/parsing_error_2.c \
				parsing/parsing_free_2.c parsing/parsing_free_3.c parsing/heredoc.c parsing/heredoc_2.c \
				parsing/parsing_vars_2.c parsing/path_utils.c signals.c parsing/bullshit.c \
				parsing/parsing_utils_3.c main_utils.c

SRCS =			$(addprefix $(SRC_DIR), $(SRC_FILES))
SRC_OBJS =		$(SRCS:.c=.o)

# Compiler settings
CC =			cc
CFLAGS =		-Wall -Wextra -Werror
INC_FLAGS =		-I $(INC_DIR)
LIBFT =			-L $(LIBFT_DIR) -lft

# Target
NAME =			minishell

all:			$(NAME)	

$(NAME):		$(SRC_OBJS)
					make bonus -C $(LIBFT_DIR)
					$(CC) $(CFLAGS) $(SRC_OBJS) -o $(NAME) $(LIBFT) -lreadline
.c.o:
				$(CC) $(CFLAGS) -c $(INC_FLAGS) $< -o ${<:.c=.o}

clean:
				make clean -C libft
				rm -rf $(SRC_OBJS) $(BONUS_OBJS)

fclean:			clean
					make fclean -C libft
					rm -f $(NAME)

re:				fclean $(NAME)

.PHONY: 		all clean fclean re 