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
#define garbage_area 1
#define bath_area 2
#define dormitory_area 3
#define FEATURE_NORMAL          0     // なにもない（通常の寮生）
#define FEATURE_NIGHT           1     // 夜間生（土日当番の候補）
#define FEATURE_6YEARS          6     // 6年制学生
#define FEATURE_DONE_LAST_MO   10     // 夜間生関連　夜間生が免除権使用、または夜間生が先月当番に入った場合
#define FEATURE_TEMP_EVADE 11         // 夜間生関連　一時待避（免除権使用によるスキップ）
#define FEATURE_RETRO_EVADE 12        // 夜間生関連　遡り待避（過去の当番確認用）
#define FEATURE_GRAD_EXEMPT    -1     // 1月〜3月掃除なし（卒業前の4年制4年・6年制6年）
#define FEATURE_EMPTY  -2             // 空き部屋・退寮者（掃除免除）
#define FEATURE_LEAVE_ABSENCE  -3     // 休学・留学
#define FEATURE_BOARD_TERM2    -5     // 後期役員免除
#define FEATURE_BOARD_TERM1   -11     // 前期役員免除
#define BUFFER_SIZE 400               // 配列外アクセスを防ぐための余裕幅

//==================================================================================
//寮のルール設定エリア（＊ルールが変わったらここを変更！）


// ▼ 何年生から免除にするか？
#define RULE_EXEMPT_GRADE_4YEARS 4   // 4年制の免除学年
#define RULE_EXEMPT_GRADE_6YEARS 6   // 6年制の免除学年

// ▼ 何月に免除にするか？（m は入力された月）
// 【変更のしかた】月を増やしたい場合は「 || (m) == 12 」のようにつなげて書くだけ！
#define RULE_EXEMPT_MONTH(m)  ((m) == 1 || (m) == 2 || (m) == 3)

//==================================================================================



typedef struct DATA{
    int room_num;
    int grade;
    int floor;
    int exemption;
    int exemption1;
    int exemption2;
    int exemption3;
    int features;
    int temp_exempt;
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
                case area_num: if((input>=1)&&(input<=4)) is_valid=1; break;
            }
            if(is_valid==1)
            {
                return input;
            }
            else
            {
                printf("error : Please enter the correct number.\n");
            }
        }
    }
}

void apply_exemptions(roomdata data[], int month)    //学年による免除についての関数
{
    if (RULE_EXEMPT_MONTH(month))      //免除が適用される月だったら
    {
        for(int i=0; i<residents; i++)
        {
            // 4年制で、設定した学年（4年）以上の人か？　もしくは，6年制で、設定した学年（6年）以上の人だったら掃除を免除
            if((((data[i].features)!=FEATURE_6YEARS)&&((data[i].grade)>=RULE_EXEMPT_GRADE_4YEARS))||(((data[i].features)==FEATURE_6YEARS)&&((data[i].grade)>=RULE_EXEMPT_GRADE_6YEARS)))
            {
                data[i].temp_exempt=1;    //学年による免除フラグをon
            }
        }
    }
}

void month4mae(roomdata data[])     //卒業生がいた部屋を空き部屋状態に初期化してリセットし，空き部屋以外の学年を１つ上げる
{
    for(int i=0;i<residents;i++)
    {
        if((((data[i].features)!=FEATURE_6YEARS)&&((data[i].grade)==4)) || (((data[i].features)==FEATURE_6YEARS)&&((data[i].grade)==6)))
        {
            data[i].features=FEATURE_EMPTY;
            data[i].grade=0;
        }
        else if(data[i].features!=FEATURE_EMPTY)
        {
            data[i].grade++;
        }
    }
}

void month4ato(roomdata data[])   //空き部屋になっていた枠に新1年生を入寮させる処理
{
    for(int i=0;i<residents;i++)
    {
        if((data[i].features)==FEATURE_EMPTY)
        {
            data[i].grade=1;
            data[i].exemption=0;
            data[i].features=FEATURE_NORMAL;
        }
    }
}

void month5mae(roomdata data[])   //新入生が入らなかった空き部屋の設定
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
                data[j].features=FEATURE_EMPTY;
                break;
            }
        }
    }
}

void month511(roomdata data[],int month)     //旧役員の「免除権」の剥奪と、一部役員の属性（6年制等）の再設定
{
    int evening;
    int six;
    int change=-1*month;
    for(int j=0;j<residents;j++)
    {
        if(data[j].features==change)
        {
            data[j].features=FEATURE_NORMAL;
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
                data[j].features=FEATURE_NIGHT;
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
                data[j].features=FEATURE_6YEARS;
                break;
            }
        }
    }
}

