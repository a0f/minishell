/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/23 11:29:55 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 16:01:29 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stddef.h>
# include <stdint.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);
int	ft_printf_char(char c, ssize_t *fcheck);
int	ft_printf_int(int n, ssize_t *fcheck);
int	ft_printf_uint(unsigned int n, ssize_t *fcheck);
int	ft_printf_ptr(uintptr_t hex_long, ssize_t *fcheck);
int	ft_printf_perc(ssize_t *fcheck);
int	ft_printf_str(const char *str, ssize_t *fcheck);
int	ft_printf_lowerhex(unsigned int hex_long, ssize_t *fcheck);
int	ft_printf_upperhex(unsigned int hex_long, ssize_t *fcheck);

#endif