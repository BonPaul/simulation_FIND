#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>             // 提供open()函数  
#include <unistd.h>  
#include <dirent.h>            // 提供目录流操作函数  
#include <string.h>  
#include <sys/stat.h>        // 提供属性操作函数  
#include <sys/types.h>         // 提供mode_t 类型  
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>

#define FILEPATH_MAX (80)

char** touvename = NULL;
char **touveexec=NULL;
char *touvegroup = NULL;
int touveuid = 0;
char touvetype = 'f';
char *touvechemin;
int touveprint = 1;
int touvegid=0;
int touveperm = 0;
char *touveuser = NULL;
int touvetime=-1;
int c_time =0;
int m_time =0;
int a_time=0;
char *touveprune = NULL;
int permfalg = -1;
int mindepth = -1;
int maxdepth = 10000;
int flagsort = 0;
int flagls = 0;



int gettouvename(char *inputchar);
int gettouvetime(char *inputchar);
void gettouveuid(char *inputchar);
void gettouveexec(char **inputchar);
void gettouveperm(char *inputchar);
void gettouvechemin(char *inputchar);
void gettouvegid(char *inputchar);
void gettouveprune(char *inputchar);
void gettouvetype(char *inputchar);
int grouppanduan(struct stat statbuf);
void scan_dir(char *dir, int flag,char *parendir,int timeflag,int depth);
int strpanduan(char *wenjian,char **namezu,int flag);
int typepanduan(struct stat statbuf,char touvetype);
int uidpanduan(struct stat statbuf);
int userpanduan(struct stat statbuf);
int gidpanduan(struct stat statbuf);
int mtimepanduan(struct stat statbuf,int timeflag);
int ctimepanduan(struct stat statbuf,int timeflag);
int atimepanduan(struct stat statbuf,int timeflag);
int permpanduan(struct stat statbuf);
int geteight(int i);
int execute(struct dirent *entry,char *parendir);
void sortscan();




