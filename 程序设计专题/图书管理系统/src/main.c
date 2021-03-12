#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>
#include "imgui.h"
#include "book.h"
#include "user.h"
#include "borrow.h"
void VisitGUI();
void UserGUI();
void StartGUI();
void ManagerLogGUI();
void ManagerGUI();
void DrawShowAbout();
void DrawShowUseWay();
#ifndef New
#define New(T) (T)malloc(sizeof(*(T)NULL))
#endif

//����
struct Date{
	char year[6];
	char month[3];
	char day[3];
};

//���صĽ������ں�Ӧ�����ڣ�����ʱ������Ϊ15��
struct BRTime{
	struct Date BTime;
	struct Date RTime;
};

//����
static double WinWidth, WinHeight;//���ڳߴ�
static int ShowGUI=0;//�л�GUI����

static char AdminID[30]="admin";//����Ա�˺�
static char AdminPassword[30]="123456";//����Ա����
static char UserIDCur[30] = "";

static int FileState=0;

static int ShowFileNew=0;//�����Ƿ���ʾ�½��ļ�����
static int ShowFileOpen=0;//�����Ƿ���ʾ���ļ�����
static int ShowFileSave=0;//�����Ƿ���ʾ�����ļ�����

static int ShowBookAdd=0;//�����Ƿ���ʾ����ͼ�����
static int ShowBookSearch=0;//�����Ƿ���ʾ����ͼ�����
static int ShowBookDelete=0;//�����Ƿ���ʾɾ��ͼ�����
static int ShowBookUpdate=0;//�����Ƿ���ʾɾ��ͼ�����
static int ShowBookPrint=0;//�����Ƿ��ӡȫ��ͼ��

static int BookSearchMode=0;
static int UserSearchMode=0;

static int ShowOneUser=0;//��ӡһ���û�
static int ShowAllUser=0;//��ӡȫ���û�
static int ShowUserRegister=0;//�û�ע��
static int ShowUserMessage=0;//�����Ƿ���ʾ�û���Ϣ
static int ShowUserUpdate=0;//�����Ƿ��޸��û�
static int ShowUserDelete=0;//�����Ƿ�ע���û�
static int ShowUserBorrow=0;//�����Ƿ���ʾ�û��������
static int ShowUserReturn=0;//�����Ƿ���ʾ�û��������
static int ShowUserSearch=0;//�����Ƿ���ʾ�û���ѯ����

static int ShowVisitSearch=0;//�����Ƿ���ʾ�οͲ�ѯ�鼮����

static int ShowUserPop=0;//�����Ƿ���ʾ�������а�
static int ShowBookPop=0;//�����Ƿ���ʾ���ܻ�ӭͼ��

static int BookNameSort=0;
static int BookIDSort=0;
static int UserNameSort=0;
static int UserIDSort=0;

static int ShowAbout = 0;
static int ShowUseWay = 0;
//����
void DisplayClear(void);//��������
void Display(void);//�û�����ʾ����

void FileNew(void);//�½��ļ��ؼ�
void FileSave(void);//�����ļ��ؼ�
void FileOpen(void);//���ļ��ؼ�

void ManagerBookAdd(void);//����ͼ��ؼ�
void ManagerBookSearch(int n);//��ѯͼ��ؼ�
void ManagerBookDelete(void);//ɾ��ͼ��ؼ�
void ManagerSearchButtons(void);//�鼮���Ұ�ť
void ManagerBookDelete(void);//�鼮ɾ��
void ManagerUpdateButtons(void);//�鼮���°�ť
void ManagerBookUpdate(int n);//�鼮����
void BookPrint(void);//��ӡȫ��ͼ��

void UserRegister(void);//�û�ע��
void UserPrint(void);//��ӡȫ���û�
void UserMessageShow(void);//�û�������Ϣ
void UserShow(void);
void UserRegister(void);//�û�ע��
void UserUpdateButtons(void);//�û����°�ť
void UserUpdate(int n);//�û��޸�
void UserDelete(void);//�û�ע��
void UserBorrow(void);//�û�����
void UserReturn(void);//�û����� 
void UserSearch(void);//�û���ѯ
void UserSearchButtons(void);//�û���ѯͼ�鰴ť
void UserSortPrint(char* FileName);
void sort0();//ͼ���������
void sort1();//�û���������
void sort2();//��������
void sort3();//ͼ��ID����
void sort4();//�û���������
void sort5();//�û�ID����
struct BRTime GetTime();
void SearchResultsGUI();
void OnlyHelp();
// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	uiGetChar(ch);
	Display();
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI��ȡ����
	Display(); // ˢ����ʾ
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event); //GUI��ȡ���
	Display(); // ˢ����ʾ
}

// �û����������
void Main() 
{
	//�رտ���̨������VS Code������ʹ�ã���ΪVS2019��VS2010��������ע�͵��������д���
	FreeConsole();
	// ��ʼ�����ں�ͼ��ϵͳ
	SetWindowTitle("ͼ�����ϵͳ");
    InitGraphics();

	// ��ô��ڳߴ�
    WinWidth = GetWindowWidth();
    WinHeight = GetWindowHeight();

    //�Զ�����ɫ
    DefineColor("GrayBlue", 0.57, 0.71, 0.83);

	// ע��ʱ����Ӧ����
	registerCharEvent(CharEventProcess); // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���

    setMenuColors("GrayBlue", "Black", "Dark Gray","White",1 );
    setButtonColors("GrayBlue", "Black", "Dark Gray","White",1 );
    setTextBoxColors("GrayBlue", "Black", "Dark Gray","White",1 );
}

//��ʾ����
void Display()
{
	// ����
	DisplayClear();

    switch(ShowGUI){
        case 0:StartGUI();break;//��ʼ����
        case 1:VisitGUI();break;//�οͽ���
        case 2:UserGUI();break;//�û�����
        case 3:ManagerLogGUI();break;//����Ա��½����
        case 4:ManagerGUI();break;//����Ա����
        case 7:SearchResultsGUI();break;
        case 8:UserShow();break;
        case 9:OnlyHelp(); break;
	    default:break;
    }
}

