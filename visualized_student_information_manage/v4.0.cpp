#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>
#include <string>


#define MAX_STUNUM 30             //某班学生最多不超过30人
#define MAX_COURSENUM 6           //考试科目最多不超过6门
#define MAX_NAMENUM 20            //学生姓名字符数不超过20

// 定义结构体
typedef struct {
    char name[MAX_NAMENUM];
    long id;
    float score[MAX_COURSENUM];
} Student;

Student students[MAX_STUNUM];    //定义学生结构体数组

struct button {
	int x,y;
    int width;
    int height;
    COLORREF color;
    char* pText;
}; 

//初始化按钮的属性函数
struct button* createbutton(int x, int y, int width, int height, COLORREF color, const char* pText) {
	struct button* pB = (struct button*)malloc(sizeof(struct button));
    if (pB) {                           
        pB->x = x;
        pB->y = y;
        pB->width = width;
        pB->height = height;
        pB->color = color;
        pB->pText = (char*)malloc(strlen(pText) + 1);
        if (pB->pText != NULL) {
            strcpy(pB->pText, pText);
            return pB;
        }
        else {
			free(pB);
			return NULL;
		}
    }
    else {
		return NULL;
	}
}
//绘制按钮函数
void drawbutton(struct button* pB) {
	setfillcolor(pB->color);                //设置按钮颜色
    settextstyle(20, 0, "楷体");          //设置字体大小
    setlinecolor(BLACK);        //设置边框颜色
    settextcolor(BLACK);		//设置字体颜色
    setbkmode(TRANSPARENT);                 //设置背景透明
	fillrectangle(pB->x, pB->y, pB->x + pB->width, pB->y + pB->height);  //绘制按钮    
	outtextxy(pB->x + 10, pB->y + 10, pB->pText);  //输出按钮文字
}
//判断鼠标是否在按钮上
int IsInButton(struct button* pB, MOUSEMSG m) {
    if (pB->x <= m.x && m.x <= pB->x + pB->width && pB->y <=m.y && m.y <= pB->height + pB->y) {
        pB->color = RED;  //鼠标在按钮上时按钮颜色变红
		return 1;
	}
    pB->color = YELLOW;  
	return 0;
}
//判断鼠标按下
int clickbutton(struct button* pB, MOUSEMSG m) {
    if (IsInButton(pB, m) && m.uMsg == WM_LBUTTONDOWN) {
		return 1;
	}
	return 0;
}


int REAL_STUNUM = 0;              //实际学生人数
int REAL_COURSENUM = 0;           //实际科目门数
int match_id = -1;                 //查询学生的索引号
int match_name = -1;               //查询学生的索引号

float stu_sumscore[MAX_STUNUM];     //每个学生的总分, 在input中先计算好 避免查询排名复杂
float stu_averscore[MAX_STUNUM];     //每个学生的平均分

float sum_score[MAX_COURSENUM];
float aver_score[MAX_COURSENUM];

float A_PER[MAX_COURSENUM] = { 0 }, B_PER[MAX_COURSENUM] = { 0 }, C_PER[MAX_COURSENUM] = { 0 };
float D_PER[MAX_COURSENUM] = { 0 }, E_PER[MAX_COURSENUM] = { 0 };
//声明函数
void Input_record();
void Cal_total_aver_score();
void Cal_total_aver_score_ofstudent();
void Sort_order_score(int (*compare)(float a, float b));
void Sort_order_id(int (*compare)(float a, float b));
void Sort_in_dictionary_order_by_name();
void Search_by_number();
void Search_by_name();
void Statistic_analysis();
void List_record();
void Write_to_a_file();
void Read_from_a_file();
void c1_display();
void c2_display();
void c3_display();
void c4_display();
void c5_display();
void c6_display();
void c7_display();
void c8_display();
void c9_display();
void c10_display();
void c11_display();
void c12_display();
void c13_display();

int Ascending(float a, float b) {
    return a > b;                   //对于升序 出现a>b错序 则交换
}
int Descending(float a, float b) {
    return a < b;                   //对于降序 出现a<b错序 则交换
}

