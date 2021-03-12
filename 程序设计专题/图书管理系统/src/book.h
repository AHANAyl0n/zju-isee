#ifndef BOOK_H
#define BOOK_H

//ͼ����Ϣ����
typedef struct BookMessage {
    char ID[30];//ͼ����
    char Name[30];//����
    char Author[50];//����
    char KeyWord[50];//�ؼ���
    char Publisher[30];//������
    char ReleaseDate[30];//��������
    char NumberAll[5];//ȫ������
    char Number[5];//��ǰʣ������
    char BorrowTime[5];//�����Ĵ���
    struct BookMessage* next;//��һ����
} *BookMessageList;

extern BookMessageList booklist;//ȫ�ֱ�������
extern char FileNameCur[30];

//����һ����ͷ����ͼ������
BookMessageList NewBookList(void);

//��ȷ����ͼ��
BookMessageList SearchBook0(BookMessageList head, int choice, char* message);

//ģ������ͼ��
BookMessageList SearchBook1(BookMessageList head, int choice, char* message);

//����ͼ��
BookMessageList InsertBook(BookMessageList head, BookMessageList nodeptr, BookMessageList obj);

//����ͼ������
void CreateBookList(char *FileName);

//�޸�ͼ����Ϣ
BookMessageList UpdateBook(BookMessageList head, int n, BookMessageList p);

//ɾ��ͼ��
BookMessageList DeleteBook(BookMessageList head, BookMessageList p);

#endif