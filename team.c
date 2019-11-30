#define _CRT_SECURE_NO_WARNINGS
#define x_IdPasswd 20
#define y_IdPasswd 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
int login();
int join();
void scan_box(int x, int y, int size, char scan_[]);//입력하는 위치로 이동해 크기만큼 입력 받는 함수
void scan_box_passwd(int x, int y, int size, char scan_passwd[]);//*을 출력하며 입력 받는 함수
void blank(int x, int y, int size);//입력 빈칸을 만들어 주는 함수
void gotoxy(int x, int y);//콘솔에서 x,y로 이동하는 함수
void setblack();
void setColor(unsigned short text, unsigned short back);
void file_subjects(char* txt, char subject[][2][100]);
int subject_count(char* txt);
int open_subject(char subject_all[][2][100]);


int session = 0;
char id_r[50], id_w[50];
int pro_or_stu;
char passwd[20];
int pw_count;

typedef struct _relative
{
	double average;//평균
	int students;//학생수
	double standard;//표준편차
} relative;

typedef struct _absolute
{
	int attend;//출석점수
	int midterm;//중간고사
	int final;//기말고사
	int quiz[5];//퀴즈
	int score[9];//ABCDF분할 기준
}absolute;

typedef struct _SUBJECT
{
	char sub_name[100], pro_name[50];
	int how;
	absolute abs;
	relative rel;
} SUBJECT;

SUBJECT subject;

int subject_count(char *txt) {
	FILE* fp;
	fp = fopen(txt, "r");
	char tmp;
	int cnt = 1;
	if (fp == NULL) {
		exit(0);
	}
	while (fscanf(fp, "%c", &tmp) != EOF) {
		if (tmp == '\n')
			cnt++;
	}
	fclose(fp);
	return cnt;
}

int open_subject(char subject_all[][2][100]) {
	int i;
	FILE* fp = fopen("all_subject.txt", "r");

	if (fp == NULL)
	{
		printf("파일 오픈 실패");
		return -1;
	}
		
	for (i = 0; i < subject_count("all_subject.txt"); i++)
	{
		fscanf(fp, "%s %s", subject_all[i][0], subject_all[i][1]);
	}
	fclose(fp);
	return 0;
}


void file_subjects(char* txt, char subject[][2][100])
{
	FILE* fp = fopen(txt, "r");
	fseek(fp, pw_count, SEEK_SET);
	for (int i =0; i < subject_count(txt)-1; i++)
	{
		fscanf(fp, "%s %s", subject[i][0],subject[i][1]);
	}
}
void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setColor(unsigned short text, unsigned short back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}
void setblack()
{
	setColor(7, 0);
}

void blank(int x, int y, int size)
{
	gotoxy(x, y);
	setColor(0, 7);
	for (int i = 0; i < size; i++)
		printf(" ");
	setblack();
}

void scan_box(int x, int y, int size, char scan_[]) {
	int i = 0;
	setColor(0, 7);
	gotoxy(x, y);
	while (1)
	{
		if (_kbhit())
		{
			scan_[i] = _getch();
			if (scan_[i] == 27) {
				session--;
				break;
			}
			else if (scan_[i] == 8)
			{
				if (i > 0)
				{
					printf("\b \b");
					i--;
				}
				continue;

			}
			else if (scan_[i] == 13 || scan_[i] == 9)
			{
				printf("\b");
				break;
			}
			else if (i >= size)
			{
				continue;
			}
			printf("%c", scan_[i]);
			i++;
		}
	}
	scan_[i] = '\0';
	setblack();
}
void scan_box_passwd(int x, int y, int size, char scan_passwd[]) {
	int i = 0;
	setColor(0, 7);
	gotoxy(x, y);
	while (1)
	{
		if (_kbhit())
		{
			scan_passwd[i] = _getch();
			if (scan_passwd[i] == 27) {
				session--;
				break;
			}         if (scan_passwd[i] == 8)
			{
				if (i > 0)
				{
					printf("\b \b");
					i--;
				}
				continue;
			}
			else if (scan_passwd[i] == 13 || scan_passwd[i] == 9)
			{
				break;
			}
			if (i >= size)
				continue;

			i++;
			printf("*");
		}
	}
	scan_passwd[i] = '\0';
	setblack();
}

