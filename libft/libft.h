/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 18:59:56 by showard       #+#    #+#                 */
/*   Updated: 2025/03/27 14:35:39 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif
# if BUFFER_SIZE <= 0
#  error BUFFER_SIZE needs to be bigger than 0. Try again nerd.
# endif

# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_map
{
	struct s_map	*next;
	char			*key;
	char			*value;
}	t_map;

char		*get_next_line(int fd);
size_t		ft_strlen_gnl(const char *s);
char		*ft_strdup_gnl(const char *s);
char		*ft_strjoin_gnl(char const *s1, char const *s2);
char		*ft_substr_gnl(char const *s, unsigned int start, size_t len);
char		*ft_strchr_gnl(const char *s, int c);
void		clean_lines(char *remainder, char **full_line, char **r_line);
void		complete_line(char *buffer, char **full_line, int fd);
char		*ft_strcpy_gnl(char *dest, const char *src);
int			ft_toupper(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_itoa(int n);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memset(void *b, int c, size_t len);
void		ft_bzero(void *s, size_t n);
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		*ft_memchr(const void *str, int c, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd, ssize_t *fcheck);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char		*ft_strnstr(const char *htk, const char *needle, size_t len);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_atoi(const char *str);
void		*ft_calloc(size_t count, size_t size);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char		*ft_strdup(const char *s1);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
void		ft_lstadd_front(t_list **lst, t_list *new);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstnew(void *content);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_write(int fd, const void *buffer, size_t count, ssize_t *fcheck);
int			ft_strcmp(const char *s1, const char *s2);

t_map		*ft_mapnew(void *key, void *value);
t_map		*ft_maplast(t_map *lst);
void		ft_mapadd_back(t_map **lst, t_map *new);
// void		ft_mapiter(t_list *lst, void (*f)(void *));
// void		ft_mapadd_front(t_list **lst, t_list *new);
// int			ft_mapsize(t_list *lst);
// t_list		*ft_maplast(t_list *lst);

#endif