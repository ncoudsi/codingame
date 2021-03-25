#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
** STRUCTURES DEFINITION < START
*/
typedef enum    e_owner
{
    me = 0,
    opponent = 1,
    neutral = 2,
    unset = 3
}               t_owner;

typedef struct  s_link
{
    int     id_from;
    int     id_to;
    int     dist;
}               t_link;

typedef struct  s_troop
{
    int     owner;
    int     id_from;
    int     id_to;
    int     nb;
    int     time_to_arrive;
}               t_troop;

typedef struct  s_factory
{
    int                 id;
    t_owner             owner;
    int                 troops;
    int                 prod;
    t_link              *links;
    struct s_factory    *targets;
}               t_factory;

typedef struct  s_distress_call
{
    int     id_from;
    int     total_troops;
    t_troop *troops;
    int     total_troops_nb;
}               t_distress_call;

typedef struct  s_game_state
{
    int         entity_count;    
    int         factory_count;
    t_factory   *factories;
    int         troops_count;
    t_troop     *troops;
    int         link_count;
    t_link      *links;
    int         ingame_turn;
    int         bombs_remaining;
}               t_game_state;
/*
** STRUCTURES DEFINITION < END
*/

/*
** PROTOTYPES < START
*/
t_troop         create_troop();
void            destroy_troop(t_troop to_destroy);
t_troop         *malloc_troop_tab(int size);
t_troop         copy_troop(t_troop from);
void            set_troops_tab(t_troop **tab, int owner, int id_to, int size);
void            free_troop_tab(t_troop *to_free, int size);
t_factory       create_target();
void            destroy_target(t_factory to_destroy);
t_factory       *malloc_target_tab(int size);
void            free_target_tab(t_factory *to_free, int size);
t_link          create_link();
void            destroy_link(t_link to_destroy);
t_link          *malloc_link_tab(int size);
void            free_link_tab(t_link *to_free, int size);
t_factory       create_factory(int link_count);
void            destroy_factory(t_factory to_destroy, int link_count);
t_factory       *malloc_factory_tab(int size);
void            free_factory_tab(t_factory *to_free, int size);
t_distress_call create_distress_call(int id_from);
void            destroy_distress_call(t_distress_call to_destroy);
t_distress_call *malloc_distress_call(int id_from);
void            free_distress_call(t_distress_call *to_free);
t_game_state    create_game_state();
void            destroy_game_state(t_game_state to_destroy);
t_game_state    *malloc_game_state();
void            free_game_state(t_game_state *to_free);
t_factory       copy_factory(t_factory src);
t_link          copy_link(t_link src);
void            print_link(t_link src);
void            print_target(t_factory src);
void            print_factory(t_factory src);
void            print_troop(t_troop src);
t_owner         check_owner(int id);
int             is_valid_target(t_factory src);
int             all_targets_valid(t_factory *src, int target_count);
int             is_valid_link(t_link src);
int             all_links_valid(t_link *src, int link_count);
int             is_valid_factory(t_factory src, int link_count);
int             all_factories_valid();
int             is_valid_troop(t_troop src);
int             all_troops_valid();
int             all_datas_valid();
t_owner         set_owner(int owner, t_factory factory);
void            set_links();
void            set_targets();
int		        ft_strlen(char *src);
char	        *ft_strnew(int len);
char	        *ft_strdup(char *src);
char	        *ft_strjoin(char *str1, char *str2);
void	        ft_str_add_suffix(char **str, char *suffix);
int	            ft_nbrlen(long long int nbr, int base_len);
char			*ft_itoa_base(long long int nbr, char *base);
char		    *ft_itoa(long long int nbr);
void            move(int src, int dst, int troops);
void            bomb(int src, int dst);
void            increase(int src);
void            wait(void);
int             get_dist(int id_from, int id_to);
int             is_own_factory(t_factory factory);
int             get_own_factories();
int             is_neutral_factory(t_factory factory);
int             get_neutral_factories();
int             is_opponent_factory(t_factory factory);
int             get_opponent_factories();
int             get_onway_troops(int owner, int id_to);
int             get_onway_troops_nb(int owner, int id_to);
int             get_closest_own_factory(int target_id);
int             get_closest_neutral_factory(int target_id);
int             get_closest_opponent_factory(int target_id);
int             get_prod_until_arrival(int id_from, int id_to);
int             turtle_half_secured();
int             turtle_conquer_neutral(int target_id);
int             turtle_conquer_opponent(int target_id);
int             turtle_defense();
int             turtle_conquer();
int             turtle_start();
/*
** PROTOTYPES < END
*/

