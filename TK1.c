#include<stdio.h>
#define residents 108

typedef struct DATA{
    int room_num;
    int floor;
    int exemption;
}roomdata;

void turn1(int anum,int dnum,int fnum,int snum,int turn,int last1,int ninsu[][anum],roomdata data[],const char fname[])
{
    FILE *fp;
    int l;
    int c=0;
    for(int i=anum-1;i>0;i--)
        {
            printf("%d\t",ninsu[0][i]);
        }
        printf("%d\n",ninsu[0][0]);
    for(int i=1;i<dnum;i++)
    {
        for(int j=0;j<anum-1;j++)
        {
            printf("#\t");
        }
        printf("#\n");
        for(int j=0;j<anum-1;j++)
        {
            printf(":\t");
        }
        printf(":\n");
        if(i==1)
        {
            l=last1+1;
        }
        if(i<snum)   //ここ変えた
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            for(int j=anum-1;j>0;j--)
            {
                printf("%d\t",ninsu[i][j]);
            }
            printf("%d\n",ninsu[i][0]);
        }
        else
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            if((snum>0)&&((ninsu[i][1]==ninsu[0][1])||(((ninsu[i][1]/100)!=fnum)&&(ninsu[i][1]!=ninsu[snum-1][0]))))
                {
                    ninsu[i][0]=ninsu[c][0];
                    c++;
                    for(int j=anum-1;j>0;j--)
                    {
                        printf("%d\t",ninsu[i][j]);
                    }
                    printf("%d\n",ninsu[i][0]);
                }
            else
            {
                while(1)
                {
                    if(((data[l].floor)==fnum)&&((data[l].exemption)>0)&&(((data[l].room_num)/100)==fnum))
                    {
                        data[l].exemption--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents-1;k++)
                            {
                                fprintf(fp,"%d %d %d\n",data[k].room_num,data[k].floor,data[k].exemption);
                            }
                            fclose(fp);
                        }
                    }
                    else if(((data[l].floor)==fnum)&&((data[l].exemption)==0)&&(((data[l].room_num)/100)==fnum))
                    {
                        ninsu[i][0]=data[l].room_num;
                        l++;
                        if(l>=residents)
                        {
                            l=0;
                        }
                        break;
                    }
                    l++;
                    if(l>=residents)
                    {
                        l=0;
                    }
                }
                for(int j=anum-1;j>0;j--)   //ここ変える
                {
                    printf("%d\t",ninsu[i][j]);
                }
                printf("%d\n",ninsu[i][0]);
            }
        }
    }
}

void turn2(int anum,int dnum,int fnum,int snum,int turn,int last1,int ninsu[][anum],roomdata data[],const char fname[])
{
    FILE *fp;
    int l;
    int c=0;
    for(int i=anum-1;i>0;i--)
        {
            printf("%d\t",ninsu[0][i]);
        }
        printf("%d\n",ninsu[0][0]);
    for(int i=1;i<dnum;i++)
    {
        for(int j=0;j<anum-1;j++)
        {
            printf("#\t");
        }
        printf("#\n");
        for(int j=0;j<anum-1;j++)
        {
            printf(":\t");
        }
        printf(":\n");
        if(i==1)
        {
            l=last1-1;
        }
        if(i<snum)   //ここ変えた
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            for(int j=anum-1;j>0;j--)
            {
                printf("%d\t",ninsu[i][j]);
            }
            printf("%d\n",ninsu[i][0]);
        }
        else
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            if((snum>0)&&((ninsu[i][1]==ninsu[0][1])||(((ninsu[i][1]/100)!=fnum)&&(ninsu[i][1]!=ninsu[snum-1][0]))))
                {
                    ninsu[i][0]=ninsu[c][0];
                    c++;
                    for(int j=anum-1;j>0;j--)
                    {
                        printf("%d\t",ninsu[i][j]);
                    }
                    printf("%d\n",ninsu[i][0]);
                }
            else
            {
                while(1)
                {
                    if(((data[l].floor)==fnum)&&((data[l].exemption)>0)&&(((data[l].room_num)/100)==fnum))
                    {
                        data[l].exemption--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents-1;k++)
                            {
                                fprintf(fp,"%d %d %d\n",data[k].room_num,data[k].floor,data[k].exemption);
                            }
                            fclose(fp);
                        }
                    }
                    else if(((data[l].floor)==fnum)&&((data[l].exemption)==0)&&(((data[l].room_num)/100)==fnum))
                    {
                        ninsu[i][0]=data[l].room_num;
                        l--;
                        if(l<0)
                        {
                            l=residents-1;
                        }
                        break;
                    }
                    l--;
                    if(l<0)
                    {
                        l=residents-1;
                    }
                }
                for(int j=1;j<anum;j++)
                {
                    ninsu[i][j]=ninsu[i-1][j-1];
                }
                for(int j=anum-1;j>0;j--)   //ここ変える
                {
                    printf("%d\t",ninsu[i][j]);
                }
                printf("%d\n",ninsu[i][0]);
            }
        }
    }
}