int join()
{
	session++;
pass1:   //아이디가 중복될 경우
	system("cls");
	printf("회원 가입");
	char pw_w[20];
	char temp[50];

	//아이디, 비밀번호, 비밀번호 체크란 출력
	for (int y = 0; y < 4; y += 2)
		blank(x_IdPasswd, y_IdPasswd + y, 16);
	gotoxy(x_IdPasswd - 20, y_IdPasswd); printf("%17s", "id :");
	gotoxy(x_IdPasswd - 20, y_IdPasswd + 2); printf("%17s", "password :");

	scan_box(x_IdPasswd, y_IdPasswd, 16, id_w);
	if (session == 0)
		return -1;
	scan_box_passwd(x_IdPasswd, y_IdPasswd + 2, 16, pw_w);
	if (session == 0)
		return -1;

	if (pro_or_stu == 1)
		strcpy(temp, "professor\\");
	else if (pro_or_stu == 2)
		strcpy(temp, "student\\");
	strcat(temp, id_w);
	strcat(temp, ".txt");
	strcpy(id_w, temp);
	//아이디 중복확인
	gotoxy(x_IdPasswd + 5, y_IdPasswd + 4);
	setColor(4, 0);
	if (fopen(id_w, "r") != NULL)
	{
		printf("이미 존재하는 아이디입니다.");
		Sleep(1000);
		goto pass1;
	}
	else
	{
		setblack();
		FILE* fID = fopen(id_w, "w+");         // PW저장파일 생성
		//암호화
		int i = 0;
		while (pw_w[i] != NULL)
		{
			pw_w[i] += 34;
			i++;
		}
		fprintf(fID, "%s", pw_w);
		printf("회원 가입이 완료되었습니다.");
		Sleep(1000);
		fclose(fID);
		session--;
		return 0;
	}
}


int login() {
	session++;
	system("cls");
	printf("로그인");
	char passwd_r[20];

	gotoxy(x_IdPasswd, y_IdPasswd); printf("%10s", "id :");
	blank(x_IdPasswd + 10, y_IdPasswd, 16);
	gotoxy(x_IdPasswd, y_IdPasswd + 2); printf("%10s", "password :");
	blank(x_IdPasswd + 10, y_IdPasswd + 2, 16);


	scan_box(x_IdPasswd + 10, y_IdPasswd, 16, id_r);
	if (session == 0)
		return -1;
	scan_box_passwd(x_IdPasswd + 10, y_IdPasswd + 2, 16, passwd_r);
	if (session == 0)
		return -1;
	char temp[70];

	if (pro_or_stu == 1)
		strcpy(temp, "professor\\");
	else if (pro_or_stu == 2)
		strcpy(temp, "student\\");

	strcat(temp, id_r);
	strcat(temp, ".txt");
	strcpy(id_r, temp);


	FILE* fp = fopen(id_r, "r");

	gotoxy(x_IdPasswd + 5, y_IdPasswd + 4);
	setColor(4, 0);
	
	if (fp != NULL) {
		fscanf(fp, "%s", passwd);
		//복호화
		pw_count = 0;
		while (passwd[pw_count] != NULL)
		{
			passwd[pw_count] -= 34;
			pw_count++;
		}


		if (strcmp(passwd, passwd_r) == 0) {
			setblack();
			printf("로그인 성공");
			Sleep(1000);
			return 0;

		}
		else {
			session--;
			printf("비밀번호가 틀렸습니다.");
			setblack();
			Sleep(1000);

			return 1;
		}
	}
	else {
		session--;
		printf("존재하지 않는 아이디입니다.");
		setblack();
		Sleep(1000);
		return 1;
	}

}

