#include<stdio.h>
#include<string.h>
#define residents 108
#define room 0
#define choice 1
#define pnum 2
#define wnum 3
#define gnum 4
#define mnum 5
#define area_num 6
#define day_time 0
#define night_time 1

typedef struct DATA{
    int room_num;
    int grade;
    int exemption1;
    int exemption2;
    int exemption3;
    int features;
}roomdata;

int get_valid_input(const char *message,roomdata data[],int mode)  //適切な入力がされなかった時の関数
{
    int input,result,found;
    while(1)
    {
        printf("%s",message);
        result=scanf("%d",&input);
        if(result!=1)                                       //文字が入力された場合
        {
            printf("error : Please enter a number.\n");
            while(getchar()!='\n');
            continue;
        }
        if(mode==0)                                     //部屋番号を入力する場合
        {
            found=0;
            for(int i=0;i<residents;i++)
            {
                if(data[i].room_num==input)
                {
                    found=1;
                    break;
                }
            }
            if(found==1)
            {
                return input;
            }
            else
            {
                printf("error : Room number %d does not exist.\n",input);
            }
        }
        else
        {
            int is_valid=0; //正しい入力かどうかのフラグ
            switch(mode)
            {
                case choice: if((input==0)||(input==1)) is_valid=1; break;
                case pnum: if(input>=0) is_valid=1; break;
                case wnum: if((input>=1)&&(input<=7)) is_valid=1; break;
                case gnum: if ((input==1)||(input==4)) is_valid=1; break;
                case mnum: if((input>=1)&&(input<=12)) is_valid=1; break;
                case area_num: if((input>=1)&&(input<=3)) is_valid=1; break;
            }
            if(is_valid==1)
            {
                return input;
            }
            else
            {
                printf("error : Please enter the correct number.");
            }
        }
    }
}

void month123(roomdata data[])   //4年制の4年生と，6年制の6年生のfeaturesを-1にする
{
    for(int i=0;i<residents;i++)
    {
        if((((data[i].grade)==4)&&((data[i].features)!=6))||((data[i].grade)==6))
        {
            data[i].features=-1;
        }
    }
}