void OnlyHelp() {
    double fH = GetFontHeight();//�ָ�
    double w = WinWidth / 4;//��ť���
    double h = fH * 2.5;//��ť�߶�
    int selection;//�˵�ѡ��
    double x;
    double y;
    static char* menuListHelp[] = { "����",
        "���ڱ����",
        "ʹ�÷���", };
    static char* selectedLabel1 = NULL;
    static char* selectedLabel2 = NULL;

    drawMenuBar(0, WinHeight - fH * 1.5, WinWidth, fH * 1.5);
    selection = menuList(GenUIID(0), 0, WinHeight - fH * 1.5, w / 3, TextStringWidth(menuListHelp[1]) * 1.2, fH * 1.5, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
    if (selection > 0) {
        selectedLabel1 = menuListHelp[0];
        selectedLabel2 = menuListHelp[selection];
    }
    if (selection == 1) {
        ShowAbout = 1;
        ShowUseWay = 0;
        ShowGUI = 9;
    }
    if (selection == 2) {
        ShowAbout = 0;
        ShowUseWay = 1;
        ShowGUI = 9;
    }
    if (ShowAbout) {
        DrawShowAbout();
    }
    if (ShowUseWay) {
        DrawShowUseWay();
    }
    drawMenuBar(0, 0, WinWidth, fH * 3);//��һ��������ʾ��
    if (button(GenUIID(0), WinWidth - WinWidth / 12 - fH, fH * 4, WinWidth / 12, h, "����")) {
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowGUI = 0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
    }
    SetPenColor("Red");
    drawLabel(0, fH * 2, "��һ���˵�����Ϊ��");
    SetPenColor("Black");
    drawLabel(0, fH * 0.5, selectedLabel1);
    drawLabel(w / 3, fH * 0.5, selectedLabel2);
}

//��ʼ����
void StartGUI(){
    double fH=GetFontHeight();//�ָ�
    double w=WinWidth/4;//��ť���
    double h=fH*2.5;//��ť�߶�
    int selection;//�˵�ѡ��
    double x;
    double y;
	static char* menuListHelp[] = { "����",
		"���ڱ����",
		"ʹ�÷���", };
	static char * selectedLabel1 = NULL;
    static char * selectedLabel2 = NULL;

    drawMenuBar(0, WinHeight-fH*1.5, WinWidth, fH*1.5);//�˵���

	selection = menuList(GenUIID(0), 0, WinHeight-fH*1.5, w/3, TextStringWidth(menuListHelp[1])*1.2, fH*1.5, menuListHelp, sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListHelp[0]; 
        selectedLabel2 = menuListHelp[selection];
    }
    if (selection == 1) {
        ShowAbout = 1;
        ShowUseWay = 0;
        ShowGUI =9;
    }
    if (selection == 2) {
        ShowAbout = 0;
        ShowUseWay = 1;
        ShowGUI = 9;
    }
    if (ShowAbout) {
        DrawShowAbout();
    }
    if (ShowUseWay) {
        DrawShowUseWay();
    }
    drawMenuBar(0, 0, WinWidth, fH*3);//��һ��������ʾ��

    if(button(GenUIID(0), WinWidth/2-w/2, WinHeight/2+7*fH , w, h, "�ο͵�¼")) {
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=1; 
    }
    if(button(GenUIID(0), WinWidth/2-w/2, WinHeight/2+3.5*fH , w, h, "�û���¼")) {
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=2; 
    }
    if(button(GenUIID(0), WinWidth/2-w/2, WinHeight/2, w, h, "����Ա��¼")) {
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=3;
    } 
    if(button(GenUIID(0), WinWidth/2-w/2, WinHeight/2-3.5*fH, w, h, "�˳�")) exit(-1);

   
    SetPenColor("Red");
    drawLabel(0, fH*2, "��һ���˵�����Ϊ��");
    SetPenColor("Black");
    drawLabel(0, fH*0.5, selectedLabel1);
    drawLabel(w/3, fH*0.5, selectedLabel2);
}

//�οͽ���
void VisitGUI(){
    double fH=GetFontHeight();//�ָ�
    double w=WinWidth/9;//�ؼ����
    double h=fH*2;//�ı���߶�
    double x=WinWidth/5*2;
    double y=WinHeight/5*3;
    int selection;//�˵�ѡ��
	static char* menuListBook[] = { "ͼ��",
		"��ѯ  | Ctrl-P" };
	static char* menuListHelp[] = { "����",
		"���ڱ����",
		"ʹ�÷���"};
	static char * selectedLabel1 = NULL;
    static char * selectedLabel2 = NULL;

    drawMenuBar(0, WinHeight-fH*1.5, WinWidth, fH*1.5);//�˵���

	//ͼ��˵�
	selection = menuList(GenUIID(0), 0, WinHeight-fH*1.5, w, TextStringWidth(menuListBook[1])*1.2, fH*1.5, menuListBook, sizeof(menuListBook)/sizeof(menuListBook[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListBook[0]; 
        selectedLabel2 = menuListBook[selection];
    }
	if( selection==1 ) {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "ͼ�����δ���ţ�");
        }
        else {
            ShowVisitSearch = 1;
            ShowAbout = 0;
            ShowUseWay = 0;
        }
	}

	if(ShowVisitSearch){
        CreateBookList(FileNameCur);
        BookSearchMode=3;
		ManagerSearchButtons();
    }

	// Help �˵�
	selection = menuList(GenUIID(0), w, WinHeight-fH*1.5, w, TextStringWidth(menuListHelp[1])*1.2, fH*1.5, menuListHelp, sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListHelp[0]; 
        selectedLabel2 = menuListHelp[selection];
    }
    if (selection == 1) {
        ShowVisitSearch = 0;
         ShowAbout = 1;
         ShowUseWay = 0;
    }
    if (selection == 2) {
        ShowVisitSearch = 0;
        ShowAbout = 0;
        ShowUseWay = 1;
    }
    if (ShowAbout) {
        DrawShowAbout();
    }
    if (ShowUseWay) {
        DrawShowUseWay();
    }

    drawMenuBar(0, 0, WinWidth, fH*3);//��һ��������ʾ��

    if(button(GenUIID(0), WinWidth-WinWidth/12-fH, fH*4 , WinWidth/12, h, "����")) {
		ShowVisitSearch=0;
        ShowAbout = 0;
        ShowUseWay = 0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=0; 
    }    

    SetPenColor("Red");
    drawLabel(0, fH*2, "��һ���˵�����Ϊ��");
    SetPenColor("Black");
    drawLabel(0, fH*0.5, selectedLabel1);
    drawLabel(WinWidth/12, fH*0.5, selectedLabel2);
}

//�û�����
void UserGUI(){
	FILE *fp;
    double fH=GetFontHeight();//�ָ�
    double w=WinWidth/9;//�ؼ����
    double h=fH*2;//�ı���߶�
    double x=WinWidth/5*2;
    double y=WinHeight/5*3;
    int selection;//�˵�ѡ��
    static char* menuListUser[] = { "�û�",
        "ע��",
		"�鿴",
		"�޸�",
        "ע��" };
	static char* menuListBook[] = { "ͼ��",
		"��ѯ  | Ctrl-P" };
	static char* menuListBR[] = { "����/����",
		"����",
		"����" };
	static char* menuListHelp[] = { "����",
		"���ڱ����",
		"ʹ�÷���"};
	static char * selectedLabel1 = NULL;
    static char * selectedLabel2 = NULL;

    drawMenuBar(0, WinHeight-fH*1.5, WinWidth, fH*1.5);//�˵���

    //�û��˵�
    selection = menuList(GenUIID(0), 0, WinHeight-fH*1.5, w, TextStringWidth(menuListUser[1])*1.5, fH*1.5, menuListUser, sizeof(menuListUser)/sizeof(menuListUser[0]));
    if( selection>0 ) {
        selectedLabel1 = menuListUser[0]; 
        selectedLabel2 = menuListUser[selection];
    }

	if(selection==1){
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowUserRegister=1;
        ShowUserMessage=0;
        ShowUserUpdate=0;
        ShowUserDelete=0;
        ShowUserBorrow=0;
        ShowUserReturn=0;
        ShowUserSearch=0;
	}
    if( selection==2 ) {
		if((fp=fopen("user.txt", "r+"))==NULL){
			SetPenColor("Black");
			drawLabel(WinWidth/2, WinHeight/2, "�����û���");
		}
        else{
			fclose(fp);
			ShowAbout = 0;
			ShowUseWay = 0;
			ShowUserRegister=0;
			ShowUserMessage=1;
			ShowUserUpdate=0;
			ShowUserDelete=0;
			ShowUserBorrow=0;
			ShowUserReturn=0;
			ShowUserSearch=0;
		}
    }
    if( selection==3 ) {
		if((fp=fopen("user.txt", "r+"))==NULL){
			SetPenColor("Black");
			drawLabel(WinWidth/2, WinHeight/2, "�����û���");
		}
        else{
			fclose(fp);
			ShowAbout = 0;
			ShowUseWay = 0;
			ShowUserRegister=0;
			ShowUserMessage=0;
			ShowUserUpdate=1;
			ShowUserDelete=0;
			ShowUserBorrow=0;
			ShowUserReturn=0;
			ShowUserSearch=0;
		}
    }
    if( selection==4 ) {
		if((fp=fopen("user.txt", "r+"))==NULL){
			SetPenColor("Black");
			drawLabel(WinWidth/2, WinHeight/2, "�����û���");
		}
        else{
			fclose(fp);
			ShowAbout = 0;
			ShowUseWay = 0;
			ShowUserRegister=0;
			ShowUserMessage=0;
			ShowUserUpdate=0;
			ShowUserDelete=1;
			ShowUserBorrow=0;
			ShowUserReturn=0;
			ShowUserSearch=0;
		}
    }


    if(ShowUserRegister){
        UserRegister();
    }
	if(ShowUserMessage){
		UserSearchMode=2;
		UserMessageShow();
	}
    if(ShowUserUpdate){
		CreateUserList();
		UserUpdateButtons();
    }
    if(ShowUserDelete){
		CreateUserList();
		UserDelete();
    }

	//ͼ��˵�
	selection = menuList(GenUIID(0), w, WinHeight-fH*1.5, w, TextStringWidth(menuListBook[1])*1.2, fH*1.5, menuListBook, sizeof(menuListBook)/sizeof(menuListBook[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListBook[0]; 
        selectedLabel2 = menuListBook[selection];
    }
	if( selection==1 ) {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/2, WinHeight/2, "ͼ�����δ���ţ�");
        }
        else{     
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowUserRegister=0;
            ShowUserMessage=0;
            ShowUserUpdate=0;
            ShowUserDelete=0;
            ShowUserBorrow=0;
            ShowUserReturn=0;
            ShowUserSearch=1;
        }
    }

	if(ShowUserSearch){
        CreateBookList(FileNameCur);
        BookSearchMode=2;
        ManagerSearchButtons();
    }

	//����/����
	selection = menuList(GenUIID(0), 2*w, WinHeight-fH*1.5, w*1.5, TextStringWidth(menuListBR[1])*1.5, fH*1.5, menuListBR, sizeof(menuListBR)/sizeof(menuListBR[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListBR[0]; 
        selectedLabel2 = menuListBR[selection];
    }
	if( selection==1 ) {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/2, WinHeight/2, "ͼ�����δ���ţ�");
        }
		else if((fp=fopen("user.txt", "r+"))==NULL){
			SetPenColor("Black");
			drawLabel(WinWidth/2, WinHeight/2, "�����û���");
		}
        else{
			fclose(fp);
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowUserRegister=0;
            ShowUserMessage=0;
            ShowUserUpdate=0;
            ShowUserDelete=0;
            ShowUserBorrow=1;
            ShowUserReturn=0;
            ShowUserSearch=0;
        }
    }
	if( selection==2 ) {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/2, WinHeight/2, "ͼ�����δ���ţ�");
        }
		else if((fp=fopen("user.txt", "r+"))==NULL){
			SetPenColor("Black");
			drawLabel(WinWidth/2, WinHeight/2, "�����û���");
		}
        else{
			fclose(fp);
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowUserRegister=0;
            ShowUserMessage=0;
            ShowUserUpdate=0;
            ShowUserDelete=0;
            ShowUserBorrow=0;
            ShowUserReturn=1;
            ShowUserSearch=0;
        }
    }

    if(ShowUserBorrow){
        CreateBookList(FileNameCur);
        CreateUserList();
        UserBorrow();
    }
    
    if(ShowUserReturn){
        CreateBookList(FileNameCur);
        CreateUserList();
        UserReturn();
    }


	// Help �˵�
	selection = menuList(GenUIID(0),3.5*w, WinHeight-fH*1.5, w, TextStringWidth(menuListHelp[1])*1.2, fH*1.5, menuListHelp, sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListHelp[0]; 
        selectedLabel2 = menuListHelp[selection];
    }
    if (selection == 1) {
        ShowAbout = 1;
        ShowUseWay = 0;
        ShowUserRegister = 0;
        ShowUserMessage = 0;
        ShowUserUpdate = 0;
        ShowUserDelete = 0;
        ShowUserBorrow = 0;
        ShowUserReturn = 0;
        ShowUserSearch = 0;
    }
    if (selection == 2) {
        ShowAbout = 0;
        ShowUseWay = 1;
        ShowUserRegister = 0;
        ShowUserMessage = 0;
        ShowUserUpdate = 0;
        ShowUserDelete = 0;
        ShowUserBorrow = 0;
        ShowUserReturn = 0;
        ShowUserSearch = 0;
    }
    if (ShowAbout) {
        DrawShowAbout();
    }
    if (ShowUseWay) {
        DrawShowUseWay();
    }

    drawMenuBar(0, 0, WinWidth, fH*3);//��һ��������ʾ��

    if(button(GenUIID(0), WinWidth-WinWidth/12-fH, fH*4 , WinWidth/12, h, "����")) {
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowUserRegister=0;
        ShowUserMessage=0;
        ShowUserUpdate=0;
        ShowUserDelete=0;
        ShowUserBorrow=0;
        ShowUserReturn=0;
        ShowUserSearch=0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=0; 
    }    

    //Сͼ����
    //������ť
    x = 0;
    y = WinHeight/15*11-1.5*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, "")) {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/2, WinHeight/2, "ͼ�����δ���ţ�");
        }
        else{
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowUserRegister=0;
            ShowUserMessage=0;
            ShowUserUpdate=0;
            ShowUserDelete=0;
            ShowUserBorrow=0;
            ShowUserReturn=0;
            ShowUserSearch=1;
        }
    } 
    MovePen(x+1.4*h,y+1*h);
    DrawArc(0.4*h,0,360);
    MovePen(x+1.25*h,y+1*h);
    DrawArc(0.25*h,0,360);
    MovePen(x+0.2*h,y+0.2*h);
    DrawLine(0.55*h,0.55*h);

    //������ť
    x = 0;
    y = WinHeight/15*11-3*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, ""))    {  
        static int times = 0; 
        usePredefinedColors(++times);
   }
    MovePen(x+0.3*1.5*h,y+0.8*1.5*h);
    DrawLine(-0.2*1.5*h,-0.3*1.5*h);
    DrawLine(0.1*1.5*h,-0.1*1.5*h);
    DrawLine(0.1*1.5*h,0.067*1.5*h);
    DrawLine(0,-0.3*1.5*h);
    DrawLine(0.4*1.5*h,0);
    DrawLine(0,0.3*1.5*h);
    DrawLine(0.1*1.5*h,-0.067*1.5*h);
    DrawLine(0.1*1.5*h,0.1*1.5*h);
    DrawLine(-0.2*1.5*h,0.3*1.5*h);
    DrawLine(-0.4*1.5*h,0);

    //��ҳ��ť
    x = 0;
    y = WinHeight/15*11-4.5*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, "")) {
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowUserRegister=0;
        ShowUserMessage=0;
        ShowUserUpdate=0;
        ShowUserDelete=0;
        ShowUserBorrow=0;
        ShowUserReturn=0;
        ShowUserSearch=0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=0; 
    } 
    MovePen(x+0.5*1.5*h,y+0.8*1.5*h);
    DrawLine(-0.3*1.5*h,-0.3*1.5*h);
    DrawLine(0.6*1.5*h,0);
    DrawLine(-0.3*1.5*h,0.3*1.5*h);
    MovePen(x+0.35*1.5*h,y+0.5*1.5*h);
    DrawLine(0,-0.3*1.5*h);
    DrawLine(0.3*1.5*h,0);
    DrawLine(0,0.3*1.5*h);

    //���ذ�ť
    x = 0;
    y = WinHeight/15*11-6*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, "")) {
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowUserRegister=0;
        ShowUserMessage=0;
        ShowUserUpdate=0;
        ShowUserDelete=0;
        ShowUserBorrow=0;
        ShowUserReturn=0;
        ShowUserSearch=0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=0; 
    } 
    MovePen(x+0.7*h,y+1.1*h);
    DrawLine(-0.5*h,-0.4*h);
    DrawLine(1.1*h,0);
    DrawLine(-1.1*h,0);
    DrawLine(0.5*h,-0.4*h);

    //ע����ť
    x = 0;
    y = WinHeight/15*11-7.5*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, ""))  exit(-1);
    MovePen(x+0.8*1.5*h,y+0.5*1.5*h);
    DrawArc(0.3*1.5*h,0,360); 

    SetPenColor("Red");
    drawLabel(0, fH*2, "��һ���˵�����Ϊ��");
    SetPenColor("Black");
    drawLabel(0, fH*0.5, selectedLabel1);
    drawLabel(WinWidth/12, fH*0.5, selectedLabel2);
}