void swap(float* a, float* b) {  //用指针交换数组中的元素
    float temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void swap_id(long* a, long* b) {  //用指针交换id数组中的元素
    long temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void choice(int num) {       //根据用户输入选项Switch不同函数
    switch (num) {
    case 1:
        c1_display();
        break;
    case 2:
        c2_display();
        break;
    case 3:
        c3_display();
        break;
    case 4:
        c4_display();
        break;
    case 5:
        c5_display();
        break;
    case 6:
        c6_display();
        break;
    case 7:
        c7_display();
        break;
    case 8:
        c8_display();
        break;
    case 9:
        c9_display();
        break;
    case 10:
        c10_display();
        break;
    case 11:
        c11_display();
        break;
    case 12:
        c12_display();
        break;
    case 13:
        c13_display();
        break;
    case 0:
        exit(0);
    }
}

void Input_record() {
    int i;
    printf("\nInput Record:\n");
    printf("How many students?(number<=30)\n");
    scanf("%d", &REAL_STUNUM);
    printf("How many courses?(number<=6)\n");
    scanf("%d", &REAL_COURSENUM);
    if (REAL_STUNUM > MAX_STUNUM) {
        printf("输入人数超过30! 程序中止");
        exit(1);
    }
    if (REAL_COURSENUM > MAX_COURSENUM) {
        printf("输入科目数超过6! 程序中止");
        exit(1);
    }
    for (i = 0; i < REAL_STUNUM; i++) {
        int j;
        printf("Enter id and name of NUM.%d\n", i + 1);
        scanf("%ld %s", &students[i].id, students[i].name);
        for (j = 0; j < REAL_COURSENUM; j++) {
            printf("Enter score of course NUM.%d of student NUM.%d\n", j + 1, i + 1);
            scanf("%f", &students[i].score[j]);
            stu_sumscore[i] += students[i].score[j];
        }
    }
}

// void Input_record(){
//     printf("Please enter the student's information in the following format:\n");
//     printf("ID Name Score1 Score2 ... ScoreN(每个数据中间穿插一个空格)\n");
//     int i = 0;
//     char input[256];
//     if(fgets(input, sizeof(input), stdin) != NULL){  // 如果没有读取到EOF
//         sscanf(input, "%ld %s", &students[REAL_STUNUM].id, students[REAL_STUNUM].name);  // 读取学生的ID和姓名
//         char *p = strchr(input, ' ');  // 找到第一个空格
//         if(p != NULL){  // 如果找到了第一个空格
//             p = strchr(p + 1, ' ');  // 找到第二个空格
//             if(p != NULL){  // 如果找到了第二个空格
//                 for(i = 0; i < REAL_COURSENUM; i++){  // 读取学生的每门课程的成绩
//                     p = strchr(p + 1, ' ');  // 找到下一个空格
//                     if(p == NULL){  // 如果没有找到下一个空格
//                         break;
//                     }
//                     sscanf(p, "%f", &students[REAL_STUNUM].score[i]);  // 读取成绩
//                 }
//             }
//         }
//     }

//     // 计算学生的总分
//     stu_sumscore[REAL_STUNUM] = 0;
//     for(i = 0; i < REAL_COURSENUM; i++){
//         stu_sumscore[REAL_STUNUM] += students[REAL_STUNUM].score[i];
//     }

//     REAL_STUNUM++;
// }

void Cal_total_aver_score() {
    int i, j, k;
    int flag = 1;
    printf("\nCalculate total and average score of course:\n");
    for (j = 0; j < REAL_COURSENUM; j++) {
        sum_score[j] = 0;                                //总分初始化
        for (i = 0; i < REAL_STUNUM; i++) {
            sum_score[j] += students[i].score[j];                          //每门课程的总分
        }
    }
    for (k = 0; k < REAL_COURSENUM; k++) {
        if (sum_score[k] == 0) {
            do {
                printf("似乎成绩有误, 是否继续计算? 输入1:继续, 0:结束\n");
                scanf("%d", &flag);
                if (flag == 0) {
                    exit(0);
                }
            } while (flag != 1 && flag != 0);
        }
        aver_score[k] = sum_score[k] / REAL_STUNUM;               //每门课程的平均分
    }

    printf("%-10s%-10s\n", "Total", "Average");  // 输出总分和平均分的标题，每个标题占10个字符宽度

    // 输出分隔线
    for (i = 0; i < 30; i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < REAL_COURSENUM; i++) {
        printf("%-10.2f%-10.2f\n", sum_score[i], aver_score[i]);  // 输出每门课程的总分和平均分，每个占10个字符宽度，保留2位小数

    }
}

void Cal_total_aver_score_ofstudent() {
    int i, j;
    for (i = 0; i < REAL_STUNUM; i++) {
        stu_sumscore[i] = 0;
        for (j = 0; j < REAL_COURSENUM; j++) {
            stu_sumscore[i] += students[i].score[j];  // 计算每个学生的总分
        }
        stu_averscore[i] = stu_sumscore[i] / REAL_COURSENUM;  // 计算每个学生的平均分    
    }

    printf("%-10s%-20s%-10s%-10s\n", "Id", "Name", "Total", "Average");  // 输出总分和平均分的标题，每个标题占10个字符宽度

    // 输出分隔线
    for (i = 0; i < 30; i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < REAL_STUNUM; i++) {
        printf("%-10ld%-20s", students[i].id, students[i].name);  // 输出学生的ID和姓名，每个占10个字符宽度
        printf("%-10.2f%-10.2f", stu_sumscore[i], stu_averscore[i]);  // 输出每student的总分和平均分，每个占10个字符宽度，保留2位小数
        printf("\n");
    }
}

void Sort_order_score(int (*compare)(float a, float b)) {
    printf("\nSort in descending order by score:\n");
    int i, j;
    for (i = 0; i < REAL_STUNUM - 1; i++) {
        for (j = 0; j < REAL_STUNUM - 1 - i; j++) {
            if ((*compare)(stu_sumscore[j], stu_sumscore[j + 1])) {  // 如果前一个学生的总分小于后一个学生的总分，则交换他们的位置
                long temp_id = students[j].id;
                char temp_name[20];
                strcpy(temp_name, students[j].name);
                float temp_score[MAX_COURSENUM];
                memcpy(temp_score, students[j].score, sizeof(float) * REAL_COURSENUM);
                float temp_sumscore = stu_sumscore[j];

                students[j].id = students[j + 1].id;
                strcpy(students[j].name, students[j + 1].name);
                memcpy(students[j].score, students[j + 1].score, sizeof(float) * REAL_COURSENUM);
                stu_sumscore[j] = stu_sumscore[j + 1];

                students[j + 1].id = temp_id;
                strcpy(students[j + 1].name, temp_name);
                memcpy(students[j + 1].score, temp_score, sizeof(float) * REAL_COURSENUM);
                stu_sumscore[j + 1] = temp_sumscore;
            }
        }
    }

    // 输出排序后的学生信息
    printf("\nSort and Order by Score:\n");
    printf("%-10s%-20s", "ID", "Name");  // 输出ID和姓名的标题，每个标题占10个字符宽度
    for (j = 0; j < REAL_COURSENUM; j++) {
        printf("%-10s", "Score");  // 输出每门课程的成绩的标题，每个标题占10个字符宽度
    }
    printf("%-10s\n", "Sum");  // 输出总分的标题，占10个字符宽度

    // 输出分隔线
    for (i = 0; i < 10 * (REAL_COURSENUM + 3); i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < REAL_STUNUM; i++) {
        printf("%-10ld%-20s", students[i].id, students[i].name);  // 输出学生的ID和姓名，每个占10个字符宽度
        for (j = 0; j < REAL_COURSENUM; j++) {
            printf("%-10.2f", students[i].score[j]);  // 输出学生的每门课程的成绩，每个成绩占10个字符宽度，保留2位小数
        }
        printf("%-10.2f\n", stu_sumscore[i]);  // 输出学生的总分，占10个字符宽度，保留2位小数
    }
}

void Sort_order_id(int (*compare)(float a, float b)) {
    int i, j;
    for (i = 0; i < REAL_STUNUM - 1; i++) {
        for (j = 0; j < REAL_STUNUM - 1 - i; j++) {
            if (students[j].id > students[j + 1].id) {  // 如果前一个学生的ID大于后一个学生的ID，则交换他们的位置
                long temp_id = students[j].id;
                char temp_name[20];
                strcpy(temp_name, students[j].name);
                float temp_score[MAX_COURSENUM];
                memcpy(temp_score, students[j].score, sizeof(float) * REAL_COURSENUM);
                float temp_sumscore = stu_sumscore[j];

                students[j].id = students[j + 1].id;
                strcpy(students[j].name, students[j + 1].name);
                memcpy(students[j].score, students[j + 1].score, sizeof(float) * REAL_COURSENUM);
                stu_sumscore[j] = stu_sumscore[j + 1];

                students[j + 1].id = temp_id;
                strcpy(students[j + 1].name, temp_name);
                memcpy(students[j + 1].score, temp_score, sizeof(float) * REAL_COURSENUM);
                stu_sumscore[j + 1] = temp_sumscore;
            }
        }
    }

    // 输出排序后的学生信息
    printf("\nSort and Order by ID:\n");
    printf("%-10s%-20s", "ID", "Name");  // 输出ID和姓名的标题，每个标题占10个字符宽度
    for (j = 0; j < REAL_COURSENUM; j++) {
        printf("%-10s", "Score");  // 输出每门课程的成绩的标题，每个标题占10个字符宽度
    }
    printf("%-10s\n", "Sum");  // 输出总分的标题，占10个字符宽度

    // 输出分隔线
    for (i = 0; i < 10 * (REAL_COURSENUM + 3); i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < REAL_STUNUM; i++) {
        printf("%-10ld%-20s", students[i].id, students[i].name);  // 输出学生的ID和姓名，每个占10个字符宽度
        for (j = 0; j < REAL_COURSENUM; j++) {
            printf("%-10.2f", students[i].score[j]);  // 输出学生的每门课程的成绩，每个成绩占10个字符宽度，保留2位小数
        }
        printf("%-10.2f\n", stu_sumscore[i]);  // 输出学生的总分，占10个字符宽度，保留2位小数
    }
}

void Sort_in_dictionary_order_by_name() {
    int i, j;
    for (i = 0; i < REAL_STUNUM - 1; i++) {
        for (j = 0; j < REAL_STUNUM - 1 - i; j++) {
            if (strcmp(students[j].name, students[j + 1].name) > 0) {  // 如果前一个学生的姓名大于后一个学生的姓名，则交换他们的位置
                long temp_id = students[j].id;                        //下文相当于大型swap对id、name、score、sumscore进行交换
                char temp_name[20];
                strcpy(temp_name, students[j].name);                // 交换学生的姓名
                float temp_score[MAX_COURSENUM];
                memcpy(temp_score, students[j].score, sizeof(float) * REAL_COURSENUM);  // 交换学生的成绩
                float temp_sumscore = stu_sumscore[j];

                students[j].id = students[j + 1].id;
                strcpy(students[j].name, students[j + 1].name);
                memcpy(students[j].score, students[j + 1].score, sizeof(float) * REAL_COURSENUM);
                stu_sumscore[j] = stu_sumscore[j + 1];

                students[j + 1].id = temp_id;
                strcpy(students[j + 1].name, temp_name);
                memcpy(students[j + 1].score, temp_score, sizeof(float) * REAL_COURSENUM);
                stu_sumscore[j + 1] = temp_sumscore;
            }
        }
    }
    // 输出排序后的学生信息
    printf("\nSort in dictionary order by name:\n");
    printf("%-10s%-20s", "ID", "Name");  // 输出ID和姓名的标题，每个标题占10个字符宽度
    for (j = 0; j < REAL_COURSENUM; j++) {
        printf("%-10s", "Score");  // 输出每门课程的成绩的标题，每个标题占10个字符宽度
    }
    printf("%-10s\n", "Sum");  // 输出总分的标题，占10个字符宽度

    // 输出分隔线
    for (i = 0; i < 10 * (REAL_COURSENUM + 3); i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < REAL_STUNUM; i++) {
        printf("%-10ld%-20s", students[i].id, students[i].name);  // 输出学生的ID和姓名，每个占10个字符宽度
        for (j = 0; j < REAL_COURSENUM; j++) {
            printf("%-10.2f", students[i].score[j]);  // 输出学生的每门课程的成绩，每个成绩占10个字符宽度，保留2位小数
        }
        printf("%-10.2f\n", stu_sumscore[i]);  // 输出学生的总分，占10个字符宽度，保留2位小数
    }
}

void Search_by_number() {
    long id;        //用学号查询成绩 的 索引学号
    printf("\nSearch by number:\n");
    int i, j;
    printf("Enter the id:\n");
    scanf("%d", &id);
    match_id = -1;  // 初始化匹配的学生ID的索引号
    for (i = 0; i < REAL_STUNUM; i++) {
        if (students[i].id == id) {  // 如果找到了匹配的学生ID
            match_id = i;
            printf("\nSearch Result:\n");
            printf("%-10s%-20s", "ID", "Name");  // 输出ID和姓名的标题，每个标题占10个字符宽度
            for (j = 0; j < REAL_COURSENUM; j++) {
                printf("%-10s", "Score");  // 输出每门课程的成绩的标题，每个标题占10个字符宽度
            }
            printf("%-10s\n", "Sum");  // 输出总分的标题，占10个字符宽度

            // 输出分隔线
            for (j = 0; j < 10 * (REAL_COURSENUM + 3); j++) {
                printf("-");
            }
            printf("\n");

            printf("%-10ld%-20s", students[i].id, students[i].name);  // 输出学生的ID和姓名，每个占10个字符宽度
            for (j = 0; j < REAL_COURSENUM; j++) {
                printf("%-10.2f", students[i].score[j]);  // 输出学生的每门课程的成绩，每个成绩占10个字符宽度，保留2位小数
            }
            printf("%-10.2f\n", stu_sumscore[i]);  // 输出学生的总分，占10个字符宽度，保留2位小数
            return;
        }
    }

    printf("\n----------\nNo student with the ID %ld was found.\n----------\n", id);
}

void Search_by_name() {
    char name[20];  //用姓名查询成绩 的 索引姓名
    printf("\nSearch by name:\n");
    int i, j;
    printf("Enter the name:\n");
    scanf("%s", name);
    name[strlen(name)] = '\0';  // 确保字符串以空字符结束
    match_name = -1;  // 初始化匹配的学生姓名的索引号
    for (i = 0; i < REAL_STUNUM; i++) {
        if (strcmp(students[i].name, name) == 0) {  // 如果找到了匹配的学生姓名
            match_name = i;
            printf("\nSearch Result:\n");
            printf("%-10s%-20s", "ID", "Name");  // 输出ID和姓名的标题，每个标题占10个字符宽度
            for (j = 0; j < REAL_COURSENUM; j++) {
                printf("%-10s", "Score");  // 输出每门课程的成绩的标题，每个标题占10个字符宽度
            }
            printf("%-10s\n", "Sum");  // 输出总分的标题，占10个字符宽度

            // 输出分隔线
            for (j = 0; j < 10 * (REAL_COURSENUM + 3); j++) {
                printf("-");
            }
            printf("\n");

            printf("%-10ld%-20s", students[i].id, students[i].name);  // 输出学生的ID和姓名，每个占10个字符宽度
            for (j = 0; j < REAL_COURSENUM; j++) {
                printf("%-10.2f", students[i].score[j]);  // 输出学生的每门课程的成绩，每个成绩占10个字符宽度，保留2位小数
            }
            printf("%-10.2f\n", stu_sumscore[i]);  // 输出学生的总分，占10个字符宽度，保留2位小数
            return;
        }
    }
    printf("\n----------\nNo student with the name %s was found.\n----------\n", name);
}

void Statistic_analysis() {
    printf("\nStatic analysis:\n");
    int i, j;
    int A_COUNT[MAX_COURSENUM] = { 0 }, B_COUNT[MAX_COURSENUM] = { 0 }, C_COUNT[MAX_COURSENUM] = { 0 };
    int D_COUNT[MAX_COURSENUM] = { 0 }, E_COUNT[MAX_COURSENUM] = { 0 };
    for (j = 0; j < REAL_COURSENUM; j++) {                 //遍历
        for (i = 0; i < REAL_STUNUM; i++) {
            if (students[i].score[j] <= 100 && students[i].score[j] >= 90) {
                A_COUNT[j]++;
            }
            if (students[i].score[j] < 90 && students[i].score[j] >= 80) {
                B_COUNT[j]++;
            }
            if (students[i].score[j] < 80 && students[i].score[j] >= 70) {
                C_COUNT[j]++;
            }
            if (students[i].score[j] < 70 && students[i].score[j] >= 60) {
                D_COUNT[j]++;
            }
            if (students[i].score[j] < 60) {
                E_COUNT[j]++;
            }
        }
        A_PER[j] = (float)A_COUNT[j] / REAL_STUNUM;                 //避免整型除法
        B_PER[j] = (float)B_COUNT[j] / REAL_STUNUM;
        C_PER[j] = (float)C_COUNT[j] / REAL_STUNUM;
        D_PER[j] = (float)D_COUNT[j] / REAL_STUNUM;
        E_PER[j] = (float)E_COUNT[j] / REAL_STUNUM;

        printf("course NUM.%d: 优秀人数：%d, 所占百分比%.2f%%\n", j + 1, A_COUNT[j], A_PER[j] * 100);
        printf("course NUM.%d: 良好人数：%d, 所占百分比%.2f%%\n", j + 1, B_COUNT[j], B_PER[j] * 100);
        printf("course NUM.%d: 中等人数：%d, 所占百分比%.2f%%\n", j + 1, C_COUNT[j], C_PER[j] * 100);
        printf("course NUM.%d: 及格人数：%d, 所占百分比%.2f%%\n", j + 1, D_COUNT[j], D_PER[j] * 100);
        printf("course NUM.%d: 不及格人数：%d, 所占百分比%.2f%%\n", j + 1, E_COUNT[j], E_PER[j] * 100);
    }
}

void List_record() {
    printf("\nList record:\n");
    int i, j;

    printf("%-10s%-20s", "ID", "Name");  // 输出ID和姓名的标题，每个标题占10个字符宽度
    for (j = 0; j < REAL_COURSENUM; j++) {
        printf("%-10s", "Score");  // 输出每门课程的成绩的标题，每个标题占10个字符宽度
    }
    printf("%-10s", "Sum");  // 输出总分的标题，占10个字符宽度
    printf("%-10s\n", "Aver");           //输出平均分的标题，占10个字符宽度
    // 输出分隔线
    for (i = 0; i < 10 * (REAL_COURSENUM + 3); i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < REAL_STUNUM; i++) {
        printf("%-10ld%-20s", students[i].id, students[i].name);  // 输出学生的ID和姓名，每个占10个字符宽度
        for (j = 0; j < REAL_COURSENUM; j++) {
            printf("%-10.2f", students[i].score[j]);  // 输出学生的每门课程的成绩，每个成绩占10个字符宽度，保留2位小数
        }
        printf("%-10.2f", stu_sumscore[i]);  // 输出学生的总分，占10个字符宽度，保留2位小数
        printf("%-10.2f\n", stu_averscore[i]);   // 输出学生的平均分，占10个字符宽度，保留2位小数
    }
    printf("-------------------\n");
    printf("-------------------\n");
    printf("%-10s%-10s\n", "Total", "Average");  // 输出总分和平均分的标题，每个标题占10个字符宽度

    // 输出分隔线
    for (i = 0; i < 30; i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < REAL_COURSENUM; i++) {
        printf("%-10.2f%-10.2f\n", sum_score[i], aver_score[i]);  // 输出每门课程的总分和平均分，每个占10个字符宽度，保留2位小数

    }
}
char lujin[128];
void Write_to_a_file() {
    printf("保存路径：\n");
    scanf("%s",lujin);                      //让用户指定存储路径
    FILE* file = fopen(lujin, "w");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }
    fprintf(file, "学生数量：%d", REAL_STUNUM);
    fprintf(file, "科目数量：%d\n", REAL_COURSENUM);
    for (int i = 0; i < REAL_STUNUM; i++) {
        fprintf(file, "姓名: %s\n", students[i].name);
        fprintf(file, "学号: %ld\n", students[i].id);
        fprintf(file, "分数: ");
        for (int j = 0; j < REAL_COURSENUM; j++) {
            fprintf(file, "%.2f ", students[i].score[j]);
        }
        fprintf(file, "\n总分: %.2f\n", stu_sumscore[i]);
        fprintf(file, "平均分: %.2f\n\n", stu_averscore[i]);
        fflush(file);  // 立即将数据写入磁盘
    }
    printf("写入文件成功\n");
    fclose(file);
}

void Read_from_a_file() {
    printf("读取路径：\n");
    scanf("%s", lujin);                      //让用户指定路径
    FILE* file = fopen(lujin, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    fscanf(file, "学生数量：%d", &REAL_STUNUM);
    fscanf(file, "科目数量：%d\n", &REAL_COURSENUM);
    for (int i = 0; i < REAL_STUNUM; i++) {
		fscanf(file, "姓名: %s\n", students[i].name);
		fscanf(file, "学号: %ld\n", &students[i].id);
		fscanf(file, "分数: ");
        for (int j = 0; j < REAL_COURSENUM; j++) {
			fscanf(file, "%f", &students[i].score[j]);
		}
		fscanf(file, "\n总分: %f\n", &stu_sumscore[i]);
		fscanf(file, "平均分: %f\n\n", &stu_averscore[i]);
		printf("姓名: %s\n", students[i].name);
		printf("学号: %ld\n", students[i].id);
		printf("分数: ");
        for (int j = 0; j < REAL_COURSENUM; j++) {
			printf("%.2f ", students[i].score[j]);
		}
		printf("\n总分: %.2f\n", stu_sumscore[i]);
		printf("平均分: %.2f\n\n", stu_averscore[i]);
	}

    printf("读取文件成功\n");
    fclose(file);
}

//修改学生信息*

//修改部分学生信息 前提函数
//void modi_infor() {
//    printf("以学号 还是 姓名 为凭修改目标学生信息？\n输入1:学号, 2:姓名\n");
//    unsigned int choi=0;
//    scanf("%d",&choi);
//    if (choi == 1 || choi == 2) {
//        if (choi == 1) {
//            Search_by_number();
//            printf("改第几科？\n");
//            int which_course = -1;
//            scanf("%d",&which_course);
//            if (which_course == -1) {
//                printf("wrong which_course\n");
//            }
//            else {
//                printf("修改为：\n"); 
//                float after_modi_score = -1;
//                scanf("%d", &which_course);
//                if (which_course == -1) {
//                    printf("wrong modi_score");
//                else {
//                    for (int i = 0; i < REAL_STUNUM;i++) {
//                        if(match_id == )
//                    }
//                }
//            }
//        }
//        if (choi == 2) {
//            Search_by_name();
//        }
//
//    }
//    else printf("wrong choice\n");
//} 

void menu_display() {
    initgraph(800, 600);
    IMAGE mm;
    loadimage(&mm, "background.png", 800, 600);

    struct button* c1 = createbutton(100, 200, 200, 50, YELLOW, "录入信息");
    struct button* c2 = createbutton(100, 250, 200, 50, YELLOW, "算各科总分,均分");
    struct button* c3 = createbutton(100, 300, 200, 50, YELLOW, "算各学生总分,均分");
    struct button* c4 = createbutton(100, 350, 200, 50, YELLOW, "学生总分降序");
    struct button* c5 = createbutton(100, 400, 200, 50, YELLOW, "学生总分升序");
    struct button* c6 = createbutton(100, 450, 200, 50, YELLOW, "学号升序");
    struct button* c7 = createbutton(100, 500, 200, 50, YELLOW, "首字母顺序");
    struct button* c8 = createbutton(100, 550, 200, 50, YELLOW, "以学号检索");
    struct button* c9 = createbutton(400, 200, 200, 50, YELLOW, "以姓名检索");
    struct button* c10 = createbutton(400, 250, 200, 50, YELLOW, "各分数段分布");
    struct button* c11 = createbutton(400, 300, 200, 50, YELLOW, "列出信息");
    struct button* c12 = createbutton(400, 350, 200, 50, YELLOW, "写入文件");
    struct button* c13 = createbutton(400, 400, 200, 50, YELLOW, "读取文件");
    struct button* c0 = createbutton(400, 450, 200, 50, YELLOW, "退出程序");

    int choice_num = -1;
    while (1) {
        BeginBatchDraw();       // 开始批量绘图
        putimage(0, 0, &mm);
        drawbutton(c1);
        drawbutton(c2);
        drawbutton(c3);
        drawbutton(c4);
        drawbutton(c5);
        drawbutton(c6);
        drawbutton(c7);
        drawbutton(c8);
        drawbutton(c9);
        drawbutton(c10);
        drawbutton(c11);
        drawbutton(c12);
        drawbutton(c13);
        drawbutton(c0);
        MOUSEMSG m = GetMouseMsg();
        if (clickbutton(c1, m)) {
            choice_num = 1;
            break;
        }
        if (clickbutton(c2, m)) {
            choice_num = 2;
            break;
        }
        if (clickbutton(c3, m)) {
            choice_num = 3;
            break;
        }
        if (clickbutton(c4, m)) {
            choice_num = 4;
            break;
        }
        if (clickbutton(c5, m)) {
            choice_num = 5;
            break;
        }
        if (clickbutton(c6, m)) {
            choice_num = 6;
            break;
        }
        if (clickbutton(c7, m)) {
            choice_num = 7;
            break;
        }
        if (clickbutton(c8, m)) {
            choice_num = 8;
            break;
        }
        if (clickbutton(c9, m)) {
            choice_num = 9;
            break;
        }
        if (clickbutton(c10, m)) {
            choice_num = 10;
            break;
        }
        if (clickbutton(c11, m)) {
            choice_num = 11;
            break;
        }
        if (clickbutton(c12, m)) {
            choice_num = 12;
            break;
        }
        if (clickbutton(c13, m)) {
            choice_num = 13;
            break;
        }
        if (clickbutton(c0, m)) {
            choice_num = 0;
            break;
        }

        EndBatchDraw();           // 结束批量绘图           
    }
    closegraph();
    choice(choice_num);
}

void c1_display() {
    Input_record();
    initgraph(1240, 960);
    IMAGE mm;
    loadimage(&mm, "background.png", 1240, 960);
    struct button* back_to_menu = createbutton(0,0,200,50,YELLOW,"返回菜单");
    while (1) {
        BeginBatchDraw();
        putimage(0, 0, &mm);
        drawbutton(back_to_menu);
        MOUSEMSG m = GetMouseMsg();
        outtextxy(0, 100, "录入成功");
        outtextxy(0,150,("学生人数："+ std::to_string(REAL_STUNUM)).c_str());
        outtextxy(0, 200, ("科目数：" + std::to_string(REAL_COURSENUM)).c_str());
        if (clickbutton(back_to_menu, m)) {
            
            break;
        }
        EndBatchDraw();
    }

    closegraph();
    menu_display();
}
void c2_display() {
	Cal_total_aver_score();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040,910,200,50,YELLOW,"返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
		outtextxy(0, 910, "计算成功");
		outtextxy(0, 0, "各科总分均分");
        for (int i = 0; i < REAL_COURSENUM; i++) {
			char course[20];
			sprintf(course, "Course %d", i + 1);
			outtextxy(10,20 + i * 20, course);
		}
		outtextxy(200, 0, "Total");
		outtextxy(300, 0, "Average");
        for (int i = 0; i < REAL_COURSENUM; i++) {
			char total[20];
			sprintf(total, "%.2f", sum_score[i]);
			outtextxy(200 , 20 + i * 20, total);
			char average[20];
			sprintf(average, "%.2f", aver_score[i]);
			outtextxy(300 , 20 + i * 20, average);
		}
        if (clickbutton(back_to_menu, m)) {
			break;
		}
		EndBatchDraw();
	}

	closegraph();
	menu_display();
        
}
void c3_display() {
    Cal_total_aver_score_ofstudent();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040,910,200,50,YELLOW,"返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();

		outtextxy(0, 910, "计算成功");
        outtextxy(0, 0, "各学生总分均分");
        settextstyle(20, 0, "楷体");
        setbkmode(TRANSPARENT);

        outtextxy(10, 30, "ID");
        outtextxy(100, 30, "Name");
        for (int i = 0; i < REAL_COURSENUM; ++i) {
            char course[20];
            sprintf(course, "Course %d", i + 1);
            outtextxy(200 + i * 100, 30, course);
        }
        outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
        outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");

        for (int i = 0; i < REAL_STUNUM; ++i) {
            char id[20];
            sprintf(id, "%ld", students[i].id);
            outtextxy(10, 50 + i * 20, id);

            outtextxy(100, 50 + i * 20, students[i].name);

            float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
                total += students[i].score[j];
                char score[20];
                sprintf(score, "%.2f", students[i].score[j]);
                outtextxy(200 + j * 100, 50 + i * 20, score);
            }

            char totalStr[20];
            sprintf(totalStr, "%.2f", total);
            outtextxy(200 + REAL_COURSENUM * 100, 50 + i * 20, totalStr);

            char averageStr[20];
            sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
            outtextxy(300 + REAL_COURSENUM * 100, 50 + i * 20, averageStr);
        }

        if (clickbutton(back_to_menu, m)) {
			break;
		}
		EndBatchDraw();
	}

	closegraph();
	menu_display();
}
void c4_display() {
	Sort_order_score(Descending);
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040,910,200,50,YELLOW,"返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
		outtextxy(0, 910, "排序成功");
		outtextxy(0, 0, "学生总分降序");
		outtextxy(10, 30, "ID");
		outtextxy(100, 30, "Name");
        for (int i = 0; i < REAL_COURSENUM; ++i) {
			char course[20];
			sprintf(course, "Course %d", i + 1);
			outtextxy(200 + i * 100, 30, course);
		}
		outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
		outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");

        for (int i = 0; i < REAL_STUNUM; ++i) {
			char id[20];
			sprintf(id, "%ld", students[i].id);
			outtextxy(10, 50 + i * 20, id);

			outtextxy(100, 50 + i * 20, students[i].name);

			float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
				total += students[i].score[j];
				char score[20];
				sprintf(score, "%.2f", students[i].score[j]);
				outtextxy(200 + j * 100, 50 + i * 20, score);
			}

			char totalStr[20];
			sprintf(totalStr, "%.2f", total);
			outtextxy(200 + REAL_COURSENUM * 100, 50 + i * 20, totalStr);

			char averageStr[20];
			sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
			outtextxy(300 + REAL_COURSENUM * 100, 50 + i * 20, averageStr);
        }
        if (clickbutton(back_to_menu, m)) {
            break;
        }
        EndBatchDraw();

    }
    closegraph();
    menu_display();
}
void c5_display() {
    Sort_order_score(Ascending);
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040,910,200,50,YELLOW,"返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
		outtextxy(0, 910, "排序成功");
		outtextxy(0, 0, "学生总分升序");
		outtextxy(10, 30, "ID");
		outtextxy(100, 30, "Name");
        for (int i = 0; i < REAL_COURSENUM; ++i) {
			char course[20];
			sprintf(course, "Course %d", i + 1);
			outtextxy(200 + i * 100, 30, course);
		}
		outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
		outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");

        for (int i = 0; i < REAL_STUNUM; ++i) {
			char id[20];
			sprintf(id, "%ld", students[i].id);
			outtextxy(10, 50 + i * 20, id);

			outtextxy(100, 50 + i * 20, students[i].name);

			float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
				total += students[i].score[j];
				char score[20];
				sprintf(score, "%.2f", students[i].score[j]);
				outtextxy(200 + j * 100, 50 + i * 20, score);
			}

			char totalStr[20];
			sprintf(totalStr, "%.2f", total);
			outtextxy(200 + REAL_COURSENUM * 100, 50 + i * 20, totalStr);

			char averageStr[20];
			sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
			outtextxy(300 + REAL_COURSENUM * 100, 50 + i * 20, averageStr);
        }
        if (clickbutton(back_to_menu, m)) {
			break;
		}
        EndBatchDraw();
    }
    closegraph();
    menu_display();
}
void c6_display() {
    Sort_order_id(Ascending);
    initgraph(1240, 960);
    IMAGE mm;
    loadimage(&mm, "background.png", 1240, 960);
    struct button* back_to_menu = createbutton(1040, 910, 200, 50, YELLOW, "返回菜单");
    while (1) {
        BeginBatchDraw();
        putimage(0, 0, &mm);
        drawbutton(back_to_menu);
        MOUSEMSG m = GetMouseMsg();
        outtextxy(0, 910, "排序成功");
        outtextxy(0, 0, "学号升序");
        outtextxy(10, 30, "ID");
        outtextxy(100, 30, "Name");
        for (int i = 0; i < REAL_COURSENUM; ++i) {
            char course[20];
            sprintf(course, "Course %d", i + 1);
            outtextxy(200 + i * 100, 30, course);
        }
        outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
        outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");

        for (int i = 0; i < REAL_STUNUM; ++i) {
            char id[20];
            sprintf(id, "%ld", students[i].id);
            outtextxy(10, 50 + i * 20, id);

            outtextxy(100, 50 + i * 20, students[i].name);

            float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
                total += students[i].score[j];
                char score[20];
                sprintf(score, "%.2f", students[i].score[j]);
                outtextxy(200 + j * 100, 50 + i * 20, score);
            }

            char totalStr[20];
            sprintf(totalStr, "%.2f", total);
            outtextxy(200 + REAL_COURSENUM * 100, 50 + i * 20, totalStr);

            char averageStr[20];
            sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
            outtextxy(300 + REAL_COURSENUM * 100, 50 + i * 20, averageStr);
        }
        if (clickbutton(back_to_menu, m)) {
            break;
        }
        EndBatchDraw();
    }
    closegraph();
    menu_display();
}
void c7_display() {
    Sort_in_dictionary_order_by_name();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040, 910, 200, 50, YELLOW, "返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
		outtextxy(0, 910, "排序成功");
		outtextxy(0, 0, "首字母顺序");
		outtextxy(10, 30, "ID");
		outtextxy(100, 30, "Name");
        for (int i = 0; i < REAL_COURSENUM; ++i) {
			char course[20];
			sprintf(course, "Course %d", i + 1);
			outtextxy(200 + i * 100, 30, course);
		}
		outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
		outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");

        for (int i = 0; i < REAL_STUNUM; ++i) {
			char id[20];
			sprintf(id, "%ld", students[i].id);
			outtextxy(10, 50 + i * 20, id);

			outtextxy(100, 50 + i * 20, students[i].name);

			float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
				total += students[i].score[j];
				char score[20];
				sprintf(score, "%.2f", students[i].score[j]);
				outtextxy(200 + j * 100, 50 + i * 20, score);
			}

			char totalStr[20];
			sprintf(totalStr, "%.2f", total);
			outtextxy(200 + REAL_COURSENUM * 100, 50 + i * 20, totalStr);

			char averageStr[20];
			sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
			outtextxy(300 + REAL_COURSENUM * 100, 50 + i * 20, averageStr);
        }
        if (clickbutton(back_to_menu, m)) {
            break;
        }
        EndBatchDraw();
    }
    closegraph();
    menu_display();
}
void c8_display() {
    Search_by_number();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040, 910, 200, 50, YELLOW, "返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
        if (match_id == -1) {
            outtextxy(0, 910, "查询失败");
            outtextxy(0, 0, "以学号检索");
            
            if (clickbutton(back_to_menu, m)) {
				break;
			}
            EndBatchDraw();
        }
        else {
            outtextxy(0, 910, "查询成功");
            outtextxy(0, 0, "以学号检索");
            outtextxy(10, 30, "ID");
            outtextxy(100, 30, "Name");
            for (int i = 0; i < REAL_COURSENUM; ++i) {
                char course[20];
                sprintf(course, "Course %d", i + 1);
                outtextxy(200 + i * 100, 30, course);
            }
            outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
            outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");


            char id[20];
            sprintf(id, "%ld", students[match_id].id);
            outtextxy(10, 50 + match_id * 20, id);

            outtextxy(100, 50 + match_id * 20, students[match_id].name);

            float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
                total += students[match_id].score[j];
                char score[20];
                sprintf(score, "%.2f", students[match_id].score[j]);
                outtextxy(200 + j * 100, 50 + match_id * 20, score);
            }

            char totalStr[20];
            sprintf(totalStr, "%.2f", total);
            outtextxy(200 + REAL_COURSENUM * 100, 50 + match_id * 20, totalStr);

            char averageStr[20];
            sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
            outtextxy(300 + REAL_COURSENUM * 100, 50 + match_id * 20, averageStr);

            if (clickbutton(back_to_menu, m)) {
                break;
            }
            EndBatchDraw();
        }
		
    }

    closegraph();
    menu_display();
}
void c9_display() {
    Search_by_name();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040, 910, 200, 50, YELLOW, "返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
        if (match_name == -1) {
            outtextxy(0, 910, "查询失败");
            outtextxy(0, 0, "以姓名检索");
            if (clickbutton(back_to_menu, m)) {
				break;
			}

            EndBatchDraw();

        }
        else {
            outtextxy(0, 910, "查询成功");
            outtextxy(0, 0, "以姓名检索");
            outtextxy(10, 30, "ID");
            outtextxy(100, 30, "Name");
            for (int i = 0; i < REAL_COURSENUM; ++i) {
                char course[20];
                sprintf(course, "Course %d", i + 1);
                outtextxy(200 + i * 100, 30, course);
            }
            outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
            outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");


            char id[20];
            sprintf(id, "%ld", students[match_name].id);
            outtextxy(10, 50 + match_name * 20, id);

            outtextxy(100, 50 + match_name * 20, students[match_name].name);

            float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
                total += students[match_name].score[j];
                char score[20];
                sprintf(score, "%.2f", students[match_name].score[j]);
                outtextxy(200 + j * 100, 50 + match_name * 20, score);
            }

            char totalStr[20];
            sprintf(totalStr, "%.2f", total);
            outtextxy(200 + REAL_COURSENUM * 100, 50 + match_name * 20, totalStr);

            char averageStr[20];
            sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
            outtextxy(300 + REAL_COURSENUM * 100, 50 + match_name * 20, averageStr);

            if (clickbutton(back_to_menu, m)) {
                break;
            }
            EndBatchDraw();
        }
		
    }
    closegraph();
    menu_display();
}
void c10_display() {
    Statistic_analysis();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040, 910, 200, 50, YELLOW, "返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
		outtextxy(0, 910, "统计成功");
		outtextxy(0, 0, "各分数段分布");
		outtextxy(10, 30, "Course");
		outtextxy(200, 30, "A");
		outtextxy(300, 30, "B");
		outtextxy(400, 30, "C");
		outtextxy(500, 30, "D");
		outtextxy(600, 30, "E");

        for (int i = 0; i < REAL_COURSENUM; ++i) {
			char course[20];
			sprintf(course, "Course %d", i + 1);
			outtextxy(10, 50 + i * 20, course);

			char A[20];
			sprintf(A, "%.2f%%", A_PER[i] * 100);
			outtextxy(200, 50 + i * 20, A);

			char B[20];
			sprintf(B, "%.2f%%", B_PER[i] * 100);
			outtextxy(300, 50 + i * 20, B);

			char C[20];
			sprintf(C, "%.2f%%", C_PER[i] * 100);
			outtextxy(400, 50 + i * 20, C);

			char D[20];
			sprintf(D, "%.2f%%", D_PER[i] * 100);
			outtextxy(500, 50 + i * 20, D);

			char E[20];
			sprintf(E, "%.2f%%", E_PER[i] * 100);
			outtextxy(600, 50 + i * 20, E);
		}
        if (clickbutton(back_to_menu, m)) {
			break;
		}
		EndBatchDraw();
    }
    closegraph();
    menu_display();
}
void c11_display() {
    List_record();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040, 910, 200, 50, YELLOW, "返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
		outtextxy(0, 910, "列出成功");
		outtextxy(0, 0, "列出信息");
		outtextxy(10, 30, "ID");
		outtextxy(100, 30, "Name");
        for (int i = 0; i < REAL_COURSENUM; ++i) {
			char course[20];
			sprintf(course, "Course %d", i + 1);
			outtextxy(200 + i * 100, 30, course);
		}
		outtextxy(200 + REAL_COURSENUM * 100, 30, "Total");
		outtextxy(300 + REAL_COURSENUM * 100, 30, "Average");

        for (int i = 0; i < REAL_STUNUM; ++i) {
			char id[20];
			sprintf(id, "%ld", students[i].id);
			outtextxy(10, 50 + i * 20, id);

			outtextxy(100, 50 + i * 20, students[i].name);

			float total = 0;
            for (int j = 0; j < REAL_COURSENUM; ++j) {
				total += students[i].score[j];
				char score[20];
				sprintf(score, "%.2f", students[i].score[j]);
				outtextxy(200 + j * 100, 50 + i * 20, score);
			}

			char totalStr[20];
			sprintf(totalStr, "%.2f", total);
			outtextxy(200 + REAL_COURSENUM * 100, 50 + i * 20, totalStr);

			char averageStr[20];
			sprintf(averageStr, "%.2f", total / REAL_COURSENUM);
			outtextxy(300 + REAL_COURSENUM * 100, 50 + i * 20, averageStr);

        }
        // 各科总分均分
        outtextxy(0, 50 + REAL_STUNUM * 20, "各科总分均分");
        for (int i = 0; i < REAL_COURSENUM; i++) {
			char course[20];
			sprintf(course, "Course %d", i + 1);
			outtextxy(10, 70 + REAL_STUNUM * 20 + i * 20, course);
		}
        outtextxy(200, 50 + REAL_STUNUM * 20, "Total");
        outtextxy(300, 50 + REAL_STUNUM * 20, "Average");
        for (int i = 0; i < REAL_COURSENUM; i++) {
            char total[20];
            sprintf(total, "%.2f", sum_score[i]);
            outtextxy(200, 70 + REAL_STUNUM * 20 + i * 20, total);
            char average[20];
            sprintf(average, "%.2f", aver_score[i]);
            outtextxy(300, 70 + REAL_STUNUM * 20 + i * 20, average);

        }

        if (clickbutton(back_to_menu, m)) {
            break;
        }
        EndBatchDraw();
    }
    closegraph();
    menu_display();
}
void c12_display() {
    Write_to_a_file();
	initgraph(1240, 960);
	IMAGE mm;
	loadimage(&mm, "background.png", 1240, 960);
	struct button* back_to_menu = createbutton(1040, 910, 200, 50, YELLOW, "返回菜单");
    while (1) {
		BeginBatchDraw();
		putimage(0, 0, &mm);
		drawbutton(back_to_menu);
		MOUSEMSG m = GetMouseMsg();
		outtextxy(0, 910, "写入成功");
        if (clickbutton(back_to_menu, m)) {
			break;
		}
		EndBatchDraw();
	}
	closegraph();
	menu_display();

}
void c13_display() {
    Read_from_a_file();
    c11_display();

}
int main() {
    menu_display();
    return 0;
}