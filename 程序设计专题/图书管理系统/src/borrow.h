#ifndef BORROW_H
#define BORROW_H

//ͼ���û���ϵ��
typedef struct BorrowMessage {
    char UserID[30];//������ID
    char BookID[30];//ͼ����
    char BookName[30];//����
    char BorrowDate[30];//��������
    char ReturnDue[30];//Ӧ������
    struct Borrow* next;//��һ����Ӧ��ϵ
} *BorrowMessageList;

extern BorrowMessageList borrowlist;

BorrowMessageList NewBorrowList(void);
BorrowMessageList InsertBR(BorrowMessageList head, BorrowMessageList nodeptr, BorrowMessageList obj);
BorrowMessageList SearchBR(BorrowMessageList head, char *message1, char *message2);
BorrowMessageList SearchBR1(BorrowMessageList head, char *message);
void CreateBorrowList();
BorrowMessageList DeleteBR(BorrowMessageList head, BorrowMessageList p);

#endif