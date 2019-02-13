#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

//学生的定义
typedef struct {
    char name[20];
    int ID;
    float score[6];
} student;

//链表的定义
typedef struct stulist {
    student item;
    struct stulist* next;
} Stulist;

//链表管理器定义
typedef struct {
    Stulist* head;
    Stulist* lend;
    int itemnum;
    int scorenum;
} ListMgr;

static ListMgr aMgr;

//清除缓冲区
void clearall()
{
    char ch;
    while((ch = getchar() != '\n') && ch != '\0')
        continue;
};

//获取学生科目数---------初次调用获取输入，以后调用得到学生科目数
int getscorenum(int it)
{
    //同等于0则未初始化
    if(aMgr.scorenum == 0 && it == 0) {
        printf("**********************\nInput  the num of subjects:");
        while((scanf("%d", &aMgr.scorenum) != 1) || aMgr.scorenum < 1 || aMgr.scorenum > 6) {
            printf("Wrong!!!(for 1-6):");
            clearall();
        }
        printf("**********************\n");
    } else if(it != 0)
        aMgr.scorenum = it;
    return aMgr.scorenum;
}

//追加链表
Stulist* list_append()
{
    Stulist* temp = aMgr.head;
    //判断链表头部是否存在
    if(aMgr.head == NULL || aMgr.itemnum == 0) {
        aMgr.head = aMgr.lend = (Stulist*)malloc(sizeof(Stulist));
        temp = aMgr.head;
        //初始化
        aMgr.itemnum = 1;
    } else {
        temp = aMgr.lend;
        aMgr.lend = (Stulist*)malloc(sizeof(Stulist));
        temp->next = aMgr.lend;
        temp = aMgr.lend;
        aMgr.itemnum++;
    }
    return temp;
};

//删除pos位置的链表元素
void list_delete(int pos)
{
    //保存链表头部
    Stulist* temp = aMgr.head;
    if(pos > aMgr.itemnum) {
        return;
    }
    if(pos == 1) {
        //头部后移
        aMgr.head = aMgr.head->next;
        //内存释放
        free(temp);
    } else {
        int i;
        //移动到要删除的位置前一位以删除pos位置的元素
        for(i = 1; i != pos - 1; i++) {
            temp = temp->next;
        }
        //如果删除的是尾节点，则更新尾节点为尾节点的前一个
        if(pos == aMgr.itemnum)
            aMgr.lend = temp;
        //保存该位置元素
        Stulist* itempos = temp->next;
        // pos位置更改为pos+1位元素
        temp->next = itempos->next;
        //内存释放
        free(itempos);
    }
    aMgr.itemnum--;
}

//释放所有内存
void list_free()
{
    while(aMgr.itemnum != 0)
        list_delete(1);
}

//显示菜单
void showmenu()
{
    printf("Management for Students' scores\n"
           "1.Input record\n"
           "2.Caculate total and average score of every course\n"
           "3.Caculate total and average score of every student\n"
           "4.Sort in descending order by total score \n"
           "5.Sort in ascending order by total score \n"
           "6.Sort in ascending order by ID\n"
           "7.Sort in ascending order by name\n"
           "8.Search by ID\n"
           "9.Search by name\n"
           "10.Statistic analyse for every course\n"
           "11.List record\n"
           "12.Write to a file\n"
           "13.Read from a file\n"
           "0.Exit\n"
           "*****************************************************************\n"
           "Please enter your choice:");
}

//录入学生数据
void inputstu(student* stu)
{
    printf("Input Student Name(Less than 20 chars):");
    fgets(stu->name, sizeof(stu->name), stdin);
    //去掉最后一位的换行符
    int len = strlen(stu->name) - 1;
    stu->name[len] = '\0';

    printf("Input ID:");
    //录入并进行错误检查
    while(scanf("%d", &stu->ID) != 1) {
        printf("Wrong!!!\nInput again:");
        clearall();
    }
    clearall();
    int i;
    for(i = 0; i != getscorenum(0); i++) {
        printf("Input the score of subject[%d]:", i);
        // 0<成绩<=100
        while((scanf("%f", &stu->score[i]) != 1) || stu->score[i] < 0 || stu->score[i] > 100) {
            printf("Wrong!!!\nInput again:");
            clearall();
        }
    }
    clearall();
}

