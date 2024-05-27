# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/26 22:41:20 by abbouzid          #+#    #+#              #
#    Updated: 2024/05/26 23:42:50 by abbouzid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TARGET = libft.a

cc = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS =		ft_write.c			\
			ft_strlen.c			\
			ft_memcpy.c			\
			roundup.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $(TARGET) $(OBJS)


.c.o:
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)
