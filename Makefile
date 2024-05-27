# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abbouzid <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 18:48:01 by abbouzid          #+#    #+#              #
#    Updated: 2024/05/27 19:14:28 by abbouzid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC		= gcc
CFLAGS	= -Wall -Wextra -Werror

LIBFT =	src/libft/libft.a

TARGET=	libft_malloc_$(HOSTTYPE).so

SRCS =		src/block.c	\
			src/zone.c 		\
			src/debug.c		\
			src/free.c		\
			src/malloc.c	\
			src/realloc.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -shared -L./src/libft -lft $(OBJS) -o $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -fPIC $< -o $@

$(LIBFT):
	make -C src/libft

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)
	make -C ./src/libft fclean