//添加一个学生
void addstu()
{
    char ch;
    while(1) {
        inputstu(&list_append()->item);
        //是否继续
        printf("To Continue?(Y,N)");
        ch = getchar();
        if(ch != 'Y' && ch != 'y')
            break;
        clearall();
    };
}

//打印一个元素
void printstu(const student stu)
{
    int i;
    printf("ID:   %05d  Name:%10s", stu.ID, stu.name);
    for(i = 0; i != getscorenum(0); i++)
        printf("  score[%d]:%5.2f", i, stu.score[i]);
    putchar('\n');
}

//打印全部
void printall()
{
    int i;
    Stulist* temp = aMgr.head;
    printf("-------------------------******-------------------------\n");
    for(i = 0; i != aMgr.itemnum; i++) {
        printstu(temp->item);
        temp = temp->next;
    }
    printf("-------------------------******------------------------\n");
}

void deletestu()
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    int pos;
    printf("Input the pos:");
    while(scanf("%d", &pos) != 1) {
        printf("Wrong!!!\nInput again:");
        clearall();
    }
    if((pos < aMgr.itemnum) && (pos > 0))
        list_delete(pos);
    else
        printf("No this item!!!\n");
}

//从文件读取数据
void getfromfile()
{
    FILE* fp;
    if((fp = fopen("Student.txt", "r")) == NULL) {
        fprintf(stdout, "Cant open \"Student.txt\" file.\n");
        exit(EXIT_FAILURE);
    }
    int scorenum, stunum, i;
    //读入科目数，学生数
    fscanf(fp, " %d %d", &scorenum, &stunum);
    getscorenum(scorenum);
    for(i = 0; i != stunum; i++) {
        student* stu = &list_append()->item;
        fscanf(fp, "%s", stu->name);
        fscanf(fp, "%d", &stu->ID);
        for(scorenum = 0; scorenum != getscorenum(0); scorenum++)
            fscanf(fp, "%f", stu->score + scorenum);
    }
    fclose(fp);
    printf("Import Successfully!\n");
}

//写入数据
void savetofile()
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    FILE* fp;
    if((fp = fopen("Student.txt", "w")) == NULL) {
        fprintf(stdout, "Cant creat \"Student.txt\" file.\n");
        exit(EXIT_FAILURE);
    }
    //写入科目数，学生数
    fprintf(fp, "%d %d\n", getscorenum(0), aMgr.itemnum);
    int i, j;
    const Stulist* temp = aMgr.head;
    for(i = 0; i != aMgr.itemnum; i++) {
        fprintf(fp, "%s %d", temp->item.name, temp->item.ID);
        for(j = 0; j != aMgr.scorenum; j++)
            fprintf(fp, "%5.2f", temp->item.score[j]);
        fprintf(fp, "\n");
        temp = temp->next;
    }
    fclose(fp);
    printf("Export Successfully!\n");
}

//交换两个item数据
void swapstu(student* a, student* b)
{
    //交换name
    char tempstr[20];
    strcpy(tempstr, a->name);
    strcpy(a->name, b->name);
    strcpy(b->name, tempstr);
    //交换ID
    int tempID;
    tempID = a->ID;
    a->ID = b->ID;
    b->ID = tempID;
    //交换成绩;
    int i;
    float tempscore;
    for(i = 0; i != getscorenum(0); i++) {
        tempscore = a->score[i];
        a->score[i] = b->score[i];
        b->score[i] = tempscore;
    }
}

//计算每门科目总分和平均分
void CacuCourse()
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    float total;
    printf("Sort in dictionary order by name:\n");
    int i, j;
    for(i = 0; i != getscorenum(0); i++) {
        const Stulist* temp = aMgr.head;
        total = 0;
        for(j = 0; j != aMgr.itemnum; j++) {
            total += temp->item.score[i];
            temp = temp->next;
        }
        printf("Sub[%d]    Total:%5.2f    Average:%5.2f\n", i, total, total / aMgr.itemnum);
    }
    printf("----------------------------------------\n");
}

//学生总成绩
float stutotalscore(student stu)
{
    float total = 0;
    int i;
    for(i = 0; i != getscorenum(0); i++)
        total += stu.score[i];
    return total;
}

//每个学生的总分和平均分
void CacuStuCourse()
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    float total = 0;
    int i;
    const Stulist* temp = aMgr.head;
    printf("----------------------------------------\n");
    for(i = 0; i != aMgr.itemnum; i++) {
        total = stutotalscore(temp->item);
        printf("StuID:%04d  Name:%8s  Total:%5.2f   Average:%5.2f\n", temp->item.ID, temp->item.name, total,
            total / getscorenum(0));
        temp = temp->next;
    }
    printf("----------------------------------------\n");
}