//����Ա��¼����
void ManagerLogGUI(){
    double fH=GetFontHeight();//�ָ�
    double w=WinWidth/4;//�ı�����
    double h=fH*2;//�ı���߶�
    double x=WinWidth/5*2;
    double y=WinHeight/5*3;
    int selection;//�˵�ѡ��
	static char* menuListHelp[] = { "����",
		"���ڱ����",
		"ʹ�÷���", };
	static char * selectedLabel1 = NULL;
    static char * selectedLabel2 = NULL;
    static char ID[30]={'\0'};//����Ա�˺�
    static char password[30]={'\0'};//����Ա����
    static char inputID[30]={'\0'};//�����˺�
    static char inputPassword[30]={'\0'};//��������

    drawMenuBar(0, WinHeight-fH*1.5, WinWidth, fH*1.5);//�˵���
	selection = menuList(GenUIID(0), 0, WinHeight-fH*1.5, w/3, TextStringWidth(menuListHelp[1])*1.2, fH*1.5, menuListHelp, sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListHelp[0]; 
        selectedLabel2 = menuListHelp[selection];
    }
	if( selection==1 ) ;
    if( selection==2 ) ;

    drawMenuBar(0, 0, WinWidth, fH*3);//��һ��������ʾ��

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("�˺�"), y+fH*0.9, "�˺�");
	if( textbox(GenUIID(0), x, y+fH*0.4, w, h, ID, sizeof(ID)) )
		sprintf(inputID,"%s", ID);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("����"), y-1.9*fH, "����");
	if( textbox(GenUIID(0), x, y-2.6*fH, w, h, password, sizeof(password)) )
		sprintf(inputPassword,"%s", password);

    if(button(GenUIID(0), x+w/3*2, y-5.6*fH , w/3, h, "��¼")) {
        if(strcmp(inputID, AdminID)==0 && strcmp(inputPassword, AdminPassword)==0){
            ShowGUI=4;//����Ա�˺�����ƥ�����¼
        }
        else{
            SetPenColor("Black");
            drawLabel(x, y-8.6*fH, "�������");
        }
    }

    if(button(GenUIID(0), WinWidth-w/3-fH, fH*4 , w/3, h, "����")) {
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=0; 
    } 

    SetPenColor("Red");
    drawLabel(0, fH*2, "��һ���˵�����Ϊ��");
    SetPenColor("Black");
    drawLabel(0, fH*0.5, selectedLabel1);
    drawLabel(w/3, fH*0.5, selectedLabel2);
}