int main(void)
{
	system("mode con cols=100 lines=20");
	int num,number, score_wish, i, score;
	double score_input[7], quiz_score_total=0;
	int selection;
	double std; // 기준
	double z;//z점수
	
	char subject_mine[10][2][100];
	char subject_allname[50][2][100];
	

	while (1) {
		system("cls");
		
		printf("1. 귀하의 직책은 무엇입니까? (1. 교수  2. 학생)  :"); scanf("%d", &pro_or_stu);
		if (pro_or_stu == 1)
			printf("\n  반갑습니다! 교수를 선택하셨습니다.\n\n");
		else if (pro_or_stu == 2)
			printf("\n  환영합니다! 학생을 선택하셨습니다.\n\n");
		Sleep(1000);
		system("cls");
		printf("1.무엇을 하시겠습니까?(1.로그인 2.회원가입) :"); scanf("%d", &selection);

		if (selection == 1) {
			while (1)
			{
				if (login() == 1) {
					continue;
				}
				else
					break;
			}
		}
		else if (selection == 2)
		{
			join();
		}
		if (session == 1)
			break;
	}
	while (1)
	{
		system("cls");
		printf("====================\n");
		file_subjects(id_r, subject_mine);
		for (int i = 0; i < subject_count(id_r) - 1; i++) {
			printf("%d. %s %s\n",i+1, subject_mine[i][0], subject_mine[i][1]);
		}
		printf("====================\n");

		if (pro_or_stu == 1)//교수 방식
		{//strcat

			printf("====================\n");
			printf("=    1.과목추가     =\n");
			printf("=    2.과목삭제     =\n");
			printf("=    3.종료하기     =\n");
			printf("====================\n");
			printf("원하는 항목을 입력하세요:");
			scanf("%d", &selection);
			switch (selection) {
			case 1:
				printf("과목이름을 입력하세요:"); //두번 저장
				char temp[70] = "subject\\";
				scanf("%s", subject.sub_name);
				printf("교수이름을 입력하세요:");
				scanf("%s", subject.pro_name);

				strcat(temp, subject.sub_name);
				strcat(temp, subject.pro_name);
				strcat(temp, ".txt");

				FILE* fp = fopen(temp, "a");
				FILE* fall = fopen("all_subject.txt", "a");
				FILE* fid = fopen(id_r, "a");

				fprintf(fid, "\n%s %s", subject.sub_name, subject.pro_name);//id에 저장
				fprintf(fall, "%s %s\n", subject.sub_name, subject.pro_name);//전체 과목 목록에 저장

				printf("절대평가면 0을 상대평가면 1을 입력하세요:");
				scanf("%d", &subject.how);
				fprintf(fp, "%d\n", subject.how);
				if (subject.how == 0) {//수정해야함
					printf("출석점수 비율을 입력하세요:");
					scanf("%d", &subject.abs.attend);
					fprintf(fp, "%d\n", subject.abs.attend);

					printf("중간고사 비율을 입력하세요:");
					scanf("%d", &subject.abs.midterm);
					fprintf(fp, "%d\n", subject.abs.midterm);

					printf("기말고사 비율을 입력하세요:");
					scanf("%d", &subject.abs.final);
					fprintf(fp, "%d\n", subject.abs.final);

					printf("과제/퀴즈 개수를 입력하세요:");
					scanf("%d", &num);
					for (int i = 0; i < 5; i++) {
						if (i >= num)
						{
							fprintf(fp, "%d\n", 0);
							continue;
						}
						printf("과제/퀴즈 %d의 비율을 입력하세요", i + 1);
						scanf("%d", &subject.abs.quiz[i]);
						fprintf(fp, "%d\n", subject.abs.quiz[i]);

					}
					//밑에 모두 &붙일것
					printf("A+는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[0]);
					fprintf(fp, "%d\n", subject.abs.score[0]);
					printf("A0는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[1]);
					fprintf(fp, "%d\n", subject.abs.score[1]);
					printf("B+는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[2]);
					fprintf(fp, "%d\n", subject.abs.score[2]);
					printf("B0는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[3]);
					fprintf(fp, "%d\n", subject.abs.score[3]);
					printf("C+는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[4]);
					fprintf(fp, "%d\n", subject.abs.score[4]);
					printf("C0는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[5]);
					fprintf(fp, "%d\n", subject.abs.score[5]);
					printf("D+는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[6]);
					fprintf(fp, "%d\n", subject.abs.score[6]);
					printf("D0는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[7]);
					fprintf(fp, "%d\n", subject.abs.score[7]);
					printf("F는 몇점까지입니까?");
					scanf("%d", &subject.abs.score[8]);
					fprintf(fp, "%d\n", subject.abs.score[8]);
				}
				else if (subject.how == 1) {
					printf("전체평균점수를 입력하세요:");
					scanf("%lf", &subject.rel.average);
					fprintf(fp, "%lf\n", subject.rel.average);

					printf("학생수를 입력하세요:");
					scanf("%d", &subject.rel.students);
					fprintf(fp, "%d\n", subject.rel.students);

					printf("표준편차를 입력하세요:");
					scanf("%lf", &subject.rel.standard);
					fprintf(fp, "%lf\n", subject.rel.standard);
				}
				fclose(fall);
				fclose(fp);
				break;
			case 2:
				file_subjects(id_r, subject_mine);
				printf("삭제하고 싶으신 과목의 번호를 입력하십시오: ");
				int sub_number, del_count;
				char pass[30];
				scanf("%d", &sub_number);

				del_count = subject_count(id_r) - 1;
				fid = fopen(id_r, "r");
				fscanf(fid, "%s", pass);
				fid = fopen(id_r, "w");
				fprintf(fid, "%s", pass);

				for (int i = 0; i < del_count; i++)
				{
					if (i==sub_number-1)
						continue;
					fprintf(fid, "\n%s %s", subject_mine[i][0], subject_mine[i][1]);
				}
				fclose(fid);
				break;
			case 3:
				exit(0);
				break;
			}
		}
		else if (pro_or_stu == 2)//학생 방식
		{
			printf("====================\n");
			printf("=    1.과목추가     =\n");//교수님들의 모든 목록중에 고르기
			printf("=    2.과목삭제     =\n");
			printf("=    3.예상학점분석 =\n");
			printf("=    4.종료하기     =\n");
			printf("====================\n");
			printf("원하는 항목을 입력하세요:");
			scanf("%d", &selection);
			switch (selection)
			{
			case 1:
				////과목선택 - 보여주는 함수 사용할 것 
				open_subject(subject_allname);
				int select_sub;
				for (int i = 0; i < subject_count("all_subject.txt")-1; i++)
					printf("%d. %s %s\n", i + 1, subject_allname[i][0], subject_allname[i][1]);
				scanf("%d",&select_sub);
				FILE* fid = fopen(id_r, "a");
				fprintf(fid, "\n%s %s", subject_allname[select_sub - 1][0], subject_allname[select_sub - 1][1]);
				break;
			case 2:
				file_subjects(id_r, subject_mine);
				printf("삭제하고 싶으신 과목의 번호를 입력하십시오: ");
				int sub_number, del_count;
				char pass[30];
				scanf("%d", &sub_number);

				del_count = subject_count(id_r) - 1;
				fid = fopen(id_r, "r");
				fscanf(fid, "%s", pass);
				fid = fopen(id_r, "w");
				fprintf(fid, "%s", pass);

				for (int i = 0; i < del_count; i++)
				{
					if (i == sub_number - 1)
						continue;
					fprintf(fid, "\n%s %s", subject_mine[i][0], subject_mine[i][1]);
				}
				fclose(fid);
				break;
			case 3://예상학점분석 - 
				//과목 선택하는 것 추가 해야함
				file_subjects(id_r, subject_mine);
				//printf("예상학점을 입력하세요. (정수로 입력하세요.)");
				printf("학점을 알고싶은 과목번호를 입력하시오:");
				scanf("%d", &number);//변수바꾸기
				char temp[80]="subject\\";
				strcat(temp, subject_mine[number - 1][0]);
				strcat(temp, subject_mine[number - 1][1]);
				strcat(temp, ".txt");
				FILE* fsub = fopen(temp, "r");
				if (fsub == NULL)
				{
					printf("파일 오픈 실패");
					return -1;
				}
				fscanf(fsub, "%d\n",&subject.how);
				switch (subject.how) {//변수바꾸기
				case 0://절대평가
					fscanf(fsub, "%d\n%d\n%d\n",&subject.abs.attend,&subject.abs.midterm,&subject.abs.final);
					for (int i = 0; i < 5; i++)
						fscanf(fsub, "%d\n", &subject.abs.quiz[i]);
					for (int i = 0; i < 9; i++)
						fscanf(fsub, "%d\n", &subject.abs.score[i]);
					printf("중간고사 점수는 몇 점입니까?(100점 만점 기준)");
					scanf("%lf", &score_input[0]);
					printf("예상하는 출석 점수는 몇 점입니까?(100점 만점 기준)");
					scanf("%lf", &score_input[1]);
					for (i = 0; i < 5; i++)
					{
						printf("예상하는 과제(퀴즈)%d는 몇 점 입니까?(100점 만점 기준)",i+1);
						scanf("%lf", &score_input[2 + i]);
					}
					printf("원하는 학점은 몇 점 입니까? (숫자 기준 예. A: 4.0, B+:3.5)");
					scanf("%lf", &score_wish);

					if (score_wish == 4.5)//A+
						std = subject.abs.score[0];
					else if (score_wish == 4.0)//A0
						std = subject.abs.score[1];
					else if (score_wish == 3.5)//B+
						std = subject.abs.score[2];
					else if (score_wish == 3.0)//B0
						std = subject.abs.score[3];
					else if (score_wish == 2.5)//C+
						std = subject.abs.score[4];
					else if (score_wish == 2.0)//C0
						std = subject.abs.score[5];
					else if (score_wish == 1.5)//D+
						std = subject.abs.score[6];
					else if (score_wish == 1.0)//D0
						std = subject.abs.score[7];
					else//F
						std = subject.abs.score[8];


					for (i = 0; i < subject_count("all_subject.txt"); i++)
					{
						score_input[2 + i] *= subject.abs.quiz[i];
						quiz_score_total += score_input[2 + i];
					}
					score = (subject.abs.midterm * score_input[0]) + (subject.abs.attend * score_input[1]) + quiz_score_total;
					
					if (score >= std) {
						setColor(6, 0);
						printf("충분히 가능 합니다! 꾸준히 노력하세요!");
						setblack();
					}
					else {
						setColor(4, 0);
						printf("현실과 이상은 조금은 다르기 마련이죠. 그래도 포기하지 말고 꾸준히 노력하면 좋은 결과가 나올 수 있을 거에요!");
						setblack();
					}

					Sleep(5000);
					break;


				case 1://상대평가
					fscanf(fsub, "%f\n%d\n%f", &subject.rel.average, &subject.rel.students, &subject.rel.standard);

					z = (score_input[0] - subject.rel.average) / subject.rel.standard;

					if (z > 0.0793)
					{
						setColor(6, 0);
						printf("상위 30%에 들어갈 확률이 높습니다. ");
						setblack();
					}
					else if (z < -0.0793)
					{
						setColor(6, 0);
						printf("하위 30%에 들어갈 확률이 높습니다. ");
						setblack();
					}
					else
					{setColor(4, 0);
						printf("상위 30%~70%에 위치할 확률이 높습니다.");
						setblack();
					}
					Sleep(5000);

					break;
				}
				break;
			case 4:
				exit(0);
				break;
			}
		}
	}
}