void turn3(int anum,int dnum,int fnum,int snum,int turn,int last1,int ninsu[][anum],roomdata data[],const char fname[])
{
    FILE *fp;
    int l;
    int c=0;
    for(int i=0;i<anum-1;i++)
        {
            printf("%d\t",ninsu[0][i]);
        }
        printf("%d\n",ninsu[0][anum-1]);
    for(int i=1;i<dnum;i++)
    {
        for(int j=0;j<anum-1;j++)
        {
            printf("#\t");
        }
        printf("#\n");
        for(int j=0;j<anum-1;j++)
        {
            printf(":\t");
        }
        printf(":\n");
        if(i==1)
        {
            l=last1+1;
        }
        if(i<snum)   //ここ変えた
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            for(int j=0;j<anum-1;j++)   //ここ変える
            {
                printf("%d\t",ninsu[i][j]);
            }
            printf("%d\n",ninsu[i][anum-1]);
        }
         else
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            if((snum>0)&&((ninsu[i][1]==ninsu[0][1])||(((ninsu[i][1]/100)!=fnum)&&(ninsu[i][1]!=ninsu[snum-1][0]))))
                {
                    ninsu[i][0]=ninsu[c][0];
                    c++;
                    for(int j=0;j<anum-1;j++)   //ここ変える
                    {
                        printf("%d\t",ninsu[i][j]);
                    }
                    printf("%d\n",ninsu[i][anum-1]);
                }
            else
            {
                while(1)
                {
                    if(((data[l].floor)==fnum)&&((data[l].exemption)>0)&&(((data[l].room_num)/100)==fnum))
                    {
                        data[l].exemption--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents-1;k++)
                            {
                                fprintf(fp,"%d %d %d\n",data[k].room_num,data[k].floor,data[k].exemption);
                            }
                            fclose(fp);
                        }
                    }
                    else if(((data[l].floor)==fnum)&&((data[l].exemption)==0)&&(((data[l].room_num)/100)==fnum))
                    {
                        ninsu[i][0]=data[l].room_num;
                        l++;
                        if(l>=residents)
                        {
                            l=0;
                        }
                        break;
                    }
                    l++;
                    if(l>=residents)
                    {
                        l=0;
                    }
                }
                for(int j=1;j<anum;j++)
                {
                    ninsu[i][j]=ninsu[i-1][j-1];
                }
                for(int j=0;j<anum-1;j++)   //ここ変える
                {
                    printf("%d\t",ninsu[i][j]);
                }
                printf("%d\n",ninsu[i][anum-1]);
            }
        }
    }
}

void turn4(int anum,int dnum,int fnum,int snum,int turn,int last1,int ninsu[][anum],roomdata data[],const char fname[])
{
    FILE *fp;
    int l;
    int c=0;
    for(int i=0;i<anum-1;i++)
        {
            printf("%d\t",ninsu[0][i]);
        }
        printf("%d\n",ninsu[0][anum-1]);
    for(int i=1;i<dnum;i++)
    {
        for(int j=0;j<anum-1;j++)
        {
            printf("#\t");
        }
        printf("#\n");
        for(int j=0;j<anum-1;j++)
        {
            printf(":\t");
        }
        printf(":\n");
        if(i==1)
        {
            l=last1-1;
        }
        if(i<snum)   //ここ変えた
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            for(int j=0;j<anum-1;j++)   //ここ変える
            {
                printf("%d\t",ninsu[i][j]);
            }
            printf("%d\n",ninsu[i][anum-1]);
        }
         else
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            if((snum>0)&&((ninsu[i][1]==ninsu[0][1])||(((ninsu[i][1]/100)!=fnum)&&(ninsu[i][1]!=ninsu[snum-1][0]))))
                {
                    ninsu[i][0]=ninsu[c][0];
                    c++;
                    for(int j=0;j<anum-1;j++)   //ここ変える
                    {
                        printf("%d\t",ninsu[i][j]);
                    }
                    printf("%d\n",ninsu[i][anum-1]);
                }
            else
            {
                while(1)
                {
                    if(((data[l].floor)==fnum)&&((data[l].exemption)>0)&&(((data[l].room_num)/100)==fnum))
                    {
                        data[l].exemption--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents-1;k++)
                            {
                                fprintf(fp,"%d %d %d\n",data[k].room_num,data[k].floor,data[k].exemption);
                            }
                            fclose(fp);
                        }
                    }
                    else if(((data[l].floor)==fnum)&&((data[l].exemption)==0)&&(((data[l].room_num)/100)==fnum))
                    {
                        ninsu[i][0]=data[l].room_num;
                        l--;
                        if(l<0)
                        {
                            l=residents-1;
                        }
                        break;
                    }
                    l--;
                    if(l<0)
                    {
                        l=residents-1;
                    }
                }
                for(int j=1;j<anum;j++)
                {
                    ninsu[i][j]=ninsu[i-1][j-1];
                }
                for(int j=0;j<anum-1;j++)   //ここ変える
                {
                    printf("%d\t",ninsu[i][j]);
                }
                printf("%d\n",ninsu[i][anum-1]);
            }
        }
    }
}

