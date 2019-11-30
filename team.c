#define _CRT_SECURE_NO_WARNINGS
#define x_IdPasswd 20
#define y_IdPasswd 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
int login();
int join();
void scan_box(int x, int y, int size, char scan_[]);//�Է��ϴ� ��ġ�� �̵��� ũ�⸸ŭ �Է� �޴� �Լ�
void scan_box_passwd(int x, int y, int size, char scan_passwd[]);//*�� ����ϸ� �Է� �޴� �Լ�
void blank(int x, int y, int size);//�Է� ��ĭ�� ����� �ִ� �Լ�
void gotoxy(int x, int y);//�ֿܼ��� x,y�� �̵��ϴ� �Լ�
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
	double average;//���
	int students;//�л���
	double standard;//ǥ������
} relative;

typedef struct _absolute
{
	int attend;//�⼮����
	int midterm;//�߰����
	int final;//�⸻���
	int quiz[5];//����
	int score[9];//ABCDF���� ����
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
		printf("���� ���� ����");
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
pass1:   //���̵� �ߺ��� ���
	system("cls");
	printf("ȸ�� ����");
	char pw_w[20];
	char temp[50];

	//���̵�, ��й�ȣ, ��й�ȣ üũ�� ���
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
	//���̵� �ߺ�Ȯ��
	gotoxy(x_IdPasswd + 5, y_IdPasswd + 4);
	setColor(4, 0);
	if (fopen(id_w, "r") != NULL)
	{
		printf("�̹� �����ϴ� ���̵��Դϴ�.");
		Sleep(1000);
		goto pass1;
	}
	else
	{
		setblack();
		FILE* fID = fopen(id_w, "w+");         // PW�������� ����
		//��ȣȭ
		int i = 0;
		while (pw_w[i] != NULL)
		{
			pw_w[i] += 34;
			i++;
		}
		fprintf(fID, "%s", pw_w);
		printf("ȸ�� ������ �Ϸ�Ǿ����ϴ�.");
		Sleep(1000);
		fclose(fID);
		session--;
		return 0;
	}
}


