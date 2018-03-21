#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "fs.h"
#include "user.h"

int get_line_number(char *text[]);
void show_text(char *text[]);
void text_ins(char *text[], int n);
void text_mod(char *text[], int n);
void text_del(char *text[], int n);
void text_help(char *text[]);
void text_save(char *text[], char *path);
void text_exit(char *text[], char *path);

//����Ƿ���Ĺ�
int changed = 0;
//�����Ƿ��Զ�show 
int auto_show = 1;

//argc����ָ��word�ĸ��� 
//argv[0]����"/editor",argv[1]������ļ����� 
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf(1, "Please enter the instructions like [editor file_name]\n");
		exit();
	}
	
	char *text[MAX_LINE_NUMBER] = {};// ����ļ�������(�����������������ò���memset) 
	text[0] = malloc(MAX_LINE_LENGTH);//
	memset(text[0], 0, MAX_LINE_LENGTH);//��ʼ������(����һ��ȫ����0) 
	//�洢��ǰ�����кţ���0��ʼ������line_number == x�����text[0]��text[x]����
	int line_number = 0;
	//���ļ�����ȡ���������
	int fd = open(argv[1], O_CREATE);//�ļ�������ʱ���Զ������µ��ļ� 
	printf(1, "File open successfully\n");
	char buf[BUF_SIZE] = {};//���������ַ��� 
	int len = 0;
	//���ļ����ݴ�ŵ�text�� 
	while ((len = read(fd, buf, BUF_SIZE)) > 0)
	{
		int i = 0;
		int next = 0;
		int is_full = 0;// 
		while (i < len)
		{
			for (i = next; i < len && buf[i] != '\n'; i++)//��i�ߵ��ַ������ 
				;
			strcat(text[line_number], buf+next, i-next);//����"\n"֮ǰ������
			//������text�½�һ��
			if (i < len && buf[i] == '\n')
			{
				if (line_number >= MAX_LINE_NUMBER - 1)
					is_full = 1;
				else
				{
					line_number++;
					//��text�½�һ��
					text[line_number] = malloc(MAX_LINE_LENGTH);
					//��ʼ������(����һ��ȫ����0) 
					memset(text[line_number], 0, MAX_LINE_LENGTH);
				}
			}
			if (is_full == 1 || i >= len - 1)
				break;
			else
				next = i + 1;
		}
		if (is_full == 1)
			break;
	}
	close(fd);
	
	//����ļ�����
	show_text(text);
	
	//��������
	char input[MAX_LINE_LENGTH] = {};//�����ָ�� 
	while (1)
	{
		printf(1, "\nPlease enter an instruction:\n");
		memset(input, 0, MAX_LINE_LENGTH);
		gets(input, MAX_LINE_LENGTH-1);//ֻ������255λ 
		int len = strlen(input);
		input[len] = '\0';//���һλ��'\0' 
		
		//ins
		if (input[0] == 'i' && input[1] == 'n' && input[2] == 's')
		{
			if (input[3] == '-'&&atoi(&input[4])>=0)
			{
				text_ins(text, atoi(&input[4])-1);//���� 
				line_number = get_line_number(text);//�����к�
			}
			else if(input[3] == ' '||input[3] == '\0'||input[3] == '\n')
			{
				text_ins(text, line_number);
                line_number = get_line_number(text);
//               printf(2,"%d\n",line_number);
			}
			else
			{
				printf(1, "invalid instruction.\n");
				text_help(text);
			}
		}
		//mod
		else if (input[0] == 'm' && input[1] == 'o' && input[2] == 'd')
		{
			if (input[3] == '-'&&atoi(&input[4])>=0){
				text_mod(text, atoi(&input[4]));
				line_number = get_line_number(text);
			}
			else if(input[3] == ' '||input[3] == '\0'||input[3] == '\n'){
				text_mod(text, line_number);
				line_number = get_line_number(text);
			}
			else
			{
				printf(1, "invalid instruction.\n");
				text_help(text);
			}
		}
		//del
		else if (input[0] == 'd' && input[1] == 'e' && input[2] == 'l')
		{
			if (input[3] == '-'&&atoi(&input[4])>=0)
			{
				text_del(text, atoi(&input[4]));
				line_number = get_line_number(text);
			}	
			else if(input[3]=='\0'||input[3] == '\n')
			{
				text_del(text, line_number);
				line_number = get_line_number(text);
			}
			else
			{
				printf(1, "Invalid instruction.\n");
				text_help(text);
			}
			
		}
		else if (strcmp(input, "showtext") == 0)
		{
			show_text(text);
		}
		else if (strcmp(input, "show") == 0)
		{
			auto_show = 1;
			printf(1, "Enable show current contents after text changed.\n");
		}
		else if (strcmp(input, "hide") == 0)
		{
			auto_show = 0;
			printf(1, "Disable show current contents after text changed.\n");
		}
		else if (strcmp(input, "help") == 0)
			text_help(text);
		else if (strcmp(input, "save") == 0 || strcmp(input, "CTRL+S\n") == 0)
			text_save(text, argv[1]);
		else if (strcmp(input, "exit") == 0)
			text_exit(text, argv[1]);
		else
		{
			printf(1, "Invalid instruction.\n");
			text_help(text);
		}
	}
	exit();
}