// ID排序
int cmpID(student a, student b)
{
    //小ID在前
    if(b.ID < a.ID)
        return 1;
    else
        return 0;
}
//姓名 字典序
int Sortbyname(student a, student b)
{
    if(strcmp(a.name, b.name) < 0)
        return 0;
    else
        return 1;
}

//成绩升序
int cmpup(student a, student b)
{
    //成绩低在前
    if(stutotalscore(a) > stutotalscore(b))
        return 1;
    else
        return 0;
}

//成绩降序
int cmpdown(student a, student b)
{
    //成绩高在前
    if(stutotalscore(a) < stutotalscore(b))
        return 1;
    else
        return 0;
}

//排序
void sortstu(int (*compare)(student a, student b))
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    int i, j;
    Stulist *temp1, *temp2;
    //初始化为头部
    temp1 = aMgr.head;
    for(i = 0; i != aMgr.itemnum; i++) {
        // temp1与其之后的元素进行比较
        temp2 = temp1->next;
        for(j = i + 1; j != aMgr.itemnum; j++) {
            if((*compare)(temp1->item, temp2->item) == 1)
                swapstu(&temp1->item, &temp2->item);
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    puts("Sort successfully!!!\n");
}

//返回某一成绩的排名
int get_grade(const student toget)
{
    float score_get = stutotalscore(toget);
    //默认第一名
    int grade = 1;
    //索引计数
    int i;
    const Stulist* Temp = aMgr.head;
    for(i = 0; i != aMgr.itemnum; i++) {
        if(stutotalscore(Temp->item) > score_get)
            grade++;
        Temp = Temp->next;
    }
    return grade;
}

//查找ID
void stufindID()
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    int ID;
    int i;
    int sign = 0;
    printf("Input the number you want to search:");
    scanf("%d", &ID);
    const Stulist* temp = aMgr.head;
    //遍历查找
    for(i = 0; i != aMgr.itemnum; i++) {
        if(temp->item.ID == ID) {
            printf("NO.%d  ", get_grade(temp->item));
            printstu(temp->item);
            sign = 1;
            //找到目标终止
            // return;
        }
        temp = temp->next;
    }
    if(sign == 0)
        printf("Can't find it!!!\n");
}

//查找姓名
void stufindname()
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    char name[20];
    int i;
    int sign = 0;
    printf("Input the name you want to search:");
    //去除尾部回车
    fgets(name, sizeof(name), stdin);
    int len = strlen(name) - 1;
    name[len] = '\0';
    const Stulist* temp = aMgr.head;
    for(i = 0; i != aMgr.itemnum; i++) {
        if(strcmp(name, temp->item.name) == 0) {
            printf("NO.%d  ", get_grade(temp->item));
            printstu(temp->item);
            sign = 1;
            //找到目标终止
            // return;
        }
        temp = temp->next;
    }
    if(sign == 0)
        printf("Can't find it!!!\n");
}

//分析学生成绩分布
void analyze()
{
    if(aMgr.itemnum == 0) {
        printf("NO DATA!!!\n");
        return;
    }
    const Stulist* temp;
    //计数
    int count[5] = { 0, 0, 0, 0, 0 };
    int i, j, k;
    for(i = 0; i != aMgr.scorenum; i++) {
        temp = aMgr.head;
        for(j = 0; j != aMgr.itemnum; j++) {
            switch((int)temp->item.score[i] / 10) {
            // 90~100为A
            case 10:
            case 9:
                count[0]++;
                break;
            // 80~90为B
            case 8:
                count[1]++;
                break;
            // 70~80为C
            case 7:
                count[2]++;
                break;
            // 60~70为D
            case 6:
                count[3]++;
                break;
            // 60以下为F
            default:
                count[4]++;
                break;
            }
            temp = temp->next;
        }
        printf("-------Course[%d]-------\n", i);
        //循环输出成绩分析结果
        for(j = 'A', k = 0; j != 'F'; j++, k++) {
            printf("%c--Num:%4d", j, count[k]);
            printf("      Percent:%4.2f%%\n", (float)count[k] / aMgr.itemnum * 100);
            //复位
            count[k] = 0;
        }
    }
}