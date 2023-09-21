#include "minishell.h"

int ft_pwd(void)
{
   char    *path;

    path = getcwd(0, 0);//현재 작업 경로를 알 수 있게 해주는 함수이다. getcwd함수는 (char *buf, size_t size);의 형식을 받게되는데 이때 buf의 크기가 작으면 NULL을 return하고 errno에 ERANGE가 설정 하지만 buf를 NULL로 설정하면 malloc을 통해서 할당된 메모리에 작업디렉토리를 저장해 리턴하게됨 따라서 free해줘야됨.
    if (!path)//getcwd가 실패하면 NULL을 반환하게되고 pwd가 실패하면 양의 값을 리턴하는데 값을 아직 잘모르겠으므로 1로 일단 해뒀음
        return (-1);
    printf ("%s\n", path);
    free (path);
    return (0);
}