void show_text(char *text[])
{
	printf(1, "********************************************\n");
	printf(1, "The content of the file is:\n");
	int j = 0;
	//ǰ����%d��ʾ�кţ�text[j]Ϊ�������� 
	for (; text[j] != NULL; j++)
		printf(1, "%d%d%d:%s\n", (j+1)/100, ((j+1)%100)/10, (j+1)%10, text[j]);
	printf(1, "********************************************\n");
}

//��ȡ��ǰ�����кţ���0��ʼ����Χtext[0]��text[x]
int get_line_number(char *text[])
{
	int i = 0;
	for (; i < MAX_LINE_NUMBER; i++)
		if (text[i] == NULL)
			return i - 1;
	return i - 1;
}

//�������nΪ�û�������кţ���1��ʼ
void text_ins(char *text[], int n)
{
	if (n < 0 || n > get_line_number(text))
	{
		printf(1, "Invalid line number\n");
		return;
	}
	if( get_line_number(text) >= MAX_LINE_LENGTH - 2)//�ж��ļ��Ƿ����� (�˴���2�ǵ��Գ�����)
	{
		printf(1, "The file is full!\n");
		return;
	}
	char input[MAX_LINE_LENGTH] = {};
	printf(1, "Please input the content:\n");
	gets(input, MAX_LINE_LENGTH-1);
	input[strlen(input)] = '\0';
	
	int i = MAX_LINE_NUMBER - 1;//�������ǰ�� 
	for (; i > n; i--)//һֱ���û�������ĵ�n�н��� 
	{
		if (text[i-1] == NULL)//��ǰ��Ϊ�ձ������ǰ 
			continue;
		//��ǰ��Ϊ�գ���һ�в�Ϊ�գ�Ϊ��ǰ�п��ٿռ䣬����һ�зŵ���ǰ�� 
		else if (text[i] == NULL && text[i-1] != NULL)
		{
			text[i] = malloc(MAX_LINE_LENGTH);
			memset(text[i], 0, MAX_LINE_LENGTH);
			strcpy(text[i], text[i-1]);
		}
		//��ǰ�в�Ϊ�գ���һ��Ҳ��Ϊ�գ�����һ�зŵ���ǰ�� 
		else if (text[i] != NULL && text[i-1] != NULL)
		{
			memset(text[i], 0, MAX_LINE_LENGTH);
			strcpy(text[i], text[i-1]);
		}
	}
	if (text[n] == NULL)//������Ϊ�գ��ȿ��ٿռ� 
	{
		text[n] = malloc(MAX_LINE_LENGTH);
	}
	memset(text[n], 0, MAX_LINE_LENGTH);//���� 
	strcpy(text[n], input);//������д��ȥ 
	changed = 1;
	if (auto_show == 1)
		show_text(text);
}

