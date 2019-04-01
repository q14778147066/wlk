#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
 
void get_mode(int u, int g, int o, int r, int w, int x,
		int sign, mode_t *mode)
{
	switch(sign)
	{
		case '+':
			if(u)
			{
				if(r)
					*mode |= S_IRUSR;
				if(w)
					*mode |= S_IWUSR;
				if(x)
					*mode |= S_IXUSR;
			}
 
			if(g)
			{
				if(r)
					*mode |= S_IRGRP;
				if(w)
					*mode |= S_IWGRP;
				if(x)
					*mode |= S_IXGRP;
			}
 
			if(o)
			{
				if(r)
					*mode |= S_IROTH;
				if(w)
					*mode |= S_IWOTH;
				if(x)
					*mode |= S_IXOTH;
			}
			break;
 
    case '-':
            if(u)
			{
				if(r)
					*mode &= ~S_IRUSR;
				if(w)
					*mode &= ~S_IWUSR;
				if(x)
					*mode &= ~S_IXUSR;
			}
 
			if(g)
			{
				if(r)
					*mode &= ~S_IRGRP;
				if(w)
					*mode &= ~S_IWGRP;
				if(x)
					*mode &= ~S_IXGRP;
			}
 
			if(o)
			{
				if(r)
					*mode &= ~S_IROTH;
				if(w)
					*mode &= ~S_IWOTH;
				if(x)
					*mode &= ~S_IXOTH;
			}
			break;
 
		case '=':
			if(u)
			{
				*mode &= (S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
				if(r)
					*mode |= S_IRUSR;
				if(w)
					*mode |= S_IWUSR;
				if(x)
					*mode |= S_IXUSR;
			}
 
			if(g)
			{
				*mode &= (S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IXOTH);
				if(r)
					*mode |= S_IRGRP;
				if(w)
					*mode |= S_IWGRP;
				if(x)
					*mode |= S_IXGRP;
			}
 
			if(o)
			{
				*mode &= (S_IRGRP|S_IWGRP|S_IXGRP|S_IRUSR|S_IWUSR|S_IXUSR);
 
				if(r)
					*mode |= S_IROTH;
				if(w)
					*mode |= S_IWOTH;
				if(x)
					*mode |= S_IXOTH;
			}
			break;
 
		default:
			printf("sign error\n");
			break;
	}
}
int main(int argc, char *argv[])
{
	int i;
	int k;
	mode_t mode;		//总权限
	int mode_u;		//所有者权限
	int mode_g;		//组所属权限
	int mode_o;		//其他人权限
	int flag_u;		//标记所有者是否需要设置
	int flag_g;		//标记组所属是否需要设置
	int flag_o;		//标记其他人是否需要设置
	int r;
	int w;
	int x;
	int sign;		//标记+, -, =
	struct stat buf;	//lstat();
 
	//检查命令行是否输入错误
	if(argc < 3)
	{
		printf("%s <mode> <target file>\n", argv[0]);
		exit(0);
	}
 
	//有可能是多文件一起使用chmod， 所以用循环执行
	for(k = 2; k < argc; k++)
	{
		//区分出是数字格式， 还是字符串格式
		if(argv[1][0] >= '0' && argv[1][0] <= '7')		//数字格式
		{
			mode = (unsigned)atoi(argv[1]);			//字符串转换成无符号整型, mode_t是无符号类型
			if(mode > 777 || mode < 0)			//设置权限出错的话
			{
				printf("mode num error!\n");
				exit(0);
			}
 
			//转换成八进制
			mode_u = mode / 100;
			mode_g = (mode - (mode_u*100)) / 10;
			mode_o = mode - (mode_u*100) - mode_g*10;
			mode = mode_u*8*8 + mode_g*8 + mode_o;
		}
		else
        {
            i = 0;
            //获取文件本身的权限
            if(lstat(argv[k], &buf) == -1)
            {
                fprintf(stderr, "line %d ", __LINE__);
                perror("lstat");
                exit(1);
            }
            mode = buf.st_mode;
            while(argv[1][i] != '\0')
            {
                r = w = x = 0;
                flag_u = flag_g = flag_o = 0;
                while(argv[1][i] != ',' && argv[1][i] != '\0')
                {
                    if(argv[1][i] == 'u')
                        flag_u = 1;
                    else if(argv[1][i] == 'g')
                        flag_g = 1;
                    else if(argv[1][i] == 'o')
                        flag_o = 1;
                    else if(argv[1][i] == 'a')
                        flag_u = flag_g = flag_o = 1;
                    else if(argv[1][i] == '+')
                        sign = '+';
                    else if(argv[1][i] == '-')
                        sign = '-';
                    else if(argv[1][i] == '=')
                        sign = '=';
                    else if(argv[1][i] == 'r')
                        r = 'r';
                    else if(argv[1][i] == 'w')
                        w = 'w';
                    else if(argv[1][i] == 'x')
                        x = 'x';
                    i++;
                }
                    get_mode(flag_u, flag_g, flag_o, r, w, x, sign, &mode);
                if(argv[1][i] == ',')
                    i++;
            }
		}
 
		if( chmod(argv[k], mode) == -1)
		{
			perror("chmod error");
			exit(1);
		}
 
	}
	return 0;
}