void slide(int *wday,int *day_idx,int *target_idx,int days,int day[],roomdata data[])  //ターゲットを土日に配置する
{
    int shift_days;
    int weekend_step=0;
    int prevention=0;
    if(*wday==7)
    {
        shift_days=0;
        weekend_step=1;
    }
    else
    {
        shift_days=6-*wday;
    }
    //当番表の枠にまだ空きがあるかチェック
    for(int i=0;i<days+BUFFER_SIZE;i++)
    {
        if(day[i]==0)
        {
            prevention=1;
        }
    }
    if(prevention==0)
    {
        ;    //空きが無かったら夜間の人は今月は当番に入れない
    }
    else
    {
        while(((((*day_idx)+shift_days)<days+BUFFER_SIZE))&&(day[(*day_idx)+shift_days]!=0))  //まず，dayの配列より大きくなってないかを確認
        {
            if(weekend_step==0)
            {
                shift_days++;
                weekend_step++;
            }
            else if(weekend_step==1)
            {
                shift_days+=6;
                weekend_step--;
            }
        }
        if(((*day_idx)+shift_days)<days+BUFFER_SIZE)  //daysの大きさを超えてたらdayの配列にその人を入れない
        {
            day[(*day_idx)+shift_days]=data[(*target_idx)].room_num;
        }
    }
}

void target_dormitory(int judge,int *wday,int *day_idx,int *target_idx,int days,int day[],roomdata data[])
{
    if((judge==night_time)&&((*wday)>=1)&&((*wday)<=5))
    {
        slide(wday,day_idx,target_idx,days,day,data);
    }
    else
    {
        while(day[(*day_idx)]!=0)
        {
            (*day_idx)++;
            if((*wday)>=7)
            (*wday)=1;
            else
            (*wday)++;
        }
        if((judge==night_time)&&(((*wday)>=1)&&((*wday)<=5)))
        {
            slide(wday,day_idx,target_idx,days,day,data);
        }
        else
        {
            day[(*day_idx)]=data[(*target_idx)].room_num;
            (*day_idx)++;
            if((*wday)>=7)
            (*wday)=1;
            else
            (*wday)++;
        }
    }
}

void assign_duty(roomdata data[],int *target_idx,int day[],int i,int area)   //当番に入れる人を探す
{
    //無限ループ防止
    int prevention=0;
    for(int j=0;j<residents;j++)
    {
        if(data[j].features>=FEATURE_NORMAL)
        {
            prevention=1;
            break;
        }
    }
    if(prevention==0)
    {
        printf("error : Everyone's features are negative.\nPlease terminate the program by pressing Ctrl-C.\n");
    }

    else
    {
        while(1)    //当番に入る人の選定
        {
            if(((data[*target_idx].features)<FEATURE_NORMAL)||(data[*target_idx].temp_exempt==1))
            {
                ;
            }
            else
            {
                int *ex;
                if(area==1)
                ex=&data[*target_idx].exemption1;
                if(area==2)
                ex=&data[*target_idx].exemption2;
                if(area==3) 
                ex=&data[*target_idx].exemption3;
                if((*ex)>0)
                {
                    (*ex)--;
                }
                else if((*ex)==0)
                {
                    day[i]=data[*target_idx].room_num;
                    (*target_idx)++;
                    if(*target_idx>=residents)
                    {
                        *target_idx=0;
                    }
                    break;
                }
            }
            (*target_idx)++;
            if(*target_idx>=residents)
            {
                *target_idx=0;
            }
        }
    }
}

void garbage(int month,int fdays1,int sdays1,int area,roomdata data[])     //ゴミ当番
{
    int wday,lastm;
    int days=fdays1+sdays1;
    int day[days];
    printf("-----About garbage duty-----\n");
    printf("What day of the week does it start on?\n");
    wday=get_valid_input("Monday (1) / Thursday (4)",data,gnum);
    int last,target_idx;
    lastm=get_valid_input("Who was on duty last month? :",data,room);
    for(int i=0;i<residents;i++)
    {
        if(lastm==data[i].room_num)
        {
            last=i;
            break;
        }
    }
    target_idx=last+1;
    if(target_idx>=residents)
    {
        target_idx=0;
    }
    for(int i=0;i<days;i++)
    {
        if(wday==1)
        {
            assign_duty(data,&target_idx,day,i,garbage_area);  //当番に入る人の選定
            printf("%d",day[i]);
            printf("\n\n\n");
            wday+=3;
        }
        else if(wday==4)
        {
            assign_duty(data,&target_idx,day,i,garbage_area);   //当番に入る人の選定
            printf("%d",day[i]);
            printf("\n\n\n\n");
            wday-=3;
        }
        if(i==fdays1-1)
        {
            printf("\n\n\n\n");
        }
    }
}

