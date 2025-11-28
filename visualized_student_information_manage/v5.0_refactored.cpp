/**
 * 学生信息管理系统 - 重构版本
 * 主要改进:
 * 1. 消除代码重复，提取公共函数
 * 2. 使用结构体封装相关数据
 * 3. 消除全局变量，使用参数传递
 * 4. 修复内存泄漏
 * 5. 使用有意义的函数命名
 * 6. 使用循环替代递归避免栈溢出
 * 7. 添加输入验证和错误处理
 */

#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>
#include <string>

// ==================== 常量定义 ====================
namespace Config {
    constexpr int MAX_STUDENTS = 30;
    constexpr int MAX_COURSES = 6;
    constexpr int MAX_NAME_LEN = 20;
    constexpr int MAX_PATH_LEN = 128;
    
    // GUI 常量
    constexpr int MENU_WIDTH = 800;
    constexpr int MENU_HEIGHT = 600;
    constexpr int DISPLAY_WIDTH = 1240;
    constexpr int DISPLAY_HEIGHT = 960;
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    constexpr int COLUMN_WIDTH = 100;
    constexpr int ROW_HEIGHT = 20;
    
    // 分数等级边界
    constexpr float GRADE_A_MIN = 90.0f;
    constexpr float GRADE_B_MIN = 80.0f;
    constexpr float GRADE_C_MIN = 70.0f;
    constexpr float GRADE_D_MIN = 60.0f;
}

// ==================== 数据结构定义 ====================

// 学生信息结构体 - 将总分和平均分整合进来
struct Student {
    char name[Config::MAX_NAME_LEN];
    long id;
    float scores[Config::MAX_COURSES];
    float totalScore;   // 移入结构体
    float avgScore;     // 移入结构体
    
    void calculateScores(int courseCount) {
        totalScore = 0;
        for (int i = 0; i < courseCount; i++) {
            totalScore += scores[i];
        }
        avgScore = courseCount > 0 ? totalScore / courseCount : 0;
    }
};

// 课程统计结构体
struct CourseStats {
    float totalScore;
    float avgScore;
    int gradeCount[5];   // A, B, C, D, E 等级人数
    float gradePercent[5];
};

// 学生管理器 - 封装所有学生数据和操作
class StudentManager {
public:
    Student students[Config::MAX_STUDENTS];
    CourseStats courseStats[Config::MAX_COURSES];
    int studentCount;
    int courseCount;
    
    StudentManager() : studentCount(0), courseCount(0) {
        memset(students, 0, sizeof(students));
        memset(courseStats, 0, sizeof(courseStats));
    }
    
    // 计算所有学生的总分和平均分
    void calculateStudentScores() {
        for (int i = 0; i < studentCount; i++) {
            students[i].calculateScores(courseCount);
        }
    }
    
    // 计算各科目统计信息
    void calculateCourseStats() {
        for (int j = 0; j < courseCount; j++) {
            courseStats[j].totalScore = 0;
            memset(courseStats[j].gradeCount, 0, sizeof(courseStats[j].gradeCount));
            
            for (int i = 0; i < studentCount; i++) {
                float score = students[i].scores[j];
                courseStats[j].totalScore += score;
                
                // 统计各等级人数
                if (score >= Config::GRADE_A_MIN) courseStats[j].gradeCount[0]++;
                else if (score >= Config::GRADE_B_MIN) courseStats[j].gradeCount[1]++;
                else if (score >= Config::GRADE_C_MIN) courseStats[j].gradeCount[2]++;
                else if (score >= Config::GRADE_D_MIN) courseStats[j].gradeCount[3]++;
                else courseStats[j].gradeCount[4]++;
            }
            
            courseStats[j].avgScore = studentCount > 0 ? 
                courseStats[j].totalScore / studentCount : 0;
            
            // 计算百分比
            for (int k = 0; k < 5; k++) {
                courseStats[j].gradePercent[k] = studentCount > 0 ? 
                    (float)courseStats[j].gradeCount[k] / studentCount : 0;
            }
        }
    }
    
    // 通用交换函数 - 消除重复代码
    void swapStudents(int i, int j) {
        Student temp = students[i];
        students[i] = students[j];
        students[j] = temp;
    }
    