int main(int argc, char  **argv)
{
	int flag = 5;
  int timeflag;
	
	if (argv[1]==NULL)
	{
		printf("no command");
	}

	else if (strcmp(argv[1], "find")!=0)
	{
		printf("it's not the command find\n");
	}

	else
	{
		if (argv[2]==NULL)
		{
			printf("no chemin\n");
		}
		else
		{
			int i;
			gettouvechemin(argv[2]);
			for (i = 3; i < argc; ++i)
			{
				
				if (strcmp(argv[i], "-name")==0)
				{
					if (argv[i+1]==NULL)
					{
						printf("manquer la parametre\n");
					}
					else
					{
						flag = gettouvename(argv[i+1]);
						i = i+1;
					}					
				}
        else if (strcmp(argv[i], "-uid")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            gettouveuid(argv[i+1]);
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-mindepth")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            mindepth = atoi(argv[i+1]);
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-ls")==0)
        {
          
            flagls = 1;
        }
        else if (strcmp(argv[i], "-maxdepth")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            maxdepth = atoi(argv[i+1]);
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-exec")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            if (touveprint==1)
            {
              touveprint = 0;
            }
            gettouveexec(&argv[i+1]);
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-atime")==0||strcmp(argv[i], "-mtime")==0||strcmp(argv[i], "-ctime")==0)
        {
          if (strcmp(argv[i], "-atime")==0)
          {
            a_time = 1;
          }
          else if (strcmp(argv[i], "-ctime")==0)
          {
            c_time = 1;
          }
          else if (strcmp(argv[i], "-mtime")==0)
          {
            m_time = 1;
          }
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            timeflag=gettouvetime(argv[i+1]);
            i = i+1;
          }   

        }
        else if (strcmp(argv[i], "-gid")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            gettouvegid(argv[i+1]);
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-sort")==0)
        {
          
            flagsort = 1;
            
        }
        else if (strcmp(argv[i], "-perm")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            gettouveperm(argv[i+1]);
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-prune")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            touveprune = argv[i+1];
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-group")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            touvegroup = argv[i+1];
            i = i+1;
          }   
        }
        else if (strcmp(argv[i], "-user")==0)
        {
          if (argv[i+1]==NULL)
          {
            printf("manquer la parametre\n");
          }
          else
          {
            touveuser = argv[i+1];
            i= i+1;
          }   
        }
				else if (strcmp(argv[i], "-type")==0)
				{
					if (argv[i+1]==NULL)
					{
						printf("manquer la parametre\n");
					}
					else
					{
						gettouvetype(argv[i+1]);
						i = i+1;
					}		
				}
				else if (strcmp(argv[i], "-print")==0)
				{
					//printf("print\n");
					touveprint = 2;
				}
			}
		}	
		char *parendir=NULL;
    if (mindepth<=maxdepth&&flagsort==0)
    {
      scan_dir(touvechemin,flag,parendir,timeflag,1); 
    }
    else if (flagsort==1)
    {
      sortscan();
    }
	}

}

int gettouvename(char *inputchar)
	
{   
	int flag=5;

	if (inputchar[0]!='*'&&inputchar[strlen(inputchar)-1]!='*')
	{
		flag = 0;
	}
	else if (inputchar[0]!='*')
	{
		flag = 1;
	}
	else if (inputchar[strlen(inputchar)-1]!='*')
	{
		flag = 2;
	}
    char *delim = "*";
    char *p;
    int i = 1;
    touvename = (char**)malloc(20*sizeof(char*));
    touvename[0] = strtok(inputchar, delim);
   	while((p = strtok(NULL, delim)))
   	{
   		touvename[i] = p;
   		i++;
   	}
   	//printf("%s\n", touvename[0]);
   	return flag;

}

void gettouveexec(char **inputchar)
  
{   
  int i = 0,j = 0;
  touveexec = (char**)malloc(20*sizeof(char*));
  while(inputchar[i]!=NULL)
  {
    if (inputchar[i]==NULL)
    {
      printf("wrong\n");
      exit(1);
    }
    if (strcmp(inputchar[i],"{}"))
    {
      if (touveexec[j]==NULL)
      {
        touveexec[j] = (char *)malloc(200*sizeof(char));
      }
      strcat(touveexec[j]," ");
      strcat(touveexec[j],inputchar[i]);
    }
    else 
      j++;
    i++;
  }
}

void gettouvetype(char *inputchar)
{
	touvetype = inputchar[0];
}

void gettouveuid(char *inputchar)
{
  touveuid=atoi(inputchar);
}
void gettouveperm(char *inputchar)
{
  char *time;
  //printf("%c\n",inputchar[0]);
  if (inputchar[0]=='/')
  {
    if (inputchar[1]=='\0')
    {
      printf("illgle parametre");
      exit(1);
    }
    else 
    {
      permfalg = 1;
      time = &inputchar[1];
      touveperm=atoi(time);
    }
  }
  else if (inputchar[0]=='-')
  {
    if (inputchar[1]=='\0')
    {
      printf("illgle parametre");
      exit(1);
    }
    else 
    {
      permfalg = 2;
      time = &inputchar[1];
      touveperm=atoi(time);
    }
  }
  else if (inputchar[0]>='1'&&inputchar[0]<='9')
  {
    permfalg = 0;
    touveperm = atoi(inputchar);
  }
  else
  {
    printf("illegle parametre");
    exit(1);
  }
}

void gettouvegid(char *inputchar)
{
  touvegid=atoi(inputchar);
}

void gettouvechemin(char *inputchar)
{

	if (inputchar[0]=='.')
	{
    	touvechemin = (char *)malloc(FILEPATH_MAX);
    	getcwd(touvechemin,FILEPATH_MAX);
	}
	else
		touvechemin = inputchar;
}

int gettouvetime(char *inputchar)
{
  char *time;
  //printf("%c\n",inputchar[0]);
  if (inputchar[0]=='+')
  {
    if (inputchar[1]=='\0')
    {
      printf("illgle parametre");
      exit(1);
    }
    else 
    {
      time = &inputchar[1];
      touvetime=atoi(time);
    }
    return 1;
  }
  else if (inputchar[0]=='-')
  {
    if (inputchar[1]=='\0')
    {
      printf("illgle parametre");
      exit(1);
    }
    else 
    {
      time = &inputchar[1];
      touvetime=atoi(time);
    }
    return 2;
  }
  else if (inputchar[0]>='1'&&inputchar[0]<='9')
  {
    touvetime = atoi(inputchar);
    return 0;
  }
  else
  {
    printf("illegle parametre");
    exit(1);
  }
}

void sortscan()
{
  struct dirent **namelist;
  int i, total;
  total = scandir(touvechemin, &namelist, 0, alphasort);
  if(total < 0)
      perror("scandir");
  else
  {
    for(i = 2; i < total; i++)
    printf("%s\n", namelist[i]->d_name);
  }
}

void scan_dir(char *dir,int flag,char* parendir,int timeflag,int depth)   // 定义目录扫描函数  
{  
	  char p[200];
    DIR *dp;                      // 定义子目录流指针  
    struct dirent *entry;         // 定义dirent结构指针保存后续目录  
    struct stat statbuf;          // 定义statbuf结构保存文件属性  
    char *route = NULL;
    if((dp = opendir(dir)) == NULL) // 打开目录，获取子目录流指针，判断操作是否成功  
    {  
        puts("can't open dir.");  
        return;  
    }  
    chdir (dir);                     // 切换到当前目录 

    while((entry = readdir(dp)) != NULL)  // 获取下一级目录信息，如果未否则循环  
    {  
        lstat(entry->d_name, &statbuf); // 获取下一级成员属性  
        if(S_IFDIR &statbuf.st_mode)    // 判断下一级成员是否是目录  
        {  
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)  
              continue;  

          	if (parendir==NULL)
          	{
          		strcpy(p,dir);
          		strcat(p,"/");
           		strcat(p,entry->d_name);
           		strcat(p,"/");
         	  }
         	  else
          	{
          		strcpy(p,parendir);
          		strcat(p,entry->d_name);
          		strcat(p,"/");      		
          	}
            
              if (touvetype=='d'&&depth>mindepth)//pour affichier 
              {
                if (strpanduan(entry->d_name,touvename,flag)==1&&(uidpanduan(statbuf)==1)&&(userpanduan(statbuf)==1)&&(gidpanduan(statbuf)==1)&&(ctimepanduan(statbuf,timeflag)==1)&&(mtimepanduan(statbuf,timeflag)==1)&&(atimepanduan(statbuf,timeflag)==1)&&(grouppanduan(statbuf)==1)&&permpanduan(statbuf)==1)
                {
                  if (touveexec!=NULL)
                {

                  execute(entry,parendir);
                  
                }
                
                if (touveprint!=0)
                {

                  if (flagls==1)
                  {
                 // printf("%d\t",statbuf. st_ino);
                  printf("%d\t",statbuf.st_mode);
                  printf("%s\t",getpwuid(statbuf.st_uid)->pw_name);
                  struct tm *ptr;
                  ptr=localtime(&statbuf.st_atime);
                  printf("%s",asctime(ptr));
                  //printf("\n");
                  }

                  if (parendir==NULL)
                  {
                    
                    printf("%s/",dir);
                  }
                  else
                  {
                 
                    printf("%s",parendir);
                  }
                  printf("%s\n", entry->d_name);
                  }
                }
              }
            
            

            if (touveprune!=NULL)
            {

              if (strcmp(touveprune,p)!=0)
              {
                if (depth<=maxdepth)
                {
                  scan_dir(entry->d_name,flag,p,timeflag,depth+1);
                }
              }
            }
            else
            {  
              if (depth<=maxdepth)
              {
                scan_dir(entry->d_name,flag,p,timeflag,depth+1);
              }
            }  
            
                          // 递归调用自身，扫描下一级目录的内容  
        }  
        else if(typepanduan(statbuf,touvetype)==1&&(strpanduan(entry->d_name,touvename,flag)==1)&&(uidpanduan(statbuf)==1)&&(userpanduan(statbuf)==1)&&(gidpanduan(statbuf)==1)&&(ctimepanduan(statbuf,timeflag)==1)&&(mtimepanduan(statbuf,timeflag)==1)&&(atimepanduan(statbuf,timeflag)==1)&&(grouppanduan(statbuf)==1)&&permpanduan(statbuf)==1)
        {
          if (touveexec!=NULL)
          {
            
            execute(entry,parendir);
          }
          
          if (touveprint!=0)
          {
            if (flagls==1)
            {
                 // printf("%d\t",statbuf. st_ino);
              printf("%d\t",statbuf.st_mode);
              printf("%s\t",getpwuid(statbuf.st_uid)->pw_name);
              struct tm *ptr;
              ptr=localtime(&statbuf.st_atime);
              printf("%s",asctime(ptr));
              //printf("\n");
            }
            if (parendir==NULL)
            {
             printf("%s/",dir);

            }
            else
            {
              printf("%s",parendir);
            }
            printf("%s\n", entry->d_name);
          }
            
        }

        
       // 输出属性不是目录的成员  
    }  
    chdir("..");                                                  // 回到上级目录  
    closedir(dp);                                                 // 关闭子目录流  
}  