void bath(int month,int fdays1,int sdays1,int area,roomdata data[])   //風呂当番
{
    int lastm;
    int days=fdays1+sdays1;
    int day[days];
    int last,target_idx;
    printf("-----About bath duty-----\n");
    lastm=get_valid_input("Who was on duty last month? :",data,room);
    for(int i=0;i<residents;i++)
    {
        if(lastm==data[i].room_num)
        {
            last=i;
            break;
        }
    }
    target_idx=last+1;
    if(target_idx>=residents)
    {
        target_idx=0;
    }
    for(int i=0;i<days;i++)
    {
        assign_duty(data,&target_idx,day,i,bath_area);   //当番に入る人の選定
    }
    for(int i=0;i<days;i++)                 //表示
    {
        printf("%d\n",day[i]);
        if(i==fdays1-1)
        {
            printf("\n\n\n");
        }
    }
}

void dormitory(int month,int fdays1,int sdays1,roomdata data[])    //寮務当番
{
    int wday;
    int days=fdays1+sdays1;
    int day[days+BUFFER_SIZE];
    for(int i=0;i<days+BUFFER_SIZE;i++)
    {
        day[i]=0;
    }
    printf("-----About dormitory duty-----\n");
    printf("What day of the week does it start on?\n");
    wday=get_valid_input("Monday (1) / Tuesday (2) / Wednesday (3) / Thursday (4) / Friday (5) / Saturday (6) / Sunday (7)",data,wnum);
    int lastm,last,target_idx;
    int count=0;
    int day_idx=0;
    lastm=get_valid_input("Who was on duty last month? :",data,room);
    for(int i=0;i<residents;i++)
    {
        if(lastm==data[i].room_num)
        {
            last=i;
            break;
        }
    }
    int prevention=0;
    // 先月の最後の当番を起点に、さらに過去へ遡って基準となる人を探す。
    // （先月当番をやった人10が続く限り、前日の当番を遡り続ける）
    while(1)
    {
        if(data[last].features==FEATURE_DONE_LAST_MO)
        {
            data[last].features=FEATURE_RETRO_EVADE;     // この人は先月当番をやっているので、一旦待避(12)にしてさらに前日の当番を探す
            lastm=get_valid_input("Who was on duty the day before? :",data,room);
            for(int i=0;i<residents;i++)
            {
                if(lastm==data[i].room_num)
                {
                    last=i;
                    break;
                }
            }
            prevention++;
            if(prevention>=residents)  //無限ループ防止
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
        if((data[night].features)==FEATURE_NIGHT)
        {
            int assign_choice;
            printf("Room %d\n",data[night].room_num);
            assign_choice=get_valid_input("Should we assign this person to this month's duty? Yes (0) /No (1)",data,choice);
            if(assign_choice==0)
            {
                if(data[night].exemption3>0)
                {
                    data[night].exemption3--;   //whが０でも，当番に入らないので注意．
                    data[night].features=FEATURE_TEMP_EVADE;    //当番表を作り終えた後，待避（11）から（10）にする．
                }
                else                            
                {
                    slide(&wday,&day_idx,&night,days,day,data);
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
        if(data[i].features==FEATURE_DONE_LAST_MO)
        {
            data[i].features=FEATURE_NIGHT;
        }
    }
    for(int i=0; i<residents; i++)
    {
        if(data[i].features==FEATURE_TEMP_EVADE)
        {
            data[i].features=FEATURE_DONE_LAST_MO;
        }
    }
    target_idx=last+1;                //入る予定の人
    if(target_idx>=residents)
    {
        target_idx=0;
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
        prevention=0;
        for(int i=0;i<residents;i++)
        {
            if(data[i].features>=FEATURE_NORMAL)
            {
                prevention=1;
                break;
            }
        }
        if(prevention==0)
        {
            printf("error : Everyone's features are negative.\nPlease terminate the program by pressing Ctrl-C.\n");
            break;
        }
        else
        {
            while(1)   //入る人の選定
            {
                if(((data[target_idx].features)<FEATURE_NORMAL)||(data[target_idx].temp_exempt==1))
                {
                    ;
                }
                else if(data[target_idx].features==FEATURE_RETRO_EVADE)
                {
                    data[target_idx].features=FEATURE_NIGHT;
                }
                else if(data[target_idx].exemption3>0)
                {
                    data[target_idx].exemption3--;
                    if((data[target_idx].features==FEATURE_NIGHT)||(data[target_idx].features)==FEATURE_DONE_LAST_MO)
                    {
                        data[target_idx].features=FEATURE_DONE_LAST_MO;  //免除権を使って当番をスキップした夜間生は，ここで（10）にする
                    }
                }
                else if((((data[target_idx].exemption3)==0))&&((((data[target_idx].features)!=FEATURE_NIGHT))&&(((data[target_idx].features)!=FEATURE_DONE_LAST_MO))))
                {
                    target_dormitory(day_time,&wday,&day_idx,&target_idx,days,day,data);
                    target_idx++;
                    if(target_idx>=residents)
                    {
                        target_idx=0;
                    }
                    break;
                }
                else if((((data[target_idx].exemption3)==0))&&(((data[target_idx].features)==FEATURE_NIGHT)||((data[target_idx].features)==FEATURE_DONE_LAST_MO)))
                {
                    target_dormitory(night_time,&wday,&day_idx,&target_idx,days,day,data);
                    target_idx++;
                    if(target_idx>=residents)
                    {
                        target_idx=0;
                    }
                    break;
                }
                (target_idx)++;
                if(target_idx>=residents)
                {
                    target_idx=0;
                }
            }
        }
    }
    for(int i=0;i<days;i++)    //表示
    {
        printf("%d\n",day[i]);
        if(i==fdays1-1)
        {
            printf("\n\n\n");
        }
    }
    //夜間生のfeaturesを変更
    for(int i=0;i<days;i++)
    {
        for(int j=0;j<residents;j++)
        {
            if((day[i]==data[j].room_num)&&(data[j].features==FEATURE_NIGHT))
            {
                data[j].features=FEATURE_DONE_LAST_MO;
            }
        }
    }
}

int main (void)
{
    FILE *fp;
    FILE *fp2;
    int month,area,fdays1,sdays1,fdays2=0,sdays2=0,problem,ans,first_shift;
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

    area=get_valid_input("What duty is it? Garbage duty (1) / Bath duty (2) / Dormitory duty (3) / All area (4)",data,area_num);
    if(area==4)
    {
        fdays2=get_valid_input("-----About garbage duty-----\nHow many days is the first half?",data,pnum);
        sdays2=get_valid_input("How many days is the second half?",data,pnum);
        printf("-----About bath duty and dormitory duty-----\n");
    }
    fdays1=get_valid_input("How many days is the first half?",data,pnum);
    sdays1=get_valid_input("How many days is the second half?",data,pnum);
    
    if((fp=fopen(fname,"r"))==NULL)
    {
        printf("The file cannot be opened.\n");
        return 0;
    }
    else
    {   
        for(int i=0;i<residents;i++)
        {
            fscanf(fp,"%d,%d,%d,%d,%d,%d,%d,%d",&data[i].room_num,&data[i].grade,&data[i].floor,&data[i].exemption,&data[i].exemption1,&data[i].exemption2,&data[i].exemption3,&data[i].features);
            data[i].temp_exempt=0;
        }
        fclose(fp);
        if(first_shift==0)
        {
            if(month==4)
            month4mae(data);  //卒業生がいた部屋を空き部屋に設定して，寮生の学年を１つ上げる．
            if(month==5)
            month5mae(data);   //空き部屋の情報を入れる
        }
        apply_exemptions(data,month);
        if(area==1||area==4)
        {
            if(area==1)
            garbage(month,fdays1,sdays1,area,data);
            if(area==4)
            garbage(month,fdays2,sdays2,area,data);
        }
        if(area==2||area==4)
        {
            bath(month,fdays1,sdays1,area,data);
        }
        if(area==3||area==4)
        {
            dormitory(month,fdays1,sdays1,data);
        }
        int floor_changed=0;
        for(int i=0;i<residents;i++)
        {
            if((data[i].room_num/100)!=(data[i].floor))
            {
                floor_changed=1;
                break;
            }
        }
        //次の当番表を作るときのために準備
        if(month==4||month==5||month==11||floor_changed==1)
        {
            int all;
            all=get_valid_input("Have you finished creating all the duty rosters? yes (0) /no (1)",data,choice);
            if(all==0)
            {
                if(month==4)
                month4ato(data);         //全ての空室に入寮生（１年生）をいったん入れる
                if(month==5||month==11)
                month511(data,month);   //役員の人の掃除免除権を無くす
                if(floor_changed==1)   //朝掃除サポートの人をもとの階数にする
                {
                    for(int i=0;i<residents;i++)
                    {
                        data[i].floor=data[i].room_num/100;
                    }
                }
            }
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
                    fprintf(fp2,"%d,%d,%d,%d,%d,%d,%d,%d\n",data[j].room_num,data[j].grade,data[j].floor,data[j].exemption,data[j].exemption1,data[j].exemption2,data[j].exemption3,data[j].features);
                }
                fclose(fp2);
                printf("The preservation was successful.");
            }
        }
    }

    return 0;
}