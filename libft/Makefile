# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/06 09:52:20 by ppfiel            #+#    #+#              #
#    Updated: 2023/09/06 09:52:20 by ppfiel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FILES =		ft_isdigit.c ft_isalpha.c ft_isalnum.c ft_isascii.c ft_isprint.c \
				ft_tolower.c ft_toupper.c \
				ft_strlen.c ft_strlcpy.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strncmp.c \
				ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
				ft_memset.c ft_memcpy.c ft_bzero.c ft_memccpy.c ft_memcmp.c ft_memchr.c ft_memmove.c\
				ft_atoi.c ft_calloc.c ft_itoa.c \
				ft_strmapi.c ft_striteri.c \
				ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c
SRC_OBJS =		$(SRC_FILES:.c=.o)

BONUS_FILES =	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c
BONUS_OBJS =	$(BONUS_FILES:.c=.o)

# Compiler settings
CC =			cc
CFLAGS =		-Wall -Wextra -Werror -I.

# Target
NAME =			libft.a

all:			$(NAME)	

$(NAME):		$(SRC_OBJS)
					ar rcs $(NAME) $(SRC_OBJS)

.c.o:
				$(CC) $(CFLAGS) -c $(INC_FLAGS) $< -o ${<:.c=.o}

clean:
				rm -rf $(SRC_OBJS) $(BONUS_OBJS)

fclean:			clean
					rm -f $(NAME)

re:				fclean $(NAME)

bonus:			$(SRC_OBJS) $(BONUS_OBJS)
					ar rcs $(NAME) $(SRC_OBJS) $(BONUS_OBJS)

.PHONY: 		all clean fclean re bonus test
