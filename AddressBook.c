#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE_NAME "Address.dat"

void ReleaseList();

//링크드리스트 노드
typedef struct _USERDATA
{
	char szName[32];
	char szPhone[32];

	struct _USERDATA* pNext;
}USERDATA;

//헤드 더미노드
USERDATA g_head = {0};

//이름으로 찾기
USERDATA* FindNode(char* pszName)
{
	USERDATA* pTmp = g_head.pNext;
	while (pTmp != NULL) {
		if (strcmp(pTmp->szName, pszName) == 0) return pTmp;

		pTmp = pTmp->pNext;
	}
	return NULL;
}

//새 노드 추가하기
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

//입력받아서 주소록에 추가하기
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

//이름 입력받고 노드 찾아서 출력하기
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
	else puts("ERROR : 데이터를 찾을 수 없습니다.");

	_getch();
}

//리스트에 있는 모든 데이터 출력하기
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

//이름으로 노드를 검색하고 삭제하기
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

//이름을 입력받아 자료를 검색하고 삭제하기
void Remove()
{
	char szName[32] = { 0 };

	printf("Input name : ");
	while(getchar()!='\n');
	gets_s(szName, sizeof(szName));

	RemoveNode(szName);
}

//메뉴를 출력하는 UI 함수
int PrintUI()
{
	int nInput = 0;

	system("cls");
	printf("[1] Add\t [2] Search\t [3] Print\t [4] Remove\t [0] Exit\n");

	scanf_s("%d", &nInput);
	return nInput;
}

//데이터파일에서 노드들을 읽어와 리스트를 만드는 함수
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

//리스트 형태로 존재하는 정보를 파일에 저장하는 함수
int SaveList(char* pszFileName)
{
	FILE* fp = NULL;
	USERDATA* pTmp = g_head.pNext;
	fopen_s(&fp, pszFileName, "wb");

	if (fp == NULL) {
		puts("ERROR : 리스트 파일을 쓰기 모드로 열지 못했습니다.");
		_getch();
		return 0;
	}

	while (pTmp != NULL) {
		if (fwrite(pTmp, sizeof(USERDATA), 1, fp) != 1)
			printf("ERROR : %s에 대한 정보를 저장하는 데 실패했습니다.\n", pTmp->szName);
		pTmp = pTmp->pNext;
	}
	fclose(fp);

	return 1;
}

//리스트의 모든 데이터를 삭제하는 함수
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

	//메인이벤트 반복문
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

	//종료 전에 파일로 저장하고 메모리 해제
	SaveList(DATA_FILE_NAME);
	ReleaseList();
}