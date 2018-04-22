#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>

//Colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//Memory Limits
#define cache_size 9999

//Keybindings
#define ENTER 13
#define ARROW 224
#define LEFT 75
#define RIGHT 77
#define SEPARATOR 219
#define DELETE 83
#define BACKSPACE 8
#define NL 10

//Display Operations
#define clear() printf("\033[H\033[J")
#define cursor() printf(KWHT "|"KRED "|"KWHT);

//Function Prototypes
void display(int pos);
void jump_line_up();
void jump_line_down();
int insert(char* read);

char filename[] = "new";
char buffer[256];
int pos = 0;
int display_max = 0;
int frame = 0;
int nlines = 15;

int left()
{
        if(pos>0)
                pos--;
        display(pos);
        return 0;
}
int right()
{
        if(pos<display_max)
                pos++;
        printf("RIGHT\n");
        display(pos);
        return 0;
}
int up()
{
        if(frame>0)
        {
                frame--;
                jump_line_down();
        }
        display(pos);
        return 0;
}

int down()
{
        if(frame>=0)
        {
                frame++;
                jump_line_up();
        }
        display(pos);
        return 0;
}

int main()
{
        //setlocale(LC_ALL,"en_US.utf8");
        //const wchar_t cur = 0x1d173;
        //printf("%c\n",cur);
        //cursor();
	FILE *fp = fopen(filename, "ab+");
	fclose(fp);
        strcpy(buffer,"\nACTION:");
        while(1)
        {
                rl_bind_keyseq("\\e[A",up);
                rl_bind_keyseq("\\e[B",down);
                rl_bind_keyseq("\\e[D",left);
                rl_bind_keyseq("\\e[C",right);
                //rl_bind_keyseq("_ins",insert);
                display(pos);
                printf("\nCharacters=%d\n",display_max);
                char* read = readline(buffer);
                //printf("\n >>>>>%s<<<<<\n",read);
                //sleep(1);
                insert(read);

                //break;
        }
        return 0;
}

int insert(char* readin)
{
        if(strlen(readin)==0)
        {
          readin = strdup("\n");
        }
        printf("\nINSERT : \n");
        //char* readin = strdup("hello");//buffer;

        //printf("\n%s\n",read);

        //scanf("%s",read);
        FILE * file = fopen(filename,"r");

        char * data = malloc(cache_size*sizeof(char));
        char * data2 = malloc(cache_size*sizeof(char));
        int ii;
        for(ii=0;ii<cache_size;ii++)
        {
          data[ii] = '\0';
          data2[ii] = '\0';
        }
        int cur = 0;
        while(1)
        {
                if(feof(file))
                {
                        fclose(file);
                        break;
                }
                char symbol = fgetc(file);
                if(symbol==-1)
                {
                        fclose(file);
                        break;
                }
                data[cur++] = symbol;
                printf("Data Fetched %d - %c\n",cur,symbol);
        }
        data[cur] = '\0';
        cur = 0;
        strncpy(data2,data,pos);
        strcat(data2,readin);
        strcat(data2,data+pos);
        FILE * file2 = fopen(filename,"w");
        fprintf(file2,"%s",data2);
        fclose(file2);
        free(data);
        free(data2);
        pos+=strlen(readin);
        return 0;
}

void display(int pos)
{
        FILE * file = fopen(filename,"r");
        int cur = 0;
        clear();
        int i;
        display_max = 0;
        int count_nl=0;
        if(file!=NULL)
        {
                while(1)
                {
                        if(pos == cur)
                        {
                                cursor();
                        }
                        else
                        {
                                display_max++;
                        }
                        cur++;
                        if(feof(file))
                        {
                                fclose(file);
                                printf("\n");
                                break;
                        }
                        char symbol = fgetc(file);
                        if(symbol==-1)
                        {
                                fclose(file);
                                printf("\n");
                                break;
                        }
                        if(symbol==10)
                        {
                                count_nl++;
                        }
                        if(count_nl>(frame+nlines))
                        {
                                fclose(file);
                                printf(KWHT ""KBLU "EOF\n");
                                break;
                        }
                        if(count_nl<frame)
                        {
                                continue;
                        }
                        fputc(symbol,stdout);
                        //printf(" %d ",symbol);
                }
                printf("\n"KRED "_______________________________________________________________________________________________________________\n\n" );
                fflush(stdout);
        }
}

void jump_line_up()
{
        FILE * file = fopen(filename,"r");
        int cur = 0;
        int count_nl=0;
        int current_line_found=0;
        int current_line=0;
        if(file!=NULL)
        {
                while(1)
                {
                        cur++;
                        if(feof(file))
                        {
                                fclose(file);
                                printf("\n");
                                break;
                        }
                        char symbol = fgetc(file);
                        if(symbol==-1)
                        {
                                fclose(file);
                                printf("\n");
                                break;
                        }
                        if(symbol==10)
                        {
                                count_nl++;
                        }
                        if((cur>=pos)&&(current_line_found==0))
                        {
                                current_line_found = 1;
                                current_line = count_nl;
                        }
                        if(current_line_found==1)
                        {
                                if(count_nl>current_line)
                                {
                                        pos = cur;
                                        fclose(file);
                                        break;
                                }
                        }
                }
                fflush(stdout);
        }
}

void jump_line_down()
{
        FILE * file = fopen(filename,"r");
        int cur = 0;
        int count_nl=0;
        int current_line_found=0;
        int current_line=0;
        int prev_line_head=0;
        if(file!=NULL)
        {
                while(1)
                {
                        cur++;
                        if(feof(file))
                        {
                                fclose(file);
                                printf("\n");
                                break;
                        }
                        char symbol = fgetc(file);
                        if(symbol==-1)
                        {
                                fclose(file);
                                printf("\n");
                                break;
                        }
                        if(symbol==10)
                        {
                                count_nl++;
                                if(current_line_found==0)
                                        prev_line_head = cur-1;
                        }
                        if((cur>=pos)&&(current_line_found==0))
                        {
                                current_line_found = 1;
                                current_line = count_nl;
                        }
                        if(current_line_found==1)
                        {
                                pos = prev_line_head;
                                fclose(file);
                                break;
                        }
                }
                fflush(stdout);
        }
}