int strpanduan(char *wenjian,char **namezu,int flag)
{
	//printf("%s\n",wenjian );
  int i = 1;
  int j = 0;
  int num=0;
  char* strtem = NULL;
  if (namezu==NULL)
  {
    return 1;
  }
  while(namezu[num]){
    num++;
  }
  if (num==0)
  {
    return 1;
  }

  if (flag==1)//ne commence pas par * 
  {
    strtem =strstr(wenjian,namezu[0]);
    //printf("%s\n",strtem );
    if (strtem!=wenjian)
    {
      return 0;
    }
  }
  else if (flag==2)//ne finit pas par * 
  {
    strtem = strstr(wenjian,namezu[num-1]);
    if (strtem==NULL)
    {
      return 0;
    }
    else if (strcmp(strtem,namezu[num-1])!=0)
    {
      return 0;
    }
  }
  else if (flag==0)//ne commence pas par * et ne finit pas par * 
  {
    strtem =strstr(wenjian,namezu[0]);
    if (strtem!=wenjian)
    {
      return 0;
    }
    strtem = strstr(wenjian,namezu[num-1]);
    if (strtem==NULL)
    {
      return 0;
    }
    else if (strcmp(strtem,namezu[num-1])!=0)
    {
      return 0;
    }
  }
  while(namezu[j])
  {
    strtem = strstr(wenjian,namezu[j]);
    wenjian = strtem;
    j++;
    if (strtem==NULL)
    {
      i = 0;
      break;
    }
  }
  return i;
}