void month4(roomdata data[])    //入寮生の情報を入力
{
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
}
void month5mae(roomdata data[])   //空き部屋の情報を入れる
{
    int empty;
    empty=get_valid_input("\nHow many empty rooms are there?",data,pnum);
    for(int i=0;i<empty;i++)
    {
        int target;
        target=get_valid_input("target:",data,room);
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

void month511(roomdata data[],int month)     //前C会役員の情報を更新
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
    evening=get_valid_input("\nHow many of the C-kai board members who are leaving have evening classes?",data,pnum);
    for(int i=0;i<evening;i++)
    {
        int target;
        target=get_valid_input("target:",data,room);
        for(int j=0;j<residents;j++)
        {
            if(data[j].room_num==target)
            {
                data[j].features=1;
                break;
            }
        }
    }
    six=get_valid_input("How many of the C-kai board members who are leaving have a 6-year degree?",data,pnum);
    for(int i=0;i<six;i++)
    {
        int target;
        target=get_valid_input("target:",data,room);
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

void slide(int *wday,int *i2,int *l,int days,int day[],roomdata data[])  //ターゲットを土日に配置する
{
    int sa2;
    int sw=0;
    int con=0;
    if(*wday==7)
    {
        sa2=0;
        sw=1;
    }
    else
    {
        sa2=6-*wday;
    }
    for(int i=0;i<days+400;i++)
    {
        if(day[i]==0)
        {
            con=1;
        }
    }
    if(con==0)
    {
        ;
    }
    else
    {
        while((((*i2)+sa2<days+400))&&(day[(*i2)+sa2]!=0))  //まず，dayの配列より大きくなってないかを確認
        {
            if(sw==0)
            {
                sa2++;
                sw++;
            }
            else if(sw==1)
            {
                sa2+=6;
                sw--;
            }
        }
        if((*i2)+sa2<days+400)  //daysの大きさを超えてたらdayの配列にその人を入れない
        {
            day[(*i2)+sa2]=data[(*l)].room_num;
        }
    }
}

void target_dormitory(int judge,int *wday,int *i2,int *l,int days,int day[],roomdata data[])
{
    if((judge==night_time)&&((*wday)>=1)&&((*wday)<=5))
    {
        slide(wday,i2,l,days,day,data);
    }
    else
    {
        while(day[(*i2)]!=0)
        {
            (*i2)++;
            if((*wday)>=7)
            (*wday)=1;
            else
            (*wday)++;
        }
        if((judge==night_time)&&(((*wday)>=1)&&((*wday)<=5)))
        {
            slide(wday,i2,l,days,day,data);
        }
        else
        {
            day[(*i2)]=data[(*l)].room_num;
            (*i2)++;
            if((*wday)>=7)
            (*wday)=1;
            else
            (*wday)++;
        }
    }
}

void while1(roomdata data[],int *l,int day[],int i,int area)
{
    //無限ループ防止
    int con=0;
    for(int j=0;j<residents;j++)
    {
        if(data[j].features>=0)
        {
            con=1;
            break;
        }
    }
    if(con==0)
    {
        printf("error : Everyone's features are negative.\nPlease terminate the program by pressing Ctrl-C.\n");
    }

    else
    {
        while(1)    //当番に入る人の選定
        {
            if(data[*l].features<0)
            {
                ;
            }
            else
            {
                int *ex;
                if(area==1)
                ex=&data[*l].exemption1;
                if(area==2)
                ex=&data[*l].exemption2;
                if(area==3) 
                ex=&data[*l].exemption3;
                if((*ex)>0)
                {
                    (*ex)--;
                }
                else if((*ex)==0)
                {
                    day[i]=data[*l].room_num;
                    (*l)++;
                    if(*l>=residents)
                    {
                        *l=0;
                    }
                    break;
                }
            }
            (*l)++;
            if(*l>=residents)
            {
                *l=0;
            }
        }
    }
}

void garbage(int month,int fdays,int sdays,int area,roomdata data[])     //ゴミ当番
{
    int wday,lastm;
    int days=fdays+sdays;
    int day[days];
    printf("What day of the week does it start on?\n");
    wday=get_valid_input("Monday (1) / Thursday (4)",data,gnum);
    int last,l;
    lastm=get_valid_input("Who was on duty last month? :",data,room);
    for(int i=0;i<residents;i++)
    {
        if(lastm==data[i].room_num)
        {
            last=i;
            break;
        }
    }
    l=last+1;
    if(l>=residents)
    {
        l=0;
    }
    for(int i=0;i<days;i++)
    {
        if(wday==1)
        {
            while1(data,&l,day,i,area);  //当番に入る人の選定
            printf("%d",day[i]);
            printf("\n\n\n");
            wday+=3;
        }
        else if(wday==4)
        {
            while1(data,&l,day,i,area);   //当番に入る人の選定
            printf("%d",day[i]);
            printf("\n\n\n\n");
            wday-=3;
        }
        if(i==fdays-1)
        {
            printf("\n\n\n\n");
        }
    }
}

void bath(int month,int fdays,int sdays,int area,roomdata data[])   //風呂当番
{
    int lastm;
    int days=fdays+sdays;
    int day[days];
    int last,l;
    lastm=get_valid_input("Who was on duty last month? :",data,room);
    for(int i=0;i<residents;i++)
    {
        if(lastm==data[i].room_num)
        {
            last=i;
            break;
        }
    }
    l=last+1;
    if(l>=residents)
    {
        l=0;
    }
    for(int i=0;i<days;i++)
    {
        while1(data,&l,day,i,area);   //当番に入る人の選定
    }
    for(int i=0;i<days;i++)                 //表示
    {
        printf("%d\n",day[i]);
        if(i==fdays-1)
        {
            printf("\n\n\n");
        }
    }
}

void dormitory(int month,int fdays,int sdays,roomdata data[])    //寮務当番
{
    int wday;
    int days=fdays+sdays;
    int day[days+400];
    for(int i=0;i<days+400;i++)
    {
        day[i]=0;
    }
    printf("What day of the week does it start on?\n");
    wday=get_valid_input("Monday (1) / Tuesday (2) / Wednesday (3) / Thursday (4) / Friday (5) / Saturday (6) / Sunday (7)",data,wnum);
    int lastm,last,l;
    int count=0;
    int i2=0;
    lastm=get_valid_input("Who was on duty last month? :",data,room);
    for(int i=0;i<residents;i++)
    {
        if(lastm==data[i].room_num)
        {
            last=i;
            break;
        }
    }
    int con=0;
    // 先月の最後の当番を起点に、さらに過去へ遡って基準となる人を探す。
    // （先月当番をやった人10が続く限り、前日の当番を遡り続ける）
    while(1)
    {
        if(data[last].features==10)
        {
            data[last].features=12;     // この人は先月当番をやっているので、一旦待避(12)にしてさらに前日の当番を探す
            lastm=get_valid_input("Who was on duty the day before? :",data,room);
            for(int i=0;i<residents;i++)
            {
                if(lastm==data[i].room_num)
                {
                    last=i;
                    break;
                }
            }
            con++;
            if(con>=residents)  //無限ループ防止
            {
                printf("error : There is no one available to take on the duty because everyone's features are set to 10.\n");
                printf("Please terminate the program by pressing Ctrl-C.\n");
                break;
            }
        }
        else
        {
            break;    // 先月当番をやっていない人（10以外）に辿り着いたら、その人を基準として遡りを終了する
        }
    }
    //先月夜間の関係で飛ばされた人を，今月の当番に入れる
    int night=last-1;
    if(night<0)
    {
        night+=residents;
    }
    while(count<20)
    {
        if((data[night].features)==1)
        {
            int wh;
            printf("Room %d\n",data[night].room_num);
            wh=get_valid_input("Should we assign this person to this month's duty? Yes (0) /No (1)",data,choice);
            if(wh==0)
            {
                if(data[night].exemption3>0)
                {
                    data[night].exemption3--;   //whが０でも，当番に入らないので注意．
                    data[night].features=11;    //当番表を作り終えた後，待避（11）から（10）にする．
                }
                else                            
                {
                    slide(&wday,&i2,&night,days,day,data);
                }
            }
            else       //割り当てない人を見つけたらその時点でループを抜ける
            {
                break;
            }
        }
        night--;
        if(night<0)
        {
            night=residents-1;
        }
        count++;
    }

    for(int i=0; i<residents; i++)    //先月当番に入った夜間の人や，先月免除権を使用して当番をスキップした人（10）を（1）にする
    {
        if(data[i].features==10)
        {
            data[i].features=1;
        }
    }
    for(int i=0; i<residents; i++)
    {
        if(data[i].features==11)
        {
            data[i].features=10;
        }
    }
    l=last+1;                //入る予定の人
    if(l>=residents)
    {
        l=0;
    }
    while(1)
    {
        int full=1;
        for(int i=0;i<days;i++)  //days分が埋まったら，表示に移る
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
        //無限ループ防止
        con=0;
        for(int i=0;i<residents;i++)
        {
            if(data[i].features>=0)
            {
                con=1;
                break;
            }
        }
        if(con==0)
        {
            printf("error : Everyone's features are negative.\nPlease terminate the program by pressing Ctrl-C.\n");
            break;
        }
        else
        {
            while(1)   //入る人の選定
            {
                if(data[l].features<0)
                {
                    ;
                }
                else if(data[l].features==12)
                {
                    data[l].features=1;
                }
                else if(data[l].exemption3>0)
                {
                    data[l].exemption3--;
                    if((data[l].features==1)||(data[l].features)==10)
                    {
                        data[l].features=10;  //免除権を使って当番をスキップした夜間生は，ここで（10）にする
                    }
                }
                else if((((data[l].exemption3)==0))&&((((data[l].features)!=1))&&(((data[l].features)!=10))))
                {
                    target_dormitory(day_time,&wday,&i2,&l,days,day,data);
                    l++;
                    if(l>=residents)
                    {
                        l=0;
                    }
                    break;
                }
                else if((((data[l].exemption3)==0))&&(((data[l].features)==1)||((data[l].features)==10)))
                {
                    target_dormitory(night_time,&wday,&i2,&l,days,day,data);
                    l++;
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
    }
    for(int i=0;i<days;i++)    //表示
    {
        printf("%d\n",day[i]);
        if(i==fdays-1)
        {
            printf("\n\n\n");
        }
    }
    //夜間生のfeaturesを変更
    for(int i=0;i<days;i++)
    {
        for(int j=0;j<residents;j++)
        {
            if((day[i]==data[j].room_num)&&(data[j].features==1))
            {
                data[j].features=10;
            }
        }
    }
}

int main (void)
{
    FILE *fp;
    FILE *fp2;
    int month,area,fdays,sdays,q,problem,ans,first_shift;
    char fname[256];
    char fname2[256];
    roomdata data[residents];
    first_shift=get_valid_input("Are we creating the first shift now? yes (0) /no (1)",data,choice);
    if(first_shift==0)
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
    month=get_valid_input("Which month's duty roster should we create?",data,mnum);
    //データの入力漏れがないかの確認
    if(first_shift==0)
    {
        if(month==4)
        {
            ans=get_valid_input("Have you entered the information of residents who have left the dormitory other than graduates? Yes (0) / No (1)",data,choice);
            if(ans==1)
            {
                printf("Please try again after entering the information.\n");
                return 0;
            }
        }
        if(month==5||month==11)
        {
            ans=get_valid_input("Is the information for the dormitory residents entered correctly? Yes (0) / No (1)",data,choice);
            if(ans==1)
            {
                printf("Please try again after entering the information.\n");
                return 0;
            }
        }
        if(month==4||month==6||month==7||month==11)
        {
            ans=get_valid_input("Have you entered your information as a volunteer? Yes (0) / No (1)",data,choice);
            if(ans==1)
            {
                printf("Please try again after entering the information.\n");
                return 0;
            }
        }
        if(month==6||month==12)
        {
            ans=get_valid_input("Have you correctly entered the information of the board members who are exempt from their duties? Yes (0) / No (1)",data,choice);
            if(ans==1)
            {
                printf("Please try again after entering the information.\n");
                return 0;
            }
        }
    }

    fdays=get_valid_input("How many days is the first half?",data,pnum);
    sdays=get_valid_input("How many days is the second half?",data,pnum);
    area=get_valid_input("What duty is it? Garbage duty (1) / Bath duty (2) / Dormitory duty (3)",data,area_num);
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
        return 0;
    }
    else
    {   
        for(int i=0;i<residents;i++)
        {
            fscanf(fp,"%d,%d,%d,%d,%d,%d",&data[i].room_num,&data[i].grade,&data[i].exemption1,&data[i].exemption2,&data[i].exemption3,&data[i].features);
        }
        fclose(fp);
        if((month>=1)&&(month<=3))
        {
            month123(data);    //4年制の4年生と，6年制の6年生のfeaturesを-1にする
        }
        if(month==5)
        {
            if(first_shift==0)
            month5mae(data);      //空き部屋の情報を入れる
        }
        if(area==1)
        {
            garbage(month,fdays,sdays,area,data);
        }
        if(area==2)
        {
            bath(month,fdays,sdays,area,data);
        }
        if(area==3)
        {
            dormitory(month,fdays,sdays,data);
        }
        //次の当番表を作るときのために準備
        if(month==4)
        {
            q=get_valid_input("Have you finished creating three duty rosters? yes(0) / no(1)",data,choice);
            if(q==0)
            month4(data);   //入寮生の情報を入力
        }
        if((month==5)||(month==11))
        {
            q=get_valid_input("Have you finished creating three duty rosters? yes(0) / no(1)",data,choice);
            if(q==0)
            month511(data,month);    //前C会役員の情報を更新
        }
        //問題がないかのチェック
        problem=get_valid_input("Are there any mistakes? no problem (0) / there is a problem (1)",data,choice);
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
                for(int j=0;j<residents;j++)
                {
                    fprintf(fp2,"%d,%d,%d,%d,%d,%d\n",data[j].room_num,data[j].grade,data[j].exemption1,data[j].exemption2,data[j].exemption3,data[j].features);
                }
                fclose(fp2);
                printf("The preservation was successful.");
            }
        }
    }

    return 0;
}