int login() {
	session++;
	system("cls");
	printf("�α���");
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
		//��ȣȭ
		pw_count = 0;
		while (passwd[pw_count] != NULL)
		{
			passwd[pw_count] -= 34;
			pw_count++;
		}


		if (strcmp(passwd, passwd_r) == 0) {
			setblack();
			printf("�α��� ����");
			Sleep(1000);
			return 0;

		}
		else {
			session--;
			printf("��й�ȣ�� Ʋ�Ƚ��ϴ�.");
			setblack();
			Sleep(1000);

			return 1;
		}
	}
	else {
		session--;
		printf("�������� �ʴ� ���̵��Դϴ�.");
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
	double std; // ����
	double z;//z����
	
	char subject_mine[10][2][100];
	char subject_allname[50][2][100];
	

	while (1) {
		system("cls");
		
		printf("1. ������ ��å�� �����Դϱ�? (1. ����  2. �л�)  :"); scanf("%d", &pro_or_stu);
		if (pro_or_stu == 1)
			printf("\n  �ݰ����ϴ�! ������ �����ϼ̽��ϴ�.\n\n");
		else if (pro_or_stu == 2)
			printf("\n  ȯ���մϴ�! �л��� �����ϼ̽��ϴ�.\n\n");
		Sleep(1000);
		system("cls");
		printf("1.������ �Ͻðڽ��ϱ�?(1.�α��� 2.ȸ������) :"); scanf("%d", &selection);

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

		if (pro_or_stu == 1)//���� ���
		{//strcat

			printf("====================\n");
			printf("=    1.�����߰�     =\n");
			printf("=    2.�������     =\n");
			printf("=    3.�����ϱ�     =\n");
			printf("====================\n");
			printf("���ϴ� �׸��� �Է��ϼ���:");
			scanf("%d", &selection);
			switch (selection) {
			case 1:
				printf("�����̸��� �Է��ϼ���:"); //�ι� ����
				char temp[70] = "subject\\";
				scanf("%s", subject.sub_name);
				printf("�����̸��� �Է��ϼ���:");
				scanf("%s", subject.pro_name);

				strcat(temp, subject.sub_name);
				strcat(temp, subject.pro_name);
				strcat(temp, ".txt");

				FILE* fp = fopen(temp, "a");
				FILE* fall = fopen("all_subject.txt", "a");
				FILE* fid = fopen(id_r, "a");

				fprintf(fid, "\n%s %s", subject.sub_name, subject.pro_name);//id�� ����
				fprintf(fall, "%s %s\n", subject.sub_name, subject.pro_name);//��ü ���� ��Ͽ� ����

				printf("�����򰡸� 0�� ����򰡸� 1�� �Է��ϼ���:");
				scanf("%d", &subject.how);
				fprintf(fp, "%d\n", subject.how);
				if (subject.how == 0) {//�����ؾ���
					printf("�⼮���� ������ �Է��ϼ���:");
					scanf("%d", &subject.abs.attend);
					fprintf(fp, "%d\n", subject.abs.attend);

					printf("�߰���� ������ �Է��ϼ���:");
					scanf("%d", &subject.abs.midterm);
					fprintf(fp, "%d\n", subject.abs.midterm);

					printf("�⸻��� ������ �Է��ϼ���:");
					scanf("%d", &subject.abs.final);
					fprintf(fp, "%d\n", subject.abs.final);

					printf("����/���� ������ �Է��ϼ���:");
					scanf("%d", &num);
					for (int i = 0; i < 5; i++) {
						if (i >= num)
						{
							fprintf(fp, "%d\n", 0);
							continue;
						}
						printf("����/���� %d�� ������ �Է��ϼ���", i + 1);
						scanf("%d", &subject.abs.quiz[i]);
						fprintf(fp, "%d\n", subject.abs.quiz[i]);

					}
					//�ؿ� ��� &���ϰ�
					printf("A+�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[0]);
					fprintf(fp, "%d\n", subject.abs.score[0]);
					printf("A0�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[1]);
					fprintf(fp, "%d\n", subject.abs.score[1]);
					printf("B+�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[2]);
					fprintf(fp, "%d\n", subject.abs.score[2]);
					printf("B0�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[3]);
					fprintf(fp, "%d\n", subject.abs.score[3]);
					printf("C+�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[4]);
					fprintf(fp, "%d\n", subject.abs.score[4]);
					printf("C0�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[5]);
					fprintf(fp, "%d\n", subject.abs.score[5]);
					printf("D+�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[6]);
					fprintf(fp, "%d\n", subject.abs.score[6]);
					printf("D0�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[7]);
					fprintf(fp, "%d\n", subject.abs.score[7]);
					printf("F�� ���������Դϱ�?");
					scanf("%d", &subject.abs.score[8]);
					fprintf(fp, "%d\n", subject.abs.score[8]);
				}
				else if (subject.how == 1) {
					printf("��ü��������� �Է��ϼ���:");
					scanf("%lf", &subject.rel.average);
					fprintf(fp, "%lf\n", subject.rel.average);

					printf("�л����� �Է��ϼ���:");
					scanf("%d", &subject.rel.students);
					fprintf(fp, "%d\n", subject.rel.students);

					printf("ǥ�������� �Է��ϼ���:");
					scanf("%lf", &subject.rel.standard);
					fprintf(fp, "%lf\n", subject.rel.standard);
				}
				fclose(fall);
				fclose(fp);
				break;
			case 2:
				file_subjects(id_r, subject_mine);
				printf("�����ϰ� ������ ������ ��ȣ�� �Է��Ͻʽÿ�: ");
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
		else if (pro_or_stu == 2)//�л� ���
		{
			printf("====================\n");
			printf("=    1.�����߰�     =\n");//�����Ե��� ��� ����߿� ����
			printf("=    2.�������     =\n");
			printf("=    3.���������м� =\n");
			printf("=    4.�����ϱ�     =\n");
			printf("====================\n");
			printf("���ϴ� �׸��� �Է��ϼ���:");
			scanf("%d", &selection);
			switch (selection)
			{
			case 1:
				////������ - �����ִ� �Լ� ����� �� 
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
				printf("�����ϰ� ������ ������ ��ȣ�� �Է��Ͻʽÿ�: ");
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
			case 3://���������м� - 
				//���� �����ϴ� �� �߰� �ؾ���
				file_subjects(id_r, subject_mine);
				//printf("���������� �Է��ϼ���. (������ �Է��ϼ���.)");
				printf("������ �˰���� �����ȣ�� �Է��Ͻÿ�:");
				scanf("%d", &number);//�����ٲٱ�
				char temp[80]="subject\\";
				strcat(temp, subject_mine[number - 1][0]);
				strcat(temp, subject_mine[number - 1][1]);
				strcat(temp, ".txt");
				FILE* fsub = fopen(temp, "r");
				if (fsub == NULL)
				{
					printf("���� ���� ����");
					return -1;
				}
				fscanf(fsub, "%d\n",&subject.how);
				switch (subject.how) {//�����ٲٱ�
				case 0://������
					fscanf(fsub, "%d\n%d\n%d\n",&subject.abs.attend,&subject.abs.midterm,&subject.abs.final);
					for (int i = 0; i < 5; i++)
						fscanf(fsub, "%d\n", &subject.abs.quiz[i]);
					for (int i = 0; i < 9; i++)
						fscanf(fsub, "%d\n", &subject.abs.score[i]);
					printf("�߰���� ������ �� ���Դϱ�?(100�� ���� ����)");
					scanf("%lf", &score_input[0]);
					printf("�����ϴ� �⼮ ������ �� ���Դϱ�?(100�� ���� ����)");
					scanf("%lf", &score_input[1]);
					for (i = 0; i < 5; i++)
					{
						printf("�����ϴ� ����(����)%d�� �� �� �Դϱ�?(100�� ���� ����)",i+1);
						scanf("%lf", &score_input[2 + i]);
					}
					printf("���ϴ� ������ �� �� �Դϱ�? (���� ���� ��. A: 4.0, B+:3.5)");
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
						printf("����� ���� �մϴ�! ������ ����ϼ���!");
						setblack();
					}
					else {
						setColor(4, 0);
						printf("���ǰ� �̻��� ������ �ٸ��� ��������. �׷��� �������� ���� ������ ����ϸ� ���� ����� ���� �� ���� �ſ���!");
						setblack();
					}

					Sleep(5000);
					break;


				case 1://�����
					fscanf(fsub, "%f\n%d\n%f", &subject.rel.average, &subject.rel.students, &subject.rel.standard);

					z = (score_input[0] - subject.rel.average) / subject.rel.standard;

					if (z > 0.0793)
					{
						setColor(6, 0);
						printf("���� 30%�� �� Ȯ���� �����ϴ�. ");
						setblack();
					}
					else if (z < -0.0793)
					{
						setColor(6, 0);
						printf("���� 30%�� �� Ȯ���� �����ϴ�. ");
						setblack();
					}
					else
					{setColor(4, 0);
						printf("���� 30%~70%�� ��ġ�� Ȯ���� �����ϴ�.");
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

