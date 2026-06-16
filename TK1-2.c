#include<stdio.h>
#include<string.h>
#define residents 108

typedef struct DATA{
    int room_num;
    int grade;
    int floor;
    int features;
    int exemption;
}roomdata;

void month4mae(roomdata data[])
{
    for(int i=0;i<residents;i++)
    {
        if(((data[i].features)==-1)&&((data[i].grade)!=5))
        {
            data[i].features=-2;
        }
    }
    for(int i=0;i<residents;i++)
    {
        if((((data[i].grade)==3)&&((data[i].features)!=6))||(((data[i].grade)>=4)&&((data[i].features)==6)))
        {
            data[i].features=-1;
        }
    }
}

void monthly(roomdata data[])
{
    for(int i=0;i<residents;i++)
    {
        if((((data[i].features)!=6)&&((data[i].features)!=-2)&&((data[i].grade)==4))||(((data[i].features)==6)&&(data[i].grade)>=5))
        {
            data[i].features=-1;
        }
    }
}

void month4ato(roomdata data[])
{
    for(int i=0;i<residents;i++)
    {
        if((data[i].features)==-2)
        {
            data[i].grade=1;
            data[i].exemption=0;
            data[i].features=0;
        }
        else
        {
            data[i].grade++;
        }
    }
}

void month5(roomdata data[])
{
    int empty;
    printf("\nHow many empty rooms are there?");
    scanf("%d",&empty);
    for(int i=0;i<empty;i++)
    {
        int target;
        printf("target:");
        scanf("%d",&target);
        for(int j=0;j<residents;j++)
        {
            if(data[j].room_num==target)
            {
                data[j].grade=0;
                data[j].features=-2;
                break;
            }
        }
    }
}

void month511(roomdata data[],int month)
{
    int evening;
    int six;
    int change=-1*month;
    for(int j=0;j<residents;j++)
    {
        if(data[j].features==change)
        {
            data[j].features=0;
        }
    }
    printf("How many of the C-kai board members who are leaving have a 6-year degree?");
    scanf("%d",&six);
    for(int i=0;i<six;i++)
    {
        int target;
        printf("target:");
        scanf("%d",&target);
        for(int j=0;j<residents;j++)
        {
            if(data[j].room_num==target)
            {
                data[j].features=6;
                break;
            }
        }
    }
}

void search(int day,int place,int anum,int fnum,int turn,int *l,roomdata data[],int ninsu[][anum]) //次に入る人を探す
{
    while(1)
    {
        if((data[(*l)].features<0)||(data[(*l)].exemption)<0)
        {
            ;
        }
        else if(((data[(*l)].room_num)/100)==fnum)
        {
            if((data[(*l)].floor)!=fnum)
            {
                if((data[(*l)].exemption)>0)
                {
                    data[(*l)].exemption--;
                }
            }
            else if((data[(*l)].exemption)>0)
            {
                data[(*l)].exemption--;
            }
            else if((data[(*l)].exemption)==0)
            {
                ninsu[day][place]=data[(*l)].room_num;
                if(turn==1||turn==3)
                {
                    (*l)++;
                    if((*l)>=residents)
                    {
                        (*l)=0;
                    }
                }
                if(turn==2||turn==4)
                {
                    (*l)--;
                    if((*l)<0)
                    {
                        (*l)=residents-1;
                    }
                }
                break;
            }
        }
        if(turn==1||turn==3)
        {
            (*l)++;
            if((*l)>=residents)
            {
                (*l)=0;
            }
        }
        if(turn==2||turn==4)
        {
            (*l)--;
            if((*l)<0)
            {
                (*l)=residents-1;
            }
        }
    }
}