//����Ա����
void ManagerGUI(){
    FILE *fp;
    double fH=GetFontHeight();//�ָ�
    double w=WinWidth/9;//�ؼ����
    double h=fH*2;//�ı���߶�
    double x=WinWidth/5*2;
    double y=WinHeight/5*3;
    int selection;//�˵�ѡ��
    static char* menuListFile[] = { "�ļ�",
		"�½�  | Ctrl-N",
		"��  | Ctrl-O",
		"����  | Ctrl-S",
		"�˳�  | Ctrl-Q" };
	static char* menuListBook[] = { "ͼ��",
		"����  | Ctrl-A",
		"��ѯ  | Ctrl-P",
		"�޸�  | Ctrl-R",
		"ɾ��  | Ctrl-D",
		"�鿴ȫ��ͼ��" };
	static char* menuListBR[] = { "�û�",
		"�鿴һ���û�",
		"�鿴ȫ���û�" };
    static char* menuListSort[] = { "����",
        "����������",
        "��ͼ��������",
        "���û�������",
		"���û��������"};
    static char* menuListStatictics[] = { "ͳ��",
        "ͼ��������а�",
        "�û��������а�"};
	static char* menuListHelp[] = { "����",
		"���ڱ����",
		"ʹ�÷���"};
	static char * selectedLabel1 = NULL;
    static char * selectedLabel2 = NULL;


    drawMenuBar(0, WinHeight-fH*1.5, WinWidth, fH*1.5);//�˵���

    //�ļ��˵�
    selection = menuList(GenUIID(0), 0, WinHeight-fH*1.5, w, TextStringWidth(menuListFile[1])*1.2, fH*1.5, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
    if( selection>0 ) {
        selectedLabel1 = menuListFile[0]; 
        selectedLabel2 = menuListFile[selection];
    }
    if( selection==1 ) {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=1;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
    }
    if( selection==2 ) {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=1;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
    }
    if( selection==3 ) {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;           
            drawLabel(WinWidth/3, WinHeight/2, "����ɹ���");
    }
    if( selection==4 ) exit(-1);

    //��ʾ�½��ļ�����
    if(ShowFileNew){
        FileNew();
    }

    //��ʾ���ļ�����
    if(ShowFileOpen){
        FileOpen();
        FileState = 1;
    }


	//ͼ��˵�
	selection = menuList(GenUIID(0), w, WinHeight-fH*1.5, w, TextStringWidth(menuListBook[1])*1.5, fH*1.6, menuListBook, sizeof(menuListBook)/sizeof(menuListBook[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListBook[0]; 
        selectedLabel2 = menuListBook[selection];
    }
	if( selection==1 ) {
        if(strcmp(FileNameCur, "")==0){
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=1;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if( selection==2 ) {
        if(strcmp(FileNameCur, "")==0){
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=1;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if( selection==3 ) {
        if(strcmp(FileNameCur, "")==0){
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=1;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if( selection==4 ) {
        if(strcmp(FileNameCur, "")==0){
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=1;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if( selection==5 )  {
        if(strcmp(FileNameCur, "")==0){
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=1;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }

    if(ShowBookAdd){
        ManagerBookAdd();
    }
    if(ShowBookSearch){
        CreateBookList(FileNameCur);
        BookSearchMode=1;
        ManagerSearchButtons();
    }
	if(ShowBookUpdate){
        CreateBookList(FileNameCur);
        ManagerUpdateButtons();
    }
	if(ShowBookDelete){
        CreateBookList(FileNameCur);
        ManagerBookDelete();
    }
	if(ShowBookPrint){
        CreateBookList(FileNameCur);
        BookPrint();  
    }

	//�û�
	selection = menuList(GenUIID(0), 2*w, WinHeight-fH*1.5, w, TextStringWidth(menuListBR[1])*1.2, fH*1.5, menuListBR, sizeof(menuListBR)/sizeof(menuListBR[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListBR[0]; 
        selectedLabel2 = menuListBR[selection];
    }
	if( selection==1 ) {
        if( (fp=fopen("user.txt", "r+"))==NULL ){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "����ע���û���");
        }
        else {
            fclose(fp);
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=1;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if( selection==2 ) {
        if( (fp=fopen("user.txt", "r+"))==NULL ){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "����ע���û���");
        }
        else {
            fclose(fp);
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=1;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if(ShowOneUser){
		UserSearchMode=1;
		UserMessageShow();
	}
	if(ShowAllUser){
        UserPrint();
    }
    //����
	selection = menuList(GenUIID(0), 3*w, WinHeight-fH*1.5, w, TextStringWidth(menuListSort[2])*1.3, fH*1.5, menuListSort, sizeof(menuListSort)/sizeof(menuListSort[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListSort[0]; 
        selectedLabel2 = menuListSort[selection];
    }
	if( selection==1 )  {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=1;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if( selection==2 )  {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=1;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
    if( selection==3 ) {
        if( (fp=fopen("user.txt", "r+"))==NULL ){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "����ע���û���");
        }
        else {
            fclose(fp);
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=1;
            UserIDSort=0;
        }
    }
    if( selection==4 ) {
        if( (fp=fopen("user.txt", "r+"))==NULL ){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "����ע���û���");
        }
        else {
            fclose(fp);
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=1;
        }
    }
    if(BookNameSort){
        CreateBookList(FileNameCur);
        sort2();
    }
    if(BookIDSort){
        CreateBookList(FileNameCur);
        sort3();
    }
    if(UserNameSort){
        CreateUserList();
        sort4();
    }
    if(UserIDSort){
        CreateUserList();
        sort5();
    }
    //ͳ��
	selection = menuList(GenUIID(0), 4*w, WinHeight-fH*1.5, w, TextStringWidth(menuListStatictics[1])*1.5, fH*1.5, menuListStatictics, sizeof(menuListStatictics)/sizeof(menuListStatictics[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListStatictics[0]; 
        selectedLabel2 = menuListStatictics[selection];
    }
	if( selection==1 )  {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "�ļ���δ�򿪣�����ļ���");
        }
        else {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=1;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if( selection==2 ) {
        if( (fp=fopen("user.txt", "r+"))==NULL ){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/2, "����ע���û���");
        }
        else {
            fclose(fp);
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=0;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=1;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
        }
    }
	if(ShowBookPop){
        CreateBookList(FileNameCur);
        sort0();
    }
	if(ShowUserPop){
        CreateUserList();
        sort1();
    }


	// Help �˵�
	selection = menuList(GenUIID(0),5*w, WinHeight-fH*1.5, w, TextStringWidth(menuListHelp[1])*1.2, fH*1.5, menuListHelp, sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) {
        selectedLabel1 = menuListHelp[0]; 
        selectedLabel2 = menuListHelp[selection];
    }
	if( selection==1 ) {
        ShowAbout = 1;
        ShowUseWay = 0;
        ShowFileNew = 0;
        ShowFileOpen = 0;
        ShowFileSave = 0;
        ShowFileSave = 0;
        ShowBookAdd = 0;
        ShowBookSearch = 0;
        ShowBookDelete = 0;
        ShowBookUpdate = 0;
        ShowBookPrint = 0;
        ShowOneUser = 0;
        ShowAllUser = 0;
        ShowBookPop = 0;
        ShowUserPop = 0;
        BookNameSort = 0;
        BookIDSort = 0;
        UserNameSort = 0;
        UserIDSort = 0;
    }
	if( selection==2 ) {
        ShowAbout = 0;
        ShowUseWay = 1;
        ShowFileNew = 0;
        ShowFileOpen = 0;
        ShowFileSave = 0;
        ShowFileSave = 0;
        ShowBookAdd = 0;
        ShowBookSearch = 0;
        ShowBookDelete = 0;
        ShowBookUpdate = 0;
        ShowBookPrint = 0;
        ShowOneUser = 0;
        ShowAllUser = 0;
        ShowBookPop = 0;
        ShowUserPop = 0;
        BookNameSort = 0;
        BookIDSort = 0;
        UserNameSort = 0;
        UserIDSort = 0;
    }

    if (ShowAbout) {
        DrawShowAbout();
    }
    if (ShowUseWay) {
        DrawShowUseWay();
    }
    drawMenuBar(0, 0, WinWidth, fH*3);//��һ��������ʾ��

    //Сͼ����
    //������ť
    x = 0;
    y = WinHeight/15*11-1.5*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, "")) {
            ShowAbout = 0;
            ShowUseWay = 0;
            ShowFileNew=0;
            ShowFileOpen=0;
            ShowFileSave=0;
            ShowFileSave=0;
            ShowBookAdd=0;
            ShowBookSearch=1;
            ShowBookDelete=0;
            ShowBookUpdate=0;
            ShowBookPrint=0;
            ShowOneUser=0;
            ShowAllUser=0;
            ShowBookPop=0;
            ShowUserPop=0;
            BookNameSort=0;
            BookIDSort=0;
            UserNameSort=0;
            UserIDSort=0;
    } 
    MovePen(x+1.4*h,y+1*h);
    DrawArc(0.4*h,0,360);
    MovePen(x+1.25*h,y+1*h);
    DrawArc(0.25*h,0,360);
    MovePen(x+0.2*h,y+0.2*h);
    DrawLine(0.55*h,0.55*h);

    //������ť
    x = 0;
    y = WinHeight/15*11-3*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, ""))    {  
        static int times = 0; 
        usePredefinedColors(++times);
   }
    MovePen(x+0.3*1.5*h,y+0.8*1.5*h);
    DrawLine(-0.2*1.5*h,-0.3*1.5*h);
    DrawLine(0.1*1.5*h,-0.1*1.5*h);
    DrawLine(0.1*1.5*h,0.067*1.5*h);
    DrawLine(0,-0.3*1.5*h);
    DrawLine(0.4*1.5*h,0);
    DrawLine(0,0.3*1.5*h);
    DrawLine(0.1*1.5*h,-0.067*1.5*h);
    DrawLine(0.1*1.5*h,0.1*1.5*h);
    DrawLine(-0.2*1.5*h,0.3*1.5*h);
    DrawLine(-0.4*1.5*h,0);

    //��ҳ��ť
    x = 0;
    y = WinHeight/15*11-4.5*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, "")) {
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowFileNew=0;
        ShowFileOpen=0;
        ShowFileSave=0;
        ShowFileSave=0;
        ShowBookAdd=0;
        ShowBookSearch=0;
        ShowBookDelete=0;
        ShowBookUpdate=0;
        ShowBookPrint=0;
        ShowOneUser=0;
        ShowAllUser=0;
        ShowBookPop=0;
        ShowUserPop=0;
        BookNameSort=0;
        BookIDSort=0;
        UserNameSort=0;
        UserIDSort=0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=0; 
    } 
    MovePen(x+0.5*1.5*h,y+0.8*1.5*h);
    DrawLine(-0.3*1.5*h,-0.3*1.5*h);
    DrawLine(0.6*1.5*h,0);
    DrawLine(-0.3*1.5*h,0.3*1.5*h);
    MovePen(x+0.35*1.5*h,y+0.5*1.5*h);
    DrawLine(0,-0.3*1.5*h);
    DrawLine(0.3*1.5*h,0);
    DrawLine(0,0.3*1.5*h);

    //���ذ�ť
    x = 0;
    y = WinHeight/15*11-6*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, "")) {
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowFileNew=0;
        ShowFileOpen=0;
        ShowFileSave=0;
        ShowFileSave=0;
        ShowBookAdd=0;
        ShowBookSearch=0;
        ShowBookDelete=0;
        ShowBookUpdate=0;
        ShowBookPrint=0;
        ShowOneUser=0;
        ShowAllUser=0;
        ShowBookPop=0;
        ShowUserPop=0;
        BookNameSort=0;
        BookIDSort=0;
        UserNameSort=0;
        UserIDSort=0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=3; 
    } 
    MovePen(x+0.7*h,y+1.1*h);
    DrawLine(-0.5*h,-0.4*h);
    DrawLine(1.1*h,0);
    DrawLine(-1.1*h,0);
    DrawLine(0.5*h,-0.4*h);

    //ע����ť
    x = 0;
    y = WinHeight/15*11-7.5*h;
    if(button(GenUIID(0) ,x, y, 1.5*h, 1.5*h, ""))  exit(-1);
    MovePen(x+0.8*1.5*h,y+0.5*1.5*h);
    DrawArc(0.3*1.5*h,0,360); 

    if(button(GenUIID(0), WinWidth-WinWidth/12-fH, fH*4 , WinWidth/12, h, "����")) {
        ShowAbout = 0;
        ShowUseWay = 0;
        ShowFileNew=0;
        ShowFileOpen=0;
        ShowFileSave=0;
        ShowFileSave=0;
        ShowBookAdd=0;
        ShowBookSearch=0;
        ShowBookDelete=0;
        ShowBookUpdate=0;
        ShowBookPrint=0;
        ShowOneUser=0;
        ShowAllUser=0;
        ShowBookPop=0;
        ShowUserPop=0;
        BookNameSort=0;
        BookIDSort=0;
        UserNameSort=0;
        UserIDSort=0;
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        ShowGUI=3; 
    }  

    SetPenColor("Red");
    drawLabel(0, fH*2, "��һ���˵�����Ϊ��");
    SetPenColor("Black");
    drawLabel(0, fH*0.5, selectedLabel1);
    drawLabel(WinWidth/12, fH*0.5, selectedLabel2);
}


//�½��ļ�
void FileNew(){
    FILE *fp;
    static char FileName[30] = "";
    static char results[30] = "";

    SetPenColor("Black");
    drawLabel(WinWidth/3, WinHeight/3*2, "�������׺��Ϊ.txt���ļ���");
	if(textbox(GenUIID(0), WinWidth/3, WinHeight/3*2-3*GetFontHeight(), WinWidth/4, 2*GetFontHeight(), FileName, sizeof(FileName)) )
		sprintf(results,"%s", FileName);
    if(button(GenUIID(0), WinWidth/2, WinHeight/3*2-6*GetFontHeight() , WinWidth/12, 2*GetFontHeight(), "ȷ��")){
        fp = fopen(results, "w+");
        fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "ͼ����", "����", "����", "�ؼ���", "������", "��������", "�ܿ��", "��ǰ����", "�ܽ��Ĵ���");
        fclose(fp);
        SetPenColor("Black");
        drawLabel(WinWidth/3, WinHeight/3*2-9*GetFontHeight(), "�½��ɹ�");
    }
}

//���ļ�
void FileOpen(){
    FILE *fp;
    static char FileName[30] = "";
    static char results[30] = "";

    SetPenColor("Black");
    drawLabel(WinWidth/3, WinHeight/3*2, "������Ҫ�򿪵��ļ���");
	if(textbox(GenUIID(0), WinWidth/3, WinHeight/3*2-3*GetFontHeight(), WinWidth/4, 2*GetFontHeight(), FileName, sizeof(FileName)) )
		sprintf(results,"%s", FileName);
    if(button(GenUIID(0), WinWidth/2, WinHeight/3*2-6*GetFontHeight() , WinWidth/12, 2*GetFontHeight(), "ȷ��")){
        if((fp = fopen(results, "r+"))==NULL){
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/3*2-9*GetFontHeight(),"�ļ������ڣ�");
        }
        else{
            strcpy(FileNameCur, results);
            SetPenColor("Black");
            drawLabel(WinWidth/3, WinHeight/3*2-9*GetFontHeight(),"�򿪳ɹ���");
            fclose(fp);
        }
    }
}

//����ͼ��
void ManagerBookAdd(){
    FILE *fp;
	static char input1[20] = "";
	static char input2[30] = "";
	static char input3[50] = "";
	static char input4[20] = "";
	static char input5[30] = "";
	static char input6[30] = "";
    static char input7[5] = "";
	static char ID[20] = "";
    static char Name[30] = "";
	static char KeyWord[50] = "";
    static char Author[20] = "";
    static char Publisher[30] = "";
    static char ReleaseDate[30];
    static char NumberAll[5] ="";
    static char BorrowTime[5] = "0";
	double fH = GetFontHeight();
	double h = fH*2; 
    double w=WinWidth/4;
	double x = 6*h;
	double y = WinWidth/2+1.8*h;

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("ID"), (y-=h*1.5)+fH*0.7, "ID");
	if( textbox(GenUIID(0), x, y, w, h, input1, sizeof(input1)))
		sprintf(ID,"%s", input1);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("����"), (y-=h*1.5)+fH*0.7, "����");
	if( textbox(GenUIID(0), x, y, w, h, input2, sizeof(input2)))
		sprintf(Name,"%s", input2);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("����"), (y-=h*1.5)+fH*0.7, "����");
	if( textbox(GenUIID(0), x, y, w, h, input3, sizeof(input3)))
		sprintf(Author,"%s", input3);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("�ؼ���"), (y-=h*1.5)+fH*0.7, "�ؼ���");
	if( textbox(GenUIID(0), x, y, w, h, input4, sizeof(input4)))
		sprintf(KeyWord,"%s", input4);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("������"), (y-=h*1.5)+fH*0.7, "������");
	if( textbox(GenUIID(0), x, y, w, h, input5, sizeof(input5)))
		sprintf(Publisher,"%s", input5);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("��������"), (y-=h*1.5)+fH*0.7, "��������");
	if( textbox(GenUIID(0), x, y, w, h, input6, sizeof(input6)))
		sprintf(ReleaseDate,"%s", input6);
    
    SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("���"), (y-=h*1.5)+fH*0.7, "���");
	if( textbox(GenUIID(0), x, y, w, h, input7, sizeof(input7)))
		sprintf(NumberAll,"%s", input7);
    	
	if(button(GenUIID(0), x+w/2, y-=h*1.5, w/2, h, "����")){
        CreateBookList(FileNameCur);
        if(SearchBook0(booklist, 3, ID)!=NULL){
            SetPenColor("Black");
            drawLabel(x, y-=h*2, "ͼ�����ظ���");
        }
        else if (strlen(ID) != 8) {
            SetPenColor("Black");
            drawLabel(x, y -= h * 2, "�������λ��Чͼ��ID��");
        }
        else{
            fp = fopen(FileNameCur, "at+");
            fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", ID, Name, Author, KeyWord, Publisher, ReleaseDate, NumberAll, NumberAll, BorrowTime);
            fclose(fp);
            SetPenColor("Black");
            drawLabel(x, y-=h*2, "�����ɹ���");
            strcpy(input1, "");
            strcpy(input2, "");
            strcpy(input3, "");
            strcpy(input4, "");
            strcpy(input5, "");
            strcpy(input6, "");
            strcpy(input7, "");
        }
	}
}

//����Ա��ѯ�鼮��ť
void ManagerSearchButtons()
{
	double fH = GetFontHeight();
	double h = fH*2;  // �ؼ��߶�
	double x = 0;  
	double y = WinHeight/15*11; 
	double w = WinWidth/9; // �ؼ����
	static int AccordingToName=0;
	static int AccordingToID=0;
    static int AccordingToAuthor=0;
    static int AccordingToKeyWord=0;

	if(button(GenUIID(0), x+3*h, y , w, h, "����Ų�ѯ")) {
        AccordingToKeyWord=0;
		AccordingToName=0;
        AccordingToAuthor=0;
		AccordingToID=1;
	} 
	if(button(GenUIID(0), x+3*h, y-1.5*h, w, h, "��������ѯ")) {
        AccordingToKeyWord=0;
		AccordingToID=0;
        AccordingToAuthor=0;
		AccordingToName=1; 
	}
	if(button(GenUIID(0), x+3*h, y-3*h, w, h, "�����߲�ѯ")) {
        AccordingToKeyWord=0;
		AccordingToID=0;
		AccordingToName=0; 
        AccordingToAuthor=1;
	}
	if(button(GenUIID(0), x+3*h, y-4.5*h, w, h, "���ؼ��ֲ�ѯ")) {
		AccordingToID=0;
		AccordingToName=0; 
        AccordingToAuthor=0;
        AccordingToKeyWord=1;
    }
    if(AccordingToID) ManagerBookSearch(1);
	if(AccordingToName) ManagerBookSearch(2);
	if(AccordingToAuthor) ManagerBookSearch(3);
    if(AccordingToKeyWord) ManagerBookSearch(4);
}

//��ѯͼ��
void ManagerBookSearch(int n){
    static char input[80] = "";//Ԥ������
    static char results[200] = "";
    double fH = GetFontHeight();
    double h = fH*2; // �ؼ��߶�
    double w = WinWidth/4; // �ؼ����
    double x = WinWidth/3-1.5*h;
    double y = WinHeight/15*11;
    BookMessageList ptr=NULL;

    SetPenColor("Black"); 
    switch(n){
        case 1: drawLabel(x-fH/2-TextStringWidth("���"), y+fH*0.7, "���"); break;
        case 2: drawLabel(x-fH/2-TextStringWidth("����"), y+fH*0.7, "����"); break;
        case 3: drawLabel(x-fH/2-TextStringWidth("����"), y+fH*0.7, "����"); break;
        case 4: drawLabel(x-fH/2-TextStringWidth("�ؼ���"), y+fH*0.7, "�ؼ���"); break;
        default: break;
    }

    if( textbox(GenUIID(0), x, y, w, h, input, sizeof(input)))
        sprintf(results,"%s", input);
    else strcpy(results, input);

    //����ѯͼ�갴ť
    if(button(GenUIID(0), x+w+0.5*h, y, w/2, h, "��ѯ")) {
        if(n==1){
        ptr = SearchBook0(booklist, 1, results);
        if(ptr==NULL){
            SetPenColor("Black"); 
            drawLabel(x, y-2*h+fH*0.7, "�鼮������");
        }
        else
            ShowGUI = 7;
        
        }      
        else{
        ptr = SearchBook1(booklist, n-1, results);
        if(ptr->next==NULL){
            SetPenColor("Black"); 
            drawLabel(x, y-2*h+fH*0.7, "�鼮������");
        }
        else
                ShowGUI = 7;
    }
    } 
}

//�������
void SearchResultsGUI(){
    FILE* fa = NULL;
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double x = WinWidth/18;
	double y = WinHeight/15*14;
	static char ID[20] = "";//�鼮���
	static char Name[30] = "";//����
	static char Author[20] = "";//���ߣ�Ŀǰֻʵ���˵�һ���߲���
	static char KeyWord[50] = "";//�ؼ���
	static char Publisher[30] = "";//������
	static char ReleaseDate[30];//��������
	static char NumberAll[5] = "";
    static char Number[5] = "";
	static int i=0;
	static char* line, * record;
	static char buffer[1024];
	static char* result = NULL;
	static int j = 0;

	SetPenColor("Black");
	drawLabel(x, y, "ͼ����");
	drawLabel(x+3*h, y, "����");
	drawLabel(x+6*h, y, "����");
	drawLabel(x+10*h, y, "�ؼ���");
	drawLabel(x+16*h, y, "������");
	drawLabel(x+20*h, y, "��������");
	drawLabel(x+23*h, y, "������");
    drawLabel(x+25*h, y, "ʣ������");
	drawMenuBar(0, WinHeight-fH*1.5, WinWidth, fH*1.5);//�˵���
	drawMenuBar(0, 0, WinWidth, fH*3);//��һ��������ʾ��

	y = y-h;

	if ((fa = fopen("return.txt", "r+")) != NULL)
	{
		while ((line = fgets(buffer, sizeof(buffer), fa)) != NULL)
		{
		record = strtok(line, "\t");//���Ʊ���ָ�������������
		while (record != NULL)
		{
			switch (j) {
			case 0:strcpy(ID, record); break;
			case 1:strcpy(Name, record); break;
			case 2:strcpy(Author, record); break;
			case 3:strcpy(KeyWord, record); break;
			case 4:strcpy(Publisher, record); break;
			case 5:strcpy(ReleaseDate, record); break;
			case 6:strcpy(NumberAll, record); break;
            case 7:strcpy(Number, record); break;
            case 8:break;
			default:break;
			}
			record = strtok(NULL, "\t");
			j++;
		}
		SetPenColor("Black");
		drawLabel(x, y-i*h, ID);
		drawLabel(x+3*h, y-i*h, Name);
		drawLabel(x+6*h, y-i*h, Author);
		drawLabel(x+10*h, y-i*h, KeyWord);
		drawLabel(x+16*h, y-i*h, Publisher);
		drawLabel(x+20*h, y-i*h, ReleaseDate);
		drawLabel(x+23*h, y-i*h, NumberAll);
        drawLabel(x+25*h, y-i*h, Number);
		i++;
		j = 0;

		}
		fclose(fa);
		fa = NULL;
	}
	i=0;

	if(button(GenUIID(0), WinWidth-WinWidth/12-fH, fH*4 , WinWidth/12, h, "����")) {
		remove("return.txt");
		switch(BookSearchMode){
			case 1:ShowGUI=4; break;
			case 2:ShowGUI=2; break;
			case 3:ShowGUI=1; break;
			default:break;
		}
	} 
}

//ɾ��ͼ��
void ManagerBookDelete(){

	static char input[80] = "";
	static char results[200] = "";
	static int DeleteMode=0;

	BookMessageList ptr = New(BookMessageList);
	BookMessageList p = New(BookMessageList);

	SetPenColor("Black");
	drawLabel(WinWidth/3, WinHeight/3*2, "��������Ҫɾ����ͼ����");

	if (textbox(GenUIID(0), WinWidth/3, WinHeight/3*2-3*GetFontHeight(), WinWidth/4, 2*GetFontHeight(), input, sizeof(input)))
		sprintf(results, "%s", input);
	else strcpy(results, input);
	if (button(GenUIID(0), WinWidth/2, WinHeight/3*2-6*GetFontHeight() , WinWidth/12, 2*GetFontHeight(), "ȷ��")){
		DeleteMode = 1;
	}
	if (DeleteMode) {
		ptr = SearchBook0(booklist, 3, results);
		if (ptr != NULL) {
		 DeleteBook(booklist, ptr);
		}
		SetPenColor("Black");
		drawLabel(WinWidth/2, WinHeight/3*2-9*GetFontHeight(), "ɾ���ɹ�");
	}

	DeleteMode = 0;
}

//�޸�ͼ�鰴ť
void ManagerUpdateButtons(){
	double fH = GetFontHeight();
	double h = fH*2;  // �ؼ��߶�
	double x = 0;  
	double y = WinHeight/15*11; 
	double w = WinWidth/9; // �ؼ����
	static int AccordingToName=0;
    static int AccordingToAuthor=0;
    static int AccordingToKeyWord=0;
	static int AccordingToPublisher=0;
	static int AccordingToReleaseDate=0;
	static int AccordingToNumber=0;


	if(button(GenUIID(0), x+3*h, y, w, h, "�޸�����")) {
        AccordingToKeyWord=0;
        AccordingToAuthor=0;
		AccordingToPublisher=0;
		AccordingToNumber=0;
		AccordingToReleaseDate=0;
		AccordingToName=1; 
	}

	if(button(GenUIID(0), x+3*h, y-1.5*h, w, h, "�޸�����")) {
		AccordingToName=0; 
        AccordingToAuthor=1;
		AccordingToPublisher=0;
		AccordingToReleaseDate=0;
		AccordingToNumber=0;
        AccordingToKeyWord=0;
    }

	if(button(GenUIID(0), x+3*h, y-3*h, w, h, "�޸Ĺؼ���")) {
        AccordingToKeyWord=1;
		AccordingToName=0; 
		AccordingToPublisher=0;
		AccordingToReleaseDate=0;
		AccordingToNumber=0;
        AccordingToAuthor=0;
	}

	if(button(GenUIID(0), x+3*h, y-4.5*h, w, h, "�޸ĳ�����")) {
		AccordingToName=0; 
        AccordingToAuthor=0;
		AccordingToReleaseDate=0;
		AccordingToPublisher=1;
		AccordingToNumber=0;
        AccordingToKeyWord=0;
    }

	if(button(GenUIID(0), x+3*h, y-6*h, w, h, "�޸ĳ�������")) {
		AccordingToName=0; 
        AccordingToAuthor=0;
		AccordingToReleaseDate=1;
		AccordingToPublisher=0;
		AccordingToNumber=0;
        AccordingToKeyWord=0;
    }

	if(button(GenUIID(0), x+3*h, y-7.5*h, w, h, "�޸Ŀ��")) {
		AccordingToName=0; 
        AccordingToAuthor=0;
		AccordingToPublisher=0;
		AccordingToReleaseDate=0;
		AccordingToNumber=1;
        AccordingToKeyWord=0;
    }

	if(AccordingToName)
		ManagerBookUpdate(1);

	if(AccordingToAuthor)
		ManagerBookUpdate(2);

    if(AccordingToKeyWord)
        ManagerBookUpdate(3);

    if(AccordingToPublisher)
        ManagerBookUpdate(4);

    if(AccordingToReleaseDate)
        ManagerBookUpdate(5);

    if(AccordingToNumber)
        ManagerBookUpdate(6);
}

//�޸�ͼ��
void ManagerBookUpdate(int n){
	static char input[80] = "";//id������
	static char input2[80] = "";//����Ϣ����
	static char results[200] = "";
	static char results2[200] = "";
	static char ID[20] = "";//�鼮���
    static char Name[30] = "";//����
    static char Author[20] = "";//���ߣ�Ŀǰֻʵ���˵�һ���߲���
    static char KeyWord[50] = "";//�ؼ���
    static char Publisher[30] = "";//������
    static char ReleaseDate[30];//��������
    static char NumberAll[5] = "";//���Ĵ���
	static int changeMode = 0;
	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double w = WinWidth / 4; // �ؼ����
	double x = WinWidth/3-1.5*h;
    double y = WinHeight/15*11;
	BookMessageList ptr = New(BookMessageList);
	BookMessageList p = New(BookMessageList);

    SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("��"), y+fH*0.7, "ͼ����");

    switch(n){
        case 1: drawLabel(x-fH/2-TextStringWidth("��"), y-1.5*h+fH*0.7, "������"); break;
        case 2: drawLabel(x-fH/2-TextStringWidth("��"), y-1.5*h+fH*0.7, "������"); break;
        case 3: drawLabel(x-fH/2-TextStringWidth("��"), y-1.5*h+fH*0.7, "�¹ؼ���"); break;
		case 4: drawLabel(x-fH/2-TextStringWidth("��"), y-1.5*h+fH*0.7, "�³�����"); break;
		case 5: drawLabel(x-fH/2-TextStringWidth("��"), y-1.5*h+fH*0.7, "�³�������"); break;
		case 6: drawLabel(x-fH/2-TextStringWidth("��"), y-1.5*h+fH*0.7, "�¿��"); break;
        default: break;
    }

	if (textbox(GenUIID(0), x+TextStringWidth("�³�������"), y, w, h, input, sizeof(input)))
		sprintf(results, "%s", input);
	else strcpy(results, input);

	if (textbox(GenUIID(0), x+TextStringWidth("�³�������"), y-1.5*h, w, h, input2, sizeof(input2)))
		sprintf(results2, "%s", input2);
	else strcpy(results2, input2);

	if(button(GenUIID(0), x+w/2+TextStringWidth("�³�������"), y-3*h, w/2, h, "ȷ��")){
		changeMode = 1;
	}

	if (changeMode) {
		ptr = SearchBook0(booklist,3, results);
		if (ptr != NULL) {
			if (n == 1) {
				strcpy(ptr->Name, results2);
				p = UpdateBook(booklist,2 ,ptr);
			}
			if (n == 2) {
				strcpy(ptr->Author, results2);
				p = UpdateBook(booklist,2 ,ptr);
			}	
			if (n == 3) {
				strcpy(ptr->KeyWord, results2);
				p = UpdateBook(booklist,2, ptr);
			}	
			if (n == 4) {
				strcpy(ptr->Publisher, results2);
				p = UpdateBook(booklist,2, ptr);
			}	
			if (n == 5) {
				strcpy(ptr->ReleaseDate, results2);
				p = UpdateBook(booklist,2, ptr);
			}	
			if (n == 6) {
				strcpy(ptr->NumberAll, results2);
                strcpy(ptr->Number, results2);
				p = UpdateBook(booklist,2, ptr);
			}	
			SetPenColor("Black");
			drawLabel(WinWidth/2, WinHeight/2, "�޸ĳɹ���");
		}
		changeMode = 0;
	}
}

//��ʾȫ��ͼ��
void BookPrint(){
    FILE* fa = NULL;
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double x = WinWidth/12;
    double y = WinHeight/15*11;
	static char ID[20] = "";//�鼮���
    static char Name[30] = "";//����
    static char Author[20] = "";//����
    static char KeyWord[50] = "";//�ؼ���
    static char Publisher[30] = "";//������
    static char ReleaseDate[30];//��������
    static char NumberAll[5] = "";//ʣ������
	static char Number[5] = "";
	static int i=0;
	static char* line, * record;
	static char buffer[1024];
	static char* result = NULL;
	static int j = 0;

	SetPenColor("Black");
    drawLabel(x, y, "ͼ����");
    drawLabel(x+3*h, y, "����");
    drawLabel(x+6*h, y, "����");
    drawLabel(x+9*h, y, "�ؼ���");
    drawLabel(x+12*h, y, "������");
    drawLabel(x+15*h, y, "��������");
    drawLabel(x+18*h, y, "�ܿ��");
	drawLabel(x+21*h, y, "ʣ������");

    y = y-h;

	if ((fa = fopen(FileNameCur, "r+")) != NULL)
		{
			fgets(buffer, sizeof(buffer), fa);
			while ((line = fgets(buffer, sizeof(buffer), fa)) != NULL)
			{
				record = strtok(line, "\t");//���Ʊ���ָ�������������
				while (record != NULL)
				{
					switch (j) {
					case 0:strcpy(ID, record); break;
					case 1:strcpy(Name, record); break;
					case 2:strcpy(KeyWord, record); break;
					case 3:strcpy(Author, record); break;
					case 4:strcpy(Publisher, record); break;
					case 5:strcpy(ReleaseDate, record); break;
					case 6:strcpy(NumberAll, record); break;
					case 7:strcpy(Number, record); break;
                    case 8:break;
					default:break;
					}
					record = strtok(NULL, "\t");
					j++;
				}
				SetPenColor("Black");
				drawLabel(x, y-i*h, ID);
				drawLabel(x+3*h, y-i*h, Name);
				drawLabel(x+6*h, y-i*h, Author);
				drawLabel(x+9*h, y-i*h, KeyWord);
				drawLabel(x+12*h, y-i*h, Publisher);
				drawLabel(x+15*h, y-i*h, ReleaseDate);
				drawLabel(x+18*h, y-i*h, NumberAll);
				drawLabel(x+21*h, y-i*h, Number);
				i++;
				j = 0;

			}
			fclose(fa);
			fa = NULL;
		}
	i=0;
}

//�û�ע��
void UserRegister(){
    FILE *fp;
	double fH = GetFontHeight();
	double h = fH*2; // �ı���߶�
    double w=WinWidth/4;//�ؼ����
	double x = 6*h;
	double y = WinWidth/2+1.8*h;
    int selection;//�˵�ѡ��
	static char input1[20] = "";//
	static char input2[30] = "";//
	static char input3[5] = "";//
	static char input4[50] = "";//
	static char ID[20] = "";
	static char Name[30] = "";
	static char Gender[5] = "";
	static char WorkUnit[50] = "";

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("ID"), (y-=h*1.5)+fH*0.7, "ID");
	if( textbox(GenUIID(0), x, y, w, h, input1, sizeof(input1)))
		sprintf(ID,"%s", input1);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("����"), (y-=h*1.5)+fH*0.7, "����");
	if( textbox(GenUIID(0), x, y, w, h, input2, sizeof(input2)))
		sprintf(Name,"%s", input2);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("�Ա�"), (y-=h*1.5)+fH*0.7, "�Ա�");
	if( textbox(GenUIID(0), x, y, w, h, input3, sizeof(input3)))
		sprintf(Gender,"%s", input3);

	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("������λ"), (y-=h*1.5)+fH*0.7, "������λ");
	if( textbox(GenUIID(0), x, y, w, h, input4, sizeof(input4)))
		sprintf(WorkUnit,"%s", input4);

	if(button(GenUIID(0), x+w/2, y-=h*1.5, w/2, h, "ע��")){
        if((fp=fopen("user.txt", "r+"))==NULL){
            fp = fopen("user.txt", "w+");
            fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n", "�û����", "����", "�Ա�", "������λ", "�ܽ�����", "��ǰ������");
            fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n", ID, Name, Gender, WorkUnit, "0", "0");
            fclose(fp);
            SetPenColor("Black");
            drawLabel(x, y-=h*2, "ע��ɹ���");
        }
        else{
			fclose(fp);
            CreateUserList();
            if(SearchUser(userlist, ID)!=NULL){
                SetPenColor("Black");
                drawLabel(x, y-=h*2, "�û�����ظ���");
            }
            else if (strlen(ID)!=10) {
                SetPenColor("Black");
                drawLabel(x, y -= h * 2, "������ʮλ��Ч�û�ID��");
            }
            else{
                fp = fopen("user.txt", "at+");
                fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n", ID, Name, Gender, WorkUnit, "0", "0");
                fclose(fp);
                SetPenColor("Black");
                drawLabel(x, y-=h*2, "ע��ɹ���");
                strcpy(input1, "");
                strcpy(input2, "");
                strcpy(input3, "");
                strcpy(input4, "");
            }
        }
	}
}