    // 按总分排序 (使用函数指针实现升序/降序)
    void sortByTotalScore(bool ascending) {
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - 1 - i; j++) {
                bool shouldSwap = ascending ? 
                    (students[j].totalScore > students[j + 1].totalScore) :
                    (students[j].totalScore < students[j + 1].totalScore);
                if (shouldSwap) {
                    swapStudents(j, j + 1);
                }
            }
        }
    }
    
    // 按学号排序
    void sortById() {
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - 1 - i; j++) {
                if (students[j].id > students[j + 1].id) {
                    swapStudents(j, j + 1);
                }
            }
        }
    }
    
    // 按姓名字典序排序
    void sortByName() {
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - 1 - i; j++) {
                if (strcmp(students[j].name, students[j + 1].name) > 0) {
                    swapStudents(j, j + 1);
                }
            }
        }
    }
    
    // 按学号查找，返回索引，-1表示未找到
    int findById(long id) const {
        for (int i = 0; i < studentCount; i++) {
            if (students[i].id == id) return i;
        }
        return -1;
    }
    
    // 按姓名查找
    int findByName(const char* name) const {
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].name, name) == 0) return i;
        }
        return -1;
    }
    
    // 写入文件
    bool saveToFile(const char* filepath) const {
        FILE* file = fopen(filepath, "w");
        if (!file) return false;
        
        fprintf(file, "学生数量：%d\n", studentCount);
        fprintf(file, "科目数量：%d\n", courseCount);
        
        for (int i = 0; i < studentCount; i++) {
            fprintf(file, "姓名: %s\n", students[i].name);
            fprintf(file, "学号: %ld\n", students[i].id);
            fprintf(file, "分数: ");
            for (int j = 0; j < courseCount; j++) {
                fprintf(file, "%.2f ", students[i].scores[j]);
            }
            fprintf(file, "\n总分: %.2f\n", students[i].totalScore);
            fprintf(file, "平均分: %.2f\n\n", students[i].avgScore);
        }
        
        fclose(file);
        return true;
    }
    
    // 从文件读取
    bool loadFromFile(const char* filepath) {
        FILE* file = fopen(filepath, "r");
        if (!file) return false;
        
        if (fscanf(file, "学生数量：%d\n", &studentCount) != 1) {
            fclose(file);
            return false;
        }
        if (fscanf(file, "科目数量：%d\n", &courseCount) != 1) {
            fclose(file);
            return false;
        }
        
        for (int i = 0; i < studentCount; i++) {
            fscanf(file, "姓名: %s\n", students[i].name);
            fscanf(file, "学号: %ld\n", &students[i].id);
            fscanf(file, "分数: ");
            for (int j = 0; j < courseCount; j++) {
                fscanf(file, "%f", &students[i].scores[j]);
            }
            fscanf(file, "\n总分: %f\n", &students[i].totalScore);
            fscanf(file, "平均分: %f\n\n", &students[i].avgScore);
        }
        
        fclose(file);
        return true;
    }
};

// ==================== GUI 组件 ====================

// 按钮结构体
struct Button {
    int x, y;
    int width, height;
    COLORREF normalColor;
    COLORREF hoverColor;
    char text[64];
    bool isHovered;
    
    void init(int px, int py, int w, int h, COLORREF color, const char* pText) {
        x = px;
        y = py;
        width = w;
        height = h;
        normalColor = color;
        hoverColor = RED;
        isHovered = false;
        strncpy(text, pText, sizeof(text) - 1);
        text[sizeof(text) - 1] = '\0';
    }
    
    void draw() const {
        setfillcolor(isHovered ? hoverColor : normalColor);
        settextstyle(20, 0, "楷体");
        setlinecolor(BLACK);
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        fillrectangle(x, y, x + width, y + height);
        outtextxy(x + 10, y + 10, text);
    }
    
    bool contains(int mx, int my) const {
        return mx >= x && mx <= x + width && my >= y && my <= y + height;
    }
    
    void updateHover(int mx, int my) {
        isHovered = contains(mx, my);
    }
    
    bool isClicked(const MOUSEMSG& m) const {
        return contains(m.x, m.y) && m.uMsg == WM_LBUTTONDOWN;
    }
};