void make1(int snum,int anum,int fnum,int e,int d,int last1,int turn,int first[],roomdata data[],int ninsu[][anum],int last[],int *l) //1日目のシフトを作る
{
    if(turn==1||turn==3)
    {

        (*l)=last1+1;
        if((*l)>=residents)
        {
            (*l)=0;
        }
    }
    if(turn==2||turn==4)
    {
        (*l)=last1-1;
        if((*l)<0)
        {
            (*l)=residents-1;
        }
    }
    if(snum>0)
    {
        e=1;
    }
    for(int i=anum-1;i>=e;i--)
    {
        int found=0;  
        while(d>=0)
        {
            if(((data[last[d]].features)>=0)&&((data[last[d]].floor)==fnum))
            {
                ninsu[0][i]=data[last[d]].room_num;
                d--;
                found=1;
                break;
            }
            d--;
        }
        if(found==0)
        {
            search(0,i,anum,fnum,turn,l,data,ninsu);
        }
    }

}

void print(int i,int start,int end,int turn,int anum,int ninsu[][anum])
{
    for(int j=start;j!=end;)
    {
        printf("%d\t",ninsu[i][j]);
        if(turn==1||turn==2)
        {
            j--;
        }
        if(turn==3||turn==4)
        {
            j++;
        }

    }
    printf("%d\n",ninsu[i][end]);
}

void turn_second(int anum,int dnum,int snum,int fnum,int turn,int c,int *l,int ninsu[][anum],roomdata data[])
{
    int start;
    int end;
    if(turn==1||turn==2)
    {
        start=anum-1;
        end=0;
    }
    if(turn==3||turn==4)
    {
        start=0;
        end=anum-1;
    }
    for(int i=start;i!=end;)
    {
        printf("%d\t",ninsu[0][i]);
        if(turn==1||turn==2)
        {
            i--;
        }
        if(turn==3||turn==4)
        {
            i++;
        }

    }
    printf("%d\n",ninsu[0][end]);
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
        if(i<snum)   //ここ変えた
        {
            for(int j=1;j<anum;j++)
            {
                ninsu[i][j]=ninsu[i-1][j-1];
            }
            print(i,start,end,turn,anum,ninsu);
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
                    print(i,start,end,turn,anum,ninsu);
                }
            else
            {
                search(i,0,anum,fnum,turn,l,data,ninsu);
                print(i,start,end,turn,anum,ninsu);
            }
        }
    }
}

void turn_main(int anum,int dnum,int fnum,int snum,int turn,int last1,int ninsu[][anum],roomdata data[],int first[],int last[])
{
    int l;
    int c=0;
    int d=anum-2;
    int e=0;
    make1(snum,anum,fnum,e,d,last1,turn,first,data,ninsu,last,&l);
    turn_second(anum,dnum,snum,fnum,turn,c,&l,ninsu,data);
}