/*
** GLOBALS < START
*/
t_game_state    *game_state = NULL;
char            *actions_buffer = NULL;
/*
** GLOBALS < END
*/

/*
** STRUCTURES MANAGEMENT < START
*/
t_troop     create_troop()
{
    t_troop result;

    result.owner = 0;
    result.id_from = -1;
    result.id_to = -1;
    result.nb = -1;
    result.time_to_arrive = -1;
    return (result);
}

void        destroy_troop(t_troop to_destroy)
{
    (void)to_destroy;
}

t_troop     *malloc_troop_tab(int size)
{
    t_troop *result;
    int     troop_index;

    result = (t_troop *)malloc(sizeof(t_troop) * size);
    if (result == NULL)
        return (NULL);
    troop_index = 0;
    while (troop_index < size)
    {
        result[troop_index] = create_troop();
        troop_index++;
    }
    return (result);
}

t_troop     copy_troop(t_troop from)
{
    t_troop result;

    result.id_from = from.id_from;
    result.id_to = from.id_to;
    result.nb = from.nb;
    result.owner = from.owner;
    result.time_to_arrive = from.time_to_arrive;
    return (result);
}

void        set_troops_tab(t_troop **tab, int owner, int id_to, int size)
{
    int tab_index;
    int troop_index;

    tab_index = 0;
    troop_index = 0;
    while (troop_index < game_state->troops_count && tab_index < size)
    {
        if (game_state->troops[troop_index].id_to == id_to && game_state->troops[troop_index].owner == owner)
        {
            (*tab)[tab_index] = copy_troop(game_state->troops[troop_index]);
            tab_index++;
        }
        troop_index++;
    }
}

void        free_troop_tab(t_troop *to_free, int size)
{
    int troop_index;

    troop_index = 0;
    while (troop_index < size)
    {
        destroy_troop(to_free[troop_index]);
        troop_index++;
    }
    free(to_free);
}

t_factory   create_target()
{
    t_factory   result;

    result.id = -1;
    result.owner = unset;
    result.troops = -1;
    result.prod = -1;
    result.links = NULL;
    result.targets = NULL;
    return (result);
}

void        destroy_target(t_factory to_destroy)
{
    (void)to_destroy;
}

t_factory   *malloc_target_tab(int size)
{
    t_factory   *result;
    int         target_index;

    result = (t_factory *)malloc(sizeof(t_factory) * size);
    if (result == NULL)
        return (NULL); 
    target_index = 0;
    while (target_index < size)
    {
        result[target_index] = create_target();
        target_index++;
    }
    return (result);
}

void        free_target_tab(t_factory *to_free, int size)
{
    int target_index;

    target_index = 0;
    while (target_index < size)
    {
        destroy_target(to_free[target_index]);
        target_index++;
    }
    free(to_free);
}

t_link      create_link()
{
    t_link  result;

    result.id_from = -1;
    result.id_to = -1;
    result.dist = 21;
    return (result);
}

void        destroy_link(t_link to_destroy)
{
    (void)to_destroy;
}

t_link      *malloc_link_tab(int size)
{
    t_link  *result;
    int     link_index;

    result = (t_link *)malloc(sizeof(t_link) * size);
    if (result == NULL)
        return (NULL);
    link_index = 0;
    while (link_index < size)
    {
        result[link_index] = create_link();
        link_index++;
    }
    return (result);
}

void        free_link_tab(t_link *to_free, int size)
{
    int link_index;

    link_index = 0;
    while (link_index < size)
    {
        destroy_link(to_free[link_index]);
        link_index++;
    }
    free(to_free);
}

t_factory   create_factory(int link_count)
{
    t_factory   result;

    result.id = -1;
    result.owner = unset;
    result.troops = -1;
    result.prod = -1;
    result.links = malloc_link_tab(link_count);    
    result.targets = malloc_target_tab(link_count);
    return (result);
}

void        destroy_factory(t_factory to_destroy, int link_count)
{
    free_link_tab(to_destroy.links, link_count);
    free_target_tab(to_destroy.targets, link_count);
}

