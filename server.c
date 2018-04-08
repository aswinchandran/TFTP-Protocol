
/******************************************************************************
**+-------------------------------------------------------------------------+**
**|                                 *******                                 |**
**|                                *********                                |**
**|                               ****   ****                               |**
**|                              **** *** ****                              |**
**|                             **** ***** ****                             |**
**|                            ****         ****                            |**
**|                           ****           ****                           |**
**|                                                                         |**
**|     Copyright (c) 2014 - 2018 Aswin Chadran                             |**
**|     Author: Aswin Chandran                                              |**
**|     Author details: https://github.com/aswinchandran                    |**
**|     Contact details: https://www.linkedin.com/in/aswin-chandran/        |**
**|                                                                         |**
**| This program is free software: you can redistribute it and/or modify    |**
**| it under the terms of the GNU General Public License as published by    |**
**| the Free Software Foundation, either version 3 of the License, or       |**
**| (at your option) any later version.                                     |**
**|                                                                         |**
**| This program is distributed in the hope that it will be useful,         |**
**| but WITHOUT ANY WARRANTY; without even the implied warranty of          |**
**| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           |**
**| GNU General Public License for more details.                            |**
**|                                                                         |**
**| You should have received a copy of the GNU General Public License       |**
**| along with this program.  If not, see <https://www.gnu.org/licenses/>   |**
**|                                                                         |**
**+-------------------------------------------------------------------------+**
******************************************************************************/
/**===========================================================================

	\file	: 
	\Author	: Aswin chandran
	\Date	: 10/02/2014
	\brief	: 
	\see	: 
  ==========================================================================*/


#include "server_header.h"



int main()
{
    printf("server\n");
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr; 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT_NO); 

	struct req_p rcv;
	struct ack_p ack;
	struct data_p dat;
	struct auth_client auth,temp;
DIR *d,*d1;
struct dirent *dir,*dir1;
d = opendir(".");

   	int b= bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	if(b!=-1)
	printf("\n\t\t\tbinded\n");
    	listen(listenfd, 10); 

	int ch,n=1;
	char uname[20],pswd[20];
	int flag1=0,qq=1,fd,flag=1;
 	char buff[512],b1[30];    


	printf("connecting for clients...\n");
	struct sockaddr_in cli_addr; 
	int l=sizeof(cli_addr);
        int connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &l); 
	printf("\n\tconnect fd=%d\n",connfd);
	while(1)
	{
		while(flag)
		{
		flag=1;
        	int r=read(connfd,&ch,sizeof(ch));
		printf("%d\n",ch);
		FILE *fp=fopen("psswd","a+");
		switch(ch)
		{
		case 1:
			printf("sign in options\n");
		        r=read(connfd,temp.uname,20);
			temp.uname[r]='\0';
		//	puts(temp.uname);
		        r=read(connfd,temp.pswd,20);
			temp.pswd[r]='\0';
		//	puts(temp.pswd);
			while((fread(&auth,sizeof(struct auth_client),1,fp))!=0)
			{
			//printf("n is %s\n",auth.uname);
			//printf("p is %s\n",auth.pswd);

				if((strcmp(temp.uname,auth.uname))==0)
				{
					if((strcmp(temp.pswd,auth.pswd))==0)
					{
						r=write(connfd,"account matching",20);
						flag=0;
					//	printf("if n is %s\n",auth.uname);
						n=0;
						break;
					}
				}
			}
			if(n)
			{
				r=write(connfd,"account not matching",20);
				write(connfd,(int *)flag,sizeof(int));
			}
			fclose(fp);
			break;
		case 2:
			printf("sign up options\n");
		        r=read(connfd,temp.uname,20);
			temp.uname[r]='\0';
			puts(temp.uname);
		        r=read(connfd,temp.pswd,20);
			temp.pswd[r]='\0';
			puts(temp.pswd);
			 flag=0;
			while((fread(&auth,sizeof(struct auth_client),1,fp))!=0)
			{
				//printf("n is %s\n",auth.uname);
				//printf("p is %s\n",auth.pswd);
				flag=0;
				if((strcmp(temp.uname,auth.uname))==0)
				{
				
					r=write(connfd,"user already exists",20);
				//	continue;
					flag=1;
					printf("if n is %s\n",auth.uname);
					//printf("if p is %s\n",auth.pswd);
					break;

				}
			}
			if(flag==0)
			{
				r=write(connfd,"welcome new user",20);
				fwrite(&temp,sizeof(struct auth_client),1,fp);
				fclose(fp);
			}
			break;
		default :
			printf("client is disconnected\n");
			exit(0);
	
		}
		write(connfd,&flag,4);    
	}

	int r=read(connfd,&rcv,sizeof(rcv));
	printf("\n\t\t\tTFTP REQUEST PACKET recieved r=%d\n\t\t\tREQUEST FRAME CONTENTS ARE\n",r);
	
	printf("\n\topcode=%d",rcv.opc);
	printf("\n\tFile name=%s",rcv.file);
	printf("\n\tmid=%d",rcv.mid);
	printf("\n\tmod=%s\n",rcv.mode);
	
	switch(rcv.opc)
	{
	case 1:	printf("\nread rqst rcvd\n");
		d = opendir(".");
  		if (d)
  		{
   	 		while ((dir = readdir(d)) != NULL)
    			{
				flag1=0;
				if(strcmp(rcv.file,dir->d_name)==0)
				{
					write(connfd,&flag1,sizeof(flag1));	
					printf("\n\t\t\tFile found with name : %s",rcv.file);

					flag1=1;		
					fd=open(rcv.file,O_RDONLY,0666);
					char ch;
					int p=1,r1;
					__fpurge(stdin);
					printf("\t file %s is sending",rcv.file);
					while(r1=read(fd,dat.data,512))
					{
						printf(".");
						dat.data[r1]='\0';
						dat.opc=3;
						dat.blk=p;
						dat.size=r1;
						write(connfd,&dat,sizeof(dat)); 
						p++;
					}
					__fpurge(stdin);
					dat.opc=3;
					dat.blk=0;
					dat.size=0;
					write(connfd,&dat,sizeof(dat));
					printf("\nfile send\n");
					flag1=1;
   			 	//	closedir(d); 
					break;
   				}
			}
		//	closedir(d); 
		}
		if(flag1==0)
		{
			write(connfd,&flag1,sizeof(flag1));	

		}
		closedir(d);
		break;

	case 2:	printf("\nwrite rqst rcvd\n");
		__fpurge(stdin);
		ack.opc=4;
		ack.blk=0;
		int x=write(connfd,&ack,sizeof(ack));
		int n1;
		fd=open(rcv.file,O_CREAT|O_RDWR|O_TRUNC,0666);
		printf("file %s is receiving",rcv.file);
		while(read(connfd,&dat,sizeof(dat)))
		{
			printf(".");
			if(dat.size==0)
			{
				printf("\nfile received\n");
				close(fd);
				break;
			}
			n1=write(fd,dat.data,dat.size);
		}
		break;

	case 5: 
		d = opendir(".");
		if (d)
  		{
    			while ((dir1 = readdir(d)) != NULL)
    			{
				write(connfd,&dir->d_type,sizeof(dir->d_type));
    			}
			write(connfd,"end 0",10);
       		}
		closedir(d);
		break;
	}
	}
}