int main (void)
{
    FILE *fp;
    FILE *fp2;
    int a,b,q,all,ans;
    int last1=0;
    char fname[256];
    char fname2[256];
    int anum,month,dnum,fnum,snum,turn,change,problem;
    roomdata data[residents];
    printf("Are we creating the first shift now? yes (0) /no (1)");
    scanf("%d",&q);
    if(q==0)
    {
        printf("read file name:");
        scanf("%s",fname);
        printf("write file name:");
        scanf("%s",fname2);
    }
    else
    {
        printf("file name:");
        scanf("%s",fname);
        strcpy(fname2,fname);
    }
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        for(int i=0;i<residents;i++)
        {
            fscanf(fp,"%d,%d,%d,%d,%d",&data[i].room_num,&data[i].grade,&data[i].floor,&data[i].features,&data[i].exemption);
        }
        fclose(fp);
        printf("Which month's duty roster should we create?");
        scanf("%d",&month);
        if(q==0)
        {
            if(month==4)
            {
                printf("Have you entered the information of residents who have left the dormitory other than graduates? Yes (0) / No (1)");
                scanf("%d",&ans);
                if(ans==1)
                {
                    printf("Please try again after entering the information.\n");
                    return 0;
                }
            }
            if(month==5||month==11)
            {
                printf("Is the information for the dormitory residents entered correctly? Yes (0) / No (1)");
                scanf("%d",&ans);
                if(ans==1)
                {
                    printf("Please try again after entering the information.\n");
                    return 0;
                }
            }
            if(month==4||month==6||month==7||month==11)
            {
                printf("Have you entered your information as a volunteer? Yes (0) / No (1)");
                scanf("%d",&ans);
                if(ans==1)
                {
                    printf("Please try again after entering the information.\n");
                    return 0;
                }
            }
            if(month==6||month==12)
            {
                printf("Have you correctly entered the information of the board members who are exempt from their duties? Yes (0) / No (1)");
                scanf("%d",&ans);
                if(ans==1)
                {
                    printf("Please try again after entering the information.\n");
                    return 0;
                }
            }
        }
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
        int ninsu[dnum][anum];
        int first[anum];
        int last[anum];
        int staff[snum];
        monthly(data);
        if(q==0)
        {
            if(month==4)
            month4mae(data);
            if(month==5)
            month5(data);
        }
        if(snum>0)
        {
            for(int i=0;i<snum;i++)
            {
                printf("staff[%d]:",i+1);
                scanf("%d",&b);
                for(int j=0;j<residents;j++)
                {
                    if(b==data[j].room_num)
                    {
                        staff[i]=j;
                        break;
                    }
                }
                ninsu[i][0]=data[staff[i]].room_num;
                data[staff[i]].floor=fnum;
            }
        }
        printf("What was your last shift last month?\n");

        int start2;
        int end2;
        if(turn==1||turn==2)
        {
            start2=anum-1;
            end2=-1;
        }
        if(turn==3||turn==4)
        {
            start2=0;
            end2=anum;
        }
        int count=1;
        for(int i=start2;i!=end2;)
        {
            printf("(%d):",count);
            scanf("%d",&first[i]);
            for(int k=0;k<residents;k++)
            {
                if(first[i]==data[k].room_num)
                {
                    last[i]=k;
                }
            }
            if(turn==1||turn==2)
            {
                i--;
            }
            if(turn==3||turn==4)
            {
                i++;
            }
            count++;
        }
        for(int i=0;i<anum;i++)
        {
            if((data[last[i]].floor)==fnum)
            {
                last1=last[i];
                break;
            }
        }
        turn_main(anum,dnum,fnum,snum,turn,last1,ninsu,data,first,last);
        if(snum>0)
        {
            for(int i=0;i<snum;i++)
            {
                for(int j=0;j<dnum;j++)
                {
                    if(ninsu[j][anum-1]==data[staff[i]].room_num)
                    {
                        data[staff[i]].exemption++;
                    }
                }
            }
        }
        int sf=0;
        for(int i=0;i<residents;i++)
        {
            if((data[i].room_num/100)!=(data[i].floor))
            {
                sf=1;
                break;
            }
        }
        if(month==4||month==5||month==11||sf==1)
        {
            printf("Have you finished creating all the duty rosters? yes (0) /no (1)");
            scanf("%d",&all);
            if(all==0)
            {
                if(month==4)
                month4ato(data);
                if(month==5||month==11)
                month511(data,month);
                if(sf==1)
                {
                    for(int i=0;i<residents;i++)
                    {
                        data[i].floor=data[i].room_num/100;
                    }
                }
            }
        }
        printf("Are there any mistakes? no problem (0) / there is a problem (1)");
        scanf("%d",&problem);
        if(problem==1)
        {
            printf("The program will close without saving.\nPlease try again.");
        }
        else
        {
            if((fp2=fopen(fname2,"w"))==NULL)
            {
                printf("The file cannot be opened.\n");
                perror("Error details");
            }
            else
            {
                for(int i=0;i<residents;i++)
                {
                    fprintf(fp2,"%d,%d,%d,%d,%d\n",data[i].room_num,data[i].grade,data[i].floor,data[i].features,data[i].exemption);
                }
                fclose(fp2);
                printf("The preservation was successful.");
            }
        }
    }

    return 0;
}