// 按钮管理器 - 统一管理菜单按钮
class ButtonManager {
public:
    static constexpr int MAX_BUTTONS = 15;
    Button buttons[MAX_BUTTONS];
    int buttonCount;
    
    ButtonManager() : buttonCount(0) {}
    
    int addButton(int x, int y, int w, int h, COLORREF color, const char* text) {
        if (buttonCount >= MAX_BUTTONS) return -1;
        buttons[buttonCount].init(x, y, w, h, color, text);
        return buttonCount++;
    }
    
    void drawAll() const {
        for (int i = 0; i < buttonCount; i++) {
            buttons[i].draw();
        }
    }
    
    void updateAllHover(int mx, int my) {
        for (int i = 0; i < buttonCount; i++) {
            buttons[i].updateHover(mx, my);
        }
    }
    
    int getClickedButton(const MOUSEMSG& m) const {
        for (int i = 0; i < buttonCount; i++) {
            if (buttons[i].isClicked(m)) return i;
        }
        return -1;
    }
    
    void clear() {
        buttonCount = 0;
    }
};

// ==================== GUI 渲染器 ====================

class GUIRenderer {
public:
    // 绘制表头 - 消除重复代码
    static void drawTableHeader(int startY, int courseCount, bool showAverage = true) {
        settextstyle(20, 0, "楷体");
        setbkmode(TRANSPARENT);
        settextcolor(BLACK);
        
        outtextxy(10, startY, "ID");
        outtextxy(100, startY, "Name");
        
        for (int i = 0; i < courseCount; i++) {
            char course[20];
            sprintf(course, "Course %d", i + 1);
            outtextxy(200 + i * Config::COLUMN_WIDTH, startY, course);
        }
        
        outtextxy(200 + courseCount * Config::COLUMN_WIDTH, startY, "Total");
        if (showAverage) {
            outtextxy(300 + courseCount * Config::COLUMN_WIDTH, startY, "Average");
        }
    }
    
    // 绘制学生行 - 消除重复代码
    static void drawStudentRow(const Student& student, int rowIndex, int startY, int courseCount) {
        char buffer[32];
        int y = startY + rowIndex * Config::ROW_HEIGHT;
        
        sprintf(buffer, "%ld", student.id);
        outtextxy(10, y, buffer);
        outtextxy(100, y, student.name);
        
        for (int j = 0; j < courseCount; j++) {
            sprintf(buffer, "%.2f", student.scores[j]);
            outtextxy(200 + j * Config::COLUMN_WIDTH, y, buffer);
        }
        
        sprintf(buffer, "%.2f", student.totalScore);
        outtextxy(200 + courseCount * Config::COLUMN_WIDTH, y, buffer);
        
        sprintf(buffer, "%.2f", student.avgScore);
        outtextxy(300 + courseCount * Config::COLUMN_WIDTH, y, buffer);
    }
    
    // 绘制所有学生列表
    static void drawStudentList(const StudentManager& mgr, int startY) {
        drawTableHeader(startY, mgr.courseCount);
        for (int i = 0; i < mgr.studentCount; i++) {
            drawStudentRow(mgr.students[i], i, startY + Config::ROW_HEIGHT, mgr.courseCount);
        }
    }
    
    // 绘制课程统计
    static void drawCourseStats(const StudentManager& mgr, int startY) {
        outtextxy(10, startY, "各科总分均分");
        outtextxy(200, startY, "Total");
        outtextxy(300, startY, "Average");
        
        for (int i = 0; i < mgr.courseCount; i++) {
            char buffer[32];
            int y = startY + Config::ROW_HEIGHT + i * Config::ROW_HEIGHT;
            
            sprintf(buffer, "Course %d", i + 1);
            outtextxy(10, y, buffer);
            
            sprintf(buffer, "%.2f", mgr.courseStats[i].totalScore);
            outtextxy(200, y, buffer);
            
            sprintf(buffer, "%.2f", mgr.courseStats[i].avgScore);
            outtextxy(300, y, buffer);
        }
    }
    