int execute(struct dirent *entry,char *parendir)
{
  char *command;
  int j = 0;
  while(touveexec[j])
  { 
    //strcat(command," ");
    command = touveexec[j];
    strcat(command," ");
    if (parendir==NULL)
    {
      strcat(command,touvechemin);
      strcat(command,"/");
    }
    else
      strcat(command,parendir);
    strcat(command,entry->d_name);
    system(command);
    j++;
  }
  return 1;
}

int typepanduan(struct stat statbuf,char touvetype)
{
  if (touvetype=='f')
  {
    return 1;    
  } 

  else if (touvetype=='l'&&(S_IFLNK &statbuf.st_mode))
  {

    if ((S_IFLNK &statbuf.st_mode)==S_IFLNK)
    {
      return 1; 
    }              
 
  }

  else if (touvetype=='b'&&(S_IFBLK &statbuf.st_mode))
  {

    if ((S_IFBLK &statbuf.st_mode)==S_IFBLK)
    {
      return 1; 
   }   
 
  }

  else if (touvetype=='c'&&(S_IFCHR&statbuf.st_mode))
  {

    if ((S_IFCHR&statbuf.st_mode)==S_IFCHR)
    {
      return 1;  
    }
           

  }

  else if (touvetype=='s'&&(S_IFSOCK &statbuf.st_mode))
  {
    if ((S_IFSOCK &statbuf.st_mode)==S_IFSOCK)
    {
      return 1; 
    }   
  
  }
  else if(touvetype!='s'&&touvetype!='l'&&touvetype!='f'&&touvetype!='b'&&touvetype!='c'&&touvetype!='d')
  {
    //printf("%c\n",touvetype );
   // printf("no this lettre\n");
    return 0;
  }
  else
  return 0;
}

