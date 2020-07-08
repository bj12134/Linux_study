#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

int main(){
  
	// 1.获取命令行标准输入
	char cmd_buf[1024];
	printf("[vagrant@localhost]$");
	fgets(cmd_buf, 1023, stdin);
	cmd_buf[strlen(cmd_buf) - 1] = '\0';
	// 2.对标准输入的命令行进行解析
	char* argv[32] = {NULL};
	int argc = 0;
	char* ptr = cmd_buf;
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
		// 4.子进程进行程序替换
		execvp(argv[0], argv);
		perror("execvp error");
		exit(0);
	}
	// 5.父进程等待子进程退出
	wait(NULL);
	return 0;
}