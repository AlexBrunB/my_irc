/*
** server.c for server in /home/abrun/delivery/Tek2/PSU/PSU_2016_myirc
** 
** Made by Alexandre Brun
** Login   <abrun@epitech.net>
** 
** Started on  Tue May 23 10:57:13 2017 Alexandre Brun
** Last update Sun Jun 11 23:08:39 2017 Etienne Labrot
*/

#include "server.h"

void			init_stuffs(t_env *my_env, char *av)
{
  memset(my_env->fd_type, FD_FREE, MAX_FD);
  my_env->port = atoi(av);
  my_env->chans = NULL;
  my_env->users = NULL;
  add_server(my_env);
}

void	init_select(struct timeval *tv, int *i, int *fd_max, fd_set *fd_read)
{
  tv->tv_sec = 20;
  tv->tv_usec = 0;
  FD_ZERO(fd_read);
  *fd_max = 0;
  *i = 0;
}

void			select_task(t_env *my_env)
{
  struct timeval	tv;
  int			i;
  int			fd_max;
  fd_set		fd_read;

  init_select(&tv, &i, &fd_max, &fd_read);
  while (i < MAX_FD)
    {
      if (my_env->fd_type[i] != FD_FREE)
	{
	  FD_SET(i, &fd_read);
	  fd_max = i;
	}
      i++;
    }
  if (select(fd_max + 1, &fd_read, NULL, NULL, &tv) == -1)
    perror("select");
  i = 0;
  while ( i < MAX_FD)
    {
      if (FD_ISSET(i, &fd_read))
	my_env->fct_read[i](my_env, i);
      i++;
    }
}

int			main(int ac, char **av)
{
  t_env			my_env;

  if (ac != 2)
    {
      printf("USAGE : %s [PORT]\n", av[0]);
      return (84);
    }
  init_stuffs(&my_env, av[1]);
  while (1)
    select_task(&my_env);
  return (0);
}