//�޸����nΪ�û�������кţ���1��ʼ
void text_mod(char *text[], int n)
{
	if (n <= 0 || n > get_line_number(text) )
	{
		printf(1, "Invalid line number\n");
		return;
	}
	char input[MAX_LINE_LENGTH] = {};
	printf(1, "Please input content:\n");
	gets(input, MAX_LINE_LENGTH-1);
	input[strlen(input)] = '\0';
	
	memset(text[n-1], 0, MAX_LINE_LENGTH);
	strcpy(text[n-1], input);
	changed = 1;
	if (auto_show == 1)
		show_text(text);
}

//ɾ�����nΪ�û�������кţ���1��ʼ
void text_del(char *text[], int n)
{
	if (n <= 0 || n > get_line_number(text) )
	{
		printf(1, "Invalid line number\n");
		return;
	}
	memset(text[n-1], 0, MAX_LINE_LENGTH);//�Ѵ������� 
	int i = n - 1;
	//ѭ����text[i+1]����text[i] 
	for (; text[i+1] != NULL; i++)
	{
		strcpy(text[i], text[i+1]);
		memset(text[i+1], 0, MAX_LINE_LENGTH);
	}
	//����ǵ�һ�в��Һ���û���������ͷſռ� 
	if (i != 0)
	{
		free(text[i]);
		text[i] = 0;
	}
	changed = 1;
	if (auto_show == 1)
		show_text(text);
}

void text_help(char *text[])
{
	printf(1,"********************************************\n");
	printf(1, "Instructions for user:\n");
	printf(1, "ins-n, insert a line after line n\n");
	printf(1, "mod-n, modify line n\n");
	printf(1, "del-n, delete line n\n");
	printf(1, "ins, insert a line after the last line\n");
	printf(1, "mod, modify the last line\n");
	printf(1, "del, delete the last line\n");
	printf(1, "show, enable show current contents after executing an instruction.\n");
	printf(1, "hide, disable show current contents after executing an instruction.\n");
	printf(1, "save, save the file\n");
	printf(1, "exit, exit editor\n");
	printf(1,"********************************************\n");
}

void text_save(char *text[], char *path)
{
	//ɾ�������ļ�
	unlink(path);//in system call 
	//�½��ļ�����
	//O_WRONLY����Ϊд�ļ�  O_CREATE����Ϊ�����ļ� 
	int fd = open(path, O_WRONLY|O_CREATE);
	if (fd == -1)
	{
		printf(1, "Save failed, file can't open:\n");
		exit();
	}
	if (text[0] == NULL)
	{
		close(fd);
		return;
	}
	//д����
	//write��������text[0]�г���Ϊlen��д���ļ�fd�� 
	write(fd, text[0], strlen(text[0]));
	int i = 1;
	for (; text[i] != NULL; i++)
	{
		printf(fd, "\n");//д��'\n' 
		write(fd, text[i], strlen(text[i]));//д��text���� 
	}
	close(fd);
	printf(1, "Saved successfully\n");
	changed = 0;
	return;
}

void text_exit(char *text[], char *path)
{
	//ѯ���Ƿ񱣴�
	while (changed == 1)
	{
		printf(1, "Save the file? y/n\n");
		char input[MAX_LINE_LENGTH] = {};
		gets(input, MAX_LINE_LENGTH-1);
		input[strlen(input)] = '\0';
		if (strcmp(input, "y") == 0)
			text_save(text, path);
		else if(strcmp(input, "n") == 0)
			break;
		else
			printf(1, "Invalid answer!\n");
	}
	//�ͷ��ڴ�
	int i = 0;
	for (; text[i] != NULL; i++)
	{
		free(text[i]);
		text[i] = 0;
	}
	exit();
}