//��ӡȫ���û�
void UserPrint(){
    FILE* fa = NULL;
	double fH = GetFontHeight();
	double h = fH*2;
	double x = WinWidth/12;
    double y = WinHeight/15*11;
	static char ID[20] = "";
    static char Name[30] = "";
    static char Gender[20] = "";
	static char WorkUnit[50] = "";
    static char BorrowTimeAll[30] = "";
    static char BorrowTimeNow[30] = "";
	static int i=0;
	static char* line, * record;
	static char buffer[1024];
	static char* result = NULL;
	static int j = 0;

	SetPointSize(4);
	SetPenColor("Black");
    drawLabel(x, y, "�û�ID");
    drawLabel(x+4*h, y, "����");
    drawLabel(x+8*h, y, "�Ա�");
    drawLabel(x+12*h, y, "������λ");
    drawLabel(x+16*h, y, "�ܽ�����");
    drawLabel(x+20*h, y, "��ǰ������");

    y = y-h;

	if ((fa = fopen("user.txt", "r+")) != NULL)
		{
			fgets(buffer, sizeof(buffer), fa);
			while ((line = fgets(buffer, sizeof(buffer), fa)) != NULL)
			{
				record = strtok(line, "\t");//���Ʊ���ָ�������������
				while (record != NULL)
				{
					switch (j) {
					case 0:strcpy(ID, record); break;
					case 1:strcpy(Name, record); break;
					case 2:strcpy(Gender, record); break;
					case 3:strcpy(WorkUnit, record);break;
					case 4:strcpy(BorrowTimeAll, record); break;
					case 5:strcpy(BorrowTimeNow, record); break;
					default:break;
					}
					record = strtok(NULL, "\t");
					j++;
				}
				SetPenColor("Black");
				drawLabel(x, y-i*h, ID);
				drawLabel(x+4*h, y-i*h, Name);
				drawLabel(x+8*h, y-i*h, Gender);
				drawLabel(x+12*h, y-i*h, WorkUnit);
				drawLabel(x+16*h, y-i*h, BorrowTimeAll);
				drawLabel(x+20*h, y-i*h, BorrowTimeNow);
				i++;
				j = 0;

			}
			fclose(fa);
			fa = NULL;
		}
	i=0;
}

