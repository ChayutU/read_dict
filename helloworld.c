#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"

struct t_dict
{
    unsigned int    value;
    char            *word;
    struct t_dict   *next;
};

void add_dict(struct t_dict *head, struct t_dict *dict) 
{
    struct t_dict *current;
    current = head;

    if (current == NULL)
    {
        current = dict;
        return;
    }
    
    while (1){
        if (current->next == NULL)
        {
            break;
        }
        current = current->next;
    }
    current->next = dict;
}

char *get_word_from_dict(unsigned int value, struct t_dict *dict)
{
    struct t_dict *current;
    current = dict;
    while (1)
    {
        if (current->next == NULL)
        {
            break;
        }
        if (current->value == value)
        {
            return (current->word);
        }
        
        current = current->next;
    }
    return (NULL);
}

void print_dict(struct t_dict *head)
{
    struct t_dict *current;
    current = head;
    while (1)
    {
        if (current->next == NULL)
        {
            break;
        }
        printf("value: \"%d\", word: \"%s\"\n", current->value, current->word);
        current = current->next;
    }
}

void free_dict(struct t_dict *head)
{
    while (head != NULL)
    {
        free(head);
        head = head->next;
    }
}

void print_err() {
    printf("Error\n");
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	int i;

	i = 0;

	while (src[i] && i <= n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i <= n)
	{
		dest[i++] = '\0';
	}
	return (dest);
}

int	atoi(char *str)
{
	int neg;
	int num;
	int i;

	i = 0;
	neg = 1;
	num = 0;
	while (str[i] <= ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			neg *= -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return (num * neg);
}

int main(int argc, char **argv) 
{
    char            *path;
    struct t_dict   *dicts;
    path = "number.dict";

    dicts = (struct t_dict*)malloc(sizeof(struct t_dict));
    dicts->value = -1;
    dicts->next = NULL;

    if (argc < 2 || argc > 3)
    {
        print_err();
        return (0);
    }
    
    if (argc == 3)
    {
        path = argv[2];
    }

    int     fd;
    int     b_read;
    char    buffer;

    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        close(fd);
        print_err();
        return (0);
    }
    
    b_read = 1;
    while (b_read > 0)
    {
        b_read = read(fd, &buffer, 1);

        if (buffer != ' ')
        {
            char    *a;
            int     i;
            
            i = 0;

            a = (char*)malloc(128 * sizeof(char));

            while (b_read > 0)
            {
                a[i++] = buffer;
                b_read = read(fd, &buffer, 1);
                if (buffer == ':')
                {
                    break;
                }
            }
            // printf("a >> %s\n", a);
            // printf("i >> %d\n", i);

            while (1)
            {
                if (buffer != ' ' && buffer != ':')
                {
                    break;
                }
                
                b_read = read(fd, &buffer, 1);
            }
            

            char    *b;
            int     j;

            j = 0;

            b = (char*)malloc(128 * sizeof(char));

            while (b_read > 0)
            {
                // printf("%c", buffer);
                b[j++] = buffer;
                b_read = read(fd, &buffer, 1);
                if (buffer == '\n' || buffer == ' ')
                {
                    break;
                }
            }
            // printf("b >> %s\n", b);
            // printf("j >> %d\n", j);


            struct t_dict   *new_dict;

            new_dict = (struct t_dict*)malloc(sizeof(struct t_dict));
            // TODO: validate value is unsined int
            new_dict->value = atoi(a); 
            new_dict->word = (char*)malloc(j * sizeof(char));
            new_dict->next = NULL;
            ft_strncpy(new_dict->word, b, j - 1);
            add_dict(dicts, new_dict);
            free(b);
            free(a);
        }
    }
    close(fd);
    
    print_dict(dicts);
    printf("==============\n");
    char    *str = get_word_from_dict(atoi(argv[1]), dicts);

    if (str == NULL)
    {
        /* code */
        // TODO: extact value then find in dict again.
    } else {
        printf("%s\n", str);
    }
    
    free_dict(dicts);
    return (0);
}