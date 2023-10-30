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
				./builtins/builtin_cd.c ./builtins/builtin_env.c ./builtins/builtin_pwd.c ./builtins/builtin_echo.c
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
					$(CC) $(CFLAGS) $(SRC_OBJS) -o $(NAME) $(LIBFT)
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