t_factory  *malloc_factory_tab(int size)
{
    t_factory *result;
    int factory_index;

    result = (t_factory *)malloc(sizeof(t_factory) * size);
    factory_index = 0;
    if (result == NULL)
        return (NULL);
    while (factory_index < size)
    {
        result[factory_index] = create_factory(size - 1);
        factory_index++;
    }
    return (result);
}

void        free_factory_tab(t_factory *to_free, int size)
{
    int factory_index;

    factory_index = 0;
    while (factory_index < size)
    {
        destroy_factory(to_free[factory_index], (size - 1));
        factory_index++;
    }
    free(to_free);
}

t_distress_call create_distress_call(int id_from)
{
    t_distress_call result;

    result.id_from = id_from;
    result.total_troops = get_onway_troops(-1, id_from);
    result.troops = malloc_troop_tab(result.total_troops);
    set_troops_tab(&result.troops, -1, id_from, result.total_troops);
    result.total_troops_nb = get_onway_troops_nb(-1, id_from);
    return (result);
}

void            destroy_distress_call(t_distress_call to_destroy)
{
    free_troop_tab(to_destroy.troops, to_destroy.total_troops);
}

t_distress_call *malloc_distress_call(int id_from)
{
    t_distress_call *result;

    result = (t_distress_call *)malloc(sizeof(t_distress_call));
    if (result == NULL)
        return (NULL);
    *result = create_distress_call(id_from);
    return (result);
}

void            free_distress_call(t_distress_call *to_free)
{
    destroy_distress_call(*to_free);
    free(to_free);
}

t_game_state    create_game_state()
{
    t_game_state    result;

    result.entity_count = -1;
    result.factory_count = -1;
    result.factories = NULL;
    result.troops_count = -1;
    result.troops = NULL;
    result.link_count = -1;
    result.links = NULL;
    result.ingame_turn = 0;
    result.bombs_remaining = 2;
    return (result);
}

void            destroy_game_state(t_game_state to_destroy)
{
    free_factory_tab(to_destroy.factories, to_destroy.factory_count);
    free_troop_tab(to_destroy.troops, to_destroy.troops_count);
    free_link_tab(to_destroy.links, to_destroy.link_count);
}

t_game_state    *malloc_game_state()
{
    t_game_state    *result;

    result = (t_game_state *)malloc(sizeof(t_game_state));
    if (result == NULL)
        return (NULL);
    *result = create_game_state();
    return (result);
}

void        free_game_state(t_game_state *to_free)
{
    destroy_game_state(*to_free);
    free(to_free);
}

t_factory   copy_factory(t_factory src)
{
    t_factory   result;

    result.id = src.id;
    result.owner = src.owner;
    result.prod = src.prod;
    result.troops = src.troops;
    result.links = src.links;
    result.targets = src.targets;
    return (result);
}

t_link  copy_link(t_link src)
{
    t_link  result;

    result.id_from = src.id_from;
    result.id_to = src.id_to;
    result.dist = src.dist;
    return (result);
}
/*
** STRUCTURES MANAGEMENT < END
*/

/*
** PRINTERS < START
*/
void        print_link(t_link src)
{
    fprintf(stderr, " < LINK > [ID_FROM = %d] [ID_TO = %d] [DIST = %d]\n",src.id_from, src.id_to, src.dist);
}

void        print_target(t_factory src)
{
    int owner;

    if (src.owner == me)
        owner = 1;
    else if (src.owner == neutral)
        owner = 0;
    else if (src.owner == opponent)
        owner = -1;
    else if (src.owner == unset)
        owner = 3;
    fprintf(stderr, "< TARGET > [ID = %d] [OWNER = %d] [TROOPS = %d] [PROD = %d]\n", src.id, owner, src.troops, src.prod);
}

void        print_factory(t_factory src)
{
    int owner;
    int index;

    index = 0;
    if (src.owner == me)
        owner = 1;
    else if (src.owner == neutral)
        owner = 0;
    else if (src.owner == opponent)
        owner = -1;
    else if (src.owner == unset)
        owner = 3;
    fprintf(stderr, "\n[FACTORY :]\n< FACTORY > [ID = %d] [OWNER = %d] [TROOPS = %d] [PROD = %d]\n\n[LINKS :]\n",
    src.id, owner, src.troops, src.prod);
    while (index < (game_state->factory_count - 1))
    {
        print_link(src.links[index]);
        index++;
    }
    fprintf(stderr, "\n[TARGETS :]\n");
    index = 0;
    while (index < (game_state->factory_count - 1))
    {
        print_target(src.targets[index]);
        index++;
    }
}

