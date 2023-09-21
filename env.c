#include "minishell.h"

int	ft_env(t_data *data)
{
	dll_print(&data->envdll, dll_env_print_func);
	return (0);
}