void special(int anum,int dnum,int fnum,int snum,int turn)
{
    FILE *fp;
    char fname[256];
    printf("file name:");
    scanf("%s",fname);
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        int ninsu[dnum][anum];
        int a,b,staff,last1;
        roomdata data[residents];
        for(int i=0;i<residents-1;i++)
        {
            fscanf(fp,"%d%d%d",&data[i].room_num,&data[i].floor,&data[i].exemption);
        }
        fclose(fp);
        for(int i=0;i<snum;i++)
        {
            printf("staff[%d]:",i+1);
            scanf("%d",&b);
            for(int i=0;i<residents-1;i++)
            {
                if(b==data[i].room_num)
                {
                    staff=i;
                    break;
                }
            }
            ninsu[i][0]=data[staff].room_num;
        }
        printf("Week 1 shift\n");
        for(int i=anum-1;i>0;i--)
        {
            printf("(%d):",i);
            scanf("%d",&ninsu[0][i]);
        }
        for(int i=0;i<residents-1;i++)
        {
            if(ninsu[0][1]==data[i].room_num)
            {
                last1=i;
                break;
            }
        }
        printf("How many people have used their exemption rights?");
        scanf("%d",&a);
        if(a>0)
        {
            for(int i=0;i<a;i++)
            {
                int target;
                printf("target:");
                scanf("%d",&target);
                for(int j=0;j<residents-1;j++)
                {
                    if(data[j].room_num==target)
                    {
                        data[j].exemption--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents-1;k++)
                            {
                                fprintf(fp,"%d %d %d\n",data[k].room_num,data[k].floor,data[k].exemption);
                            }
                            fclose(fp);
                        }
                        break;
                    }
                }
            }
        }
        if(turn==1)
        turn1(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
        if(turn==2)
        turn2(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
        if(turn==3)
        turn3(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
        if(turn==4)
        turn4(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
    }
}

void normal(int anum,int dnum,int fnum,int snum,int turn)
{
    FILE *fp;
    char fname[256];
    printf("file name:");
    scanf("%s",fname);
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        int ninsu[dnum][anum];
        int a,last1;
        roomdata data[residents];
        for(int i=0;i<residents-1;i++)
        {
            fscanf(fp,"%d%d%d",&data[i].room_num,&data[i].floor,&data[i].exemption);
        }
        fclose(fp);
        printf("Week 1 shift\n");
        for(int i=anum-1;i>=0;i--)
        {
            printf("(%d):",i);
            scanf("%d",&ninsu[0][i]);
        }
        for(int i=0;i<residents-1;i++)
        {
            if(ninsu[0][0]==data[i].room_num)
            {
                last1=i;
                break;
            }
        }
        printf("How many people have used their exemption rights?");
        scanf("%d",&a);
        if(a>0)
        {
            for(int i=0;i<a;i++)
            {
                int target;
                printf("target:");
                scanf("%d",&target);
                for(int j=0;j<residents-1;j++)
                {
                    if(data[j].room_num==target)
                    {
                        data[j].exemption--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents-1;k++)
                            {
                                fprintf(fp,"%d %d %d\n",data[k].room_num,data[k].floor,data[k].exemption);
                            }
                            fclose(fp);
                        }
                        break;
                    }
                }
            }
        }
        if(turn==1)
        turn1(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
        if(turn==2)
        turn2(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
        if(turn==3)
        turn3(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
        if(turn==4)
        turn4(anum,dnum,fnum,snum,turn,last1,ninsu,data,fname);
        
    }

}

int main (void)
{
    int anum,dnum,fnum,snum,turn;
    printf("How many areas need cleaning?");
    scanf("%d",&anum);
    printf("How many days?");
    scanf("%d",&dnum);
    printf("What is the floor number?");
    scanf("%d",&fnum);
    printf("How many morning cleaning staff will on that floor?");
    scanf("%d",&snum);
    printf("Which direction should it turn?");
    scanf("%d",&turn);
    if(snum==0)
    {
        normal(anum,dnum,fnum,snum,turn);
    }
    else
    {
        special(anum,dnum,fnum,snum,turn);
    }

    return 0;
}