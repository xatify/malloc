/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roundup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbouzid <abbouzid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:41:44 by abbouzid          #+#    #+#             */
/*   Updated: 2024/05/26 23:42:33 by abbouzid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	roundup(size_t size, size_t r)
{
	return ((((size - 1) / r) * r) + r);
}