//�û�����
void UserBorrow(){
    FILE *fw;
	FILE *fp;
	static char input1[80] = "";
	static char input2[80] = "";
	static char results1[200] = "";
	static char results2[200] = "";
	static char ID[20] = "";//�鼮���
	static char Name[30] = "";//����
	static char Author2[500] = "";//����
	static char Publisher[30] = "";//������
	static char ReleaseDate[30];//��������
	static char BorrowOrNot[300];
	static char Number1[5];
	static char Number2[5];
	static int searchMode = 0;
	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double w = WinWidth / 4; // �ؼ����
	double x = 16 * fH;
	double y = WinWidth / 3 * 2 - 3.6 * fH;
	int num, time,num2,time2,numall,numnow;
	struct BRTime date;
	BookMessageList ptr = New(BookMessageList);
	BookMessageList p = New(BookMessageList);
	UserMessageList ptr3 = New(UserMessageList);
	SetPenColor("Black");
	drawLabel(x - fH*1.5 - TextStringWidth("����"), (y -= h * 1.5) + fH * 0.7, "�û�ID");
	if (textbox(GenUIID(0), x, y, w, h, input1, sizeof(input1)))
	sprintf(results1, "%s", input1);
	else strcpy(results1, input1);

	SetPenColor("Black");
	drawLabel(x - fH*1.5 - TextStringWidth("����"), (y -= h * 1.5) + fH * 0.7, "����");
	if (textbox(GenUIID(0), x, y, w, h, input2, sizeof(input2)))
	sprintf(results2, "%s", input2);
	else strcpy(results2, input2);

	if (button(GenUIID(0), x + w + 0.5 * h, y, w / 2, h, "ȷ�Ͻ���"))   searchMode = 1;

	if (searchMode) {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel( x , y-h,"����Ա��δ���Ž軹�鹦��");
        }
        else{
            CreateUserList();
            CreateBookList(FileNameCur);
            ptr = SearchBook0(booklist, 2, results2);
            ptr3 = SearchUser(userlist, results1);
            if(ptr3==NULL){
                SetPenColor("Black");
                drawLabel( x + w + 0.5 * h, y-h,"�û������ڣ�");                
            }
            else if(ptr==NULL){
                SetPenColor("Black");
                drawLabel( x + w + 0.5 * h, y-h,"�鼮�����ڣ�");                     
            }
	        else{
                sprintf(ID, "��ţ�%s", ptr->ID);
                sprintf(Name, "������%s", ptr->Name);
                sprintf(Author2, "���ߣ�%s", ptr->Author);
                sprintf(Publisher, "�����磺%s", ptr->Publisher);
                sprintf(ReleaseDate, "�������ڣ�%s", ptr->ReleaseDate);
                sprintf(Number1, "ʣ��������%s", ptr->Number);
                num = atoi(ptr->Number);
                numnow = atoi(ptr3->BorrowTimeNow);
                if (num > 0&&numnow<2) {
                    time = atoi(ptr->BorrowTime);
                    time++;
                    itoa(time, ptr->BorrowTime, 10);//������Ĵ�����һ
                    num--;
                    itoa(num, ptr->Number, 10);//�������һ
                    numall = atoi(ptr3->BorrowTimeAll);
                    numall++;
                    itoa(numall, ptr3->BorrowTimeAll, 10);//�ܽ��������һ
            
                    numnow++;
                    itoa(numnow, ptr3->BorrowTimeNow, 10);//��ǰ���������һ
                    sprintf(BorrowOrNot, "����ɹ� ʣ��������%s", ptr->Number);
                    UpdateBook(booklist,1, ptr);
                    UpdateUser(userlist,1, ptr3);
                    date = GetTime();
                    
                    if ((fw = fopen("borrow.txt", "r+")) == NULL) {
                        fw = fopen("borrow.txt", "at+");
                        fprintf(fw, "%s\t%s\t%s\t%s\t%s\n", "�û�ID", "ͼ��ID", "����", "��������", "Ӧ������");
                    }
                    else {
                        fclose(fw);
                        fw = fopen("borrow.txt", "at+");
                    }
                    fprintf(fw, "%s\t%s\t%s\t%s-%s-%s\t%s-%s-%s\n", results1, ptr->ID, ptr->Name, date.BTime.year, date.BTime.month, date.BTime.day, date.RTime.year, date.RTime.month, date.RTime.day);
                    fclose(fw);
                }
                else if(num<=0) {
                    sprintf(BorrowOrNot, "���Ϊ�����ʧ��");
                }
                else if (numnow >= 2) {
                    sprintf(BorrowOrNot, "�û����������������ƽ���ʧ��");
                }
	}
        }
	    searchMode = 0;
	}
	drawLabel(x, y - fH * 3, ID);
	drawLabel(x, y - fH * 6, Name);
    drawLabel(x, y - fH * 9, Author2);
	drawLabel(x, y - fH * 12, Publisher);
	drawLabel(x, y - fH * 15, ReleaseDate);
	drawLabel(x, y - fH * 18, BorrowOrNot);
}

//��ȡ����ͻ���ʱ��
struct BRTime GetTime(){
	int year, month, day;
	int year1, month1, day1;
	struct BRTime results;
	time_t timep;
	struct tm *p;
	time(&timep);
	p=gmtime(&timep);