    // 绘制成绩分布统计
    static void drawGradeDistribution(const StudentManager& mgr, int startY) {
        const char* gradeLabels[] = {"A(90-100)", "B(80-89)", "C(70-79)", "D(60-69)", "E(<60)"};
        
        outtextxy(10, startY, "Course");
        for (int g = 0; g < 5; g++) {
            outtextxy(150 + g * 120, startY, gradeLabels[g]);
        }
        
        for (int i = 0; i < mgr.courseCount; i++) {
            char buffer[32];
            int y = startY + Config::ROW_HEIGHT + i * Config::ROW_HEIGHT;
            
            sprintf(buffer, "Course %d", i + 1);
            outtextxy(10, y, buffer);
            
            for (int g = 0; g < 5; g++) {
                sprintf(buffer, "%.1f%%", mgr.courseStats[i].gradePercent[g] * 100);
                outtextxy(150 + g * 120, y, buffer);
            }
        }
    }
};

// ==================== 控制台输入输出 ====================

class ConsoleIO {
public:
    static bool inputStudentData(StudentManager& mgr) {
        printf("\n=== 录入学生信息 ===\n");
        printf("请输入学生人数 (1-%d): ", Config::MAX_STUDENTS);
        if (scanf("%d", &mgr.studentCount) != 1 || 
            mgr.studentCount <= 0 || mgr.studentCount > Config::MAX_STUDENTS) {
            printf("输入无效!\n");
            mgr.studentCount = 0;
            return false;
        }
        
        printf("请输入科目数量 (1-%d): ", Config::MAX_COURSES);
        if (scanf("%d", &mgr.courseCount) != 1 || 
            mgr.courseCount <= 0 || mgr.courseCount > Config::MAX_COURSES) {
            printf("输入无效!\n");
            mgr.courseCount = 0;
            return false;
        }
        
        for (int i = 0; i < mgr.studentCount; i++) {
            printf("\n--- 学生 %d ---\n", i + 1);
            printf("请输入学号和姓名: ");
            if (scanf("%ld %s", &mgr.students[i].id, mgr.students[i].name) != 2) {
                printf("输入无效!\n");
                return false;
            }
            
            printf("请输入 %d 门课程成绩: ", mgr.courseCount);
            for (int j = 0; j < mgr.courseCount; j++) {
                if (scanf("%f", &mgr.students[i].scores[j]) != 1) {
                    printf("输入无效!\n");
                    return false;
                }
            }
        }
        
        mgr.calculateStudentScores();
        mgr.calculateCourseStats();
        printf("\n录入成功!\n");
        return true;
    }
    
    static void printStudentList(const StudentManager& mgr) {
        printf("\n%-10s%-20s", "ID", "Name");
        for (int j = 0; j < mgr.courseCount; j++) {
            printf("%-10s", "Score");
        }
        printf("%-10s%-10s\n", "Total", "Average");
        
        for (int i = 0; i < 10 * (mgr.courseCount + 4); i++) printf("-");
        printf("\n");
        
        for (int i = 0; i < mgr.studentCount; i++) {
            printf("%-10ld%-20s", mgr.students[i].id, mgr.students[i].name);
            for (int j = 0; j < mgr.courseCount; j++) {
                printf("%-10.2f", mgr.students[i].scores[j]);
            }
            printf("%-10.2f%-10.2f\n", mgr.students[i].totalScore, mgr.students[i].avgScore);
        }
    }
    
    static void printCourseStats(const StudentManager& mgr) {
        printf("\n%-15s%-10s%-10s\n", "Course", "Total", "Average");
        for (int i = 0; i < 35; i++) printf("-");
        printf("\n");
        
        for (int i = 0; i < mgr.courseCount; i++) {
            printf("Course %-8d%-10.2f%-10.2f\n", 
                   i + 1, mgr.courseStats[i].totalScore, mgr.courseStats[i].avgScore);
        }
    }
    
    static void printGradeDistribution(const StudentManager& mgr) {
        printf("\n=== 成绩分布统计 ===\n");
        const char* gradeNames[] = {"优秀(A)", "良好(B)", "中等(C)", "及格(D)", "不及格(E)"};
        
        for (int j = 0; j < mgr.courseCount; j++) {
            printf("\n--- Course %d ---\n", j + 1);
            for (int g = 0; g < 5; g++) {
                printf("%s: %d人 (%.1f%%)\n", 
                       gradeNames[g], 
                       mgr.courseStats[j].gradeCount[g],
                       mgr.courseStats[j].gradePercent[g] * 100);
            }
        }
    }
    