void        print_troop(t_troop src)
{
    fprintf(stderr,"\n[TROOP :]\n < TROOP > [OWNER = %d] [ID_FROM = %d] [ID_TO = %d] [NB = %d] [TIME_TO ARRIVE = %d]\n", src.owner, src.id_from, src.id_to, src.nb, src.time_to_arrive);
}
/*
** PRINTERS < END
*/

/*
** VALIDITY CHECKS < START
*/
t_owner check_owner(int id)
{
    int index;

    index = 0;
    while (game_state->factories[index].id != id)
        index++;
    return (game_state->factories[index].owner);
}

int         is_valid_target(t_factory src)
{
    if (src.id < 0 || src.owner == unset || src.troops < 0 || src.prod < 0
                                        || src.links == NULL || src.targets == NULL)
        return (0);
    return (1);
}

int         all_targets_valid(t_factory *src, int target_count)
{
    int target_index;

    target_index = 0;
    while (target_index < target_count)
    {
        if (is_valid_target(src[target_index]) == 0)
            return (0);
        target_index++;
    }
    return (1);
}

int         is_valid_link(t_link src)
{
    if (src.id_from == -1 || src.id_to == -1 || src.dist > 20 || src.dist < 0)
        return (0);
    return (1);
}

int         all_links_valid(t_link *src, int link_count)
{
    int link_index;

    link_index = 0;
    while (link_index < link_count)
    {
        if (is_valid_link(src[link_index]) == 0)
            return (0);
        link_index++;
    }
    return (1);
}

int         is_valid_factory(t_factory src, int link_count)
{
    if (src.id < 0 || src.owner == unset || src.troops < 0 || src.prod < 0
                                        || all_links_valid(src.links, link_count) == 0 || all_targets_valid(src.targets, link_count) == 0)
        return (0);
    return (1);
}

int         all_factories_valid()
{
    int factory_index;
    
    factory_index = 0;
    while (factory_index < game_state->factory_count)
    {
        if (is_valid_factory(game_state->factories[factory_index], game_state->factory_count - 1) == 0)
            return (0);
        factory_index++;
    }
    return (1);
}

int         is_valid_troop(t_troop src)
{
    if (src.id_from < 0 || src.id_to < 0 || src.nb < 0 || (src.owner != 1 && src.owner != -1) ||
                                                src.time_to_arrive < 0 || src.time_to_arrive > 20)
        return (0);
    return (1);
}

int         all_troops_valid()
{
    int troop_index;

    troop_index = 0;
    while (troop_index < game_state->troops_count)
    {
        if (is_valid_troop(game_state->troops[troop_index]) == 0)
            return (0);
        troop_index++;
    }
    return (1);
}

int         all_datas_valid()
{
    if (all_factories_valid() == 0 || all_troops_valid() == 0)
        return (0);
    return (1);
}
/*
** VALIDITY CHECKS < END
*/

/*
** SETTERS < START
*/
t_owner set_owner(int owner, t_factory factory)
{
    t_owner result;

    if (owner == 1)
        result = me;
    else if (owner == -1)
        result = opponent;
    else if (owner == 0)
        result = neutral;
    else
        result = unset;
    return (result);
}

void    set_links()
{
    int factory_index;
    int link_index;
    int links_added;
    int tmp;

    factory_index = 0;
    while (factory_index < game_state->factory_count)
    {
        link_index = 0;
        links_added = 0;
        while (link_index < game_state->link_count)
        {
            if (game_state->links[link_index].id_from == game_state->factories[factory_index].id)
            {
                game_state->factories[factory_index].links[links_added] = copy_link(game_state->links[link_index]);
                links_added++;
            }
            else if (game_state->links[link_index].id_to == game_state->factories[factory_index].id)
            {
                game_state->factories[factory_index].links[links_added] = copy_link(game_state->links[link_index]);
                tmp = game_state->factories[factory_index].links[links_added].id_from;
                game_state->factories[factory_index].links[links_added].id_from = game_state->factories[factory_index].links[links_added].id_to;
                game_state->factories[factory_index].links[links_added].id_to = tmp;
                links_added++;
            }
                link_index++;
        }
        factory_index++;
    }
}

