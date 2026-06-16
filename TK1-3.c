#include<stdio.h>
#include<string.h>
#define residents 108
#define NUMBER_OF_FLOORS 5
#define room 0
#define choice 1
#define pnum 2
#define target_num 3
#define direction_num 4
#define mnum 5
#define floor_num 6
#define FEATURE_NORMAL          0     // なにもない（通常の寮生）
#define FEATURE_NIGHT           1     // 夜間生（土日当番の候補）
#define FEATURE_6YEARS          6     // 6年制学生
#define FEATURE_GRAD_EXEMPT    -1     // 掃除なし(4年制4年・6年制5年生以上）
#define FEATURE_EMPTY  -2             // 空き部屋・退寮者（掃除免除）
#define FEATURE_LEAVE_ABSENCE  -3     // 休学・留学
#define FEATURE_BOARD_TERM2    -5     // 後期役員免除
#define FEATURE_BOARD_TERM1   -11     // 前期役員免除

//==================================================================================
//寮のルール設定エリア（＊ルールが変わったらここを変更！）


// ▼ 何年生から免除にするか？
#define RULE_EXEMPT_GRADE_4YEARS 4   // 4年制の免除学年
#define RULE_EXEMPT_GRADE_6YEARS 5   // 6年制の免除学年

// ▼ 何月に免除にするか？（m は入力された月）
// 【変更のしかた】月を増やしたい場合は「 || (m) == 12 」のようにつなげて書くだけ！
#define RULE_EXEMPT_MONTH(m)  ((m)==1||(m)==2||(m)==3||(m)==4||(m)==5||(m)==6||(m)==7||(m)==8||(m)==9||(m)==10||(m)==11||(m)==12)

//▼ 掃除場所は何か所か
#define AREA_COUNT 4

//掃除場所の回り方
#define floor1_turn 4
#define floor2_turn 4
#define floor3_turn 2
#define floor4_turn 3
#define floor5_turn 2

//===================================================================================



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
                case choice: if((input==0)||(input==1)) {is_valid=1;} break;
                case pnum: if(input>=0) {is_valid=1;} break;
                case target_num: if((input>=1)&&(input<=6)) {is_valid=1;} break;
                case direction_num: if ((input>=1)&&(input<=4)) {is_valid=1;} break;
                case mnum: if((input>=1)&&(input<=12)) {is_valid=1;} break;
                case floor_num: if((input>=1)&&(input<=5)) {is_valid=1;} break;

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

void month4mae(roomdata data[])  //卒業生がいた部屋を空き部屋状態に初期化してリセットし，空き部屋以外の学年を１つ上げる
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

