
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


#include "client_header.h"

int main()
{
	system("clear");
	struct sockaddr_in serv_addr; 
	struct req_p snd;
	struct ack_p ack;
	struct data_p dat;

	DIR *d,*d1;
	struct dirent *dir,*dir1;
	d = opendir(".");
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
        	return 1;
	} 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT_NO); 
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
	int cl=connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(cl==-1)
    	{
		printf("\n Error : Connect Failed \n");
		return 1;
   	} 

	int i;
	char recvbuff[10],choice;
	char file[20],b1[30];
	int flag=0,flag1=0,r1,cha,r2=1;
	int d2=1;
	char c[5];
	printf("client1\n");
	printf("welcome\n");
	while(d2)
	{
		int ch,r;
		char uname[20],pswd[20];
		printf("please enter 1 : signin		2 : signup\n");
		scanf("%d",&ch);
		write(sockfd,&ch,sizeof(ch));	
		if(ch==1||ch==2)
		{
			printf("Enter the Username\n");	
			scanf("%s",uname);
			write(sockfd,uname,10);	
			printf("Enter the Password\n");
			scanf("%s",pswd);
			write(sockfd,pswd,10);	
			r=read(sockfd,uname,20);
			uname[r]='\0';
			puts(uname);
			r=read(sockfd,&d2,4);
		}
	else
		{
			printf("invalid option\n");
		}
	}

//	printf("your signed in\n");
/*AUTHENTICATION PART HERE
IF ACCESS IS GRANTED GO TO BELOW CODE*/
	while(1)
	{
		printf("\n\t\t\t==================================================\n\t\t\t==\t\t\t\t\t\t==\n\t\t\t==     TRIVIAL FILE TRANSFER PROTOCOL(TFTP)     ==\n\t\t\t==\t\t\tCLIENT\t\t\t==\n\t\t\t==================================================\n\n\t\t\t1) RECIEVE FILE\n\t\t\t2) SEND FILE\n\t\t\t3) HELP\n\t\t\t4) EXIT\n\n\t\t\t       Enter your choice:");
		__fpurge(stdin);
		scanf("%c",&choice);
		switch(choice)
		{
		case '1':printf("\nrecv \n");
			printf("\n\t\tEnter filename to be rcv:");
			__fpurge(stdin);
			scanf("%s",snd.file);
			snd.opc=1;
	 		snd.mid=0;	
	 		strcpy(snd.mode,"netasci");
			snd.end=0;

			int x=write(sockfd,&snd,sizeof(snd));
			printf("\n\t\t\tTFTP READ REQUEST send\n");

			read(sockfd,&r2,sizeof(r2));
			if(r2==0)
			{
				printf("file name not found\n");
				break;
			}
			int n1;
			int fd=open(snd.file,O_CREAT|O_RDWR|O_TRUNC,0666);
			printf("\tfile %s is receiving",snd.file);
			while(read(sockfd,&dat,sizeof(dat)))
			{
				printf(".");
				if(dat.opc==0)
				{
					read(sockfd,&dat,sizeof(dat));
					printf("\t %s \n",dat.data);
					break;
				}

				else if(dat.size==0)
				{
					printf("\nfile received\n");
					close(fd);
					break;
				}
				n1=write(fd,dat.data,dat.size);
			}
			break;

		case '2':printf("\n\t\tEnter filename to be send:");
			__fpurge(stdin);
			scanf("%s",snd.file);
			d = opendir(".");
			if (d)
  			{
	    			while ((dir = readdir(d)) != NULL)
		    		{
					flag=0;
					if(strcmp(snd.file,dir->d_name)==0)
					{
						printf("\n\t\t\tFile found with name : %s",snd.file);
			 			snd.opc=2;
						snd.mid=0;	
						strcpy(snd.mode,"netasci");
						snd.end=0;
						int x=write(sockfd,&snd,sizeof(snd));
					printf("\n\t\t\tTFTP WRITE REQUEST send x=%d\n",x);
						int r=read(sockfd,&ack,sizeof(ack));
						printf("\nACK recieved\n");
						if(ack.blk!=0)
						{
							printf("\n File send Request rejected\n");
							break;
						}
						int fd=open(snd.file,O_RDONLY,0666);
						char ch;
						int p=1,r1;
						__fpurge(stdin);
						printf("\n file %s is sending",snd.file);
						while(r1=read(fd,dat.data,512))
						{
							printf(".");
							dat.data[r1]='\0';
							dat.opc=3;
							dat.blk=p;
							dat.size=r1;
							write(sockfd,&dat,sizeof(dat)); 
							p++;	
						}
						dat.opc=3;
						dat.blk=p;
						dat.size=0;
						write(sockfd,&dat,sizeof(dat));
						printf("\nfile send\n");
						flag=1;
						break;
				    		closedir(d);   
					}
    				}
			}
			if(flag==0)
			{
				d = opendir(".");
				printf("\n\t\t\tFilename you entered not found !,,Try again with anyone file from list..\n");
				if (d)
		  		{
		    			while ((dir1 = readdir(d)) != NULL)
		    			{
				 		if (dir1->d_type == DT_REG)
						printf("\n\t\t\t%s", dir1->d_name);
		    			}
					__fpurge(stdin);
		    			closedir(d);
					flag=1;
		       		}
			}

			break;

		case '3':printf("\n TFTP \n");
			snd.opc=5;
	 		snd.mid=0;	
	 		strcpy(snd.mode,"netasci");
			snd.end=0;
			strcpy(snd.file,"help");
			write(sockfd,&snd,sizeof(snd));
			d = opendir(".");
			printf("\n\t\t\tFiles present in clients are \n");
			if (d)
	  		{
	    			while ((dir1 = readdir(d)) != NULL)
	    			{
			 		if (dir1->d_type == DT_REG)
					printf("\n\t\t\t%s", dir1->d_name);
	    			}
	    			closedir(d);
	       		}
	
			break;
		case '4':printf("\n   Good bye ....\n");
			close(sockfd);
			exit(0);

		default:printf("\n\t\t\tInvalid choice !!");
			break;
		}
	}

    return 0;
}