void    set_targets()
{
    int factory_index;
    int link_index;

    factory_index = 0;
    while (factory_index < game_state->factory_count)
    {
        link_index = 0;
        while (link_index < (game_state->factory_count - 1))
        {
            game_state->factories[factory_index].targets[link_index] =
            copy_factory(game_state->factories[game_state->factories[factory_index].links[link_index].id_to]);
            link_index++;
        }
        factory_index++;
    }
}
/*
** SETTERS < END
*/

/*
** STRINGS MANAGEMENT < START
*/
int		ft_strlen(char *src)
{
	int	result;

	result = 0;
	if (src == NULL)
		return (0);
	while (src[result] != '\0')
		result++;
	return (result);
}

char	*ft_strnew(int len)
{
	char		*result;
	int		index;

	result = (char *)malloc(sizeof(char) * (len + 1));
	if (result == NULL)
		return (NULL);
	index = 0;
	while (index < len)
	{
		result[index] = '\0';
		index++;
	}
	result[index] = '\0';
	return (result);
}

char	*ft_strdup(char *src)
{
	int		index;
	int		len;
	char	*result;

	if (src == NULL)
		return (NULL);
	len = ft_strlen(src);
	result = ft_strnew(len);
	if (result == NULL)
		return (NULL);
	index = 0;
	while (index < len)
	{
		result[index] = src[index];
		index++;
	}
	return (result);
}

char	*ft_strjoin(char *str1, char *str2)
{
	int		result_len;
	int		index;
	int		str1_len;
	char	*result;

	if (str1 == NULL && str2 == NULL)
		return (NULL);
	result_len = (ft_strlen(str1) + ft_strlen(str2));
	index = 0;
	str1_len = ft_strlen(str1);
	result = ft_strnew(result_len);
	if (result == NULL)
		return (NULL);
	while (index < str1_len)
	{
		result[index] = str1[index];
		index++;
	}
	index = 0;
	while (str2 != NULL && str2[index] != '\0')
	{
		result[index + str1_len] = str2[index];
		index++;
	}
	return (result);
}

void	ft_str_add_suffix(char **str, char *suffix)
{
	char	*tmp;

	if (suffix == NULL)
		return ;
	tmp = ft_strdup(*str);
	free(*str);
	*str = ft_strjoin(tmp, suffix);
	free(tmp);
}
/*
** STRINGS MANAGEMENT < END
*/

/*
** CONVERTERS < START
*/
int	ft_nbrlen(long long int nbr, int base_len)
{
	int					    result;
	unsigned long long int	abs_nbr;

	result = 1;
	if (nbr < 0)
	{
		result++;
		abs_nbr = -nbr;
	}
	else
		abs_nbr = nbr;
	while (abs_nbr >= base_len)
	{
		abs_nbr /= base_len;
		result++;
	}
	return (result);
}

char			*ft_itoa_base(long long int nbr, char *base)
{
	int					    base_len;
	int					    nbr_len;
	char					*result;
	unsigned long long int	abs_nbr;

	base_len = ft_strlen(base);
	nbr_len = ft_nbrlen(nbr, base_len);
	result = ft_strnew(nbr_len);
	if (nbr < 0)
	{
		result[0] = '-';
		abs_nbr = -nbr;
	}
	else
		abs_nbr = nbr;
	nbr_len--;
	while (abs_nbr >= base_len)
	{
		result[nbr_len] = base[abs_nbr % base_len];
		abs_nbr /= base_len;
		nbr_len--;
	}
	result[nbr_len] = base[abs_nbr % base_len];
	return (result);
}

char		*ft_itoa(long long int nbr)
{
	char	*base;
	char	*result;

	base = "0123456789";
	result = ft_itoa_base(nbr, base);
	return (result);
}
/*
** CONVERTERS < END
*/

/*
** ACTIONS < START
*/
void    move(int src, int dst, int troops)
{
    if (actions_buffer != NULL)
        ft_str_add_suffix(&actions_buffer, ft_strdup("; "));
    ft_str_add_suffix(&actions_buffer, ft_strdup("MOVE "));
    ft_str_add_suffix(&actions_buffer, ft_itoa(src));
    ft_str_add_suffix(&actions_buffer, ft_strdup(" "));
    ft_str_add_suffix(&actions_buffer, ft_itoa(dst));
    ft_str_add_suffix(&actions_buffer, ft_strdup(" "));
    ft_str_add_suffix(&actions_buffer, ft_itoa(troops));

}

