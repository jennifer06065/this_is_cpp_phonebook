#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE_NAME "Address.dat"

void ReleaseList();

//��ũ�帮��Ʈ ���
typedef struct _USERDATA
{
	char szName[32];
	char szPhone[32];

	struct _USERDATA* pNext;
}USERDATA;

//��� ���̳��
USERDATA g_head = {0};

//�̸����� ã��
USERDATA* FindNode(char* pszName)
{
	USERDATA* pTmp = g_head.pNext;
	while (pTmp != NULL) {
		if (strcmp(pTmp->szName, pszName) == 0) return pTmp;

		pTmp = pTmp->pNext;
	}
	return NULL;
}

//�� ��� �߰��ϱ�
int AddNewNode(char* pszName, char* pszPhone) {
	USERDATA* pNewUser = NULL;

	if (FindNode(pszName) != NULL) return 0;

	pNewUser = (USERDATA*)malloc(sizeof(USERDATA));
	memset(pNewUser, 0, sizeof(USERDATA));

	sprintf_s(pNewUser->szName, sizeof(pNewUser->szName), "%s", pszName);
	sprintf_s(pNewUser->szPhone, sizeof(pNewUser->szPhone), "%s", pszPhone);
	pNewUser->pNext = g_head.pNext;
	g_head.pNext = pNewUser;

	return 1;
}

//�Է¹޾Ƽ� �ּҷϿ� �߰��ϱ�
void Add()
{
	char szName[32] = { 0 };
	char szPhone[32] = { 0 };

	printf("Input name : ");
	while (getchar() != '\n');
	gets_s(szName, sizeof(szName));

	printf("Input phone number : ");
	while (getchar() != '\n');
	gets_s(szPhone, sizeof(szPhone));

	AddNewNode(szName, szPhone);
}

//�̸� �Է¹ް� ��� ã�Ƽ� ����ϱ�
void Search()
{
	char szName[32] = { 0 };
	USERDATA* pNode = NULL;

	printf("Input name : ");
	while (getchar() != '\n');
	gets_s(szName, sizeof(szName));

	pNode = FindNode(szName);

	if (pNode != NULL) {
		printf("[%p] %s\t%s [%p]",
			pNode,
			pNode->szName,
			pNode->szPhone,
			pNode->pNext);
	}
	else puts("ERROR : �����͸� ã�� �� �����ϴ�.");

	_getch();
}

//����Ʈ�� �ִ� ��� ������ ����ϱ�
void PrintAll()
{
	USERDATA* pTmp = g_head.pNext;
	while (pTmp != NULL) {
		printf("[%p] %s\t%s [%p]",
			pTmp,
			pTmp->szName,
			pTmp->szPhone,
			pTmp->pNext);
		pTmp = pTmp->pNext;
	}

	_getch();
}

//�̸����� ��带 �˻��ϰ� �����ϱ�
int RemoveNode(char* pszName)
{
	USERDATA* pPrevNode = &g_head;
	USERDATA* pDelete = NULL;

	while (pPrevNode->pNext != NULL) {
		pDelete = pPrevNode->pNext;

		if (strcmp(pDelete->szName, pszName) == 0) {
			pPrevNode->pNext = pDelete->pNext;
			free(pDelete);

			return 1;
		}
		pPrevNode = pPrevNode->pNext;
	}
	return 0;
}

//�̸��� �Է¹޾� �ڷḦ �˻��ϰ� �����ϱ�
void Remove()
{
	char szName[32] = { 0 };

	printf("Input name : ");
	while(getchar()!='\n');
	gets_s(szName, sizeof(szName));

	RemoveNode(szName);
}

//�޴��� ����ϴ� UI �Լ�
int PrintUI()
{
	int nInput = 0;

	system("cls");
	printf("[1] Add\t [2] Search\t [3] Print\t [4] Remove\t [0] Exit\n");

	scanf_s("%d", &nInput);
	return nInput;
}

//���������Ͽ��� ������ �о�� ����Ʈ�� ����� �Լ�
int LoadList(char* pszFileName)
{
	FILE* fp = NULL;
	USERDATA user = { 0 };

	fopen_s(&fp, pszFileName, "rb");

	if (fp == NULL) return 0;

	ReleaseList();

	while (fread(&user, sizeof(USERDATA), 1, fp))
		AddNewNode(user.szName, user.szPhone);
	fclose(fp);

	return 0;
}

//����Ʈ ���·� �����ϴ� ������ ���Ͽ� �����ϴ� �Լ�
int SaveList(char* pszFileName)
{
	FILE* fp = NULL;
	USERDATA* pTmp = g_head.pNext;
	fopen_s(&fp, pszFileName, "wb");

	if (fp == NULL) {
		puts("ERROR : ����Ʈ ������ ���� ���� ���� ���߽��ϴ�.");
		_getch();
		return 0;
	}

	while (pTmp != NULL) {
		if (fwrite(pTmp, sizeof(USERDATA), 1, fp) != 1)
			printf("ERROR : %s�� ���� ������ �����ϴ� �� �����߽��ϴ�.\n", pTmp->szName);
		pTmp = pTmp->pNext;
	}
	fclose(fp);

	return 1;
}

//����Ʈ�� ��� �����͸� �����ϴ� �Լ�
void ReleaseList()
{
	USERDATA* pTmp = g_head.pNext;
	USERDATA* pDelete = NULL;

	while (pTmp != NULL) {
		pDelete = pTmp;
		pTmp = pTmp->pNext;
		free(pDelete);
	}

	memset(&g_head, 0, sizeof(USERDATA));
}

//main
void main() {
	int nMenu = 0;
	LoadList(DATA_FILE_NAME);

	//�����̺�Ʈ �ݺ���
	while ((nMenu = PrintUI()) != 0) {
		switch (nMenu) {
		case 1: //Add
			Add();
			break;
		case 2: //Search
			Search();
			break;
		case 3: //Print all
			PrintAll();
			break;
		case 4: //Remove
			Remove();
			break;
		}
	}

	//���� ���� ���Ϸ� �����ϰ� �޸� ����
	SaveList(DATA_FILE_NAME);
	ReleaseList();
}