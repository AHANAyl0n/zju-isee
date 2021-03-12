#ifndef USER_H
#define USER_H


//�û���Ϣ�ṹ
typedef struct UserMessage {
    char ID[30];//�û��˺�
    char Name[30];//�û�����
    char Gender[20];//�Ա�
    char WorkUnit[20];//������λ
    char BorrowTimeAll[10];//�ܽ�����
    char BorrowTimeNow[10];//��ǰ������
    struct UserMessage* next;
} *UserMessageList;


extern UserMessageList userlist;


//����һ����ͷ�����û�����
UserMessageList NewUserList(void);

//�����û�
UserMessageList SearchUser(UserMessageList head, char* userid);


//�����û�
UserMessageList AddUser(UserMessageList head, UserMessageList nodeptr, UserMessageList obj);

//�����û�����
void CreateUserList();

//�޸��û���Ϣ�������û�����ͷָ�룻
UserMessageList UpdateUser(UserMessageList head,int n, UserMessageList p);

//ɾ���û�,�����û�����ͷָ��
UserMessageList DeleteUser(UserMessageList head, UserMessageList p);
#endif