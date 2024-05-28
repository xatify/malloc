# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 18:48:01 by abbouzid          #+#    #+#              #
#    Updated: 2024/05/28 15:39:39 by abbouzid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC		= gcc
CFLAGS	= -g -DDEBUG -Wall -Wextra -Werror

LIBFT =	src/libft/libft.a

TARGET=	libft_malloc_$(HOSTTYPE).so

SRCS =		src/block.c	\
			src/zone.c 		\
			src/debug.c		\
			src/free.c		\
			src/malloc.c	\
			src/realloc.c  	\
			src/libft/ft_memcpy.c 	\
			src/libft/ft_strlen.c	\
			src/libft/ft_write.c	\
			src/libft/roundup.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -shared $(OBJS) -o $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -fPIC $< -o $@


clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)