void    bomb(int src, int dst)
{
    if (actions_buffer != NULL)
        ft_str_add_suffix(&actions_buffer, ft_strdup("; "));
    ft_str_add_suffix(&actions_buffer, ft_strdup("BOMB "));
    ft_str_add_suffix(&actions_buffer, ft_itoa(src));
    ft_str_add_suffix(&actions_buffer, ft_strdup(" "));
    ft_str_add_suffix(&actions_buffer, ft_itoa(dst));
}

void    increase(int src)
{
    if (actions_buffer != NULL)
        ft_str_add_suffix(&actions_buffer, ft_strdup("; "));
    ft_str_add_suffix(&actions_buffer, ft_strdup("INC "));
    ft_str_add_suffix(&actions_buffer, ft_itoa(src));
}

void    wait(void)
{
    if (actions_buffer != NULL)
        ft_str_add_suffix(&actions_buffer, ft_strdup("; "));
    ft_str_add_suffix(&actions_buffer, ft_strdup("WAIT"));
}
/*
** ACTIONS < END
*/

/*
** STRATEGIES < START
*/
int     get_dist(int id_from, int id_to)
{
    int result;
    int link_index;

    result = -1;
    link_index = 0;
    while (result < 0)
    {
        if (game_state->factories[id_from].links[link_index].id_to == id_to)
            result = game_state->factories[id_from].links[link_index].dist;
        link_index++;
    }
    return (result);
}

int     is_own_factory(t_factory factory)
{
    if (factory.owner == me)
        return (1);
    return (0);
}

int     get_own_factories()
{
    int result;
    int factory_index;

    result = 0;
    factory_index = 0;
    while (factory_index < game_state->factory_count)
    {
        if (is_own_factory(game_state->factories[factory_index]) == 1)
            result++;
        factory_index++;
    }
    return (result);
}

int     is_neutral_factory(t_factory factory)
{
    if (factory.owner == neutral)
        return (1);
    return (0);
}

int     get_neutral_factories()
{
    int result;
    int factory_index;

    result = 0;
    factory_index = 0;
    while (factory_index < game_state->factory_count)
    {
        if (is_neutral_factory(game_state->factories[factory_index]) == 1)
            result++;
        factory_index++;
    }
    return (result);
}

int     is_opponent_factory(t_factory factory)
{
    if (factory.owner == opponent)
        return (1);
    return (0);
}

int     get_opponent_factories()
{
    int result;
    int factory_index;

    result = 0;
    factory_index = 0;
    while (factory_index < game_state->factory_count)
    {
        if (is_opponent_factory(game_state->factories[factory_index]) == 1)
            result++;
        factory_index++;
    }
    return (result);
}

int     get_onway_troops(int owner, int id_to)
{
    int result;
    int troop_index;

    result = 0;
    troop_index = 0;
    while (troop_index < game_state->troops_count)
    {
        if (game_state->troops[troop_index].owner == owner && game_state->troops[troop_index].id_to == id_to)
            result ++;
        troop_index++;
    }
    return (result);
}

int     get_onway_troops_nb(int owner, int id_to)
{
    int result;
    int troop_index;

    result = 0;
    troop_index = 0;
    while (troop_index < game_state->troops_count)
    {
        if (game_state->troops[troop_index].owner == owner && game_state->troops[troop_index].id_to == id_to)
            result += game_state->troops[troop_index].nb;
        troop_index++;
    }
    return (result);
}

int     get_closest_own_factory(int target_id)
{
    int result;
    int link_index;
    int dist;

    result = 0;
    link_index = 0;
    dist = 21;
    while (link_index < (game_state->factory_count - 1))
    {
        if (game_state->factories[game_state->factories[target_id].links[link_index].id_to].owner == me &&
        game_state->factories[target_id].links[link_index].dist < dist)
        {
            dist = game_state->factories[target_id].links[link_index].dist;
            result = game_state->factories[target_id].links[link_index].id_to;
        }
        link_index++;
    }
    return (result);
}