int permpanduan(struct stat statbuf)
{
  if (permfalg==-1)
  {
    return 1;
  }
  
  else
  {
    int thisstat;
    double jj;

    thisstat = geteight(statbuf.st_mode);
    jj = (double)thisstat/1000;
    jj = jj-thisstat/1000;
    jj = jj*1000;
    thisstat = jj;
    if (permfalg==0)
    {
      if (thisstat==touveperm)
      {
        return 1;
      }
    }
    else if (permfalg==1)
    {
      if (thisstat&touveperm!=0||touveperm==0)
      {
        return 1;
      }
    }
    else if (permfalg==2)
    {
      if (thisstat&touveperm==touveperm)
      {
        return 1;
      }
    }
    else
      return 0;
  }
 
}

int uidpanduan(struct stat statbuf)
{
  if (touveuid!=statbuf.st_uid&&touveuid!=0)
  {
    return 0;
  }
  else
    return 1;
}


int ctimepanduan(struct stat statbuf,int timeflag)
{
  if (touvetime==-1||c_time==0)
  {
    return 1;
  }
  else 
  {
    int seconds,doseconds;
    seconds = time(NULL);
    doseconds = statbuf.st_ctime;
    if (timeflag==1&&(seconds-doseconds)>(touvetime*3600*24))
    {
      return 1;
    }
    else if (timeflag==2&&(seconds-doseconds)<(touvetime*3600*24))
    {
      return 1;
    }
    else if (timeflag==0&&(seconds-doseconds)<(touvetime*3600*24))
    {
      return 1;
    }
    else
      return 0;
  }
}
int atimepanduan(struct stat statbuf,int timeflag)
{
  if (touvetime==-1||a_time==0)
  {
    return 1;
  }
  else 
  {
    int seconds,doseconds;
    seconds = time(NULL);
    doseconds = statbuf.st_atime;
    if (timeflag==1&&(seconds-doseconds)>(touvetime*3600*24))
    {
      return 1;
    }
    else if (timeflag==2&&(seconds-doseconds)<(touvetime*3600*24))
    {
      return 1;
    }
    else if (timeflag==0&&(seconds-doseconds)<(touvetime*3600*24))
    {
      return 1;
    }
    else
      return 0;
  }
}
int mtimepanduan(struct stat statbuf,int timeflag)
{
  if (touvetime==-1||m_time==0)
  {
    return 1;
  }
  else 
  {
    int seconds,doseconds;
    seconds = time(NULL);
    doseconds = statbuf.st_mtime;
    if (timeflag==1&&(seconds-doseconds)>(touvetime*3600*24))
    {
      return 1;
    }
    else if (timeflag==2&&(seconds-doseconds)<(touvetime*3600*24))
    {
      return 1;
    }
    else if (timeflag==0&&(seconds-doseconds)<(touvetime*3600*24))
    {
      return 1;
    }
    else
      return 0;
  }
}

int userpanduan(struct stat statbuf)
{
  if (touveuser==NULL)
  {
    return 1;
  }
  struct passwd *user;
  user= getpwuid(statbuf.st_uid);
  if (strcmp(user->pw_name,touveuser)==0)
  {
    return 1;
  }
  else
    return 0;
}

int grouppanduan(struct stat statbuf)
{
  if (touvegroup==NULL)
  {
    return 1;
  }
  struct group *data;
  int i = 0;
  data = getgrgid(statbuf.st_gid);
  if (strcmp(data->gr_name,touvegroup)==0)
  {
    return 1;
  }
  else
    return 0;
}


int gidpanduan(struct stat statbuf)
{

  if (touvegid!=statbuf.st_gid&&touvegid!=0)
  {
    return 0;
  }
  else
  {

    return 1;
  }
    
}

int geteight(int i)
{
  int j = 0;
  if (i==0)
  {
    return 0;
  }
  else
  {
    j = geteight(i/8);
    j = j*10+i%8;
    return j;
  }
}