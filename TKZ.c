#include<stdio.h>
#define residents 108

typedef struct DATA{
    int room_num;
    int grade;
    int exemption1;
    int exemption2;
    int exemption3;
    int features;
}roomdata;

void month4(roomdata data[],const char fname[])
{
    FILE *fp;
    int empty;
    for(int i=0;i<residents;i++)
    {
        if(((data[i].features)==-1)||((data[i].features)==-2))
        {
            data[i].grade=1;
            data[i].exemption1=0;
            data[i].exemption2=0;
            data[i].exemption3=0;
            data[i].features=0;
        }
        else
        {
            data[i].grade++;
        }
    }
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
    if((fp=fopen(fname,"w"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        for(int i=0;i<residents;i++)
        {
            fprintf(fp,"%d %d %d %d %d %d\n",data[i].room_num,data[i].grade,data[i].exemption1,data[i].exemption2,data[i].exemption3,data[i].features);
        }
        fclose(fp);
    }
}

void month511(roomdata data[],const char fname[],int month)
{
    FILE *fp;
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
    printf("\nHow many of the C-kai board members who are leabing have evening classes?");
    scanf("%d",&evening);
    for(int i=0;i<evening;i++)
    {
        int target;
        printf("target:");
        scanf("%d",&target);
        for(int j=0;j<residents;j++)
        {
            if(data[j].room_num==target)
            {
                data[j].features=1;
                break;
            }
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
    if((fp=fopen(fname,"w"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        for(int i=0;i<residents;i++)
        {
            fprintf(fp,"%d %d %d %d %d %d\n",data[i].room_num,data[i].grade,data[i].exemption1,data[i].exemption2,data[i].exemption3,data[i].features);
        }
        fclose(fp);
    }
}

void garbage_while1(roomdata data[],const char fname[],int *l,int day[],int fdays,int i)
{
    FILE *fp;
    while(1)
    {
        if(data[*l].features<0)
        {
            ;
        }
        else if(data[*l].exemption1>0)
        {
            data[*l].exemption1--;
            if((fp=fopen(fname,"w"))==NULL)
            {
                printf("The file cannot be opened.\n");
            }
            else
            {
                for(int j=0;j<residents;j++)
                {
                    fprintf(fp,"%d %d %d %d %d %d\n",data[j].room_num,data[j].grade,data[j].exemption1,data[j].exemption2,data[j].exemption3,data[j].features);
                }
                fclose(fp);
            }
        }
        else if(data[*l].exemption1==0)
        {
            day[i]=data[*l].room_num;
            (*l)++;
            if(*l>=residents)
            {
                *l=0;
            }
            break;
        }
        (*l)++;
        if(*l>=residents)
        {
            *l=0;
        }
    }
    printf("%d",day[i]);
    if(i==fdays-1)
    {
        printf("\n\n\n");
    }
}

void month123(roomdata data[],const char fname[])
{
    FILE *fp;
    for(int i=0;i<residents;i++)
    {
        if((((data[i].grade)==4)&&((data[i].features)!=6))||((data[i].grade)==6))
        {
            data[i].features=-1;
            if((fp=fopen(fname,"w"))==NULL)
            {
                printf("The file cannot be opened.\n");
            }
            else
            {
                for(int j=0;j<residents;j++)
                {
                    fprintf(fp,"%d %d %d %d %d %d\n",data[j].room_num,data[j].grade,data[j].exemption1,data[j].exemption2,data[j].exemption3,data[j].features);
                }
                fclose(fp);
            }
        }
    }
}

void exemption1(roomdata data[],const char fname[])
{
    FILE *fp;
    int a;
    printf("How many people have used their exemption rights?");
        scanf("%d",&a);
        if(a>0)
        {
            for(int i=0;i<a;i++)
            {
                int target;
                printf("target:");
                scanf("%d",&target);
                for(int j=0;j<residents;j++)
                {
                    if(data[j].room_num==target)
                    {
                        data[j].exemption1--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents;k++)
                            {
                                fprintf(fp,"%d %d %d %d %d %d\n",data[k].room_num,data[k].grade,data[k].exemption1,data[k].exemption2,data[k].exemption3,data[k].features);
                            }
                            fclose(fp);
                        }
                        break;
                    }
                }
            }
        }
}

void exemption2(roomdata data[],const char fname[])
{
    FILE *fp;
    int a;
    printf("How many people have used their exemption rights?");
        scanf("%d",&a);
        if(a>0)
        {
            for(int i=0;i<a;i++)
            {
                int target;
                printf("target:");
                scanf("%d",&target);
                for(int j=0;j<residents;j++)
                {
                    if(data[j].room_num==target)
                    {
                        data[j].exemption2--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents;k++)
                            {
                                fprintf(fp,"%d %d %d %d %d %d\n",data[k].room_num,data[k].grade,data[k].exemption1,data[k].exemption2,data[k].exemption3,data[k].features);
                            }
                            fclose(fp);
                        }
                        break;
                    }
                }
            }
        }
}

void exemption3(roomdata data[],const char fname[])
{
    FILE *fp;
    int a;
    printf("How many people have used their exemption rights?");
        scanf("%d",&a);
        if(a>0)
        {
            for(int i=0;i<a;i++)
            {
                int target;
                printf("target:");
                scanf("%d",&target);
                for(int j=0;j<residents;j++)
                {
                    if(data[j].room_num==target)
                    {
                        data[j].exemption3--;
                        if((fp=fopen(fname,"w"))==NULL)
                        {
                            printf("The file cannot be opened.\n");
                        }
                        else
                        {
                            for(int k=0;k<residents;k++)
                            {
                                fprintf(fp,"%d %d %d %d %d %d\n",data[k].room_num,data[k].grade,data[k].exemption1,data[k].exemption2,data[k].exemption3,data[k].features);
                            }
                            fclose(fp);
                        }
                        break;
                    }
                }
            }
        }
}

void garbage(int month,int fdays,int sdays)
{
    FILE *fp;
    char fname[256];
    int wday,first;
    int days=fdays+sdays;
    int day[days];
    roomdata data[residents];
    printf("What day of the week does it start on?\n");
    printf("Monday (1) / Thursday (4)");
    scanf("%d",&wday);
    printf("file name:");
    scanf("%s",fname);
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        int last,l;
        
        for(int i=0;i<residents;i++)
        {
            fscanf(fp,"%d%d%d%d%d%d",&data[i].room_num,&data[i].grade,&data[i].exemption1,&data[i].exemption2,&data[i].exemption3,&data[i].features);
        }
        fclose(fp);
        if((month>=1)&&(month<=3))
        {
            month123(data,fname);
        }
        printf("the first person on duty :");
        scanf("%d",&first);
        day[0]=first;
        for(int i=0;i<residents;i++)
        {
            if(first==data[i].room_num)
            {
                last=i;
                break;
            }
        }
        exemption1(data,fname);
        printf("%d",day[0]);
        l=last+1;
        for(int i=1;i<days;i++)
        {
            if(wday==1)
            {
                printf("\n\n\n");
                garbage_while1(data,fname,&l,day,fdays,i);
                wday+=3;
            }
            else if(wday==4)
            {
                printf("\n\n\n\n");
                garbage_while1(data,fname,&l,day,fdays,i);
                wday-=3;
            }
        }
    }
}

void bath(int month,int fdays,int sdays)
{
    FILE *fp;
    char fname[256];
    int first;
    int days=fdays+sdays;
    int day[days];
    roomdata data[residents];
    printf("file name:");
    scanf("%s",fname);
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        int last,l;
        
        for(int i=0;i<residents;i++)
        {
            fscanf(fp,"%d%d%d%d%d%d",&data[i].room_num,&data[i].grade,&data[i].exemption1,&data[i].exemption2,&data[i].exemption3,&data[i].features);
        }
        fclose(fp);
        if((month>=1)&&(month<=3))
        {
            month123(data,fname);
        }
        printf("the first person on duty :");
        scanf("%d",&first);
        day[0]=first;
        for(int i=0;i<residents;i++)
        {
            if(first==data[i].room_num)
            {
                last=i;
                break;
            }
        }
        exemption2(data,fname);
        l=last+1;
        for(int i=1;i<days;i++)
        {
            while(1)
            {
                if(data[l].features<0)
                {
                    ;
                }
                else if(data[l].exemption2>0)
                {
                    data[l].exemption2--;
                    if((fp=fopen(fname,"w"))==NULL)
                    {
                        printf("The file cannot be opened.\n");
                    }
                    else
                    {
                        for(int j=0;j<residents;j++)
                        {
                            fprintf(fp,"%d %d %d %d %d %d\n",data[j].room_num,data[j].grade,data[j].exemption1,data[j].exemption2,data[j].exemption3,data[j].features);
                        }
                        fclose(fp);
                    }
                }
                else if(data[l].exemption2==0)
                {
                    day[i]=data[l].room_num;
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
        }
        for(int i=0;i<days;i++)
        {
            printf("%d\n",day[i]);
            if(i==fdays-1)
            {
                printf("\n\n\n");
            }
        }
    }
}

void dormitory(int month,int fdays,int sdays)
{
    FILE *fp;
    char fname[256];
    int wday,first;
    int days=fdays+sdays;
    int day[days+14];
    roomdata data[residents];
    for(int i=0;i<days;i++)
    {
        day[i]=0;
    }
    printf("What day of the week does it start on?\n");
    printf("Monday (1) / Tuesday (2) / Wednesday (3) / Thursday (4) / Friday (5) / Saturday (6) / Sunday (7)");
    scanf("%d",&wday);
    printf("file name:");
    scanf("%s",fname);
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
    }
    else
    {
        int last,l;
        
        for(int i=0;i<residents;i++)
        {
            fscanf(fp,"%d%d%d%d%d%d",&data[i].room_num,&data[i].grade,&data[i].exemption1,&data[i].exemption2,&data[i].exemption3,&data[i].features);
        }
        fclose(fp);
        if((month>=1)&&(month<=3))
        {
            month123(data,fname);
        }
        printf("the first person on duty :");
        scanf("%d",&first);
        day[0]=first;
        for(int i=0;i<residents;i++)
        {
            if(first==data[i].room_num)
            {
                last=i;
                break;
            }
        }
        exemption3(data,fname);
        l=last+1;
        if(wday>=7)
        {
            wday=1;
        }
        else
        {
            wday++;
        }
        int i2=1;
        while(1)
        {
            int full=1;
            for(int i=0;i<days;i++)
            {
                if(day[i]==0)
                {
                    full=0;
                    break;
                }
            }
            if(full==1)
            {
                break;
            }
            while(1)
            {
                if(data[l].features<0)
                {
                    ;
                }
                else if(data[l].exemption3>0)
                {
                    data[l].exemption3--;
                    if((fp=fopen(fname,"w"))==NULL)
                    {
                        printf("The file cannot be opened.\n");
                    }
                    else
                    {
                        for(int j=0;j<residents;j++)
                        {
                            fprintf(fp,"%d %d %d %d %d %d\n",data[j].room_num,data[j].grade,data[j].exemption1,data[j].exemption2,data[j].exemption3,data[j].features);
                        }
                        fclose(fp);
                    }
                }
                else if((((data[l].exemption3)==0))&&(((data[l].features)!=1)))
                {
                    while(day[i2]!=0)
                    {
                        i2++;
                        if(wday>=7)
                        {
                            wday=1;
                        }
                        else
                        {
                            wday++;
                        }
                    }
                    day[i2]=data[l].room_num;
                    l++;
                    if(l>=residents)
                    {
                        l=0;
                    }
                    i2++;
                    if(wday>=7)
                    {
                        wday=1;
                    }
                    else
                    {
                        wday++;
                    }
                    break;
                }
                else if((((data[l].exemption3)==0))&&(((data[l].features)==1)))
                {
                    if((wday>=1)&&(wday<=5))
                    {
                        int sa;
                        sa=6-wday;
                        while(day[i2+sa]!=0)
                        {
                            sa++;
                        }
                        day[i2+sa]=data[l].room_num;
                    }
                    else
                    {
                        while(day[i2]!=0)
                        {
                            i2++;
                            if(wday>=7)
                            {
                                wday=1;
                            }
                            else
                            {
                                wday++;
                            }
                        }
                        day[i2]=data[l].room_num;
                        i2++;
                        if(wday>=7)
                        {
                            wday=1;
                        }
                        else
                        {
                            wday++;
                        }
                    }
                    (l)++;
                    if(l>=residents)
                    {
                        l=0;
                    }
                    break;
                }
                (l)++;
                if(l>=residents)
                {
                    l=0;
                }
            }
        }
        for(int i=0;i<days;i++)
        {
            printf("%d\n",day[i]);
            if(i==fdays-1)
            {
                printf("\n\n\n");
            }
        }
        if(month==4)
        {
            month4(data,fname);
        }
        if((month==5)||(month==11))
        {
            month511(data,fname,month);
        }
    }
}

int main (void)
{
    int month,area,fdays,sdays;
    printf("Which month's duty roster should we create?");
    scanf("%d",&month);
    printf("How many days is the first half?");
    scanf("%d",&fdays);
    printf("How many days is the second half?");
    scanf("%d",&sdays);
    printf("What duty is it? Garbage duty (1) / Bath duty (2) / Dormitory duty (3)");
    scanf("%d",&area);
    if(area==1)
    {
        garbage(month,fdays,sdays);
    }
    if(area==2)
    {
        bath(month,fdays,sdays);
    }
    if(area==3)
    {
        dormitory(month,fdays,sdays);
    }

    return 0;
}