    static long inputIdForSearch() {
        long id;
        printf("请输入要查询的学号: ");
        if (scanf("%ld", &id) != 1) {
            return -1;
        }
        return id;
    }
    
    static void inputNameForSearch(char* name, int maxLen) {
        printf("请输入要查询的姓名: ");
        scanf("%s", name);
    }
    
    static void inputFilePath(char* path, int maxLen, const char* prompt) {
        printf("%s", prompt);
        scanf("%s", path);
    }
};

// ==================== 菜单选项枚举 ====================
enum MenuOption {
    MENU_EXIT = 0,
    MENU_INPUT,
    MENU_CALC_COURSE_STATS,
    MENU_CALC_STUDENT_STATS,
    MENU_SORT_SCORE_DESC,
    MENU_SORT_SCORE_ASC,
    MENU_SORT_ID,
    MENU_SORT_NAME,
    MENU_SEARCH_ID,
    MENU_SEARCH_NAME,
    MENU_GRADE_DISTRIBUTION,
    MENU_LIST_ALL,
    MENU_SAVE_FILE,
    MENU_LOAD_FILE,
    MENU_COUNT
};

// ==================== 应用程序主类 ====================

class StudentManagementApp {
private:
    StudentManager studentMgr;
    ButtonManager buttonMgr;
    IMAGE backgroundImg;
    bool isRunning;
    int lastSearchResult;
    
    void initMenuButtons() {
        buttonMgr.clear();
        
        // 左列按钮
        buttonMgr.addButton(100, 200, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "录入信息");
        buttonMgr.addButton(100, 250, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "算各科总分,均分");
        buttonMgr.addButton(100, 300, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "算各学生总分,均分");
        buttonMgr.addButton(100, 350, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "学生总分降序");
        buttonMgr.addButton(100, 400, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "学生总分升序");
        buttonMgr.addButton(100, 450, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "学号升序");
        buttonMgr.addButton(100, 500, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "首字母顺序");
        
        // 右列按钮
        buttonMgr.addButton(400, 200, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "以学号检索");
        buttonMgr.addButton(400, 250, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "以姓名检索");
        buttonMgr.addButton(400, 300, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "各分数段分布");
        buttonMgr.addButton(400, 350, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "列出信息");
        buttonMgr.addButton(400, 400, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "写入文件");
        buttonMgr.addButton(400, 450, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "读取文件");
        buttonMgr.addButton(400, 500, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "退出程序");
    }
    
    MenuOption showMenu() {
        initgraph(Config::MENU_WIDTH, Config::MENU_HEIGHT);
        loadimage(&backgroundImg, "background.png", Config::MENU_WIDTH, Config::MENU_HEIGHT);
        initMenuButtons();
        
        MenuOption result = MENU_EXIT;
        
        while (true) {
            BeginBatchDraw();
            putimage(0, 0, &backgroundImg);
            
            MOUSEMSG m = GetMouseMsg();
            buttonMgr.updateAllHover(m.x, m.y);
            buttonMgr.drawAll();
            
            int clicked = buttonMgr.getClickedButton(m);
            if (clicked >= 0) {
                // 将按钮索引映射到菜单选项
                // 按钮顺序: 0-6左列, 7-13右列
                static const MenuOption buttonToOption[] = {
                    MENU_INPUT, MENU_CALC_COURSE_STATS, MENU_CALC_STUDENT_STATS,
                    MENU_SORT_SCORE_DESC, MENU_SORT_SCORE_ASC, MENU_SORT_ID, MENU_SORT_NAME,
                    MENU_SEARCH_ID, MENU_SEARCH_NAME, MENU_GRADE_DISTRIBUTION,
                    MENU_LIST_ALL, MENU_SAVE_FILE, MENU_LOAD_FILE, MENU_EXIT
                };
                result = buttonToOption[clicked];
                break;
            }
            
            EndBatchDraw();
        }
        
        closegraph();
        return result;
    }
    