int     get_closest_neutral_factory(int target_id)
{
    int result;
    int link_index;
    int dist;

    result = 0;
    link_index = 0;
    dist = 21;
    while (link_index < (game_state->factory_count - 1))
    {
        if (game_state->factories[game_state->factories[target_id].links[link_index].id_to].owner == neutral &&
        game_state->factories[target_id].links[link_index].dist < dist)
        {
            dist = game_state->factories[target_id].links[link_index].dist;
            result = game_state->factories[target_id].links[link_index].id_to;
        }
        link_index++;
    }
    return (result);
}

int     get_closest_opponent_factory(int target_id)
{
    int result;
    int link_index;
    int dist;

    result = 0;
    link_index = 0;
    dist = 21;
    while (link_index < (game_state->factory_count - 1))
    {
        if (game_state->factories[game_state->factories[target_id].links[link_index].id_to].owner == opponent &&
        game_state->factories[target_id].links[link_index].dist < dist)
        {
            dist = game_state->factories[target_id].links[link_index].dist;
            result = game_state->factories[target_id].links[link_index].id_to;
        }
        link_index++;
    }
    return (result);
}

int get_prod_until_arrival(int id_from, int id_to)
{
    int result;
    int dist;
    int link_index;

    result = -1;
    link_index = 0;
    while (link_index < game_state->factory_count - 1)
    {
        if (game_state->factories[id_from].links[link_index].id_to == id_to)
        {
            dist = game_state->factories[id_from].links[link_index].dist;
            result = game_state->factories[id_to].prod * dist;
        }
        link_index++;
    }
    return (result);
}
/*
** STRATEGIES < TURTLE_START < START
*/
int turtle_half_secured()
{
    int factory_index;

    factory_index = 1;
    while (factory_index < game_state->factory_count)
    {
        if (factory_index % 2 != 0 && game_state->factories[factory_index].owner != me)
            return (0);
        factory_index++;
    }
    fprintf(stderr, "HALF SECURED\n");
    return (1);
}


int turtle_conquer_neutral(int target_id)
{
    int result;
    int attacker_id;
    int needed_troops;

    result = 0;
    attacker_id = get_closest_own_factory(target_id);
    needed_troops = game_state->factories[target_id].troops - get_onway_troops_nb(1, target_id) + 1;
    if (needed_troops > 0 && game_state->factories[attacker_id].troops >= needed_troops)
    {
        move(attacker_id, target_id, needed_troops);
        game_state->factories[attacker_id].troops -= needed_troops;
        result++;
    }
    fprintf(stderr, "CONQUER NEUTRAL ID : %d WITH %d TROOPS FROM %d FACTORY\n", target_id, needed_troops, attacker_id);
    fprintf(stderr, "ACTUAL ENNEMY TROOPS COMING TO %d : %d\n", target_id, get_onway_troops_nb(-1, target_id));
    return (result);
}

int turtle_conquer_opponent(int target_id)
{
    int result;
    int attacker_id;
    int needed_troops;
    int prod_until_arrival;

    result = 0;
    attacker_id = get_closest_own_factory(target_id);
    needed_troops = game_state->factories[target_id].troops - get_onway_troops_nb(1, target_id) + get_onway_troops_nb(-1, target_id) + 1;
    prod_until_arrival = get_prod_until_arrival(attacker_id, target_id);
    if (prod_until_arrival > 0)
        needed_troops += prod_until_arrival;
    if (needed_troops > 0 && game_state->factories[attacker_id].troops >= needed_troops)
    {
        move(attacker_id, target_id, needed_troops);
        game_state->factories[attacker_id].troops -= needed_troops;
        result++;
    }
    fprintf(stderr, "CONQUER OPPONENT ID : %d WITH %d TROOPS FROM %d FACTORY\n ENNEMY WILL PROD %d UNTIL ARRIVAL\n", target_id, needed_troops, attacker_id, prod_until_arrival);
    fprintf(stderr, "ACTUAL ENNEMY TROOPS COMING TO %d : %d\n", target_id, get_onway_troops_nb(-1, target_id));
    return (result);
}

int turtle_defense()
{
    int result;
    int factory_index;

    result = 0;
    factory_index = 0;
    while (factory_index < game_state->factory_count)
    {
        if (game_state->factories[factory_index].owner == me && game_state->factories[factory_index].troops > 10 &&
        get_onway_troops_nb(-1, factory_index) < 10 && game_state->factories[factory_index].prod < 3)
        {
            increase(factory_index);
            result++;
        }
        factory_index++;
    }
    return (result);
}