	year=1900+p->tm_year;
	month=1+p->tm_mon;
	day=p->tm_mday;
    switch(month){
        case 1: 
            if(day+15>31) {day1 = day+15-31;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 2:
            if(day+15>28){day1 = day+15-28;month1 = month+1;}
            else{day1 = day+15;month1 = month;year1=year;}
            break;
        case 3:
            if(day+15>31) {day1 = day+15-31;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 4:
            if(day+15>30) {day1 = day+15-30;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 5: 
            if(day+15>31) {day1 = day+15-31;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 6:
            if(day+15>30){day1 = day+15-30;month1 = month+1;}
            else{day1 = day+15;month1 = month;year1=year;}
            break;
        case 7:
            if(day+15>31) {day1 = day+15-31;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 8:
            if(day+15>31) {day1 = day+15-31;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 9: 
            if(day+15>30) {day1 = day+15-30;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 10:
            if(day+15>31){day1 = day+15-31;month1 = month+1;}
            else{day1 = day+15;month1 = month;year1=year;}
            break;
        case 11:
            if(day+15>30) {day1 = day+15-30;month1 = month+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
        case 12:
            if(day+15>31) {day1 = day+15-31;month1 = 1;year1=year+1;}
            else {day1 = day+15;month1 = month;year1=year;}
            break;
    }
    
	itoa(year, results.BTime.year, 10);
	itoa(day, results.BTime.day, 10);
	itoa(month, results.BTime.month, 10);

	itoa(year, results.RTime.year, 10);
	itoa(day1, results.RTime.day, 10);
	itoa(month1, results.RTime.month, 10);

	return results;
}

//�û�����
void UserReturn() {
    FILE *fw;
	FILE *fp;
	static char input1[80] = "";
	static char input2[80] = "";
	static char results1[200] = "";
	static char results2[200] = "";
	static char ID[20] = "";//�鼮���
	static char Name[300] = "";//����
	static char Author[20] = "";//���ߣ�Ŀǰֻʵ���˵�һ���߲���
	static char Publisher[30] = "";//������
	static char ReleaseDate[30];//��������
	static char BorrowDate[30];//��������
	static char Number[10];
	static int searchMode = 0;
	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double w = WinWidth / 4; // �ؼ����
	double x = 16 * fH;
	double y = WinWidth / 3 * 2 - 3.6 * fH;
	int num;
	BookMessageList ptr = New(BookMessageList);
	BorrowMessageList ptr2 = New(BorrowMessageList);
	UserMessageList ptr4 = New(UserMessageList);
	int num4;
	SetPenColor("Black");
	drawLabel(x - fH*1.5 - TextStringWidth("����"), (y -= h * 1.5) + fH * 0.7, "�û�ID");
	if (textbox(GenUIID(0), x, y, w, h, input1, sizeof(input1)))
		sprintf(results1, "%s", input1);
	else strcpy(results1, input1);

	SetPenColor("Black");
	drawLabel(x - fH*1.5 - TextStringWidth("����"), (y -= h * 1.5) + fH * 0.7, "����");
	if (textbox(GenUIID(0), x, y, w, h, input2, sizeof(input2)))
		sprintf(results2, "%s", input2);
	else strcpy(results2, input2);


	if (button(GenUIID(0), x + w + 0.5 * h, y, w / 2, h, "ȷ�ϻ���"))   searchMode = 1;
	if (searchMode) {
        if(strcmp(FileNameCur, "")==0){
            SetPenColor("Black");
            drawLabel( x , y-h,"����Ա��δ���Ž軹�鹦��");
        }
        else{
            CreateUserList();
            CreateBookList(FileNameCur);
            ptr = SearchBook0(booklist, 2, results2);
            ptr4 = SearchUser(userlist, results1);
            if(ptr4==NULL){
                SetPenColor("Black");
                drawLabel( x + w + 0.5 * h, y-h,"�û������ڣ�");                
            }
            else if(ptr==NULL){
                SetPenColor("Black");
                drawLabel( x + w + 0.5 * h, y-h,"�鼮�����ڣ�");                     
            }
		    else {
                sprintf(ID, "��ţ�%s", ptr->ID);
                sprintf(Name, "������%s", ptr->Name);
                sprintf(Author, "���ߣ�%s", ptr->Author);
                sprintf(Publisher, "�����磺%s", ptr->Publisher);
                sprintf(ReleaseDate, "�������ڣ�%s", ptr->ReleaseDate);
                num = atoi(ptr->Number);
                num++;
                itoa(num, ptr->Number, 10);//�������һ
                num4 = atoi(ptr4->BorrowTimeNow);
                num4--;
                itoa(num4, ptr4->BorrowTimeNow, 10);
			
                sprintf(Number, "%s", "����ɹ���");
                UpdateBook(booklist,2 ,ptr);
                UpdateUser(userlist,1, ptr4);
                if((fw=fopen("borrow.txt", "r+"))!=NULL){
                    fclose(fw);
                    CreateBorrowList("borrow.txt");
                    ptr2 = SearchBR(borrowlist, results1, ptr->ID);
                    sprintf(BorrowDate, "�������ڣ�%s", ptr2->BorrowDate);
                    if(ptr2!=NULL) DeleteBR(borrowlist, ptr2);
			    }
            }
		}
		searchMode = 0;
	}
	drawLabel(x, y - fH * 3, ID);
	drawLabel(x, y - fH * 6, Name);
	drawLabel(x, y - fH * 9, Author);
	drawLabel(x, y - fH * 12, Publisher);
	drawLabel(x, y - fH * 15, ReleaseDate);
	drawLabel(x, y - fH * 18, Number);
	drawLabel(x, y - fH * 21, BorrowDate);
}

//�û�ɾ��
void UserDelete() {
    static char input[80] = "";
    static char results[200] = "";
    static int DeleteMode = 0;
    UserMessageList ptr = New(UserMessageList);

    SetPenColor("Black");
    drawLabel(WinWidth / 3, WinHeight / 3 * 2, "��������Ҫע�����û�ID");

    if (textbox(GenUIID(0), WinWidth / 3, WinHeight / 3 * 2 - 3 * GetFontHeight(), WinWidth / 4, 2 * GetFontHeight(), input, sizeof(input)))
        sprintf(results, "%s", input);
    else strcpy(results, input);
    if (button(GenUIID(0), WinWidth / 2, WinHeight / 3 * 2 - 6 * GetFontHeight(), WinWidth / 12, 2 * GetFontHeight(), "ȷ��")) {
        DeleteMode = 1;
    }
    if (DeleteMode) {
        ptr = SearchUser(userlist, results);
        if (ptr != NULL) {
            DeleteUser(userlist, ptr);
        }
        SetPenColor("Black");
        drawLabel(WinWidth / 2, WinHeight / 3 * 2 - 9 * GetFontHeight(), "ɾ���ɹ�");
    }
    DeleteMode = 0;
}

//�û��޸İ�ť
void UserUpdateButtons()
{
    double fH = GetFontHeight();
    double h = fH * 2;  // �ؼ��߶�
    double x = 0;
    double y = WinHeight / 15 * 11;
    double w = WinWidth / 9; // �ؼ����
    static int accordingToName = 0;
    static int accordingToGender = 0;
    static int accordingToWorkUnit = 0;

    if (button(GenUIID(0), x + 3 * h, y, w, h, "�޸�����")) {
        accordingToGender = 0;
        accordingToWorkUnit = 0;
        accordingToName = 1;
    }

    if (button(GenUIID(0), x + 3 * h, y - 1.5 * h, w, h, "�޸��Ա�")) {
        accordingToName = 0;
        accordingToWorkUnit = 0;
        accordingToGender = 1;
    }

    if (button(GenUIID(0), x + 3 * h, y - 3 * h, w, h, "�޸Ĺ�����λ")) {
        accordingToName = 0;
        accordingToGender = 0;
        accordingToWorkUnit = 1;
    }

    if (accordingToName)
        UserUpdate(1);

    if (accordingToGender)
        UserUpdate(2);

    if (accordingToWorkUnit)
        UserUpdate(3);
}

//�û��޸�
void UserUpdate(int n)
{
    FILE *fp;
    static char input[80] = "";//id������
    static char input2[80] = "";//����Ϣ����
    static char results[200] = "";
    static char results2[200] = "";
    static char ID[30] = "";//�ʺ�
    static char Name[30] = "";//����
    static char Gender[20] = "";//�Ա�
    static char WorkUnit[30] = "";//��������
    static int changeMode = 0;
    double fH = GetFontHeight();
    double h = fH * 2; // �ؼ��߶�
    double w = WinWidth / 4; // �ؼ����
    double x = WinWidth / 3 - 1.5 * h;
    double y = WinHeight / 15 * 11;
    UserMessageList ptr = New(UserMessageList);
    UserMessageList p = New(UserMessageList);
    ptr = NULL;
    p = NULL;

    SetPenColor("Black");
    drawLabel(x - fH / 2 - TextStringWidth("����"), y + fH * 0.7, "�û�ID");

    switch (n) {
    case 1: drawLabel(x - fH / 2 - TextStringWidth("���"), y - 1.5 * h + fH * 0.7, "������"); break;
    case 2: drawLabel(x - fH / 2 - TextStringWidth("����"), y - 1.5 * h + fH * 0.7, "���Ա�"); break;
    case 3: drawLabel(x - fH / 2 - TextStringWidth("����"), y - 1.5 * h + fH * 0.7, "�¹�����λ"); break;
    default: break;
    }

    if (textbox(GenUIID(0), x + TextStringWidth("������"), y, w, h, input, sizeof(input)))
        sprintf(results, "%s", input);
    else strcpy(results, input);

    if (textbox(GenUIID(0), x + TextStringWidth("������"), y - 1.5 * h, w, h, input2, sizeof(input2)))
        sprintf(results2, "%s", input2);
    else strcpy(results2, input2);


    if (button(GenUIID(0), x + w / 2 + TextStringWidth("������"), y - h * 3, w / 2, h, "ȷ��")) {
        changeMode = 1;
    }

    if (changeMode) {
        if((fp=fopen("user.txt", "r+"))!=NULL){
			fclose(fp);
            CreateUserList();
            ptr = SearchUser(userlist, results);
            if (ptr == NULL) {
                SetPenColor("Black");
                drawLabel(x, y - fH * 8, "�û������ڣ�");
            }
            else {
                if (n == 1) {
                    strcpy(ptr->Name, results2);
                    p = UpdateUser(userlist, 2, ptr);
                }
                else if (n == 2) {
                    strcpy(ptr->Gender, results2);
                    p = UpdateUser(userlist, 2, ptr);
                }
                else if (n == 3) {
                    strcpy(ptr->WorkUnit, results2);
                    p = UpdateUser(userlist, 2, ptr);
                }
                sprintf(ID, "ID��%s", p->ID);
                sprintf(Name, "������%s", p->Name);
                sprintf(Gender, "�Ա�%s", p->Gender);
                sprintf(WorkUnit, "������λ��%s", p->WorkUnit);
            }
           
        }
        else {
            
            SetPenColor("Black");
            drawLabel(x, y - fH * 8, "�û������ڣ�");
        } 
        
        changeMode = 0;
    }

    y = y - fH * 6;
    drawLabel(x, y - fH * 2, ID);
    drawLabel(x, y - fH * 4, Name);
    drawLabel(x, y - fH * 6, Gender);
    drawLabel(x, y - fH * 8, WorkUnit);
}

//�û�������Ϣ�鿴
void UserMessageShow(){
    FILE *fp,fq;
	static char input[80] = "";//id������
	static char results[200] = "";
	static char ID[30] = "";//�ʺ�
	static char Name[30] = "";//����
	static char Gender[20] = "";//�Ա�
	static char WorkUnit[30] = "";//��������
	static char Password[30] = "";//����
	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double w = WinWidth / 4; // �ؼ����
	double x = WinWidth/3-1.5*h;
    double y = WinHeight/15*11;
	UserMessageList ptr = New(UserMessageList);
	UserMessageList p = New(UserMessageList);
	ptr = NULL;
	p = NULL;

	SetPenColor("Black");
	drawLabel(x-fH/2-TextStringWidth("����"), y+fH*0.7, "�û�ID");
	if (textbox(GenUIID(0), x+TextStringWidth("������"), y, w, h, input, sizeof(input)))
		sprintf(results, "%s", input);
	else strcpy(results, input);
	if (button(GenUIID(0), x + w*1.2 + TextStringWidth("������"), y, w/2, h, "ȷ��")) {
        if((fp=fopen("user.txt", "r+"))==NULL){
            SetPenColor("Black");
            drawLabel(x, y - fH * 8, "�û������ڣ�");
        }
        else {
            fclose(fp);
            CreateUserList();
            ptr = SearchUser(userlist, results);
            if(ptr==NULL){
                SetPenColor("Black");
                drawLabel(x, y - fH * 8, "�û������ڣ�");
            }
            else {
                if((fp=fopen("borrow.txt", "r+"))!=NULL){
					fclose(fp);
                    CreateBorrowList();
                    SearchBR1(borrowlist, results);
                }
                strcpy(UserIDCur, results);
                ShowGUI=8;
            }
        }
	}
}

//�û���ѯ
void UserShow(){
    FILE* fa = NULL;
	FILE *fb=NULL;
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double x = WinWidth/10;
	double y = WinHeight/15*12;
	static char ID[30] = "";//�ʺ�
	static char Name[30] = "";//����
	static char Gender[20] = "";//�Ա�
	static char WorkUnit[30] = "";//��������
	static char Password[30] = "";//����
	static char BookName[30] = "";
	static char BorrowDate[30] = "";
	static char ReturnDue[30] = "";
	static int i=5;
	static char* line, * record;
	static char buffer[1024];
	static char* result = NULL;
	static int j = 0;

	SetPenColor("Black");
	drawLabel(x, y, "ID");
	drawLabel(x, y-h, "����");
	drawLabel(x, y-2*h, "�Ա�");
	drawLabel(x, y-3*h, "������λ");
	drawLabel(x, y-4*h, "�ѽ���");
	drawLabel(x+3*h, y-4*h, "��������");
	drawLabel(x+6*h, y-4*h, "Ӧ������");
	drawMenuBar(0, WinHeight-fH*1.5, WinWidth, fH*1.5);//�˵���
	drawMenuBar(0, 0, WinWidth, fH*3);//��һ��������ʾ��


	if ((fa = fopen("user.txt", "r+")) != NULL)
	{
		fgets(buffer, sizeof(buffer), fa);//������һ�У���һ��Ϊ��ͷ������Ҫ���
		while ((line = fgets(buffer, sizeof(buffer), fa)) != NULL)
		{
			record = strtok(line, "\t");//���Ʊ���ָ�������������
			while (record != NULL)
			{
				switch (j) {
				case 0:strcpy(ID, record); break;
				case 1:strcpy(Name, record); break;
				case 2:strcpy(Gender, record); break;
				case 3:strcpy(WorkUnit, record); break;
				case 4:break;
				case 5:break;
				default:break;
				}
				record = strtok(NULL, "\t");
				j++;
			}
			if(strcmp(ID, UserIDCur)==0)
				break;
			j = 0;
		}
		fclose(fa);
		fa = NULL;
	}

	SetPenColor("Black");
	drawLabel(x+3*h, y, ID);
	drawLabel(x+3*h, y-h, Name);
	drawLabel(x+3*h, y-2*h, Gender);
	drawLabel(x+3*h, y-3*h, WorkUnit);


	j=0;

	if ((fb = fopen("return2.txt", "r+")) != NULL)
	{
		fgets(buffer, sizeof(buffer), fb);
		while ((line = fgets(buffer, sizeof(buffer), fb)) != NULL){
			record = strtok(line, "\t");//���Ʊ���ָ�������������
			while (record != NULL)
			{
				switch (j) {
				case 0:strcpy(BookName, record); break;
				case 1:strcpy(BorrowDate, record); break;
				case 2:strcpy(ReturnDue, record); break;
				default:break;
				}
				record = strtok(NULL, "\t");
				j++;
			}
			SetPenColor("Black");
			drawLabel(x, y-i*h, BookName);
			drawLabel(x+3*h, y-i*h, BorrowDate);
			drawLabel(x+6*h, y-i*h, ReturnDue);
			i++;
			j = 0;
		}
		fclose(fb);
		fb= NULL;
	}
	i=5;

	if(button(GenUIID(0), WinWidth-WinWidth/12-fH, fH*4 , WinWidth/12, h, "����")) {
		remove("return2.txt");
		switch(UserSearchMode){
			case 1:ShowGUI=4;break;
			case 2:ShowGUI=2;break;
			default:break;
		}
	}
}

//ͼ���ܻ�ӭ���а�
void sort0(){
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double x = WinWidth/8;
    double y = WinHeight/15*11;
	static int i=0;
	BookMessageList p=NULL, q=NULL, t=NULL, pre=NULL, head=NULL, tail=NULL, p2=NULL;
	head = booklist;

	while(head->next!=tail){
		pre = head;
		p = head->next;
		while(p->next!=tail){
			if(atoi(p->BorrowTime)<atoi(p->next->BorrowTime)){
				t = p->next;
				pre->next = p->next;
				p->next = p->next->next;
				pre->next->next = p;
				p = t;
			}
			p = p->next;
			pre = pre->next;
		}
		tail = p;
	}

	SetPenColor("Black");
    drawLabel(x, y, "ͼ����");
    drawLabel(x+4*h, y, "����");
    drawLabel(x+8*h, y, "�ۼƽ�������");

    y = y-h;

	p2 = booklist->next;
	while(p2!=NULL){
            drawLabel(x, y-i*h, p2->ID);
            drawLabel(x+4*h, y-i*h, p2->Name);
            drawLabel(x+8*h, y-i*h, p2->BorrowTime);
            i++;
			p2 = p2->next;
	}
    i=0;
}

//�û��������а�
void sort1(){
	double fH = GetFontHeight();
	double h = fH*2;
	double x = WinWidth/12;
    double y = WinHeight/15*11;
	static int i=0;
	UserMessageList p=NULL, q=NULL, t=NULL, pre=NULL, head=NULL, tail=NULL, p2=NULL;
	head = userlist;

	while(head->next!=tail){
		pre = head;
		p = head->next;
		while(p->next!=tail){
			if(atoi(p->BorrowTimeAll)<atoi(p->next->BorrowTimeAll)){
				t = p->next;
				pre->next = p->next;
				p->next = p->next->next;
				pre->next->next = p;
				p = t;
			}
			p = p->next;
			pre = pre->next;
		}
		tail = p;
	}

	SetPenColor("Black");
    drawLabel(x, y, "�û�ID");
    drawLabel(x+4*h, y, "����");
    drawLabel(x+8*h, y, "�Ա�");
    drawLabel(x+12*h, y, "�ܽ�����");

    y = y-h;

	p2 = userlist->next;
	while(p2!=NULL){
            drawLabel(x, y-i*h, p2->ID);
            drawLabel(x+4*h, y-i*h, p2->Name);
            drawLabel(x+8*h, y-i*h, p2->Gender);
            drawLabel(x+12*h, y-i*h, p2->BorrowTimeAll);
            i++;
            p2 = p2->next;
	}
    i=0;
}

//ͼ����������
void sort2(){
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double x = WinWidth/12;
    double y = WinHeight/15*11;
	static int i=0;

	BookMessageList p=NULL, q=NULL, t=NULL, pre=NULL, head=NULL, tail=NULL, p2=NULL;
	head = booklist;

	while(head->next!=tail){
		pre = head;
		p = head->next;
		while(p->next!=tail){
			if((strcmp(p->Name, p->next->Name))>0){
				t = p->next;
				pre->next = p->next;
				p->next = p->next->next;
				pre->next->next = p;
				p = t;
			}
			p = p->next;
			pre = pre->next;
		}
		tail = p;
	}

	SetPenColor("Black");
    drawLabel(x, y, "ͼ����");
    drawLabel(x+3*h, y, "����");
    drawLabel(x+6*h, y, "����");
    drawLabel(x+12*h, y, "�ؼ���");
    drawLabel(x+18*h, y, "������");
    drawLabel(x+22*h, y, "��������");
    y = y-h;
	p2 = booklist->next;
	while(p2!=NULL){
        SetPenColor("Black");
        drawLabel(x, y-i*h, p2->ID);
        drawLabel(x+3*h, y-i*h, p2->Name);
        drawLabel(x+6*h, y-i*h, p2->Author);
        drawLabel(x+12*h, y-i*h, p2->KeyWord);
        drawLabel(x+18*h, y-i*h, p2->Publisher);
        drawLabel(x+22*h, y-i*h, p2->ReleaseDate);
        i++;
        p2=p2->next;
	}
    i=0;
}

//ͼ��ID����
void sort3(){
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double x = WinWidth/12;
    double y = WinHeight/15*11;
	static int i=0;
	BookMessageList p=NULL, q=NULL, t=NULL, pre=NULL, head=NULL, tail=NULL, p2=NULL;
	head = booklist;

	while(head->next!=tail){
		pre = head;
		p = head->next;
		while(p->next!=tail){
			if((strcmp(p->ID, p->next->ID))>0){
				t = p->next;
				pre->next = p->next;
				p->next = p->next->next;
				pre->next->next = p;
				p = t;
			}
			p = p->next;
			pre = pre->next;
		}
		tail = p;
	}
	SetPenColor("Black");
    drawLabel(x, y, "ͼ����");
    drawLabel(x+3*h, y, "����");
    drawLabel(x+6*h, y, "����");
    drawLabel(x+12*h, y, "�ؼ���");
    drawLabel(x+18*h, y, "������");
    drawLabel(x+22*h, y, "��������");
    y = y-h;
	p2 = booklist->next;
	while(p2!=NULL){
        SetPenColor("Black");
        drawLabel(x, y-i*h, p2->ID);
        drawLabel(x+3*h, y-i*h, p2->Name);
        drawLabel(x+6*h, y-i*h, p2->Author);
        drawLabel(x+12*h, y-i*h, p2->KeyWord);
        drawLabel(x+18*h, y-i*h, p2->Publisher);
        drawLabel(x+22*h, y-i*h, p2->ReleaseDate);
        i++;
        p2=p2->next;
	}
    i=0;
}

//�û���������
void sort4(){
	double fH = GetFontHeight();
	double h = fH*2;
	double x = WinWidth/12;
    double y = WinHeight/15*11;
	static int i=0;

	UserMessageList p=NULL, q=NULL, t=NULL, pre=NULL, head=NULL, tail=NULL, p2=NULL;
	head = userlist;

	while(head->next!=tail){
		pre = head;
		p = head->next;
		while(p->next!=tail){
			if((strcmp(p->Name, p->next->Name))>0){
				t = p->next;
				pre->next = p->next;
				p->next = p->next->next;
				pre->next->next = p;
				p = t;
			}
			p = p->next;
			pre = pre->next;
		}
		tail = p;
	}
	SetPenColor("Black");
    drawLabel(x, y, "�û�ID");
    drawLabel(x+4*h, y, "����");
    drawLabel(x+8*h, y, "�Ա�");
    drawLabel(x+12*h, y, "������λ");

    y = y-h;

	p2 = userlist->next;
	while(p2!=NULL){
        SetPenColor("Black");
        drawLabel(x, y-i*h, p2->ID);
        drawLabel(x+4*h, y-i*h, p2->Name);
        drawLabel(x+8*h, y-i*h, p2->Gender);
        drawLabel(x+12*h, y-i*h, p2->WorkUnit);
        i++;
        p2=p2->next;
	}
    i=0;
}

//�û�ID����
void sort5(){
	double fH = GetFontHeight();
	double h = fH*2;
	double x = WinWidth/12;
    double y = WinHeight/15*11;
	static int i=0;
	UserMessageList p=NULL, q=NULL, t=NULL, pre=NULL, head=NULL, tail=NULL, p2=NULL;
	head = userlist;

	while(head->next!=tail){
		pre = head;
		p = head->next;
		while(p->next!=tail){
			if((strcmp(p->ID, p->next->ID))>0){
				t = p->next;
				pre->next = p->next;
				p->next = p->next->next;
				pre->next->next = p;
				p = t;
			}
			p = p->next;
			pre = pre->next;
		}
		tail = p;
	}

	SetPenColor("Black");
    drawLabel(x, y, "�û�ID");
    drawLabel(x+4*h, y, "����");
    drawLabel(x+8*h, y, "�Ա�");
    drawLabel(x+12*h, y, "������λ");

    y = y-h;

	p2 = userlist->next;
	while(p2!=NULL){
        SetPenColor("Black");
        drawLabel(x, y-i*h, p2->ID);
        drawLabel(x+4*h, y-i*h, p2->Name);
        drawLabel(x+8*h, y-i*h, p2->Gender);
        drawLabel(x+12*h, y-i*h, p2->WorkUnit);
        i++;
        p2=p2->next;
	}
    i=0;
}

void DrawShowAbout() {
    double fH = GetFontHeight();//�ָ�
    double w = WinWidth / 9;//�ؼ����
    double h = fH * 2;//�ı���߶�
    double x = WinWidth / 5 * 2;
    double y = WinHeight / 5 * 3;
    SetPenColor("Black");
    drawLabel(x - 12 * fH, y + 3 * fH, "���������Ϊͼ�����ϵͳ��Ϊ2020���㽭��ѧ�������ר��γ̴���ҵ��");
    drawLabel(x - 14 * fH, y + 1.5 * fH, "�������Ϊ������棬���ȴ򿪳��򽫽��������棬�����������ѡ�������");
    drawLabel(x - 14 * fH, y, "�͡��û�������Ա�������档���οͽ����У����Խ���ͼ���ѯ���Լ��鿴��");
    drawLabel(x - 14 * fH, y - 1.5 * fH, "�ܻ�ӭͼ�顣���û������У�����ע���ʺš��޸ĸ�����Ϣ����ѯͼ�顢�軹");
    drawLabel(x - 14 * fH, y - 3 * fH, "�顣�ڹ���Ա�����У����Զ��ļ����в����������޸ġ�������ɾ��ͼ�飬��");
    drawLabel(x - 14 * fH, y - 4.5 * fH, "�Բ�ѯ�û���Ϣ�����Խ���������������Կ��û��Ľ������а��Լ��鱾����");
    drawLabel(x - 14 * fH, y - 6 * fH, "���������а�");
}

void DrawShowUseWay() {
    double fH = GetFontHeight();//�ָ�
    double w = WinWidth / 9;//�ؼ����
    double h = fH * 2;//�ı���߶�
    double x = WinWidth / 5 * 2;
    double y = WinHeight / 5 * 3;
    SetPenColor("Black");
    drawLabel(x - 14 * fH, y + 3 * fH, "ע������1������������ɹ���Ա��ͼ����ļ�����ܽ��в�����");
    drawLabel(x - 14 * fH, y + 1.5 * fH, "ע������2������Ա��½�˺�������Ԥ�裬������д��ȷ�ſ���ͨ������Ա�����¼��");
    drawLabel(x - 14 * fH, y, "ע������3���û�ע��ʱ��ע���ű���Ϊʮλ������������ע��ʧ�ܡ�");
    drawLabel(x - 14 * fH, y - 1.5 * fH, "ע������4������ͼ��ʱ��ע���ű���Ϊ��λ����������������ʧ�ܡ�");
    drawLabel(x - 14 * fH, y - 3 * fH, "ע������4��ÿλ�û�����ͬʱ��������ͼ�顣");
    drawLabel(x - 14 * fH, y - 4.5 * fH, "ע������5����ͼ������޿��ʱ�����齫���ᱻ�����");
}