    void showDisplayPage(const char* title, const char* statusMsg, 
                         void (StudentManagementApp::*drawContent)()) {
        initgraph(Config::DISPLAY_WIDTH, Config::DISPLAY_HEIGHT);
        loadimage(&backgroundImg, "background.png", Config::DISPLAY_WIDTH, Config::DISPLAY_HEIGHT);
        
        Button backButton;
        backButton.init(1040, 910, Config::BUTTON_WIDTH, Config::BUTTON_HEIGHT, YELLOW, "返回菜单");
        
        while (true) {
            BeginBatchDraw();
            putimage(0, 0, &backgroundImg);
            
            // 绘制标题和状态
            settextstyle(20, 0, "楷体");
            setbkmode(TRANSPARENT);
            settextcolor(BLACK);
            outtextxy(0, 0, title);
            outtextxy(0, 910, statusMsg);
            
            // 绘制具体内容
            if (drawContent) {
                (this->*drawContent)();
            }
            
            // 绘制返回按钮
            MOUSEMSG m = GetMouseMsg();
            backButton.updateHover(m.x, m.y);
            backButton.draw();
            
            if (backButton.isClicked(m)) {
                break;
            }
            
            EndBatchDraw();
        }
        
        closegraph();
    }
    
    // 各种显示内容的绘制函数
    void drawStudentList() {
        GUIRenderer::drawStudentList(studentMgr, 30);
    }
    
    void drawCourseStats() {
        for (int i = 0; i < studentMgr.courseCount; i++) {
            char buffer[32];
            sprintf(buffer, "Course %d", i + 1);
            outtextxy(10, 20 + i * Config::ROW_HEIGHT, buffer);
        }
        outtextxy(200, 0, "Total");
        outtextxy(300, 0, "Average");
        
        for (int i = 0; i < studentMgr.courseCount; i++) {
            char buffer[32];
            sprintf(buffer, "%.2f", studentMgr.courseStats[i].totalScore);
            outtextxy(200, 20 + i * Config::ROW_HEIGHT, buffer);
            sprintf(buffer, "%.2f", studentMgr.courseStats[i].avgScore);
            outtextxy(300, 20 + i * Config::ROW_HEIGHT, buffer);
        }
    }
    
    void drawGradeDistribution() {
        GUIRenderer::drawGradeDistribution(studentMgr, 30);
    }
    
    void drawSearchResult() {
        if (lastSearchResult < 0) {
            outtextxy(10, 60, "未找到匹配的学生");
            return;
        }
        
        GUIRenderer::drawTableHeader(30, studentMgr.courseCount);
        GUIRenderer::drawStudentRow(studentMgr.students[lastSearchResult], 
                                    0, 50, studentMgr.courseCount);
    }
    
    void drawFullRecord() {
        GUIRenderer::drawStudentList(studentMgr, 30);
        int offsetY = 70 + studentMgr.studentCount * Config::ROW_HEIGHT;
        GUIRenderer::drawCourseStats(studentMgr, offsetY);
    }
    
    // 处理各菜单选项
    void handleInput() {
        ConsoleIO::inputStudentData(studentMgr);
        showDisplayPage("录入信息", "录入成功", nullptr);
    }
    
    void handleCalcCourseStats() {
        studentMgr.calculateCourseStats();
        ConsoleIO::printCourseStats(studentMgr);
        showDisplayPage("各科总分均分", "计算成功", &StudentManagementApp::drawCourseStats);
    }
    
    void handleCalcStudentStats() {
        studentMgr.calculateStudentScores();
        ConsoleIO::printStudentList(studentMgr);
        showDisplayPage("各学生总分均分", "计算成功", &StudentManagementApp::drawStudentList);
    }
    
    void handleSortByScoreDesc() {
        studentMgr.sortByTotalScore(false);
        ConsoleIO::printStudentList(studentMgr);
        showDisplayPage("学生总分降序", "排序成功", &StudentManagementApp::drawStudentList);
    }
    
    void handleSortByScoreAsc() {
        studentMgr.sortByTotalScore(true);
        ConsoleIO::printStudentList(studentMgr);
        showDisplayPage("学生总分升序", "排序成功", &StudentManagementApp::drawStudentList);
    }
    