void month5(roomdata data[])   //新入生が入らなかった空き部屋の設定
{
    int empty;
    empty=get_valid_input("How many empty rooms are there?",data,pnum);
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

void month511(roomdata data[],int month)    //旧役員の「免除権」の剥奪と、一部役員の属性（6年制等）の再設定
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

void search(int day,int place,int target_floor,int turn_direction,int *target_idx,roomdata data[],int shift_table[][AREA_COUNT]) //次に入る人を探す
{
    int loop_count=0;
    while(1)
    {
        loop_count++;
        if(loop_count>residents)  // 全員チェックしても誰もいなかった場合のフリーズ防止
        {
            printf("error : No candidates were found.\nPlease terminate the program by pressing Ctrl-C.\n");
            return;

        }
        //1.対象外の人（役員・休学・学年免除など）は無条件でスルーする
        if(((data[(*target_idx)].features)<FEATURE_NORMAL)||((data[(*target_idx)].temp_exempt)==1))
        {
            ;
        }
        //2. 他階に出張中の人（本来の階の順番が回ってきたら、免除権を使ってスキップする）
        else if(((data[(*target_idx)].room_num)/100)!=(data[(*target_idx)].floor))   //朝掃除サポートで他の階の掃除に当たってる人
        {
            if((data[(*target_idx)].exemption>0)&&(((data[(*target_idx)].room_num)/100)==target_floor))  //元の掃除場所の当番表を作っているとき
            {
                data[(*target_idx)].exemption--;    //免除権があるまでは減らしていき，０になったら何もせず飛ばす
            }
        }
        //3. 普通にその階にいる住人の場合
        else if((data[(*target_idx)].floor)==target_floor)
        {
            if(((data[(*target_idx)].room_num)/100)==target_floor)  //本当の住人かを確認し，サポートスタッフが巻き込まれるのを防ぐ
            {
                if(((data[(*target_idx)].exemption)>0))
                {
                    data[(*target_idx)].exemption--;   // 免除権があれば消費して当番を回避
                }
                else
                {
                    shift_table[day][place]=data[(*target_idx)].room_num;    // 免除がないので当番枠に決定！
                    // ルーレットの針（インデックス）を次に進める
                    if(turn_direction==1||turn_direction==3)
                    {
                        (*target_idx)++;
                        if((*target_idx)>=residents)
                        {
                            (*target_idx)=0;
                        }
                    }
                    if(turn_direction==2||turn_direction==4)
                    {
                        (*target_idx)--;
                        if((*target_idx)<0)
                        {
                            (*target_idx)=residents-1;
                        }
                    }
                    break;   // 当番が決まったのでループを抜ける
                }
            }
        }
        // 当番に選ばれなかった場合、次の候補者をチェックするためにルーレットを進める
        if(turn_direction==1||turn_direction==3)
        {
            (*target_idx)++;
            if((*target_idx)>=residents)
            {
                (*target_idx)=0;
            }
        }
        if(turn_direction==2||turn_direction==4)
        {
            (*target_idx)--;
            if((*target_idx)<0)
            {
                (*target_idx)=residents-1;
            }
        }
    }
}

void make1(int staff_count,int target_floor,int min,int last_idx,int last1,int turn_direction,int first[],roomdata data[],int shift_table[][AREA_COUNT],int last[],int *target_idx) //1日目のシフトを作る
{
    // 前回最後の当番の「次の人」からスタート位置を決める
    if(turn_direction==1||turn_direction==3)
    {

        (*target_idx)=last1+1;
        if((*target_idx)>=residents)
        {
            (*target_idx)=0;
        }
    }
    if(turn_direction==2||turn_direction==4)
    {
        (*target_idx)=last1-1;
        if((*target_idx)<0)
        {
            (*target_idx)=residents-1;
        }
    }
    if(staff_count>0)
    {
        min=1;   //サポートスタッフがいる場合、[0]の場所を空けておくための底上げ
    }
    // シフトをAREA_COUNT-1から埋めていく
    for(int i=AREA_COUNT-1;i>=min;i--)
    {
        int found=0;  
        // 過去の当番履歴（last配列）から、今回も継続して当番に入れる人を引き継ぐ
        while(last_idx>=0)
        {
            if(((data[last[last_idx]].features)>=FEATURE_NORMAL)&&((data[last[last_idx]].floor)==target_floor)&&((data[last[last_idx]].temp_exempt)==0))
            {
                shift_table[0][i]=data[last[last_idx]].room_num;
                last_idx--;
                found=1;
                break;
            }
            last_idx--;
        }
        // 引き継げる人がいなかったら、ルーレット(search)で新しい人を探して埋める
        if(found==0)
        {
            search(0,i,target_floor,turn_direction,target_idx,data,shift_table);
        }
    }

}

void print(int i,int start,int end,int turn_direction,int shift_table[][AREA_COUNT])   // 指定された方向に沿って、1日分のシフトを画面に出力する関数
{
    for(int j=start;j!=end;)
    {
        printf("%d\t",shift_table[i][j]);
        if(turn_direction==1||turn_direction==2)
        {
            j--;
        }
        if(turn_direction==3||turn_direction==4)
        {
            j++;
        }

    }
    printf("%d\n",shift_table[i][end]);
}

void turn_second(int day_count,int staff_count,int target_floor,int turn_direction,int support_staff_idx,int *target_idx,int shift_table[][AREA_COUNT],roomdata data[])
{
    int start;
    int end;
    // 表示方向の設定
    if(turn_direction==1||turn_direction==2)
    {
        start=AREA_COUNT-1;
        end=0;
    }
    if(turn_direction==3||turn_direction==4)
    {
        start=0;
        end=AREA_COUNT-1;
    }
    for(int i=start;i!=end;)
    {
        // 1日目を出力
        printf("%d\t",shift_table[0][i]);
        if(turn_direction==1||turn_direction==2)
        {
            i--;
        }
        if(turn_direction==3||turn_direction==4)
        {
            i++;
        }

    }
    printf("%d\n",shift_table[0][end]);
    // 2日目以降のシフトを作成＆出力
    for(int i=1;i<day_count;i++)
    {
        for(int j=0;j<AREA_COUNT-1;j++)
        {
            printf("#\t");
        }
        printf("#\n");
        for(int j=0;j<AREA_COUNT-1;j++)
        {
            printf(":\t");
        }
        printf(":\n");
        if(i<staff_count)
        {
            // サポートスタッフが複数いる間は、全員をローテーションに組み込む
            for(int j=1;j<AREA_COUNT;j++)
            {
                shift_table[i][j]=shift_table[i-1][j-1];
            }
            print(i,start,end,turn_direction,shift_table);
        }
        else
        {
            // 既存のメンバーを斜めにスライドさせる
            for(int j=1;j<AREA_COUNT;j++)
            {
                shift_table[i][j]=shift_table[i-1][j-1];
            }
            // 最前列 [0] に誰を入れるかの判断

            // サポートスタッフが2周目に突入する条件を満たしていれば、サポートスタッフを再配置
            if((staff_count>0)&&((shift_table[i][1]==shift_table[0][1])||(((shift_table[i][1]/100)!=target_floor)&&(shift_table[i][1]!=shift_table[staff_count-1][0]))))
            {
                shift_table[i][0]=shift_table[support_staff_idx][0];
                support_staff_idx++;
                print(i,start,end,turn_direction,shift_table);
            }
            // そうでなければ、search関数を使って新しい一般メンバーを補充する
            else
            {
                search(i,0,target_floor,turn_direction,target_idx,data,shift_table);
                print(i,start,end,turn_direction,shift_table);
            }
        }
    }
}

void turn_main(int day_count,int target_floor,int staff_count,int turn_direction,int last1,int shift_table[][AREA_COUNT],roomdata data[],int first[],int last[])  // 各日を作成する関数の橋渡しをする関数
{
    int target_idx;
    int support_staff_idx=0;
    int last_idx=AREA_COUNT-2;
    int min=0;
    make1(staff_count,target_floor,min,last_idx,last1,turn_direction,first,data,shift_table,last,&target_idx);
    turn_second(day_count,staff_count,target_floor,turn_direction,support_staff_idx,&target_idx,shift_table,data);
}

int main (void)
{
    FILE *fp;
    FILE *fp2;
    int staff_room,all,ans,first_shift,loop=1,existence=1;
    int last1=0;
    int floor_order[5];
    char fname[256];
    char fname2[256];
    char message_buf[256];
    int month,day_count,target_floor_temporary,target_floor,staff_count,turn_direction,problem;
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
        month=get_valid_input("Which month's duty roster should we create?",data,mnum);
        // 初回作成時の入力漏れ確認
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
        if(first_shift==0)
        {
            if(month==4)
            month4mae(data);
            if(month==5)
            month5(data);
        }
        apply_exemptions(data,month);   //学年による免除
        target_floor_temporary=get_valid_input("What is the floor number? (Make all floors (6))",data,target_num);
        target_floor=target_floor_temporary;
        // 全階一括作成（6）が選ばれた場合、作成順序のリストを作る
        if(target_floor_temporary==6)
        {
            loop=NUMBER_OF_FLOORS;
            printf("Order of creating the duty roster\n");
            for(int i=0;i<NUMBER_OF_FLOORS;i++)
            {
                sprintf(message_buf, "(%d):", i+1);
                floor_order[i]=get_valid_input(message_buf,data,floor_num);
            }
        }
        day_count=get_valid_input("How many days?",data,pnum);
        // ▼ ここから各階層の当番表を連続で作成するメインループ
        for(int k=0;k<loop;k++)
        {
            if(target_floor_temporary==6)
            {
                target_floor=floor_order[k];    //全ての階を一気に作るときのtarget_floorの設定
            }
            switch(target_floor)
            {
                case 1: turn_direction=floor1_turn; break;
                case 2: turn_direction=floor2_turn; break;
                case 3: turn_direction=floor3_turn; break;
                case 4: turn_direction=floor4_turn; break;
                case 5: turn_direction=floor5_turn; break;
            }
            printf("----- About floor_%d -----\n",target_floor);
            if(existence!=0)
            {
                staff_count=get_valid_input("How many morning cleaning staff will on that floor?",data,pnum);
                if(staff_count==0)
                {
                    existence=0;
                }
            }
            else
            {
                staff_count=0;
            }
            int shift_table[day_count][AREA_COUNT];
            int first[AREA_COUNT];
            int last[AREA_COUNT];
            int staff[staff_count];

            // サポートスタッフの情報を取得し、対象の階(floor)を一時的に書き換える
            if(staff_count>0)
            {
                for(int i=0;i<staff_count;i++)
                {
                    sprintf(message_buf, "staff[%d]:", i+1);   //sprintfは文字列を作る関数
                    staff_room=get_valid_input(message_buf,data,room);
                    for(int j=0;j<residents;j++)
                    {
                        if(staff_room==data[j].room_num)
                        {
                            staff[i]=j;
                            break;
                        }
                    }
                    shift_table[i][0]=data[staff[i]].room_num;
                    data[staff[i]].floor=target_floor;
                }
            }
            printf("What was your last shift last month?\n");
            int start2;
            int end2;
            // 読み込む方向のセッティング
            if(turn_direction==1||turn_direction==2)
            {
                start2=AREA_COUNT-1;
                end2=-1;
            }
            if(turn_direction==3||turn_direction==4)
            {
                start2=0;
                end2=AREA_COUNT;
            }
            
            int count=1;
            for(int i=start2;i!=end2;)
            {
                sprintf(message_buf, "(%d):", count); 
                first[i]=get_valid_input(message_buf,data,room);
                
                for(int j=0;j<residents;j++)
                {
                    if(first[i]==data[j].room_num)
                    {
                        last[i]=j;
                    }
                }
                
                if(turn_direction==1||turn_direction==2)
                {
                    i--;
                }
                if(turn_direction==3||turn_direction==4)
                {
                    i++;
                }
                count++;
            }
            for(int i=0;i<AREA_COUNT;i++)
            {
                if((data[last[i]].floor)==target_floor)
                {
                    last1=last[i];
                    break;
                }
            }
            turn_main(day_count,target_floor,staff_count,turn_direction,last1,shift_table,data,first,last);   //当番表を作り終える
            if(staff_count>0)
            {
                for(int i=0;i<staff_count;i++)
                {
                    for(int j=0;j<day_count;j++)
                    {
                        if(shift_table[j][0]==data[staff[i]].room_num)  //1つでも掃除入ったら免除権ゲット
                        {
                            data[staff[i]].exemption++;
                        }
                    }
                }
            }
        }

        // サポートスタッフとして一時的に書き換えていた floor を元に戻すための判定
        int floor_changed=0;
        for(int i=0;i<residents;i++)
        {
            if((data[i].room_num/100)!=(data[i].floor))
            {
                floor_changed=1;
                break;
            }
        }

        // 後処理（データの復元や新入寮生の追加など）
        if(month==4||month==5||month==11||floor_changed==1)
        {
            all=get_valid_input("Have you finished creating all the duty rosters? yes (0) /no (1)",data,choice);
            if(all==0)
            {
                if(month==4)
                month4ato(data);
                if(month==5||month==11)
                month511(data,month);
                if(floor_changed==1)   // 変更があった場合のみ、全員の所属階を部屋番号（百の位）に合わせて一斉リセット
                {
                    for(int i=0;i<residents;i++)
                    {
                        data[i].floor=data[i].room_num/100;
                    }
                }
            }
        }

        // 最終確認とファイル書き出し
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
                for(int i=0;i<residents;i++)
                {
                    fprintf(fp2,"%d,%d,%d,%d,%d,%d,%d,%d\n",data[i].room_num,data[i].grade,data[i].floor,data[i].exemption,data[i].exemption1,data[i].exemption2,data[i].exemption3,data[i].features);
                }
                fclose(fp2);
                printf("The preservation was successful.");
            }
        }
    }

    return 0;
}