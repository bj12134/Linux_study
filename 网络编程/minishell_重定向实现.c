#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(){

	// 1.获取命令行标准输入
	char cmd_buf[1024];
	printf("[vagrant@localhost]$");
	fgets(cmd_buf, 1023, stdin);
	cmd_buf[strlen(cmd_buf) - 1] = '\0';
	char* argv[32] = {NULL};
	int argc = 0;
	char* ptr = cmd_buf;
	// 2.解析重定向
	int redirect_flag = 0;
	char* redirect_file = NULL;
	while(*ptr != '\0'){
		if(*ptr == '>'){  // 解析出'>',是清空重定向
			redirect_flag = 1;
			*ptr = '\0';
			ptr++;
		  if(*ptr == '>'){  // 再次解析出'>',是追加重定向
				redirect_flag = 2;
				*ptr = '\0';
				ptr++;
		}
		while(*ptr == ' ' && *ptr != '\0') ptr++; // 跳过重定向之后的空格字符,用于查找到file相关的字符
		redirect_file = ptr;
		while(*ptr != ' ' && *ptr != '\0') ptr++; // 将file相关的字符解析出来
		*ptr = '\0';
		}
		ptr++;
	}
	ptr = cmd_buf;
	// 3.对标准输入的命令行进行解析
	while(*ptr != '\0'){
		if(!isspace(*ptr)){
		argv[argc] = ptr;
		while(!isspace(*ptr) && *ptr != '\0'){
			ptr++;
		}
		*ptr = '\0';
		argc++;
		}
		ptr++;
	}
	int i;
	printf("命令行解析:\n");
	for(i = 0; argv[i] != NULL; ++i){
		printf("argv[%d]:%s\n", i, argv[i]);
	}
	// 3.创建子进程
	pid_t pid = fork();
	if(pid == 0){
		if(redirect_flag == 1){
		int fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		if(fd < 0){
			perror("open error");
			return -1;
		}
		dup2(redirect_flag, 1);
		}else if(redirect_flag == 2){
		int fd = open(redirect_file, O_WRONLY | O_CREAT | O_APPEND, 0664);
		if(fd < 0){
			perror("open error");
			return -1;
		}
		dup2(redirect_flag, 1);
	}
		// 4.子进程进行程序替换
		execvp(argv[0], argv);
		perror("execvp error");
		exit(0);
	}
	// 5.父进程等待子进程退出
	wait(NULL);
	return 0;
}