    void handleSortById() {
        studentMgr.sortById();
        ConsoleIO::printStudentList(studentMgr);
        showDisplayPage("学号升序", "排序成功", &StudentManagementApp::drawStudentList);
    }
    
    void handleSortByName() {
        studentMgr.sortByName();
        ConsoleIO::printStudentList(studentMgr);
        showDisplayPage("首字母顺序", "排序成功", &StudentManagementApp::drawStudentList);
    }
    
    void handleSearchById() {
        long id = ConsoleIO::inputIdForSearch();
        lastSearchResult = studentMgr.findById(id);
        
        const char* status = (lastSearchResult >= 0) ? "查询成功" : "查询失败";
        showDisplayPage("以学号检索", status, &StudentManagementApp::drawSearchResult);
    }
    
    void handleSearchByName() {
        char name[Config::MAX_NAME_LEN];
        ConsoleIO::inputNameForSearch(name, Config::MAX_NAME_LEN);
        lastSearchResult = studentMgr.findByName(name);
        
        const char* status = (lastSearchResult >= 0) ? "查询成功" : "查询失败";
        showDisplayPage("以姓名检索", status, &StudentManagementApp::drawSearchResult);
    }
    
    void handleGradeDistribution() {
        studentMgr.calculateCourseStats();
        ConsoleIO::printGradeDistribution(studentMgr);
        showDisplayPage("各分数段分布", "统计成功", &StudentManagementApp::drawGradeDistribution);
    }
    
    void handleListAll() {
        ConsoleIO::printStudentList(studentMgr);
        ConsoleIO::printCourseStats(studentMgr);
        showDisplayPage("列出信息", "列出成功", &StudentManagementApp::drawFullRecord);
    }
    
    void handleSaveFile() {
        char path[Config::MAX_PATH_LEN];
        ConsoleIO::inputFilePath(path, Config::MAX_PATH_LEN, "请输入保存路径: ");
        
        bool success = studentMgr.saveToFile(path);
        printf(success ? "写入文件成功\n" : "写入文件失败\n");
        showDisplayPage("写入文件", success ? "写入成功" : "写入失败", nullptr);
    }
    
    void handleLoadFile() {
        char path[Config::MAX_PATH_LEN];
        ConsoleIO::inputFilePath(path, Config::MAX_PATH_LEN, "请输入读取路径: ");
        
        bool success = studentMgr.loadFromFile(path);
        if (success) {
            printf("读取文件成功\n");
            ConsoleIO::printStudentList(studentMgr);
        } else {
            printf("读取文件失败\n");
        }
        showDisplayPage("读取文件", success ? "读取成功" : "读取失败", 
                       success ? &StudentManagementApp::drawStudentList : nullptr);
    }
    
public:
    StudentManagementApp() : isRunning(true), lastSearchResult(-1) {}
    
    void run() {
        // 使用循环替代递归，避免栈溢出
        while (isRunning) {
            MenuOption option = showMenu();
            
            switch (option) {
                case MENU_INPUT:              handleInput(); break;
                case MENU_CALC_COURSE_STATS:  handleCalcCourseStats(); break;
                case MENU_CALC_STUDENT_STATS: handleCalcStudentStats(); break;
                case MENU_SORT_SCORE_DESC:    handleSortByScoreDesc(); break;
                case MENU_SORT_SCORE_ASC:     handleSortByScoreAsc(); break;
                case MENU_SORT_ID:            handleSortById(); break;
                case MENU_SORT_NAME:          handleSortByName(); break;
                case MENU_SEARCH_ID:          handleSearchById(); break;
                case MENU_SEARCH_NAME:        handleSearchByName(); break;
                case MENU_GRADE_DISTRIBUTION: handleGradeDistribution(); break;
                case MENU_LIST_ALL:           handleListAll(); break;
                case MENU_SAVE_FILE:          handleSaveFile(); break;
                case MENU_LOAD_FILE:          handleLoadFile(); break;
                case MENU_EXIT:               isRunning = false; break;
                default: break;
            }
        }
    }
};

// ==================== 主函数 ====================

int main() {
    StudentManagementApp app;
    app.run();
    return 0;
}
