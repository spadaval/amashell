#include "glob.h"

int glob(char* input, char** results)
{
        char* wildcards = malloc(MAX_ENTRY*sizeof(char));
        char* actions[MAX_ENTRY];
        char* path[MAX_ENTRY];
        char* matches[MAX_ENTRY];
        char* qreg;
        char* query;
        int count;
        path[0]=input;

        read_conf(wildcards,actions);
        set_path(path);
        query = evaluate(path);
        qreg = dvlp_reg(query,wildcards,actions);
        get_dir(path);
        //display(path);
        count = search(qreg,path,results);
        //display(results);
        return count;
}

//Other Functions
void read_conf(char* wildcards,char** actions)
{
        char buffer[BUFFER_SIZE];
        int n, i;
        FILE * file = fopen(conf_file,"r");
        fscanf(file,"%d\n",&n);
        //printf("\nLines : %d\n",n);
        for(i=0; i<n; i++)
        {
                int j = 0;
                char k;
                wildcards[i] = fgetc(file);
                fgetc(file);
                while(!feof(file))
                {
                        k = fgetc(file);
                        if(k=='\n')
                        {
                                break;
                        }
                        buffer[j++] = k;
                }
                buffer[j] = '\0';
                actions[i]=strdup(buffer);
                for(j=0; j<BUFFER_SIZE; j++)
                {
                        buffer[j] = '\0';
                }
        }
}

char* dvlp_reg(char* query,char* wildcards,char** actions)
{
        int i, j;
        for(i=0; i<strlen(query); i++)
        {
                for(j=0; j<strlen(wildcards); j++)
                {
                        //printf("HERE i=%c j=%c\n",query[i],wildcards[j]);
                        if(query[i]==wildcards[j])
                        {
                                char* data = malloc(MAX_ENTRY*sizeof(char));
                                strncpy(data, query, i);
                                data[i]='\0';
                                strcat(data,actions[j]);
                                strcat(data,query+i+1);
                                query = strdup(data);
                                free(data);
                                i+=strlen(actions[j]);
                        }
                }
        }
        //printf("\nQuery : %s\n",query);
        return query;
}

int search(char* query, char** path, char** matches)
{
        //Garbage collection
        int g;
        for(g=0; g<MAX_ENTRY; g++)
        {
                matches[g] = NULL;
        }

        int count = 0;
        void push(char* f)
        {
                matches[count++] = f;
        }
        int rv;
        regex_t exp;
        rv = regcomp(&exp, query, REG_EXTENDED);
        if (rv != 0)
        {
                printf("Configuration File Failure. Code:%d\n", rv);
        }

        int i = 1;
        int j;
        while(path[i]!=NULL)
        {
                j = match(&exp, path[i++]);
                if(j==1)
                {
                        push(path[i-1]);
                }
        }
        regfree(&exp);
        if(count == 0)
        {
                //printf("\nNo Match Found\n");
        }
        return count;
}

int match(regex_t *pexp, char *sz)
{
        regmatch_t matches[MAX_MATCHES];
        if (regexec(pexp, sz, MAX_MATCHES, matches, 0) == 0)
        {
                if((matches[0].rm_so==0)&&(matches[0].rm_eo==strlen(sz)))
                {
                        //printf("\n\"%s\" Match : Positive\n",sz);
                        return 1;
                }
                return 0;
        }
        else
        {
                //printf("\n\"%s\" Match : Negative\n", sz);
                return 0;
        }
}

char* evaluate(char** dub_path)
{
        char* path = dub_path[0];
        int pos = 0;
        int i = 0;
        while(path[i]!='\0')
        {
                if(path[i]=='/')
                {
                        pos = i;
                }
                i++;
        }
        char* root = malloc((pos+1)*sizeof(char));
        char* query = malloc(MAX_ENTRY*sizeof(char));
        strncpy(root,path,pos);
        strcpy(query,path+pos+1);
        root[pos] = '\0';
        dub_path[0] = root;
        //printf("\nPos = %d\n",pos);
        //printf("\nRoot = %s\n",root);
        //printf("\nQuery = %s\n",query);
        return strdup(query);
}

void set_path(char ** path)
{
        if(path[0][0]!='/')
        {
                char buffer[PATH_SIZE];
                strcpy(buffer,path[0]);
                int i;
                for(i=PATH_SIZE-1; i>1; i--)
                {
                        buffer[i] = buffer[i-2];
                }
                buffer[0] = '.';
                buffer[1] = '/';
                path[0] = strdup(buffer);
        }
}

void get_dir(char** path)
{
        //Garbage collection for cached Memory
        int i;
        for(i=1; i<MAX_ENTRY; i++)
        {
                path[i] = NULL;
        }
        //Fetch files
        i = 1;
        DIR *d;
        struct dirent *dir;
        d = opendir(path[0]);
        if (d)
        {
                while ((dir = readdir(d)) != NULL)
                {
                        path[i++] = strdup(dir->d_name);
                        //printf("%s\n", dir->d_name);
                }
                closedir(d);
        }
}

void display(char** path)
{
        int i=0;
        //printf("\n");
        while(path[i]!=NULL)
        {
                printf("%s\t\t",path[i++]);
        }
        printf("\n");
}
