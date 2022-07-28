#include <stdio.h>
#include <stdlib.h>

#define SINGLE_Q 1
#define DOUBLE_Q 2
#define DOLLARS 4
/*UN DEBUT PREMOETTEUR, A CHECKER AVEC LUC*/

// char	*__is_expandable(char *str)
// {
// 	static int quote_flag1 = 0;
// 	static int quote_flag2 = 0;

// 	if (*str == '\0')
// 		return (NULL);


// 	quote_flag1 ^= (((*str == '\'') * 1) + ((*str == '"') * -1));
// 	printf("quote_flag1 = %d and *str = %c\n", quote_flag1, *str);
// 	//quote_flag2 = (quote_flag1 == 1 && quote_flag1 == -2);
// 	if (*str == '$' && *(str + 1) != ' ' && !quote_flag2)
// 	{
// 		return (str);
// 	}
// 	return (__is_expandable(str + 1));
// }

// int	ft_strchr_b(const char *s, int c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == (unsigned char) c)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }


// char	*__is_expandable(char *str)
// {
// 	static char quote_flag1 = 0;

// 	if (*str == '\0')
// 		return (NULL);
// 	if (quote_flag1 == 0 || quote_flag1 == '\'')
// 		quote_flag1 ^= (*str == '\'') * '\'';
// 	if (quote_flag1 == 0 || quote_flag1 == '\"')
// 		quote_flag1 ^= (*str == '\"') * '\"';
// 	if (*str == '$' && *(str + 1) != ' ' && *(str + 1) && quote_flag1 != '\'')
// 		return (str);
// 	return (__is_expandable(str + 1));
// }



// char	*__expand_string(char *str)
// {
// 	char	quote_flag;
// 	int		i;
// 	char	*start;

// 	if (!str)
// 		return (NULL);
// 	quote_flag = 0;
// 	i = 0;
// 	start = str;
// 	while (*(str + i))
// 	{
// 		while ((quote_flag == 0 || quote_flag == *(str + i)) && ft_strchr_b("\'\"", *(str + i)))
// 		{
// 			quote_flag ^= ((*(str + i) == '\'') * '\'') + ((*(str + i) == '\"') * '\"');
// 			i++;
// 		}
// 		*str = *(str + i);
// 			str++;
// 	}
// 	*str = '\0';
// 	return (start);
// }

// int main(void)
// {
// 	char str[] = "\"je s\'\'\'\'\'\'u\"is \'\"\"\"\"\" s\'ac\"ha fgfgd\"";
// 	printf("%s\n", str);
// 	printf("%s\n", __expand_string(str));
// 	//printf("%s\n", );
// 	return (0);
// }


// single = 0100;
// double = 1000;

// quoteflag = 0000;

// if !quoteflag
// 	quoteflag |= single

int	__cnt_tablen_str(char *str)
{
	int	cnt;

	cnt = -1;
	while (1)
	{
		while (*str == -1)
			str++;
		if (*str && *str != -1)
			cnt++;
		while (*str && *str != -1)
		{
			if (*str == -2)
			*str = '\'';
			str++;
		}
		if (!*str)
			break;
	}
	return (cnt);
}

int main (void)
{
	// char *str = {
	// 	-1, -1, -1, 'q', 'q', 'q', 'q', -1, 'q', -2 ,'q',-1, -1, '\0'
	// };
	char str[] = "je'        'suis''        ''un'test         ";
	char *tmp = str;
	while (*tmp)
	{
		if (*tmp == ' ')
			*tmp = -1;
		else if (*tmp == '\'')
			*tmp = -2;
		tmp++;
	}
	printf("str = %s\n", str);
	printf("cnt = %d\n", __cnt_tablen_str(str));
	printf("after str = %s\n", str);

}
