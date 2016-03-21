#include<stdio.h> 
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include <stdlib.h>
#include<string.h>
#include<signal.h>
int p_num[100];
char *p_str[100];
int p_flag[100];
int p_flag1[100];
int pcount=1;
int count=0;
char *hist[100];
pid_t global_pid;
char q[100];
char *p;
char r[100];
char path[100];
int uni_flag=0;
int bg_flag=0;
void handler(int sig)
{
	wait(NULL);
	  //if(pid!=-1)
	printf("Pid exited.\n");
	printf("\n<%s@%s:%s> ",p,q,r);
}

void  parse(char *line, char **argv)
{
	while (*line != '\0') {     
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     
		*argv++ = line;          
		while (*line != '\0' && *line != ' ' && 
				*line != '\t' && *line != '\n') 
			line++;             
	}
	*argv = '\0';                 
}

void handle_signal(int signo)
{
	printf("\n<%s@%s:%s> ",p,q,r);
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	signal(SIGTERM, SIG_IGN);
	getcwd(path,100);
	strcpy(r,"~");
	int b;
	for(b=0;b<100;b++)
	{
		//hflag[b]=0;
		p_flag[b]=0;
		p_flag1[b]=0;
		//pipelen[b]=0;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
	//signal(SIGCHLD, SIG_IGN);
	//signal(SIGCHLD, handle_signal);
	signal(SIGTERM, handle_signal);

	pid_t pid=fork();
	if(pid==-1)
	{
		fprintf(stderr,"Error in creating Fork %d\n",errno);
		exit(-1);
	}
	if(pid==0)
	{
		execlp("/usr/bin/clear","clear",NULL);
	}
	else
		wait(NULL);
	p=getenv("USER");
	int n=gethostname(q,100);
	printf("\n<%s@%s:%s> ",p,q,r);



	char c = '\0';
	char tmp[100]="";
	while(1)
	{	
		c = getchar();
			if(c==EOF)
				continue;
			if(c=='\n'){
				//printf("%s\n",tmp);
				if (strcmp(tmp,"")==0)
				{
					//printf("hello\n");
			        	printf("<%s@%s:%s> ",p,q,r);
					continue;
				}
				char *argv[64];
				parse(tmp,argv);
				int len=0;
				while(argv[len]!=NULL)
					len++;

				//printf("%d\n",len);	
				hist[count]=strdup(argv[0]);
				if(len==2)
				{
					strcat(hist[count]," ");
					strcat(hist[count],argv[1]);
				}
				p_str[pcount]=strdup(argv[0]);
				//printf("ihist %s\n",hist[count]);
				count++;
			//	printf("len = %d\n",len);
			//	printf("%s\n",argv[4]);
				int e=0;
				int pipes=0;
				int pipe_flag=0;
				for(e=0;e<len;e++)
				{
					if(strcmp(argv[e],"|")==0)
					{
						pipe_flag=1;
						pipes++;
					}
					if(strcmp(argv[e],"<")==0)
						pipe_flag=1;
					if(strcmp(argv[e],">")==0)
						pipe_flag=1;
				}
			//	printf("| %d\n",pipes);
			//	printf("<> %d\n",pipe_flag);
				if(pipe_flag==1)
				{
					//hflag[count-1]=4;
					int g=0;
					for (g=1;g<len;g++)
					{
						hist[count-1]=strcat(hist[count-1]," ");
						hist[count-1]=strcat(hist[count-1],argv[g]);
					}
				//	printf("%s\n",hist[count-1]);
				//	pipelen[count-1]=len;
					int fd[10];
					int n5;
					char line[100];
					pipe(fd);
					pipe(fd+2);
					pipe(fd+4);
					pipe(fd+6);
					pipe(fd+8);
					int k5=0;
					int i5;
					int x5=len;
					for(i5=0;i5<x5;i5++)
					{
						//printf("hi\n");
						if((strcmp(argv[i5],"|")==0)||(strcmp(argv[i5],"<")==0))
							k5=k5+2;
						if(i5==0 && (strcmp(argv[0],"hist")==0) && ((strcmp(argv[i5+1],"|")==0)||(strcmp(argv[i5+1],">")==0)))
						{
							p_str[pcount]=strdup(argv[0]);
							FILE *ffp=fopen("hist","w");
							int sr=0;
							char *s[20];
							//printf("%d\n",count);
							for(sr=0;sr<count-1;sr++)
								fprintf(ffp,"%s\n",hist[sr]);
							fclose(ffp);
							p_num[pcount++]=getpid();
							FILE* file1 = fopen("hist", "r");
							char line1[1000];
							//close(fd[0]);
							n = read(fileno(file1), line1, 1000);
							write(fd[1], line1, n);
							fclose(file1);
							//n = read(fd[0], line1, 1000);
							//write(STDOUT_FILENO, line1, n);
							remove("hist");
							continue;

						}
						if(i5==0 && ((strcmp(argv[i5+1],"<")==0))&& x5==3)
						{
							p_str[pcount]=strdup(argv[0]);
							char *s[10];
							int j5=0;
							while((strcmp(argv[i5],"|")!=0)&&(strcmp(argv[i5],"<")!=0))
							{
								s[j5++]=strdup(argv[i5]);
								i5=i5+1;
							}
							//k=k+2;
							i5=i5+1;
							FILE *chk = fopen(argv[i5],"r");
							if( chk ) 
							{
								//printf("exists\n");
								fclose(chk);
							} 
							else
							{
								printf("File %s doesnt exist!\n",argv[i5]);
								break;
							}
						//	printf("i5= %d\n",i5);
						//	printf("%s\n",argv[i5]);
							s[j5]=NULL;
							//printf("%s\n",s[0]);
							pid_t pid;
							if ((pid = fork()) < 0)
								perror("fork error\n");
							if(pid==0)
							{
								FILE* file1 = fopen(argv[i5], "r");
								dup2(fileno(file1),0);
								//dup2(fd[1],1);
								//close(fd[0]);
								execvp(*s,s);
								_exit(0);
							}
							else
							{
								p_num[pcount++]=getpid();
								//close(fd[1]);
								wait(NULL);
							}
						}
						else if(i5==0 && ((strcmp(argv[i5+1],"<")==0) ))
						{
							p_str[pcount]=strdup(argv[0]);
							char *s[10];
							int j5=0;
							while((strcmp(argv[i5],"|")!=0)&&(strcmp(argv[i5],"<")!=0))
							{
								s[j5++]=strdup(argv[i5]);
								i5=i5+1;
							}
							//k=k+2;
							i5=i5+1;
							FILE *chk = fopen(argv[i5],"r");
							if( chk ) 
							{
								//printf("exists\n");
								fclose(chk);
							} 
							else
							{
								printf("File %s doesnt exist!\n",argv[i5]);
								break;
							}
							//	printf("i5= %d\n",i5);
							//	printf("%s\n",argv[i5]);
							s[j5]=NULL;
							//	printf("%s\n",s[0]);
							pid_t pid;
							if ((pid = fork()) < 0)
								perror("fork error\n");
							p_num[pcount++]=getpid();
							if(pid==0)
							{
								FILE* file1 = fopen(argv[i5], "r");
								dup2(fileno(file1),0);
								dup2(fd[1],1);
								close(fd[0]);
								execvp(*s,s);
								_exit(0);
							}
							else
							{
								//close(fd[1]);
								//printf("ok\n");
								wait(NULL);
							}
						}
						if(i5==0 && (strcmp(argv[i5+1],"<")!=0) )
						{
							p_str[pcount]=strdup(argv[0]);
							char *s[10];
							int j5=0;
							while((strcmp(argv[i5],"|")!=0)&&(strcmp(argv[i5],">")!=0))
							{
								s[j5++]=strdup(argv[i5]);
								i5=i5+1;
							}
							i5=i5-1;
						//	printf("i5= %d\n",i5);
							s[j5]=NULL;
						//	printf("%s\n",s[0]);
						//	printf("%s\n",s[1]);
							pid_t pid;
							if ((pid = fork()) < 0)
								perror("fork error\n");
							p_num[pcount++]=getpid();
							if(pid==0)
							{
								close(fd[0]);
								dup2(fd[1],1);
								//close(fd[1]);
								execvp(*s,s);
								_exit(0);
							}
							else
							{
						//		printf("ok\n");
							//n5 = read(fd[0], line, 100);
							//write(STDOUT_FILENO, line, n5);
								wait(NULL);
							}
						}
						if((strcmp(argv[i5],"|")==0) && ((i5==x5-2)||(i5==x5-3)))
						{

							p_str[pcount]=strdup(argv[i5+1]);
							//printf("%s\n",argv[0]);
							//printf("%s\n",argv[1]);
						//	printf("hello\n");
							i5=i5+1;
					//		printf("last i5= %d in |\n",i5);
							char *s[10];
							int j5=0;
							while(i5!=x5)
							{
								//printf("%s\n",argv[i5]);
								s[j5++]=strdup(argv[i5]);
								i5=i5+1;
							}
							i5=i5-1;
							s[j5]=NULL;
						//	printf("%s\n",s[0]);
				//	printf("%s\n",s[1]);

							pid_t pid1;
							if ((pid1 = fork()) < 0)
								perror("fork error\n");
							p_num[pcount++]=getpid();
							if(pid1==0)
							{
								//dup2(fd[k+1],1);
								close(fd[k5]);
								dup2(fd[k5-2],0);
								close(fd[k5-1]);
								execvp(*s,s);
								_exit(0);
							}
							else
							{
								close(fd[k5-1]);
								wait(NULL);
							}
						}
						else if(strcmp(argv[i5],"|")==0)
						{
							p_str[pcount]=strdup(argv[i5+1]);
						//	printf("hello\n");
							//n = read(fd[0], line, 100);
							//write(STDOUT_FILENO, line, n);
							i5=i5+1;
						//	printf("i5= %d in |\n",i5);
						//	printf("k= %d in |\n",k5);
							char *s[10];
							int j5=0;
							while((strcmp(argv[i5],"|")!=0)&&(strcmp(argv[i5],">")!=0))
							{
								s[j5++]=strdup(argv[i5]);
								i5=i5+1;
							}
						//	printf("%s\n",s[0]);
							i5=i5-1;
							s[j5]=NULL;
						//	printf("%s\n",s[1]);
							pid_t pid1;
							if ((pid1 = fork()) < 0)
								perror("fork error\n");
							p_num[pcount++]=getpid();
							if(pid1==0)
							{
								dup2(fd[k5+1],1);
								close(fd[k5]);
								dup2(fd[k5-2],0);
								close(fd[k5-1]);
								execvp(*s,s);
								_exit(0);
							}
							else
							{
								close(fd[k5-1]);
							//n5 = read(fd[k5], line, 100);
							//write(STDOUT_FILENO, line, n5);
								wait(NULL);
							}
						}
						else if((strcmp(argv[i5],">")==0) && (i5==x5-2))
						{
							//	printf("k5=%d\n",k5);
							//	printf("i5=%d\n",i5);
							//	printf("%s\n",argv[i5+1]);
							FILE *chk = fopen(argv[i5+1],"w");
							close(fd[k5+1]);
							n5 = read(fd[k5], line, 100);
							write(fileno(chk), line, n5);
							fclose(chk);
						}
					}







				}
				else if(len==1 && strcmp(argv[0],"pid")==0)
				{
					p_num[pcount++]=getpid();
					int x=getpid();
					printf("command name: ./a.out process id: %d\n",x);
				}
				else if((strcmp(argv[0],"cd")==0 && len==1)||(strcmp(argv[0],"cd")==0 && strcmp(argv[1],"~")==0))
				{
					p_num[pcount++]=getpid();
					strcpy(r,"~");
					getcwd(path,100);
					chdir(path);

				}
				else if(strcmp(argv[0],"cd")==0 && len!=1)
				{
					p_num[pcount++]=getpid();
					int flag=0;
					if(strcmp(argv[1],"..")==0)
					{
						int len=strlen(r);
						int len1=strlen(path);
						int z;
						for(z=len-1;z>=0;z--)
							if(r[z]=='/')
							{
								r[z]='\0';
								flag=1;
								break;
							}
						for(z=len1-1;z>=0;z--)
							if(path[z]=='/')
							{
								path[z]='\0';
								break;
							}
						
					}
					if(flag==0)
					{
					strcat(r,"/");
					strcat(r,argv[1]);
					strcat(path,"/");
					strcat(path,argv[1]);
					}
					chdir(path);
					
				}
				else if(strcmp(argv[0],"pid")==0 && strcmp(argv[1],"all")==0)
				{
				//	hflag[count-1]=1;
					p_flag1[pcount]=1;
					strcat(p_str[pcount]," ");
					strcat(p_str[pcount],"all");
					p_num[pcount++]=getpid();
					//strcpy(store1,"all");
					printf("List of all processes spawned from this shell:\n");
					int m;
					for(m=1;m<pcount-1;m++)
						printf("command name: %s process id: %d\n",p_str[m],p_num[m]);
				}
				else if(strcmp(argv[0],"pid")==0 && strcmp(argv[1],"current")==0)
				{
					//hflag[count-1]=2;
					p_flag1[pcount]=2;
					strcat(p_str[pcount]," ");
					strcat(p_str[pcount],"current");
					p_num[pcount++]=getpid();
					//strcpy(store2,"current");
					printf("List of current executing processes spawned from this shell:\n");
					int m;
					for(m=1;m<pcount;m++)
						if(p_flag[m]==1)
							printf("command name: %s process id: %d\n",p_str[m],p_num[m]);
				}
				else if(len==2 && strcmp(argv[1],"&")==0)
				{
					p_flag[pcount]=1;
					p_str[pcount]=strdup(argv[0]);
					argv[1]=NULL;
					pid_t pid2;
					pid2=fork();
					bg_flag=0;
					if (pid2==0) 
					{
						pid_t pid3;
						pid3=fork();
						//global_pid=pid3;
						p_num[pcount++]=pid3;
						//printf("%d\n",global_pid);
						if (pid3==0)
						{
							execvp(*argv,argv);

						}
						else
						{

							wait(pid3,NULL,0);
							bg_flag=1;
							printf("\n%s %d exited normally\n",argv[0],p_num[pcount-1]);
							int c;
							for(c=0;c<pcount;c++)
								if(p_num[c]==global_pid)
								{
									p_flag[c]=0;
									break;
								}
						}
						//p_num[pcount++]=pid3;
						//_exit(0);
					}
					if(bg_flag==0)
					printf("command %s pid %d\n",argv[0],p_num[pcount-1]);
					
				}

				else if(argv[0][0]=='h' && argv[0][3]=='t' && strlen(argv[0])==5)
				{
					p_num[pcount++]=getpid();
					//printf("hello\n");
					int num=argv[0][4]-48;
					//printf("%d\n",num);
					int hist_count=count-num-1;
					int z=0,j=1;
					for(z=hist_count;z<count-1;z++)
						printf("%d. %s\n",j++,hist[z]);

				}
				else if (strlen(argv[0])==6 && argv[0][0]=='!')
				{
					p_num[pcount++]=getpid();
					//printf("hello\n");
					int num=argv[0][5]-48;
					printf("%s\n",hist[num-1]);

				}
				else if(strlen(argv[0])==4 && strcmp(argv[0],"hist")==0 && len==1)
				{
					p_num[pcount++]=getpid();
					int z=0;
					for(z=0;z<count-1;z++)
						printf("%d. %s\n",z+1,hist[z]);
				}
				else if(strcmp(argv[0],"quit")==0)
				{
					break;
					uni_flag=1;
				}
				else 
				{
					if (strcmp(argv[0],"ls")==0)
					{
						argv[1]=strdup(path);
					argv[2]=NULL;
					}
					pid_t pid1=fork();
					p_num[pcount++]=pid1;
					if(pid1==-1)
					{
						fprintf(stderr,"Error in creating Fork %d\n",errno);
						exit(-1);
					}
					if (pid1==0)
					{
						if(execvp(*argv,argv))
						{
							char s[10]="";
							char *p=s;
							perror(p);
							_exit(0);
						}
					}
					else
						wait(NULL);
				}
			        printf("<%s@%s:%s> ",p,q,r);
				bzero(tmp, sizeof(tmp));}
			else{ strncat(tmp, &c, 1);
		}

	}
	//printf("\n");
	return 0;

}
