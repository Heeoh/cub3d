/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heson <heson@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 21:27:09 by inskim            #+#    #+#             */
/*   Updated: 2023/08/07 16:47:33 by heson            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/get_next_line_bonus.h"

char	*error_handle(t_gnl_list **lst, int fd, char *read_buf)
{
	t_gnl_list	*node;
	t_gnl_list	*pre;

	if (read_buf)
		free(read_buf);
	node = *lst;
	pre = 0;
	while (lst && node)
	{
		if (node -> fd == fd)
		{
			if (node -> str)
				free(node -> str);
			node -> str = 0;
			if (pre == 0)
				*lst = node -> next;
			else
				pre -> next = node -> next;
			free(node);
			return (0);
		}
		pre = node;
		node = node -> next;
	}
	return (0);
}

int	has_newline(t_gnl_list *lst, int fd)
{
	int	i;

	lst = get_node(lst, fd);
	if (!lst)
		return (0);
	i = 0;
	if (lst -> str == 0)
		return (0);
	while ((lst -> str)[i])
		if (lst -> str[i++] == '\n')
			return (1);
	return (0);
}

int	gnl_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (*s++)
		i++;
	return (i);
}

char	*gnl_strjoin(char *s1, char const *s2)
{
	int		len;
	char	*p;
	int		i;

	if (!s2)
		return (0);
	len = gnl_strlen(s1) + gnl_strlen(s2);
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (!p)
		return (0);
	i = 0;
	while (s1 && s1[i])
	{
		p[i] = s1[i];
		i++;
	}
	if (s1)
		free(s1);
	s1 = 0;
	while (*s2)
		p[i++] = *s2++;
	p[i] = 0;
	return (p);
}

char	*gnl_substr(char const *s, unsigned int start, int len)
{
	char			*sub;
	int				str_len;
	unsigned int	i;

	if (!s)
		return (0);
	str_len = gnl_strlen(s);
	if (start > (unsigned int)str_len)
		return (0);
	s = s + start;
	i = 0;
	while (s[i] && len-- > 0)
		i++;
	sub = (char *)malloc(sizeof(char) * (i + 1));
	if (!sub)
		return (0);
	len = i;
	i = 0;
	while (len-- > 0)
	{
		sub[i] = s[i];
		i++;
	}
	sub[i] = 0;
	return (sub);
}
