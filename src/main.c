#include "functions.h"

// #define __DEBUG_MOD__

#ifdef __DEBUG_MOD__
int main()
{
    int argc = 4;
    char argv[][50] = {"neogit", "add","-n","4"};
    chdir("/home/zahra/test");
#else
int main(int argc, char *argv[])
{
#endif
    prepareANSI();
    if (argc < 2){
        INVALID_CMD;
        return 0;
    }

    if (!strcmp(argv[1], "init"))
    {
        if (argc == 2)
        {
            if (init())
                printf("Repository initialized!\n");
            return 0;
        }
        else
        {
            INVALID_CMD;
            return 0;
        }
    }
    if (argc == 5 && !strcmp(argv[1],"config") &&!strcmp(argv[2], "-global") && !strncmp(argv[3], "alias.", 6))
        {
            char infoPath[PATH_MAX];
            snprintf(infoPath, sizeof(infoPath) + 9, "/home/zahra/.neogitconfig/alias/%s.txt",argv[3] + 6);
            if (IsValid(argv[4]))
            {
                FILE *fp = fopen(infoPath, "w");
                fprintf(fp, "%s", argv[4]);
                fclose(fp);
                printf("Alias created !\n");
            }
            else
                printf("not a valid command for making alias!\n");
            return 0;
        }
        else if (argc == 5 &&!strcmp(argv[1],"config") && !strcmp(argv[2], "-global"))
        {
            char infoPath[PATH_MAX]="/home/zahra/.neogitconfig/info.txt"; 
            if (access(infoPath, F_OK) != 0)
            {
                FILE *fp = fopen(infoPath, "w");
                if (!fp)
                {
                    perror("fopen");
                    return 1;
                }
                fprintf(fp, "name =\nemail =\n");
                fclose(fp);
            }

            if (!strcmp(argv[3], "user.name"))
                ChangeConfig(infoPath, argv[4],"name");
            else if (!strcmp(argv[3], "user.email"))
                ChangeConfig(infoPath, argv[4],"email");
            else
                INVALID_CMD;
                return 0;
        }

    char ProjPath[PATH_MAX];
    getcwd(ProjPath, sizeof(ProjPath));
    CHECK_IF_REPO(repo_place(ProjPath));

    if (IsAlias(argv[1], ProjPath))
        return 0;
    
    if (argc >= 4 && !strcmp(argv[1], "config"))
    {
        
         if (argc == 4 && !strncmp(argv[2], "alias.", 6))
        {
            strcat(ProjPath, "/.neogit/alias/");
            strcat(ProjPath, argv[2] + 6);
            strcat(ProjPath, ".txt");

            if (IsValid(argv[3]))
            {
                FILE *fp = fopen(ProjPath, "w");
                fprintf(fp, "%s", argv[3]);
                fclose(fp);
                printf("Alias created !\n");
            }
            else
                printf("not a valid command for making  alias!\n");
        }

        else if (argc == 4)
        {
            strcat(ProjPath, "/.neogit/config.txt");

            if (access(ProjPath, F_OK) != 0)
            {
                FILE *fp = fopen(ProjPath, "w");
                fprintf(fp, "name =\nemail =\n");
                fclose(fp);
            }
            if (!strcmp(argv[2], "user.name"))
                ChangeConfig(ProjPath, argv[3],"name");
            else if (!strcmp(argv[2], "user.email"))
                ChangeConfig(ProjPath, argv[3],"email");
            else
                INVALID_CMD;
        }
        else
            INVALID_CMD;
    }

    else if (argc > 2 && !strcmp(argv[1], "add"))
    {
        if (argc > 3 && !strcmp(argv[2], "-f"))
        {
            for (int i = 3; i < argc; i++)
                Staging(argv[i], ProjPath);
            newRecords(ProjPath);
        }

        else if (argc > 3 && !strcmp(argv[2], "-n"))
        {
            int depth = 0;
            for (int i = 0; i < strlen(argv[3]); i++)
            {
                depth *= 10;
                depth += (argv[3][i] - '0');
            }
            char curPath[PATH_MAX];
            getcwd(curPath, sizeof(curPath));
            char *lastSlash = strrchr(curPath, '/');
            if(lastSlash !=NULL)
                printf("Searching " _SGR_BOLD _SGR_CYANF "%s :\n\n"_SGR_RESET, lastSlash + 1);
            else
                printf("Searching " _SGR_BOLD _SGR_CYANF "%s :\n\n"_SGR_RESET, curPath);//else not
            Show_folder_n(ProjPath, curPath, depth,true);
        }
        else if (argc == 3 && !strcmp(argv[2], "-redo"))

        {
            char Staged[PATH_MAX];
            strcpy(Staged, ProjPath);
            strcat(Staged, "/.neogit/.staged");
            redo(ProjPath, Staged);
            newRecords(ProjPath);
        }
        else
        {
            for(int i=2; i<argc ; i++)
                Staging(argv[2], ProjPath);
            newRecords(ProjPath);
        }
    }

    else if (argc > 2 && !strcmp(argv[1], "reset"))
    {
        if (argc > 3 && !strcmp(argv[2], "-f"))
        {
            for (int i = 3; i < argc; i++)
                unstaging(argv[i], ProjPath);
            newRecords(ProjPath);
        }
        else if (argc == 3 && !strcmp(argv[2], "-undo"))
            resetundo(ProjPath);
        else
        {
            for (int i = 2; i < argc; i++)
                unstaging(argv[i], ProjPath);
            newRecords(ProjPath);
        }
    }

    else if (argc > 2 && !strcmp(argv[1], "commit"))
    {
        if (!strcmp(argv[2], "-m"))
        {
            if (argc > 4)
                printf("Your message should be in double qoutations!\n");
            else if (argc < 4)
                printf("You should enter a message!\n");
            else if (strlen(argv[3]) > 72)
                printf("Your message can have 72 character at most!\n");
            else
                Commit(ProjPath, argv[3]);
        }
        else if (argc == 4 && !strcmp(argv[2], "-s"))
        {
            char adrs[PATH_MAX];
            sprintf(adrs, "%s/.neogit/shortcuts/%s.txt", ProjPath, argv[3]);
            FILE *f = fopen(adrs, "r");
            if (f == NULL)
                printf("shortcut doesn't exist!\n");
            else
            {
                char message[100];
                fgets(message, sizeof(message), f);
                Commit(ProjPath, message);
            }
        }
        else
            INVALID_CMD;
    }

    else if (argc == 6 && !strcmp(argv[1], "set") && !strcmp(argv[2], "-m") && !strcmp(argv[4], "-s"))
    {
        shortcut(argv[3], argv[5], ProjPath);
        printf("shortcut message is set!\n");
    }

    else if (argc == 6 && !strcmp(argv[1], "replace") && !strcmp(argv[2], "-m") && !strcmp(argv[4], "-s"))
    {
        char shcPath[PATH_MAX];
        sprintf(shcPath, "%s/.neogit/shortcuts/%s.txt", ProjPath, argv[5]);
        if (access(shcPath, F_OK) != 0)
            printf("Shortcut doesn't exist!\n");
        else
        {
            shortcut(argv[3], argv[5], ProjPath);
            printf("shortcut message is reseted!\n");
        }
    }

    else if (argc == 4 && !strcmp(argv[1], "remove") && !strcmp(argv[2], "-s"))
    {
        char shcPath[PATH_MAX];
        sprintf(shcPath, "%s/.neogit/shortcuts/%s.txt", ProjPath, argv[3]);
        if (access(shcPath, F_OK) != 0)
            printf("Shortcut doesn't exist!\n");
        else
        {
            remove(shcPath);
            printf("shortcut message is removed !\n");
        }
    }

    else if (argc > 1 && !strcmp(argv[1], "log"))
    {   
        if (argc == 2)
            Log(ProjPath, get_ID(ProjPath,"commit") - 10000);
        else if (argc == 4 && !strcmp(argv[2], "-n"))
        {
            int depth = 0;
            for (int i = 0; i < strlen(argv[3]); i++)
            {
                depth *= 10;
                depth += (argv[3][i] - '0');
            }
            if( (get_ID(ProjPath,"commit")-9999) <depth) Log(ProjPath,get_ID(ProjPath,"commit")-10000);
            else Log(ProjPath, depth);
        }
        else if (argc == 4 && !strcmp(argv[2], "-branch"))
            Logplus(ProjPath, "Branch", argv[3]);
        else if (argc == 4 && !strcmp(argv[2], "-author"))
            Logplus(ProjPath, "Username", argv[3]);
        else if (argc == 4 && !strcmp(argv[2], "-since"))
            log_time(ProjPath, argv[3], '+');
        else if (argc == 4 && !strcmp(argv[2], "-before"))
            log_time(ProjPath, argv[3], '-');
        else if (!strcmp(argv[2], "-search"))
            for (int i = 3; i < argc; i++)
                log_message(ProjPath, argv[i]);
        
        else
            INVALID_CMD;
    }

    else if (argc == 2 && !strcmp(argv[1], "status"))
        Status(ProjPath);

    else if (!strcmp(argv[1], "branch"))
    {
        if (argc == 2)
            show_branch(ProjPath);
        else if (argc == 3)
            makebranch(argv[2], ProjPath);
        else
            INVALID_CMD;
    }

    else if (!strcmp(argv[1], "checkout"))
    {
        if (argc != 3)
            INVALID_CMD;
        else if (!strncmp(argv[2], "HEAD", 4))
            CheckoutHead(argv[2],ProjPath);
        else
            Checkout(argv[2], ProjPath);
    }

    else if (!strcmp(argv[1], "diff"))
    {
        if (!strcmp(argv[2], "-f"))
        {
            char curDir[PATH_MAX];
            getcwd(curDir, sizeof(curDir));
            char File1[PATH_MAX];
            sprintf(File1, "%s/%s", curDir, argv[3]);
            char File2[PATH_MAX];
            sprintf(File2, "%s/%s", curDir, argv[4]);
            if (argc == 5)
                Diff(File1, File2, 1, 10000, 1, 10000);
            else if (argc == 7)
            {
                if (!strcmp(argv[5], "-line2"))
                {
                    int begin = 0,end=0;
                    char *token = strtok(argv[6], "-");
                    for (int i = 0; i < strlen(token); i++)
                        begin = 10*begin + (token[i] - '0');
                    token = strtok(NULL, "-");
                    for (int i = 0; i < strlen(token); i++)
                        end = 10*end +(token[i] - '0');
                    Diff(File1, File2, 1, 10000, begin, end);
                }
                else if (!strcmp(argv[5], "-line1"))
                {
                    int begin = 0,end=0;
                    char *token = strtok(argv[6], "-");
                    for (int i = 0; i < strlen(token); i++)
                        begin =10*begin +(token[i] - '0');
                    token = strtok(NULL, "-");
                    for (int i = 0; i < strlen(token); i++)
                        end = 10*end + (token[i] - '0');
                    Diff(File1, File2, begin, end, 1, 10000);
                }
                
                else
                    INVALID_CMD;
            }
            else if (argc == 9)
            {
                if (!strcmp(argv[5], "-line1") && !strcmp(argv[7], "-line2"))
                {
                    int begin_1 = 0,end_1=0;
                    char *token = strtok(argv[6], "-");
                    for (int i = 0; i < strlen(token); i++)
                        begin_1 = 10*begin_1 + (token[i] - '0');
                    token = strtok(NULL, "-");
                    for (int i = 0; i < strlen(token); i++)
                        end_1 = 10*end_1 + (token[i] - '0');
                    int begin_2 = 0,end_2=0;
                    token = strtok(argv[8], "-");
                    for (int i = 0; i < strlen(token); i++)
                        begin_2 = 10*begin_2 + (token[i] - '0');
                    token = strtok(NULL, "-");
                    for (int i = 0; i < strlen(token); i++)
                        end_2 = 10*end_2 + (token[i] - '0');
                    Diff(File1, File2, begin_1, end_1, begin_2, end_2);
                }
                else
                    INVALID_CMD;
            }
            else
                INVALID_CMD;
        }
        else if (!strcmp(argv[2], "-c"))
        {
            if (argc == 5)
                diff_commit(argv[3],argv[4],ProjPath);
            else
                INVALID_CMD;
        }
        else
            INVALID_CMD;
    }

    else if (!strcmp(argv[1], "grep") && !strcmp(argv[2], "-f") && !strcmp(argv[4], "-p"))
    {
        if (argc == 6)
        {
            char curDir[PATH_MAX];
            getcwd(curDir, sizeof(curDir));
            char File[PATH_MAX];
            sprintf(File, "%s/%s", curDir, argv[3]);
            grep(File, argv[5], 0);
        }
        else if (argc == 7)
        {
            char curDir[PATH_MAX];
            getcwd(curDir, sizeof(curDir));
            char File[PATH_MAX];
            sprintf(File, "%s/%s", curDir, argv[3]);
            grep(File, argv[5], 1);
        }
        else if (argc == 8)
        {
            char cmPath[PATH_MAX];
            sprintf(cmPath, "%s/.neogit/commits/%s", ProjPath, argv[7]);
            grepCommit(cmPath, 0, argv[5], argv[3]);
        }
        else if (argc == 9)
        {
            char cmPath[PATH_MAX];
            sprintf(cmPath, "%s/.neogit/commits/%s", ProjPath, argv[7]);
            grepCommit(cmPath, 1, argv[5], argv[3]);
        }
        else
            INVALID_CMD;
    }


    else if(!strcmp(argv[1],"pre-commit")){
        if(argc==2) run_pre(ProjPath,1);
        else if(argc==4){
            if(!strcmp(argv[2],"hooks") && !strcmp(argv[3],"list")) {
                printf("todo-check\n");
                printf("todo_check.txt\n");
                printf("eof_blank_space.txt\n");
                printf("balance_braces.txt\n");
                printf("format_check.txt\n");
                printf("file_size_check.txt\n");
                printf("character-limit.txt\n");

            }
            else if(!strcmp(argv[2],"applied") && !strcmp(argv[3],"hooks")) Applied_Hook(ProjPath);
            else INVALID_CMD;
        }
        else if(argc==5){
            if(!strcmp(argv[2],"add") && !strcmp(argv[3],"hook")) AddHook(argv[4],ProjPath);
            else if(!strcmp(argv[2],"remove") && !strcmp(argv[3],"hook")) RemoveHook(argv[4],ProjPath);
            else INVALID_CMD;
        }
        
    }

    else if (!strcmp(argv[1], "stash")) {
        if (argc < 3)
            INVALID_CMD;
        else if (!strcmp(argv[2], "push")) {
            if (argc == 5) stash_push(1, argv[4], ProjPath);
            else stash_push(0, NULL, ProjPath);
        }
        else if (argc ==3 &&!strcmp(argv[2], "list")) {
                stash_list(ProjPath);
        }
        else 
            INVALID_CMD;
    }

    else if (!strcmp(argv[1], "merge")) {
        if (argc < 3) 
            INVALID_CMD;
        else if (argc==5 && !strcmp(argv[2], "-b")) {
            Merge(argv[3], argv[4], ProjPath);
        }
    }

    else if(!strcmp(argv[1],"tag")){
        if(argc == 2){
            Show_acsending(ProjPath);
        }
        else if(!strcmp(argv[2], "-a")){
            if(argc == 4) tagging(argv[3],NULL,NULL,0,ProjPath);
            else if( argc==5 && !strcmp(argv[4],"-f")) tagging(argv[3],NULL,NULL,1,ProjPath);
            else if(argc==6){
                if(!strcmp(argv[4],"-m")){
                    tagging(argv[3],argv[5],NULL,0,ProjPath);
                }
                else if(!strcmp(argv[4],"-c")){
                    tagging(argv[3],NULL,argv[5],0,ProjPath);
                }
            }
            else if(argc==7){
                if(!strcmp(argv[4],"-m") && !strcmp(argv[6],"-f")){
                    tagging(argv[3],argv[5],NULL,1,ProjPath);
                }
                else if(!strcmp(argv[4],"-c") && !strcmp(argv[6],"-f")){
                    tagging(argv[3],NULL,argv[5],1,ProjPath);
                }
            }
            else if(argc==8 && !strcmp(argv[4],"-m") && !strcmp(argv[6],"-c"))
                    tagging(argv[3],argv[5],argv[7],0,ProjPath);
            else if(argc==9 && !strcmp(argv[4],"-m") && !strcmp(argv[6],"-c") &&!strcmp(argv[8],"-f"))
                    tagging(argv[3],argv[5],argv[7],1,ProjPath);
        }
        else if (argc ==4 && !strcmp(argv[2], "show")) {
            Show_tag(argv[3], ProjPath);
        }
        else 
            INVALID_CMD;
    }

    else
        INVALID_CMD;
    return 0;
}