int turtle_conquer()
{
    int result;
    int factory_index;

    result = 0;
    factory_index = 0;
    fprintf(stderr, "CONQUER\n");
    while (factory_index < game_state->factory_count)
    {
        if (factory_index % 2 != 0 && game_state->factories[factory_index].owner == me && get_onway_troops_nb(-1, factory_index) >
        game_state->factories[factory_index].troops)
            //add_distress_call(factory_index);
            ;
        else if (factory_index % 2 != 0 && game_state->factories[factory_index].owner == neutral)
            result += turtle_conquer_neutral(factory_index);
        else if (factory_index % 2 != 0 && game_state->factories[factory_index].owner == opponent)
            result += turtle_conquer_opponent(factory_index);
        factory_index++;
    }
    if (game_state->ingame_turn == 0 && game_state->factories[2].prod > 1)
        bomb(1, 2);
    return (result);
}

int turtle_start()
{
    if (turtle_half_secured() == 1)
        return (turtle_defense());
    return (turtle_conquer());
}
/*
** STRATEGIES < TURTLE_START < END
*/
/*
** STRATEGIES < END
*/

/*
** MAIN < START
*/
int main()
{

    game_state = (t_game_state *)malloc(sizeof(t_game_state));
    if (game_state == NULL)
        return (1);
    scanf("%d", &(game_state->factory_count));
    scanf("%d", &(game_state->link_count));
    game_state->factories = malloc_factory_tab(game_state->factory_count);
    if (game_state->factories == NULL)
        return (1);
    game_state->links = malloc_link_tab(game_state->link_count);
    if (game_state->links == NULL)
        return (1);
    for (int i = 0; i < game_state->link_count; i++)
    {
        int factory_1;
        int factory_2;
        int distance;
        scanf("%d%d%d", &factory_1, &factory_2, &distance);
        game_state->links[i].id_from = factory_1;
        game_state->links[i].id_to = factory_2;
        game_state->links[i].dist = distance;
    }

    while (1) 
    {
        int factory_index;
        int troop_index;

        factory_index = 0;
        troop_index = 0;
        scanf("%d", &(game_state->entity_count));
        game_state->troops_count = game_state->entity_count - game_state->factory_count;
        game_state->troops = malloc_troop_tab(game_state->troops_count);
        for (int i = 0; i < game_state->entity_count; i++)
        {
            int entity_id;
            char entity_type[11];
            int arg_1;
            int arg_2;
            int arg_3;
            int arg_4;
            int arg_5;
            scanf("%d%s%d%d%d%d%d", &entity_id, entity_type, &arg_1, &arg_2, &arg_3, &arg_4, &arg_5);
            if (entity_id < game_state->factory_count)
            {
                game_state->factories[factory_index].id = entity_id;
                game_state->factories[factory_index].owner = set_owner(arg_1, game_state->factories[factory_index]);
                game_state->factories[factory_index].troops = arg_2;
                game_state->factories[factory_index].prod = arg_3;
                factory_index++;
            }
            else if (strcmp("TROOP", entity_type) == 0)
            {
                game_state->troops[troop_index].owner = arg_1;
                game_state->troops[troop_index].id_from = arg_2;
                game_state->troops[troop_index].id_to = arg_3;
                game_state->troops[troop_index].nb = arg_4;
                game_state->troops[troop_index].time_to_arrive = arg_5;
                troop_index++;
            }
        }
        if (troop_index < game_state->troops_count)
            game_state->troops_count = troop_index;
        if (game_state->ingame_turn == 0)
            set_links();
        set_targets();
        if (all_datas_valid() == 0)
        {
            fprintf(stderr, "INVALID DATAS\n");
            free_game_state(game_state);
            return (1);
        }
        else if (turtle_start() == 0)
            wait();
        printf("%s\n", actions_buffer);
        free_troop_tab(game_state->troops, game_state->troops_count);
        game_state->troops = NULL;
        free(actions_buffer);
        actions_buffer = NULL;
        game_state->ingame_turn++;
    }
    free_game_state(game_state);
    free(actions_buffer);
    return 0;
}
/*
** MAIN < END
*/