#define _GNU_SOURCE
#include "functions.h"
bool cherrt=true;


int IsValid(char *command)
{
    if (!strcmp(command, "neogit pre-commit hooks list"))
        return 1;
    if (!strcmp(command, "neogit pre-commit applied hooks"))
        return 1;
    if (!strncmp(command, "neogit pre-commit add hook ", 27))
        return 1;
    if (!strncmp(command, "neogit pre-commit remove hook ", 30))
        return 1;
    if (!strcmp(command, "neogit pre-commit"))
        return 1;
    if (!strcmp(command, "neogit pre-commit -u"))
        return 1;
    if (!strncmp(command, "neogit pre-commit -f ", 21))
        return 1;
    if (!strncmp(command, "neogit grep -f ", 15))
        return 1;
    if (!strncmp(command, "neogit revert ", 14))
        return 1;
    if (!strncmp(command, "neogit tag", 10))
        return 1;
    if (!strcmp(command, "neogit tree"))
        return 1;
    if (!strncmp(command, "neogit stash push", 17))
        return 1;
    if (!strncmp(command, "neogit stash show ", 18))
        return 1;
    if (!strncmp(command, "neogit stash branch ", 20))
        return 1;
    if (!strcmp(command, "neogit stash drop"))
        return 1;
    if (!strcmp(command, "neogit stash clear"))
        return 1;
    if (!strcmp(command, "neogit stash pop"))
        return 1;
    if (!strcmp(command, "neogit stash list"))
        return 1;
    if (!strncmp(command, "neogit diff -f ", 15))
        return 1;
    if (!strncmp(command, "neogit diff -c ", 15))
        return 1;
    if (!strncmp(command, "neogit merge -b ", 16))
        return 1;
    if (!strncmp(command, "neogit config -global user.name ", 32))
        return 1;
    if (!strncmp(command, "neogit config -global user.email ", 33))
        return 1;
    if (!strncmp(command, "neogit config user.name ", 24))
        return 1;
    if (!strncmp(command, "neogit config user.email ", 25))
        return 1;
    if (!strncmp(command, "neogit config alias.", 20))
        return 1;
    if (!strncmp(command, "neogit config -global alias.", 29))
        return 1;
    if (!strcmp(command, "neogit init"))
        return 1;
    if (!strncmp(command, "neogit add ", 11))
        return 1;
    if (!strncmp(command, "neogit reset ", 13))
        return 1;
    if (!strcmp(command, "neogit status"))
        return 1;
    if (!strncmp(command, "neogit commit -m ", 17))
        return 1;
    if (!strncmp(command, "neogit commit -s ", 17))
        return 1;
    if (!strncmp(command, "neogit set -m ", 14))
        return 1;
    if (!strncmp(command, "neogit replace -m ", 18))
        return 1;
    if (!strncmp(command, "neogit remove -s ", 17))
        return 1;
    if (!strncmp(command, "neogit log", 10))
        return 1;
    if (!strncmp(command, "neogit checkout ", 16))
        return 1;
    if (!strncmp(command, "neogit branch", 13))
        return 1;
    return 0;
}

int IsAlias(char *command, char *ProjPath)
{
    char gtmp[PATH_MAX];
    strcpy(gtmp,"/home/zahra/.neogitconfig/alias");
    if (access(gtmp , F_OK) != 0)
        mkdir(gtmp, 0755);
    char ch[100];
    sprintf(ch,"%s.txt",command);
    DIR *globDir = opendir(gtmp);
    char globAD[PATH_MAX];
    struct dirent *entry;
    while ((entry = readdir(globDir)) != NULL)
        if (!strcmp(entry->d_name, ch))
            sprintf(globAD, "%s/%s", gtmp, entry->d_name);
    closedir(globDir);
    char ltmp[PATH_MAX];
    snprintf(ltmp, sizeof(ltmp), "%s/.neogit/alias", ProjPath);
    DIR *locDir = opendir(ltmp);
    char locAD[PATH_MAX];
    while ((entry = readdir(locDir)) != NULL)
        if (!strcmp(entry->d_name, ch))
            sprintf(locAD, "%s/%s", ltmp, entry->d_name);
    closedir(locDir);
    FILE *f;
    if (globAD[0] != 0 && locAD[0] != 0)
    {
        switch (compareFileTimes(globAD, locAD))
        {
        case 1:
            f = fopen(globAD, "r");
            break;
        case 2:
            f = fopen(locAD, "r");
            break;
        }
    }
    else if (globAD[0] != 0)
        f = fopen(globAD, "r");
    else if (locAD[0] != 0)
        f = fopen(locAD, "r");
    else
        return 0;
    char line[1024];
    fgets(line, sizeof(line), f);
    fclose(f);
    system(line);
    return 1;
}

// 1 if .neogit exists, 0 if not//
int repo_place(char *path)
{
    if (!path || !(*path))
        return 0;
    DIR *dir = opendir(path);
    struct dirent *entry;
    if (dir == NULL)
        return 0;

    while ((entry = readdir(dir)) != NULL)
        if (!strcmp(entry->d_name, ".neogit"))
            return 1;
    closedir(dir);
    path[strlen(path)] = '\0';

    char *lastSlash = strrchr(path, '/');
    if (lastSlash != NULL)
        *lastSlash = '\0';

    return repo_place(path);
}
 
void ChangeConfig(char *path, char info[], char *type)
{
    if (!strcmp(type, "name"))
    {
        FILE *f = fopen(path, "r");
        char *line = malloc(1000);
        size_t n = 0;
        while (getline(&line, &n, f) != -1)
            if (strncmp(line, "email =", 7) == 0)
                break;
        fclose(f);
        f = fopen(path, "w");
        fprintf(f, "name =%s\n%s", info, line);
        free(line);
        fclose(f);
        printf("username added!\n");
    }
    else if (!strcmp(type, "email"))
    {
        FILE *f = fopen(path, "r");
        char *line = malloc(1000);
        size_t n = 0;
        while (getline(&line, &n, f) != -1)
            if (strncmp(line, "name =", 6) == 0)
                break;
        fclose(f);
        f= fopen(path, "w");
        fprintf(f, "%semail =%s\n", line, info);
        free(line);
        fclose(f);
        printf("email added!\n");
    }
}

int init()
{
    char cur_dir[PATH_MAX];
    getcwd(cur_dir, sizeof(cur_dir));
    if (repo_place(cur_dir))
    {
        printf("Repository already exists!\n");
        return 0;
    }
    getcwd(cur_dir, sizeof(cur_dir));
    strcat(cur_dir, "/.neogit");
    mkdir(cur_dir, 0755);
    strcat(cur_dir, "/config.txt");
    if (access("/home/zahra/.neogitconfig/info.txt", F_OK))
    {
        if (access("/home/zahra/.neogitconfig", F_OK) != 0)
            mkdir("/home/zahra/.neogitconfig", 0755);
        CreateFile("/home/zahra/.neogitconfig/info.txt", "name =\nemail =\n");
    }
    bash("copy","/home/zahra/.neogitconfig/info.txt",cur_dir);
    CreateFile(".neogit/branch.txt", "master_0_0\n");
    CreateFile(".neogit/head.txt", "master\n");               
    CreateFile(".neogit/current_IDs.txt", "20000\n10000\n0\n"); 

    mkdir(".neogit/.staged", 0755);
    mkdir(".neogit/.records", 0755);
    mkdir(".neogit/applied_hook",0755);
    mkdir("/home/zahra/.neogitconfig/alias", 0755); 
    mkdir(".neogit/tags", 0755);
    mkdir(".neogit/.stash", 0755);
    mkdir(".neogit/alias", 0755);
    mkdir(".neogit/shortcuts", 0755);
    mkdir(".neogit/commits", 0755);
    
    return 1;
}

// 0 if different, 1 if same//
int Difference_file(char *path1, char *path2) 
{
    int c1, c2;
    FILE *f1 = fopen(path1, "rb");
    if (f1 == NULL)
        return 0;
    FILE *f2 = fopen(path2, "rb");
    if (f2 == NULL){
        fclose(f1);
        return 0;
    }
    while ((c1 = fgetc(f1)) != EOF && (c2 = fgetc(f2)) != EOF)
        if (c1 != c2){
            fclose(f1);
            fclose(f2);
            return 0;
        }
    c2 = fgetc(f2);
    if (c1 != c2){
        fclose(f1);
        fclose(f2);
        return 0;
    }
    fclose(f1);
    fclose(f2);
    return 1;
}

void Show_folder_n(char* ProjPath, char* curadd, int depth,bool check)
{
    if (depth== 0)
        return;
    DIR *cur_dir = opendir(curadd);
    struct dirent *entry;
    while ((entry = readdir(cur_dir)) != NULL)
    {
        if ( entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name,"..") && strcmp(entry->d_name,".neogit"))
        {
            strcat(curadd, "/");
            strcat(curadd, entry->d_name);
            printf("\n");
            printf("In folder: " _SGR_BOLD _SGR_CYANF "%s : \n"_SGR_RESET, entry->d_name);
            Show_folder_n(ProjPath, curadd, depth - 1, true);
        }
        else if ( entry->d_type != DT_DIR)
        {
            char filePath[PATH_MAX];
            sprintf(filePath,"%s/%s",curadd,entry->d_name);
            char SPath[PATH_MAX];
            sprintf(SPath,"%s/.neogit/.staged%s/%s",ProjPath,curadd + strlen(ProjPath),entry->d_name);
            printf("%s", entry->d_name);
            if (!Difference_file(SPath, filePath))
            {
                printf(_SGR_REDF _SGR_BOLD "\tU\n" _SGR_RESET);
                check = false;
            }
            else
                printf(_SGR_GREENF _SGR_BOLD "\tS\n" _SGR_RESET);
        }
    }
    if (check)
        printf("  Directory is " _SGR_GREENF "staged\n\n" _SGR_RESET);
    else
        printf("  Directory is " _SGR_REDF "not staged\n\n" _SGR_RESET);
    while (curadd[strlen(curadd) - 1] != '/')
        curadd[strlen(curadd) - 1] = '\0';
    curadd[strlen(curadd) - 1] = '\0';
    closedir(cur_dir);
    return;
}
//////////////
void CreateFile(char Name[], char Text[])
{
    FILE *file = fopen(Name, "w");
    if (Text)
        fputs(Text, file);
    fclose(file);
    return;
}

void bash(char* type,char* file_src, char* file_dest){
    if(!strcmp(type,"remove")){
        FILE* f = fopen("/home/zahra/tmp.sh", "w");
        fprintf(f, "rm -r %s", file_src);
        fclose(f);
        chmod("/home/zahra/tmp.sh", 0x777);
        system("/home/zahra/tmp.sh");
        remove("/home/zahra/tmp.sh");
        return;
    }
    else if(!strcmp(type,"copy")){
         FILE *f = fopen("/home/zahra/tmp.sh", "w");
        fprintf(f, "cp -r %s %s", file_src, file_dest);
        fclose(f);
        chmod("/home/zahra/tmp.sh", 0x777);
        system("/home/zahra/tmp.sh");
        remove("/home/zahra/tmp.sh");
        return;
    }
}
/* AddID \n CommitID \n CurrentID \n */
int get_ID(char *ProjPath,char* type)
{   char path[PATH_MAX];
    sprintf(path, "%s/.neogit/current_IDs.txt",ProjPath);
    int ID = 0;
    char temp[10];
    FILE *current_IDs = fopen(path, "r");
    if(!strcmp(type,"commit")){
        fgets(temp, 10, current_IDs);
        fgets(temp, 10, current_IDs);
        for (int i = 0; i < strlen(temp) - 1; i++)
            ID = (ID *10) +(temp[i] - '0');
        fclose(current_IDs);
        return ID;
    }
    else if(!strcmp(type,"add")){
        fgets(temp, 10, current_IDs);
        for (int i = 0; i < strlen(temp) - 1; i++)
            ID = (ID * 10)+(temp[i] - '0');
        fclose(current_IDs);
        return ID;
    }
}

void change_ID(char *ProjPath, char sign,char* type)
{   char Path[PATH_MAX];
    sprintf(Path, "%s/.neogit/current_IDs.txt",ProjPath);
    FILE *current_IDs = fopen(Path, "r");
    char addID[100];
    char commitID[150];
    char commitPt[100];
    fgets(addID, 100, current_IDs);
    fgets(commitID, 150, current_IDs);
    fgets(commitPt, 100, current_IDs);
    fclose(current_IDs);
    if(!strcmp(type,"add")){
        int index = strlen(addID) - 2;
        if (sign == '+'){
            while (addID[index] == '9'){
                addID[index] = '0';
                index--;
            }
            addID[index]++;
        }
        else{
            while (addID[index] == '0'){
                addID[index] = '9';
                index--;
            }
            addID[index]--;
        }
    }
    else if(!strcmp(type,"commit")){
        int index = strlen(commitID) - 2;
        if (sign == '+'){
            while (commitID[index] == '9'){
                commitID[index] = '0';
                index--;
            }
            commitID[index]++;
        }
        else{
            while (commitID[index] == '0'){
                commitID[index] = '9';
                index--;
            }
            commitID[index]--;
        }
    }
    current_IDs = fopen(Path, "w");
    fputs(addID, current_IDs);
    fputs(commitID, current_IDs);
    fputs(commitPt, current_IDs);
    fclose(current_IDs);
    return;
}

void Staging(char *Relpath, char *ProjPath)
{
    char path[PATH_MAX];
    getcwd(path, sizeof(path));
    strcat(path, "/");
    strcat(path, Relpath);
    if (access(path, F_OK) != 0){
        printf("Path not found!\n");
        return;
    }
    char stagepath[PATH_MAX];
    strcpy(stagepath, ProjPath);
    strcat(stagepath, "/.neogit/.staged");
    strcat(stagepath, path + strlen(ProjPath));
    /*delete the file ,check if folder exists,if not ,mkdir folder*/
    while (stagepath[strlen(stagepath) - 1] != '/')
        stagepath[strlen(stagepath) - 1] = '\0';
    stagepath[strlen(stagepath) - 1] = '\0';
    if (access(stagepath, F_OK) != 0)
        mkdir(stagepath, 0755);
    bash("copy",path,stagepath);
    printf("File added!\n");
    return;
}

void newRecords(char *ProjPath)
{
    char Spath[PATH_MAX];
    strcpy(Spath, ProjPath);
    strcat(Spath, "/.neogit/.staged");
    char Rpath[PATH_MAX];
    sprintf(Rpath, "%s/.neogit/.records",ProjPath);
    bash("copy",Spath,Rpath);
    int ID = get_ID(ProjPath,"add");
    char name[PATH_MAX];
    sprintf(name, "%s/%d", Rpath, ID);
    strcat(Rpath, "/.staged");
    rename(Rpath, name);
    change_ID(ProjPath, '+',"add");
}

void redo(char *ProjPath, char *stage)
{
    DIR *cur_dir = opendir(stage);
    struct dirent *entry;
    while ((entry = readdir(cur_dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
            char wpath[PATH_MAX];
            strcpy(wpath, ProjPath);
            strcat(wpath, stage + strlen(ProjPath) + 16); //deleting /.neogit/.staged
            strcat(wpath, "/");
            strcat(wpath, entry->d_name);
            char Spath[PATH_MAX];
            strcpy(Spath, stage);
            strcat(Spath, "/");
            strcat(Spath, entry->d_name);
            if (access(wpath, F_OK) != 0){
                bash("remove",Spath,NULL);
                return;
            }
            strcat(stage, "/");
            strcat(stage, entry->d_name);
            redo(ProjPath, stage);
        }
        else if (entry->d_type != DT_DIR){
            char wpath[PATH_MAX];
            strcpy(wpath, ProjPath);
            strcat(wpath, stage + strlen(ProjPath) + 16); //deleting /.neogit/.staged
            strcat(wpath, "/");
            strcat(wpath, entry->d_name);
            char Spath[PATH_MAX];
            strcpy(Spath, stage);
            strcat(Spath, "/");
            strcat(Spath, entry->d_name);
            if (access(wpath, F_OK) != 0){
                remove(Spath);
                return;
            }
            bash("copy",wpath,Spath);
        }
    }
    while (stage[strlen(stage) - 1] != '/')
        stage[strlen(stage) - 1] = '\0';
    stage[strlen(stage) - 1] = '\0';
    closedir(cur_dir);
    return;
}

void unstaging(char *name, char *ProjPath)
{
    char path[PATH_MAX];
    getcwd(path, sizeof(path));
    char stageadd[PATH_MAX];
    sprintf(stageadd, "%s/.neogit/.staged%s/%s", ProjPath, path + strlen(ProjPath), name);
    if (access(stageadd, F_OK) != 0)
    {
        printf("File isn't staged!\n");
        return;
    }
    bash("remove",stageadd,NULL);

    printf("File removed from staging area!\n");
    return;
}

void resetundo(char *ProjPath)
{
    //getting last record and minus id
    change_ID(ProjPath, '-',"add");
    int ID = get_ID(ProjPath,"add");
    if(ID<20000) {
        printf("you should commit s.th first then try reset undo\n");
        return;
    }
    char stagepath[PATH_MAX];
    //remove last record
    sprintf(stagepath, "%s/.neogit/.records/%d", ProjPath, ID);
    bash("remove",stagepath,NULL);
    //removr current stage
    sprintf(stagepath, "%s/.neogit/.staged/", ProjPath);
    bash("remove",stagepath,NULL);
    //copying last record to address stage and rename it  to stage address
    sprintf(stagepath, "%s/.neogit/.records/%d", ProjPath, --ID);
    char new_stage[PATH_MAX];
    sprintf(new_stage, "%s/.neogit", ProjPath);
    bash("copy",stagepath,new_stage);
    sprintf(stagepath, "%s/.neogit/%d", ProjPath, ID);
    sprintf(new_stage, "%s/.neogit/.staged", ProjPath);
    rename(stagepath, new_stage);
    printf("Reset undone!\n");
    return;
}

// 1 if first one is newer than second one, 2 if reverse,0 if is the same
int compareFileTimes(const char *file1, const char *file2)
{
    struct stat stat1, stat2;
    stat(file1, &stat1);
    stat(file2, &stat2);
    if (stat1.st_mtime > stat2.st_mtime)
        return 1;
    else if (stat1.st_mtime < stat2.st_mtime)
        return 2;
    else
        return 0;
}

void BranchUpdate(char *ProjPath, char *branch, int ID)
{
    char tmp[1000];
    strncpy(tmp, branch, strlen(branch));
    strcat(tmp, "_");
    char branch_adrs[PATH_MAX];
    sprintf(branch_adrs, "%s/.neogit/branch.txt", ProjPath);
    FILE *b = fopen(branch_adrs, "r");
    char line[100][1000];
    int len = -1;
    while (fgets(line[++len], sizeof(line[len]), b) != NULL);
    fclose(b);
    b = fopen(branch_adrs, "w");
    for (int i = 0; i < len; i++)
    {
        if (!strncmp(line[i], tmp, strlen(tmp)))
        {
            int tok = strlen(line[i]) - 1;
            while (line[i][tok] != '_')
                tok--;
            sprintf(line[i] + tok, "_%d\n", ID);
        }
        fputs(line[i], b);
    }
    fclose(b);
    return;
}

void makeln(char ProjPath[], char Src[], char Dest[])
{
    DIR *cur_dir = opendir(Src);
    struct dirent *entry;
    while ((entry = readdir(cur_dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){  
            strcat(Dest, "/");
            strcat(Dest, entry->d_name);
            strcat(Src, "/");
            strcat(Src, entry->d_name);
            if(access(Dest,F_OK) !=0)
                mkdir(Dest,0755);
            makeln(ProjPath, Src, Dest);
        }
        else if (entry->d_type != DT_DIR){
            char Wpath[PATH_MAX];
            sprintf(Wpath, "%s%s", ProjPath, Src + strlen(ProjPath) + 22); //deleting /.neogit/commits/5id
            strcat(Wpath, "/");
            strcat(Wpath, entry->d_name);
            strcat(Src, "/");
            strcat(Src, entry->d_name);
            strcat(Dest, "/");
            strcat(Dest, entry->d_name);
            if (Difference_file(Src, Wpath)){
                bash("copy",Src,Dest);
            }
            while (Dest[strlen(Dest) - 1] != '/')
                Dest[strlen(Dest) - 1] = '\0';
            Dest[strlen(Dest) - 1] = '\0';
            while (Src[strlen(Src) - 1] != '/')
                Src[strlen(Src) - 1] = '\0';
            Src[strlen(Src) - 1] = '\0';
        }
    }
    while (Src[strlen(Src) - 1] != '/')
        Src[strlen(Src) - 1] = '\0';
    Src[strlen(Src) - 1] = '\0';
    while (Dest[strlen(Dest) - 1] != '/')
        Dest[strlen(Dest) - 1] = '\0';
    Dest[strlen(Dest) - 1] = '\0';
    closedir(cur_dir);
    return;
}

void shortcut(char *message, char *shortcut, char *ProjPath)
{
    char path[PATH_MAX];
    sprintf(path, "%s/.neogit/shortcuts/%s.txt", ProjPath, shortcut);
    FILE *f = fopen(path, "w");
    fputs(message, f);
    fclose(f);
    return;
}

mode_t similarmode(char *file1, char *file2)
{
    struct stat stat1, stat2;
    stat(file1, &stat1);
    stat(file2, &stat2);
    return stat1.st_mode == stat2.st_mode;
}
void Log(char *ProjPath, int n)
{
    char commits[PATH_MAX];
    sprintf(commits, "%s/.neogit/commits", ProjPath);
    int ID = get_ID(ProjPath,"commit") - 1;
    for (int i = ID; i >= ID - n + 1; i--)
    {
        char adrs[PATH_MAX];
        sprintf(adrs, "%s/%d", commits, i);
        int num = 0;
        struct stat folder;
        stat(adrs, &folder);
        time_t crt = folder.st_ctime;
        //because in the function the address will be changed
        char curadd[PATH_MAX];
        strcpy(curadd, adrs);
        number_of_files(curadd, &num);
        strcat(adrs, "/info.txt");
        FILE *f = fopen(adrs, "r");
        char line[1024];
        while (fgets(line, sizeof(line), f) != NULL)
            printf("%s", line);
        fclose(f);
         printf("Number of commited files = " _SGR_YELLOWF "%d"_SGR_RESET, num);
        printf("\ncommit ID =" _SGR_MAGENF "%d\n" _SGR_RESET "time of commit =" _SGR_CYANF "%s\n" _SGR_RESET, i, ctime(&crt));
       
    }
}

void number_of_files(char *curadd, int *num)
{
    DIR *cur_dir = opendir(curadd);
    struct dirent *entry;
    while ((entry = readdir(cur_dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            strcat(curadd, "/");
            strcat(curadd, entry->d_name);
            number_of_files(curadd, num);
        }
        else if (entry->d_type != DT_DIR)
            (*num)++;
    }
    while (curadd[strlen(curadd) - 1] != '/')
        curadd[strlen(curadd) - 1] = '\0';
    curadd[strlen(curadd) - 1] = '\0';
    closedir(cur_dir);
    return;
}

void Logplus(char *ProjPath, char *TYPEt, char *WORDt)
{   
    if(!strcmp(TYPEt,"Branch")){
        char add[PATH_MAX];
        sprintf(add,"%s/.neogit/branch.txt",ProjPath);
        FILE* gg=fopen(add,"r");
        bool c=false;
        char line[100];
        while(fgets(line,sizeof(line),gg)){
            if(!strncmp(line,WORDt,strlen(WORDt)))
            c=true;
        }
        if(c==false){
            printf("Branch doesn't exists\n");
            fclose(gg);
            return;
        }
    }
    char commits[PATH_MAX];
    sprintf(commits, "%s/.neogit/commits", ProjPath);
    int ID = get_ID(ProjPath,"commit") - 1;
    for (int i = ID; i >= 10000; i--)
    {
        char adrs[PATH_MAX];
        sprintf(adrs, "%s/%d", commits, i);

        int num = 0;
        char curAddress[PATH_MAX];
        strcpy(curAddress, adrs);
        number_of_files(curAddress, &num);
        struct stat folder;
        stat(adrs, &folder);
        time_t crt = folder.st_ctime;
    
        strcat(adrs, "/info.txt");
        FILE *f = fopen(adrs, "r");
        char line[1024];

        while (fgets(line, sizeof(line), f) != NULL)
        {
            if (!strncmp(line, TYPEt, strlen(TYPEt))){
                if (!strncmp(line + strlen(TYPEt) + 2, WORDt, strlen(WORDt)) )
                {   
                    fclose(f);
                    f = fopen(adrs, "r");
                    char tmp[1024];
                    while (fgets(tmp, sizeof(tmp), f) != NULL)
                        printf("%s", tmp);
                    fclose(f);
                    printf("Number of commited files = " _SGR_YELLOWF "%d"_SGR_RESET, num);
                    printf("\nCommit ID =" _SGR_MAGENF "%d\n" _SGR_RESET "Commit Time =" _SGR_CYANF "%s\n" _SGR_RESET, i, ctime(&crt));
                   
                    break;
                }
            }
        }
        fclose(f);
    }
}
//remember not to test blobblob
void log_message(char *ProjPath, char *target)
{   char type[20];
    strcpy(type,"Message");
    char commits[PATH_MAX];
    sprintf(commits, "%s/.neogit/commits", ProjPath);
    int ID = get_ID(ProjPath,"commit") - 1;
    for (int i = ID; i >= 10000; i--)
    {
        char adrs[PATH_MAX];
        sprintf(adrs, "%s/%d", commits, i);
        int num = 0;
        char curAddress[PATH_MAX];
        strcpy(curAddress, adrs);
        //becuase of the change in the address in func
        number_of_files(curAddress, &num);
        struct stat directory;
        stat(adrs, &directory);
        time_t crt = directory.st_ctime;
        strcat(adrs, "/info.txt");
        FILE *f = fopen(adrs, "r");
        char line[1000];
        while (fgets(line, sizeof(line), f) != NULL)
        {
            if (!strncmp(line, type, 7))
                if (strstr(line + 9, target) != NULL)
                {
                    fclose(f);
                    FILE *g = fopen(adrs, "r");
                    char lin[1000];
                    while (fgets(lin, sizeof(lin), g) != NULL)
                        printf("%s", lin);
                    fclose(g);
                    printf("Number of commited files = " _SGR_YELLOWF "%d"_SGR_RESET, num);
                    printf("\nCommit ID =" _SGR_MAGENF "%d\n" _SGR_RESET "Commit Time =" _SGR_CYANF "%s\n" _SGR_RESET, i, ctime(&crt));
                  
                    break;
                }
        }
    }
}

void log_time(char *ProjPath, char *time, char sign)
{
    char commits[PATH_MAX];
    sprintf(commits, "%s/.neogit/commits", ProjPath);
    int ID = get_ID(ProjPath,"commit") - 1;
    for (int i = ID; i >= 10000; i--)
    {
        char adrs[PATH_MAX];
        sprintf(adrs, "%s/%d", commits, i);
        int num = 0;
        char curadd[PATH_MAX];
        strcpy(curadd, adrs);
        number_of_files(curadd, &num);
        struct stat directory;
        stat(adrs, &directory);
        time_t crt = directory.st_ctime;
        strcat(adrs, "/info.txt");
        if (sign == '-') // before
        {
            if(compare_time_log(adrs, time) == 1){
                FILE *f = fopen(adrs, "r");
                char line[1024];
                while (fgets(line, sizeof(line), f) != NULL)
                    printf("%s", line);
                fclose(f);
                printf("Number of commited files = " _SGR_YELLOWF "%d"_SGR_RESET, num);
                printf("\nCommit ID =" _SGR_MAGENF "%d\n" _SGR_RESET "Commit Time =" _SGR_CYANF "%s\n" _SGR_RESET, i, ctime(&crt));
            }
        }
        else if (sign == '+') // since
        {
            if(compare_time_log(adrs, time) == 2){
                FILE *f = fopen(adrs, "r");
                char line[1024];
                while (fgets(line, sizeof(line), f) != NULL)
                    printf("%s", line);
                fclose(f);
                printf("Number of commited files = " _SGR_YELLOWF "%d\n"_SGR_RESET, num);
                printf("\nCommit ID =" _SGR_MAGENF "%d\n" _SGR_RESET "Commit Time =" _SGR_CYANF "%s\n" _SGR_RESET, i, ctime(&crt));
            }
        }
    }
}
// 1 for before,2 for since ,0 are equal
int compare_time_log(const char *filename, const char *time)
{
    struct tm input_time;
    strptime(time, "%Y/%m/%d %H:%M:%S", &input_time);
    time_t inputtime = mktime(&input_time);
    struct stat file;
    stat(filename, &file);
    time_t file_time = file.st_ctime;
    if (inputtime < file_time)
        return 2;
    else if (inputtime > file_time)
        return 1;
    else
    return 0;
}

void Status(char *ProjPath)
{      
    char adrs[PATH_MAX];
    sprintf(adrs,"%s/.neogit/current_IDs.txt",ProjPath);
    char line[100];
    FILE* f=fopen(adrs,"r");
    fgets(line,sizeof(line),f);
    fgets(line,sizeof(line),f);
    fgets(line,sizeof(line),f);
    fclose(f);
    if(line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';
    if(!strcmp(line,"0")) {
        printf("make commits first then status");
        return;
    }
    char com_path[PATH_MAX];
    sprintf(com_path, "%s/.neogit/commits/%s", ProjPath, line);
    DFSCommit(ProjPath, com_path);
    //because in func the second address will be changed
    char curadd[PATH_MAX];
    strcpy(curadd, ProjPath);
    DFSWD(ProjPath, curadd);
}

void DFSCommit(char *ProjPath, char *address)
{
    DIR *cm = opendir(address);
    struct dirent *entry;
    while ((entry = readdir(cm)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit"))
        {
            strcat(address, "/");
            strcat(address, entry->d_name);
            DFSCommit(ProjPath, address);
        }
        //in id folder of dir commits we keep info in info.txt,skip it
        else if (entry->d_type != DT_DIR && strcmp(entry->d_name, "info.txt"))
        {
            strcat(address, "/");
            strcat(address, entry->d_name);
            char Wpath[PATH_MAX];
            sprintf(Wpath, "%s%s", ProjPath, address + strlen(ProjPath) + 22); //skip ../.neogit/commits/5id
            char stagepath[PATH_MAX];
            sprintf(stagepath, "%s/.neogit/.staged%s", ProjPath, address + strlen(ProjPath) + 22); //skip ../.neogit/commits/5id
            //if it doesn't exist in wd it's d,if it does&different it's m,if it just exists in wd and not commit it's a
            if (access(Wpath, F_OK) == 0)
            {
                if (!similarmode(Wpath, address))
                    printf(_SGR_ITALIC "%s\t\t"  _SGR_BOLD _SGR_GREENF "T" _SGR_RESET "\n", Wpath + strlen(ProjPath));
                if (!Difference_file(address, Wpath))
                {
                    if (access(stagepath, F_OK) == 0)
                        printf("%s\t\t" _SGR_BOLD _SGR_MAGENF "+M" _SGR_RESET "\n", Wpath + strlen(ProjPath));
                    else
                        printf("%s\t\t" _SGR_BOLD _SGR_MAGENF "-M" _SGR_RESET "\n", Wpath + strlen(ProjPath));
                }
            }
            else
            {
                if (access(stagepath, F_OK) == 0)
                    printf("%s\t\t" _SGR_BOLD _SGR_REDF "+D" _SGR_RESET "\n", Wpath + strlen(ProjPath));
                else
                    printf("%s\t\t" _SGR_BOLD _SGR_REDF "-D" _SGR_RESET "\n", Wpath + strlen(ProjPath));
            }

            while (address[strlen(address) - 1] != '/')
                address[strlen(address) - 1] = '\0';
            address[strlen(address) - 1] = '\0';
        }
    }
    while (address[strlen(address) - 1] != '/')
        address[strlen(address) - 1] = '\0';
    address[strlen(address) - 1] = '\0';
    closedir(cm);
    return;
}

void DFSWD(char *ProjPath, char *address)
{
    DIR *cm = opendir(address);
    struct dirent *entry;
    while ((entry = readdir(cm)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit"))
        {
            strcat(address, "/");
            strcat(address, entry->d_name);
            DFSWD(ProjPath, address);
        }
        else if (entry->d_type != DT_DIR)
        {
            strcat(address, "/");
            strcat(address, entry->d_name);
            char com_path[PATH_MAX];
            int ID = get_ID(ProjPath,"commit") - 1;
            sprintf(com_path, "%s/.neogit/commits/%d%s", ProjPath, ID, address + strlen(ProjPath));
            char stagepath[PATH_MAX];
            sprintf(stagepath, "%s/.neogit/.staged%s", ProjPath, address + strlen(ProjPath));
            if (access(com_path, F_OK) != 0) // A
            {
                if (access(stagepath, F_OK) == 0)
                    printf("%s\t\t" _SGR_BOLD _SGR_YELLOWF "+A" _SGR_RESET "\n", address + strlen(ProjPath));
                else
                    printf("%s\t\t" _SGR_BOLD _SGR_YELLOWF "-A" _SGR_RESET "\n", address + strlen(ProjPath));
            }
            while (address[strlen(address) - 1] != '/')
                address[strlen(address) - 1] = '\0';
            address[strlen(address) - 1] = '\0';
        }
    }
    while (address[strlen(address) - 1] != '/')
        address[strlen(address) - 1] = '\0';
    address[strlen(address) - 1] = '\0';
    closedir(cm);
    return;
}

void makebranch(char *name, char *ProjPath)
{
    //check if branch alredy exists/if not, get the last commit id of cur branch and it's the first of new branch
    char branch_add[PATH_MAX];
    strcpy(branch_add,ProjPath);
    strcat(branch_add,"/.neogit/branch.txt");
    FILE *branch_file = fopen(branch_add, "r");
    char line[1000];
    while (fgets(line, sizeof(line), branch_file) != NULL)
    {
        strtok(line, "_");
        if (!strcmp(line, name))
        {
            printf("this branch exists!\n");
            fclose(branch_file);
            return;
        }
    }
    fclose(branch_file);
    ///////////////////////////
    char cur_head_add[PATH_MAX];
    strcpy(cur_head_add,ProjPath);
    strcat(cur_head_add,"/.neogit/head.txt");
    FILE *head_file = fopen(cur_head_add, "r");
    char HEAD[1000];
    fgets(HEAD, sizeof(HEAD), head_file); if( HEAD[strlen(HEAD)-1]=='\n') HEAD[strlen(HEAD)-1]='\0';
    fclose(head_file);
    ////////////////////////////
    branch_file = fopen(branch_add, "r");
    char *ID;
    while (fgets(line, sizeof(line), branch_file) != NULL)
    {
        strtok(line, "_");
        if (!strcmp(line, HEAD))
        {
            strtok(NULL, "_");//start of branch
            ID = strtok(NULL, "\n");//end of branch;cur commit
            break;
        }
    }
    fclose(branch_file);
    //////////////////////////
    branch_file = fopen(branch_add, "a");
    fprintf(branch_file, "%s_%s_%s\n", name, ID, ID);
    fclose(branch_file);
    printf("branch"_SGR_MAGENF" %s"_SGR_RESET "created !\n", name);
    return;
}
//additional print for head:)
void show_branch(char *ProjPath)
{
    char head_add[PATH_MAX];
    strcpy(head_add,ProjPath);
    strcat(head_add,"/.neogit/head.txt");
    FILE *head_file = fopen(head_add, "r");
    char HEAD[1000];
    fgets(HEAD, sizeof(HEAD), head_file); if (HEAD[strlen(HEAD) - 1] == '\n')  HEAD[strlen(HEAD) - 1] = '\0'; fclose(head_file);

    char branch_add[PATH_MAX];
    strcpy(branch_add,ProjPath);
    strcat(branch_add,"/.neogit/branch.txt");
    FILE *branch_file = fopen(branch_add, "r");
    char b_name[1000];
    while (fgets(b_name, sizeof(b_name), branch_file) != NULL)
    {
        strtok(b_name, "_");
        printf(_SGR_CYANF"\t%s"_SGR_RESET, b_name);
        if (!strncmp(b_name, HEAD, strlen(HEAD)))
            printf("\t-> " _SGR_REDF "HEAD"_SGR_RESET);
        printf("\n");
    }
    return;
}
/*compares the commit id folder with wd and returns 0 for differents and 1 for similars*/
int WD_DFS_forcheckout(char *ProjPath, char *address, char *ID)
{
    DIR *dir = opendir(address);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit"))
        {
            strcat(address, "/");
            strcat(address, entry->d_name);
            if( WD_DFS_forcheckout(ProjPath, address, ID) == 0)
                return 0;
        }
        else if (entry->d_type != DT_DIR)
        {
            strcat(address, "/");
            strcat(address, entry->d_name);
            char com_adrs[PATH_MAX];
            sprintf(com_adrs, "%s/.neogit/commits/%s%s", ProjPath, ID, address + strlen(ProjPath));
            if (access(com_adrs, F_OK) == 0)
            {
                if (!Difference_file(com_adrs, address))
                {
                    closedir(dir);
                    return 0;
                }
            }
            else
            {
                closedir(dir);
                return 0;
            }
            while (address[strlen(address) - 1] != '/')
                address[strlen(address) - 1] = '\0';
            address[strlen(address) - 1] = '\0';
        }
    }
    while (address[strlen(address) - 1] != '/')
        address[strlen(address) - 1] = '\0';
    address[strlen(address) - 1] = '\0';
    closedir(dir);
    return 1;
}

void Checkout(char *where, char *ProjPath)
{ // branch names mustn't have numbers within
    
    char cur_ID_path[PATH_MAX];
    sprintf(cur_ID_path, "%s/.neogit/current_IDs.txt", ProjPath);
    FILE *curID_file = fopen(cur_ID_path, "r");
    char cur_commitID[10];
    fgets(cur_commitID, 10, curID_file);
    fgets(cur_commitID, 10, curID_file);
    fgets(cur_commitID, 10, curID_file);
    if (cur_commitID[strlen(cur_commitID) - 1] == '\n')  cur_commitID[strlen(cur_commitID) - 1] = '\0';
    fclose(curID_file); 
    char here[1024];
    strcpy(here, ProjPath);
    bool IsEqual = WD_DFS_forcheckout(ProjPath, here, cur_commitID);
    if (!IsEqual){
        printf("There are modified files in working directory so you can't checkout!\n");
        return;
    }
    if (where[0] > '9' || where[0] < '0') Checkout_branch(where,ProjPath);
    else Checkout_ID(where,ProjPath);
    return;
}
void Checkout_branch(char* name,char* ProjPath){   
    //writing the name in head.txt and find the last commit id of branch &checkout there    
        char head_adrs[PATH_MAX];
        sprintf(head_adrs, "%s/.neogit/head.txt", ProjPath);
        FILE *head = fopen(head_adrs, "w");
        fprintf(head, "%s\n",name);
        fclose(head);

        char branch_adrs[PATH_MAX];
        sprintf(branch_adrs, "%s/.neogit/branch.txt", ProjPath);
        FILE *branch_file= fopen(branch_adrs, "r");
        char COMMITID[1000];
        char *ID;
        while (fgets(COMMITID, sizeof(COMMITID), branch_file) != NULL)
        {
            strtok(COMMITID, "_");
            if (!strcmp(COMMITID, name))
            {
                strtok(NULL, "_");
                ID = strtok(NULL, "\n");//the last commit id of this branch
                break;
            }
        }
        fclose(branch_file);
        //deleting wd and copy the id folder 
        DIR *WD = opendir(ProjPath);
        struct dirent *entry;
        while ((entry= readdir(WD)) != NULL)
        {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit"))
            {
                char add[1024];
                sprintf(add, "%s/%s", ProjPath, entry->d_name);
                bash("remove",add,NULL);
            }
        }
        closedir(WD);
        char commit_add[1024];
        sprintf(commit_add, "%s/.neogit/commits/%s", ProjPath, ID);
        if (commit_add[strlen(commit_add) - 1] == '\n') commit_add[strlen(commit_add) - 1] = '\0';
        DIR *Com_Dir = opendir(commit_add);
        while ((entry = readdir(Com_Dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, "info.txt"))
            {
                char add[1024];
                sprintf(add, "%s/%s", commit_add, entry->d_name);
                bash("copy",add,ProjPath);
            }
        }
        closedir(Com_Dir);
        char cur_ID_path[PATH_MAX];
        sprintf(cur_ID_path, "%s/.neogit/current_IDs.txt", ProjPath);
        FILE* cur_IDs = fopen(cur_ID_path, "r");
        char addID[10], commitID[10];
        fgets(addID, 10, cur_IDs);
        fgets(commitID, 10, cur_IDs);
        fclose(cur_IDs);
        cur_IDs = fopen(cur_ID_path, "w");
        fputs(addID, cur_IDs);
        fputs(commitID, cur_IDs);
        fprintf(cur_IDs,"%s\n",ID);
        fclose(cur_IDs);
        return;

}
void Checkout_ID(char* where,char* ProjPath)
{      //first check if the commit id is for the cur branch or not(in info of commit id the branch name was kept at line3)
        //then remove wd and cp commitid folder
        char head_add[PATH_MAX];
        sprintf(head_add, "%s/.neogit/head.txt", ProjPath);
        FILE *head_file = fopen(head_add, "r");
        char cur_head[100];
        fgets(cur_head, sizeof(cur_head), head_file);
        if (cur_head[strlen(cur_head) - 1] == '\n') cur_head[strlen(cur_head) - 1] = '\0';

        char Info_add[PATH_MAX];
        sprintf(Info_add, "%s/.neogit/commits/%s/info.txt", ProjPath, where);
        FILE *info = fopen(Info_add, "r");
        char line[1000];
        fgets(line, sizeof(line), info);
        fgets(line, sizeof(line), info);
        fgets(line, sizeof(line), info);
        strtok(line, "=");
        char *cur_branch = strtok(NULL, "\n");
        if (strcmp(cur_head, cur_branch) != 0)
        {
            printf("you should be on a branch of your commit_id to checkout on that\n");
            return;
        }

        DIR *WD= opendir(ProjPath);
        struct dirent *entry;
        while ((entry = readdir(WD)) != NULL)
        {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit"))
            {
                char add[PATH_MAX];
                sprintf(add, "%s/%s", ProjPath, entry->d_name);
                bash("remove",add,NULL);
            }
        }
        closedir(WD);

        char commit_adrs[PATH_MAX];
        sprintf(commit_adrs, "%s/.neogit/commits/%s", ProjPath, where);
        if (commit_adrs[strlen(commit_adrs) - 1] == '\n') commit_adrs[strlen(commit_adrs) - 1] = '\0';
        DIR *Commit_Dir = opendir(commit_adrs);
        while ((entry = readdir(Commit_Dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, "info.txt"))
            {
                char add[1024];
                sprintf(add, "%s/%s", commit_adrs, entry->d_name);
                bash("copy",add,ProjPath);
            }
        }
        closedir(Commit_Dir);
        char cur_ID_path[PATH_MAX];
        sprintf(cur_ID_path, "%s/.neogit/current_IDs.txt", ProjPath);
        FILE* cur_IDs = fopen(cur_ID_path, "r");
        char addID[10], commitID[10];
        fgets(addID, 10, cur_IDs);
        fgets(commitID, 10, cur_IDs);
        fclose(cur_IDs);
        cur_IDs = fopen(cur_ID_path, "w");
        fputs(addID, cur_IDs);
        fputs(commitID, cur_IDs);
        fprintf(cur_IDs,"%s\n",where);
        fclose(cur_IDs);
        return;
}
void CheckoutHead(char *where,char* ProjPath)
{
    char head_adrs[PATH_MAX];
    sprintf(head_adrs, "%s/.neogit/head.txt", ProjPath);
    FILE *head_file = fopen(head_adrs, "r");
    char HEAD[1000];
    fgets(HEAD, sizeof(HEAD), head_file);
    if (HEAD[strlen(HEAD) - 1] == '\n') HEAD[strlen(HEAD) - 1] = '\0';
    fclose(head_file);

    char cur_commitID[PATH_MAX];
    sprintf(cur_commitID, "%s/.neogit/current_IDs.txt", ProjPath);
    FILE *id_file = fopen(cur_commitID, "r");
    char COMMITID[1000];
    fgets(COMMITID, sizeof(COMMITID), id_file);
    fgets(COMMITID, sizeof(COMMITID), id_file);
    fgets(COMMITID, sizeof(COMMITID), id_file);
    if (COMMITID[strlen(COMMITID) - 1] == '\n') COMMITID[strlen(COMMITID) - 1] = '\0';
    fclose(id_file);
    //in func the adrs will be changed
    char here[PATH_MAX];
    strcpy(here, ProjPath);
    bool IsEqual = WD_DFS_forcheckout(ProjPath, here, COMMITID);
    if (!IsEqual)
    {
        printf("There are modified files in working directory so you can't checkout!\n");
        return;
    }
    if( strlen(where) ==4){
        char branch_adrs[PATH_MAX];
        sprintf(branch_adrs, "%s/.neogit/branch.txt", ProjPath);
        FILE *branch_file = fopen(branch_adrs, "r");
        char line[100];
        while (fgets(line, sizeof(line), branch_file) != NULL)
        {
            strtok(line, "_");
            if (!strcmp(line, HEAD))
            {
                strtok(NULL, "_");
                strcpy(COMMITID, strtok(NULL, "\n"));
                break;
            }
        }
        fclose(branch_file);
    }
    else
    {
        int n = 0;
        for (int i = 5; i < strlen(where); i++)
            n =n*10+(where[i] - '0');
        char commit_adrs[PATH_MAX];
        sprintf(commit_adrs, "%s/.neogit/commits", ProjPath);
        int comID = get_ID(ProjPath,"commit") - 1;
        int i;
        for (i = comID; i >= 10000; i--)
        {
            char adrs[PATH_MAX];
            sprintf(adrs, "%s/%d/info.txt", commit_adrs, i);
            FILE *info = fopen(adrs, "r");
            char line[1000];
            fgets(line, sizeof(line), info); fgets(line, sizeof(line), info); fgets(line, sizeof(line), info);
            if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
            if (!strcmp(line + 8, HEAD)) // Branch =''
                n--;
            fclose(info);
            if (n == -1)
                break;
        }
        if(n != -1){
            printf("the number entered is more than available branchs of head\n");
            return;
        }
        sprintf(COMMITID, "%d", i);
    }

    DIR *WD = opendir(ProjPath);
    struct dirent *entry;
    while ((entry = readdir(WD)) != NULL)
    {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit"))
        {
            char add[1024];
            sprintf(add, "%s/%s", ProjPath, entry->d_name);
            bash("remove",add,NULL);
        }
    }
    closedir(WD);
    char commitAdd[1024];
    sprintf(commitAdd, "%s/.neogit/commits/%s", ProjPath, COMMITID);
    DIR *Commit_Dir = opendir(commitAdd);
    while ((entry = readdir(Commit_Dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, "info.txt"))
        {
            char add[1024];
            sprintf(add, "%s/%s", commitAdd, entry->d_name);
            bash("copy",add,ProjPath);
        }
    }
    closedir(Commit_Dir);

    FILE *curID_file = fopen(cur_commitID, "r");
    char addID[10], cID[10];
    fgets(addID, 10, curID_file);
    fgets(cID, 10, curID_file);
    fclose(curID_file);
    curID_file = fopen(cur_commitID, "w");
    fputs(addID, curID_file);
    fputs(cID, curID_file);
    fprintf(curID_file,"%s\n",COMMITID);
    fclose(curID_file);

    return;
}
void tagging(char *Tagname, char *Message, char *CommitID, int state, char *ProjPath)
{
    char tag_adrs[PATH_MAX];
    sprintf(tag_adrs, "%s/.neogit/tags/%s.txt", ProjPath, Tagname);
    if (access(tag_adrs, F_OK) == 0 && !state)
    {
        printf("the tag already exists\n");
        return;
    }
    FILE *Tag_file = fopen(tag_adrs, "w");
    char name[100], email[100];
    char globalConfig[PATH_MAX], localConfig[PATH_MAX];
    sprintf(globalConfig, "/home/zahra/.neogitconfig/info.txt");
    sprintf(localConfig, "%s/.neogit/config.txt", ProjPath);
    FILE *config;
    switch (compareFileTimes(globalConfig, localConfig))
    {
    case 1:
        config = fopen(globalConfig, "r");
        break;
    case 2:
        config = fopen(localConfig, "r");
        break;
    }
    fgets(name, sizeof(name), config);
    char *Name = strtok(name, "=");
    Name = strtok(NULL, "\n\r");
    fgets(email, sizeof(email), config);
    char *Email = strtok(email, "=");
    Email = strtok(NULL, "\n\r");
    fclose(config);

    if (CommitID == NULL)
    {
        CommitID = (char *) malloc (10 * sizeof(char));
        char IDpath[1024];
        sprintf(IDpath, "%s/.neogit/current_IDs.txt", ProjPath);
        FILE *ID_f = fopen(IDpath, "r");
        fgets(CommitID, 10, ID_f);
        fgets(CommitID, 10, ID_f);
        fgets(CommitID, 10, ID_f);
        if (CommitID[strlen(CommitID) - 1] == '\n') CommitID[strlen(CommitID) - 1] = '\0';
        fclose(ID_f);
    }
    if (Message == NULL) fprintf(Tag_file, "username=%s\nuseremail =%s\ncommit_ID =%s\nMessage =-\n", Name, Email, CommitID);
    else fprintf(Tag_file, "username=%s\nuseremail =%s\ncommit_ID =%s\nMessage =%s\n", Name, Email, CommitID, Message);
    fclose(Tag_file);

    return;
}
void Show_tag(char *TagName, char *ProjPath)
{
    char tag_adrs[PATH_MAX];
    char openTag[100];
    strcpy(openTag, TagName);
    strcat(openTag, ".txt");
    strcpy(tag_adrs,ProjPath);
    strcat(tag_adrs,"/.neogit/tags/");
    strcat(tag_adrs,openTag);
    if (access(tag_adrs, F_OK))
    {
        printf("the tag doesn't exist!\n");
        return;
    }
    printf("tag %s\n", TagName);
    struct stat st1;
    stat(tag_adrs, &st1);
    time_t crt = st1.st_ctime;
    printf("DATE:" _SGR_BLUEF " %s"_SGR_RESET, ctime(&crt));
    FILE *Tag_File = fopen(tag_adrs, "r");
    char line[200];
    while (fgets(line, sizeof(line), Tag_File) != NULL)
    {
        printf("%s", line);
    }
    fclose(Tag_File);
    return;
}
int alphasort2(const struct dirent **a, const struct dirent **b)
{
    return strcoll((*a)->d_name, (*b)->d_name);
}
void Show_acsending(char *ProjPath)
{
    char tag_adrs[PATH_MAX];
    strcpy(tag_adrs,ProjPath);
    strcat(tag_adrs,"/.neogit/tags");
    DIR *Tag = opendir(tag_adrs);
    struct dirent **entry;
    int n = scandir(tag_adrs, &entry, NULL, alphasort2);
    for (int i = 0; i < n; i++)
    {
        if (strcmp(entry[i]->d_name, ".") && strcmp(entry[i]->d_name, ".."))
            printf("%s\n", entry[i]->d_name);
        free(entry[i]);
    }
    free(entry);
    closedir(Tag);
    return;
}
/////////////
void Applied_Hook(char* ProjPath){
    char path[PATH_MAX];
    sprintf(path,"%s/.neogit/applied_hook",ProjPath);
    DIR* hookdir=opendir(path);
    struct dirent* entry;
    int n=0;
    while((entry =readdir(hookdir))!=NULL){
        if(strncmp(entry->d_name,".",1)){
            printf("%s\n",entry->d_name);
            n++;
        }

    }
    closedir(hookdir);
    if(n==0) printf("no hooks added yet\n");
    return;
}
void AddHook(char* name ,char* ProjPath){
    char path[PATH_MAX];
    sprintf(path,"%s/.neogit/applied_hook/%s.txt",ProjPath,name);
    FILE* f=fopen(path,"w");
    fprintf(f,"%s\n",name);
    fclose(f);
}
void RemoveHook(char* name ,char* ProjPath){
    char path[PATH_MAX];
    sprintf(path,"%s/.neogit/applied_hook/%s.txt",ProjPath,name);
    remove(path);
    return;
}
void TODO_check(const char *filename,int mode) {
    if( !strstr(filename,".c") && !strstr(filename,".cpp") && !strstr(filename,".txt")){
        if(mode){
            printf("\"todo_check\".................................................................................." _SGR_CYANF"SKIPPED\n" _SGR_RESET);
            return;
        }
    }
    FILE *file = fopen(filename, "r");
    int Count = 0;
    char line[400];
    if(strstr(filename,".txt")){
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line,"TODO") != NULL) {
                Count++;
            }
        }
    }
    else{
         while (fgets(line, sizeof(line), file) != NULL) {
            char* str=strstr(line,"//");
            if(str){
                if (strstr(str,"TODO") != NULL) {
                    Count++;
                }
            }
        }
    }
    fclose(file);
    if (Count > 0) {
        if(mode)printf("\"todo_check\".................................................................................." _SGR_REDF"FAILED\n" _SGR_RESET);
        cherrt=false;
        return ;
    } else {
        if(mode)printf("\"todo_check\".................................................................................." _SGR_GREENF"PASSED\n" _SGR_RESET);
        return ;
    }
}
void EOF_blank_space(char*Filename,int mode){
    if( !strstr(Filename,".c") && !strstr(Filename,".cpp") && !strstr(Filename,".txt")){
        if(mode){
            printf("\"eof-blank-space\".................................................................................." _SGR_CYANF"SKIPPED\n" _SGR_RESET);
            return;
        }
    }
    FILE *file = fopen(Filename, "r");
    fseek(file, -1, SEEK_END);
    int lastChar = fgetc(file);
    fclose(file);
    if (lastChar == '\n' || lastChar == ' ' || lastChar=='\t') {
       if(mode)printf("\"eof-blank-space\".................................................................................." _SGR_REDF"FAILED\n" _SGR_RESET);
       cherrt=false;
        return;
    } else {
        if(mode)printf("\"eof-blank-space\".................................................................................." _SGR_GREENF"PASSED\n" _SGR_RESET);
        return;
    }
}
void balance_braces(char *filename,int mode) {
     if( !strstr(filename,".c") && !strstr(filename,".cpp") && !strstr(filename,".txt")){
        if(mode){
            printf("\"balance-braces\".................................................................................." _SGR_CYANF"SKIPPED\n" _SGR_RESET);
            return;
        }
    }
    FILE *file = fopen(filename, "r");
    int b1 = 0, b2 = 0,c1=0,c2=0,p1=0,p2=0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '{') b1++;
        else if (c == '}') b2++;
        else if(c =='(') c1++;
        else if(c==')') c2++;
        else if(c ==']') p2++;
        else if(c =='[') p1++;
        
    }
    fclose(file);
    if (b1 == b2 && c1==c2 && p1==p2) {
        if(mode)printf("\"balance-braces\".................................................................................." _SGR_GREENF"PASSED\n" _SGR_RESET);
        return;
    } else {
        if(mode)printf("\"balance-braces\".................................................................................." _SGR_REDF"FAILED\n" _SGR_RESET);
        cherrt=false;
        return;
    }
}
void Format_check(char* Filename,int mode){
      if( !strstr(Filename,".c") && !strstr(Filename,".cpp") && !strstr(Filename,".txt")  && !strstr(Filename,".mp4") && !strstr(Filename,".mp3")  && !strstr(Filename,".wav")  && !strstr(Filename,".png")){
        if(mode){printf("\"format-check\".................................................................................." _SGR_CYANF"SKIPPED\n" _SGR_RESET);
        return;
        }
    }
    const char *validFormats[] = {".cpp", ".c", ".txt", ".mp4",".wav",".mp3",".png"};
    int isValid = 0;
    for (int i = 0; i < 7; i++) {
        if (strstr(Filename, validFormats[i]) != NULL) {
            isValid = 1;
            break;
        }
    }
    if (isValid) {
        if(mode)printf("\"format-check\".................................................................................." _SGR_GREENF"PASSED\n" _SGR_RESET);
        return;
    } else {
        if(mode) printf("\"format-check\".................................................................................." _SGR_REDF"FAILED\n" _SGR_RESET);
        cherrt=false;
        return;
    }
}
void FileSize_check(char* Filename,int mode){
    FILE *file = fopen(Filename, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fclose(file);
    if (fileSize < 5 * 1024 * 1024) {
        if(mode)printf("\"file-size-check\".................................................................................." _SGR_GREENF"PASSED\n" _SGR_RESET);
        return;
    } else {
        if(mode)  printf("\"file-size-check\".................................................................................." _SGR_REDF"FAILED\n" _SGR_RESET);
        cherrt=false;
        return;
    }
}
 void Character_check(char* filename, int mode){
    if( !strstr(filename,".c") && !strstr(filename,".cpp") && !strstr(filename,".txt")){
       if(mode) {printf("\"character-limit\".................................................................................." _SGR_CYANF"SKIPPED\n" _SGR_RESET);
        return;
        }
    }
    FILE *file = fopen(filename, "r");
    int Count = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        Count++;
    }
    fclose(file);
    if (Count>20000 ) {
        if (mode) printf("\"character-limit\".................................................................................." _SGR_REDF"FAILED\n" _SGR_RESET);
        cherrt=false;
        return;
    } else {
        if(mode) printf("\"character-limit\".................................................................................." _SGR_GREENF"PASSED\n" _SGR_RESET);
        return;
    }
}
void RunHook(char* ProjPath,char* Filename, int mode){
    char HookAddress[PATH_MAX];
    sprintf(HookAddress,"%s/.neogit/applied_hook",ProjPath);
    DIR* HookDir=opendir(HookAddress);
    struct dirent* entry;
    printf("%s:\n",Filename);
    while((entry=readdir(HookDir))!=NULL){
        if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){
            if(!strcmp(entry->d_name,"todo-check.txt")) TODO_check(Filename,mode);
            if(!strcmp(entry->d_name,"eof-blank-space.txt")) EOF_blank_space(Filename,mode);
            if(!strcmp(entry->d_name,"balance-braces.txt")) balance_braces(Filename,mode);
            if(!strcmp(entry->d_name,"format-check.txt")) Format_check(Filename,mode);
            if(!strcmp(entry->d_name,"file-size-check.txt")) FileSize_check(Filename,mode);
            if(!strcmp(entry->d_name,"character-limit.txt"))  Character_check(Filename,mode);
        }
    }
}
void run_pre(char* ProjPath, int mode){
    char stagepath[PATH_MAX];
    sprintf(stagepath,"%s/.neogit/.staged",ProjPath);
    DIR* stage=opendir(stagepath);
    struct dirent* entry;
    while((entry= readdir(stage))!= NULL){
        if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..") && entry->d_type != DT_DIR){
            char FileName[PATH_MAX];
            sprintf(FileName,"%s/.neogit/.staged/%s",ProjPath,entry->d_name);
            RunHook(ProjPath,FileName,mode);
        } 
    }
    closedir(stage);
    return;
}
///////////////////////////////////
void Merge(char *branch1, char *branch2, char *ProjPath) {
    char branch_adrs[PATH_MAX];
    sprintf(branch_adrs, "%s/.neogit/branch.txt", ProjPath);
    FILE *branch_file = fopen(branch_adrs, "r");
    char b1[100], b2[100];
    char line[1000];
    while (fgets(line, sizeof(line), branch_file) != NULL) {
        strtok(line, "_");
        if (!strcmp(line, branch1)) {
            strtok(NULL, "_");
            strcpy(b1, strtok(NULL, "\n"));
        }
        else if (!strcmp(line, branch2)) {
            strtok(NULL, "_");
            strcpy(b2, strtok(NULL, "\n"));
        }
    }
    char b1_adrs[PATH_MAX];
    sprintf(b1_adrs, "%s/.neogit/commits/%s", ProjPath, b1);
    char b2_adrs[PATH_MAX];
    sprintf(b2_adrs, "%s/.neogit/commits/%s", ProjPath, b2);
    //in func the second address will be changed
    char tmp[PATH_MAX];
    strcpy(tmp, b1_adrs);
    if (DFSmerge(b1_adrs, tmp, b2_adrs)) {
        char name[100], email[100];
        char g_config[PATH_MAX], l_config[PATH_MAX];
        sprintf(g_config, "/home/zahra/.neogitconfig/info.txt");
        sprintf(l_config, "%s/.neogit/config.txt", ProjPath);
        FILE *config;
        switch (compareFileTimes(g_config, l_config))
        {
        case 1:
            config = fopen(g_config, "r");
            break;
        case 2:
            config = fopen(l_config, "r");
            break;
        }

        fgets(name, sizeof(name), config);strtok(name, "="); char *Name = strtok(NULL, "\n");
        fgets(email, sizeof(email), config); strtok(email, "="); char *Email = strtok(NULL, "\n");
        fclose(config);
        int ID = get_ID(ProjPath,"commit");
        change_ID(ProjPath, '+',"commit");

        char commit_adrs[PATH_MAX];
        sprintf(commit_adrs, "%s/.neogit/commits/%d", ProjPath, ID);
        if (access(commit_adrs, F_OK) != 0)
            mkdir(commit_adrs, 0755) ;

        DIR *newbranch = opendir(b2_adrs);
        struct dirent *entry;
        while ((entry = readdir(newbranch)) != NULL) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, "info.txt")) {
                char adrs[PATH_MAX];
                sprintf(adrs, "%s/%s", b2_adrs, entry->d_name);
                bash("copy",adrs,commit_adrs);
            }
        }closedir(newbranch);
        newbranch = opendir(b1_adrs);
        while ((entry = readdir(newbranch)) != NULL) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, "info.txt")) {
                char adrs[PATH_MAX];
                sprintf(adrs, "%s/%s", b1_adrs, entry->d_name);
                bash("copy",adrs,commit_adrs);
            }
        }closedir(newbranch);
        strcat(commit_adrs, "/info.txt");
        FILE *f = fopen(commit_adrs, "w");
        commit_adrs[strlen(commit_adrs) - 9] = '\0'; //deleting /info.txt
        //the last commit of two branchs are the new commit(merge)
        BranchUpdate(ProjPath, branch1, ID);
        BranchUpdate(ProjPath, branch2, ID);
        fprintf(f, "Username =%s\nUseremail =\n%sBranch =%s\nMessage =merge of %s & %s", Name, Email, branch1, branch1,branch2);
        fclose(f);
        char cur_ID_adrs[PATH_MAX];
        sprintf(cur_ID_adrs, "%s/.neogit/current_IDs.txt", ProjPath);
        FILE *cur_ID_file = fopen(cur_ID_adrs, "r");
        char addID[10], commitID[10];
        fgets(addID, 10, cur_ID_file);
        fgets(commitID, 10, cur_ID_file);
        fclose(cur_ID_file);
        cur_ID_file = fopen(cur_ID_adrs, "w");
        fputs(addID, cur_ID_file);
        fputs(commitID, cur_ID_file);
        fprintf(cur_ID_file, "%d\n", ID);
        fclose(cur_ID_file);
        struct stat st1;
        stat(commit_adrs, &st1);
        time_t crt = st1.st_ctime;
        printf("Commit was done in time" _SGR_BLUEF " %s"_SGR_RESET _SGR_REDF "ID = %d" _SGR_RESET "\nMessage =merge of branch %s & %s", ctime(&crt), ID,branch1,branch2);
    }
    else printf("You can't merge because of this conflicts!\n");

    return;
}
int DFSmerge(char *mainPath, char *curPath, char *secPath){
    DIR *com = opendir(curPath);
    struct dirent *entry;
    bool flag = true;
    while ((entry = readdir(com)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            strcat(curPath, "/");
            strcat(curPath, entry->d_name);
            flag = DFSmerge(mainPath, curPath, secPath);
            while (curPath[strlen(curPath) - 1] != '/') curPath[strlen(curPath) - 1] = '\0';
            curPath[strlen(curPath) - 1] = '\0';
        }
        else if (entry->d_type != DT_DIR && strcmp(entry->d_name, "info.txt")) {
            strcat(curPath, "/");
            strcat(curPath, entry->d_name);
            char second[PATH_MAX];
            sprintf(second, "%s%s", secPath, curPath + strlen(mainPath));
            if (access(second, F_OK) == 0) {
                if (Diff(curPath, second, 1, 100000, 1, 100000) == 0) flag = false;
            }
            while (curPath[strlen(curPath) - 1] != '/') curPath[strlen(curPath) - 1] = '\0';
            curPath[strlen(curPath) - 1] = '\0';
        }
    }
    closedir(com);
    return flag;
}
//////////////////////////////////////////////////
void grep(char *path, char *target, int n)
{
    char *name = strrchr(path, '/');
    if (name != NULL)
        name++;
    else
        name = path;
    printf("in " _SGR_YELLOWF "%s :\n" _SGR_RESET, name);
    int l = 1;
    FILE *f = fopen(path, "r");
    char line[1000];
    while (fgets(line, sizeof(line), f))
    {
        char **words = NULL;
        int index = 0;
        int p= 0;
        char* token = strtok(line, " \n\t\r");
        while (token != NULL)
        {
            words = (char **)realloc(words, (index + 1) * sizeof(char *));
            words[index] = (char *)malloc(strlen(token) + 2);//for ^
            strcpy(words[index], token);
            index++;
            token = strtok(NULL," \n\t\r");
        }
        for (int i = 0; i < index; i++)
        {
            if (!strstr(target, "*"))
            {
                if (!strcmp(target, words[i]))
                {
                    char sh[100];
                    sprintf(sh, "^%s", words[i]);
                    strcpy(words[i], sh);
                    p=1;
                }   
            }
            else
            {   if (wildcard(target, words[i]))
                {
                    char sh[100];
                    sprintf(sh, "^%s", words[i]);
                    strcpy(words[i], sh);
                    p=1;
                }
            }
        }
        if (p)
        {
            if (n) printf("%d:", l);
            printf("\t");
            for (int i = 0; i < index; i++)
            {
                if (!strncmp(words[i], "^", 1))
                    printf(_SGR_MAGENF "%s " _SGR_RESET, words[i] + 1);//skip^
                else
                    printf("%s ", words[i]);
            }
            printf("\n");
        }
        for (int i = 0; i < index; i++)
            free(words[i]);
        free(words);
        l++;
    }
}

void grepCommit(char *commit_adrs, int n, char *target, char *file)
{
    DIR *dir = opendir(commit_adrs);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            strcat(commit_adrs, "/");
            strcat(commit_adrs, entry->d_name);
            grepCommit(commit_adrs, n, target, file);
        }
        else if (entry->d_type != DT_DIR && strcmp(entry->d_name, "info.txt"))
        {
            if (!strcmp(entry->d_name, file))
            {
                char file[PATH_MAX];
                sprintf(file, "%s/%s", commit_adrs, entry->d_name);
                grep(file, target, n);
            }
        }
    }
    while (commit_adrs[strlen(commit_adrs) - 1] != '/')
        commit_adrs[strlen(commit_adrs) - 1] = '\0';
    commit_adrs[strlen(commit_adrs) - 1] = '\0';
    closedir(dir);
    return;
}

bool wildcard(char *wildcard, char *word)
{
    while (*wildcard && *word)
    {
        if (*wildcard == *word )
        {
            wildcard++;
            word++;
        }
        else if (*wildcard == '*')
        {
            while (*wildcard == '*') wildcard++;
            if (*wildcard == '\0') return true;
            while (*word && *word != *wildcard) word++;
        }
        else
            return false;
    }
    return (*wildcard == '\0' && *word == '\0');
}
////////////////////////////////////////
void stash_push(int state, char *Message, char *ProjPath)
{/* first get the head of proj,get it's last commit id, shift all folders one ahead,cp wd to stash,remove wd,cp last commit to wc*/
    char stash_adrs[PATH_MAX];
    strcpy(stash_adrs,ProjPath);
    strcat(stash_adrs,"/.neogit/.stash");
    DIR *stash = opendir(stash_adrs);
    struct dirent * entry;
    while ((entry = readdir(stash)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            char adrs[PATH_MAX];
            sprintf(adrs, "%s/%s", stash_adrs, entry->d_name);
            int ind = 0;
            for (int i = 0; i < strlen(entry->d_name); i++)
                ind=ind*10+ (entry->d_name[i]-'0');
            ind ++;
            char next_adrs[PATH_MAX];
            sprintf(next_adrs, "%s/%d", stash_adrs, ind);
            rename(adrs, next_adrs);
        }
    }

    char dest[PATH_MAX];
    sprintf(dest, "%s/0", stash_adrs);
        mkdir(dest,0755);
    DIR *WD = opendir(ProjPath);
    while ((entry = readdir(WD)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, ".neogit")) {
            char address[PATH_MAX];
            sprintf(address, "%s/%s", ProjPath, entry->d_name);
            bash("copy",address,dest);
            bash("remove",address,NULL);
        }
    }
    closedir(WD);

    char head_adrs[PATH_MAX];
    strcpy(head_adrs,ProjPath);
    strcat(head_adrs,"/.neogit/head.txt");
    FILE *head_file = fopen(head_adrs, "r");
    char HEAD[1000];
    fgets(HEAD, 1000, head_file);
    if (HEAD[strlen(HEAD) - 1] == '\n') HEAD[strlen(HEAD) - 1] = '\0';
    fclose(head_file);

    char branch_adrs[PATH_MAX];
    strcpy(branch_adrs,ProjPath);
    strcat(branch_adrs,"/.neogit/branch.txt");
    FILE *branch_file = fopen(branch_adrs, "r");
    char COMMITID[1000];
    char *ID;
    while (fgets(COMMITID, sizeof(COMMITID), branch_file) != NULL) {
        strtok(COMMITID, "_");
        if (!strcmp(COMMITID, HEAD)) {
            strtok(NULL, "_");
            ID = strtok(NULL, "\n\r");
            break;
        }
    }

    char commit_adrs[PATH_MAX];
    sprintf(commit_adrs, "%s/.neogit/commits/%s", ProjPath, ID);
    DIR *Commit_Dir = opendir(commit_adrs);
    while ((entry = readdir(Commit_Dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && strcmp(entry->d_name, "info.txt")) {
            char address[PATH_MAX];
            sprintf(address, "%s/%s", commit_adrs, entry->d_name);
            bash("copy",address,ProjPath);
        }
    }
    closedir(Commit_Dir);

    char info[PATH_MAX];
    sprintf(info, "%s/info.txt", dest);
    char text[1000];
    if (state) sprintf(text, "%s:%s:%s", HEAD, Message, ID);
    else sprintf(text, "%s:-:%s", HEAD, ID);
    CreateFile(info, text);

    char commit_info[PATH_MAX];
    sprintf(commit_info, "%s/.neogit/current_IDs.txt", ProjPath);
    FILE *CurID = fopen(commit_info, "r");
    char addID[10], commitID[10], curcommitID[10];
    fgets(addID, 10, CurID);
    fgets(commitID, 10, CurID);
    fgets(curcommitID, sizeof(curcommitID), CurID);
    fclose(CurID);
    CurID = fopen(commit_info, "w");
    fputs(addID, CurID);
    fputs(commitID, CurID);
    fputs(ID,CurID); 
    fclose(CurID);

    
    return;
}
void stash_list (char *ProjPath) {
    char stash_adrs[PATH_MAX];
    strcpy(stash_adrs,ProjPath);
    strcat(stash_adrs,"/.neogit/.stash");
    DIR *stash = opendir(stash_adrs);
    struct dirent *entry;
    while ((entry = readdir(stash)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            char info_adrs[PATH_MAX];
            sprintf(info_adrs, "%s/%s/info.txt", stash_adrs, entry->d_name);
            FILE *info = fopen(info_adrs, "r");
            char line[1000];
            fgets(line, 1000, info);
            strtok(line, ":");
            printf("stash : %s was stashed on branch %s", entry->d_name, line);
            char *message = strtok(NULL, ":");
            printf(" and message = %s\n", message);
        }
    }
    return;
}
///////////////////////////////////
//return true  for valid lines,false for lines with white space
bool validline(char *line)
{
    for (int i = 0; i < strlen(line); i++)
        if (line[i] == '\n' || line[i] == '\t' || line[i] == ' ' || line[i] != '\r')
            return false;
    return true;
}

int different_line(char *line_1, char *line_2, char *file_1, char *file_2, int which_1, int which_2)
{
    char *token;
    char **words_1 = NULL;char **words_2 = NULL;
    int idx_1 = 0,idx_2=0;
    token = strtok(line_1, " \n\t\r");
    while (token != NULL)
    {
        words_1 = (char **)realloc(words_1, (idx_1 + 1) * sizeof(char *));
        words_1[idx_1] = (char *)malloc(strlen(token) + 3);
        strcpy(words_1[idx_1], token);
        idx_1++;
        token = strtok(NULL, " \n\t\r");
    }
    token = strtok(line_2, " \n\t\r");
    while (token != NULL)
    {
        words_2 = (char **)realloc(words_2, (idx_2 + 1) * sizeof(char *));
        words_2[idx_2] = (char *)malloc(strlen(token) + 3);
        strcpy(words_2[idx_2], token);
        idx_2++;
        token = strtok(NULL, " \n\t\r");
    }

    int min;
    if(idx_1>idx_2)min= idx_2;
    else min=idx_1;
    int tmp_idx, ctr = 0;
    for (int i = 0; i < min; i++)
        if (strcmp(words_1[i], words_2[i])){
            ctr++;  tmp_idx = i;
        }
    int dif = (idx_1 > idx_2) ? idx_1 - idx_2 : idx_2 - idx_1;
    if (dif == 0)
    {
        if (ctr == 0)
            return 0;
        else if (ctr == 1)
        {
            char tmp[50];
            sprintf(tmp, ">%s<", words_1[tmp_idx]);
            strncpy(words_1[tmp_idx], tmp, strlen(tmp));
            sprintf(tmp, ">%s<", words_2[tmp_idx]);
            strcpy(words_2[tmp_idx], tmp);

            printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d\n" _SGR_RESET,file_1, which_1);
            printf("\t");
            for (int i = 0; i < idx_1; i++)
            {
                if (!strncmp(">", words_1[i], 1)) printf(_SGR_REDF "%s " _SGR_RESET, words_1[i]);
                else printf(_SGR_CYANF "%s "_SGR_RESET, words_1[i]);
            }
            printf("\n");
            printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d" _SGR_RESET "\n\t",file_2, which_2);
            for (int i = 0; i < idx_2; i++){
                if (!strncmp(">", words_2[i], 1))printf(_SGR_REDF "%s " _SGR_RESET, words_2[i]);
                else printf(_SGR_BLUEF "%s " _SGR_RESET, words_2[i]);
            }
            printf("\n\n");
        }
        else
        {
            printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d\n" _SGR_RESET,file_1, which_1);
            printf("\t");
            for (int i = 0; i < idx_1; i++) printf(_SGR_CYANF "%s "_SGR_RESET, words_1[i]);
            printf("\n");
            printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d" _SGR_RESET "\n\t",file_2, which_2);
            for (int i = 0; i < idx_2; i++) printf(_SGR_BLUEF "%s " _SGR_RESET, words_2[i]);
            printf("\n\n");
        }
    }
    else if (dif == 1)
    {
        if (ctr != 0)
        {
            printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d\n" _SGR_RESET,file_1, which_1);
            printf("\t");
            for (int i = 0; i < idx_1; i++) printf(_SGR_CYANF "%s "_SGR_RESET, words_1[i]);
            printf("\n");
            printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d" _SGR_RESET "\n\t",file_2, which_2);
            for (int i = 0; i < idx_2; i++) printf(_SGR_BLUEF "%s " _SGR_RESET, words_2[i]);
            printf("\n\n");
        }
        else
        {
            if (idx_1 > idx_2)
            {
                printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d\n" _SGR_RESET,file_1, which_1);
                printf("\t");
                for (int i = 0; i < idx_1 - 1; i++) printf(_SGR_CYANF "%s "_SGR_RESET, words_1[i]);
                printf(_SGR_REDF ">%s< " _SGR_RESET, words_1[idx_1 - 1]);
                printf("\n");
                printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d" _SGR_RESET "\n\t",file_2, which_2);
                for (int i = 0; i < idx_2; i++) printf(_SGR_BLUEF "%s " _SGR_RESET, words_2[i]);
                printf("\n\n");
            }
            else
            {
                printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d\n" _SGR_RESET,file_1, which_1);
                printf("\t");
                for (int i = 0; i < idx_1; i++) printf(_SGR_CYANF "%s "_SGR_RESET, words_1[i]);
                printf("\n");
                printf(_SGR_YELLOWF "%s" _SGR_RESET " ,line: "_SGR_GREENF"%d" _SGR_RESET "\n\t",file_2, which_2);
                for (int i = 0; i < idx_2 - 1; i++) printf(_SGR_BLUEF "%s " _SGR_RESET, words_2[i]);
                printf(_SGR_REDF ">%s< " _SGR_RESET, words_2[idx_2 - 1]);
                printf("\n\n");
            }
            return 0;
        }
    }
    else
    {
        printf(_SGR_YELLOWF "%s" _SGR_RESET " - line: "_SGR_GREENF
                            "%d\n" _SGR_RESET,
               file_1, which_1);
        printf("\t");
        for (int i = 0; i < idx_1; i++)
            printf(_SGR_CYANF "%s "_SGR_RESET, words_1[i]);
        printf("\n");
        printf(_SGR_YELLOWF "%s" _SGR_RESET " - line: "_SGR_GREENF
                            "%d" _SGR_RESET "\n\t",
               file_2, which_2);
        for (int i = 0; i < idx_2; i++)
            printf(_SGR_BLUEF "%s " _SGR_RESET, words_2[i]);
        printf("\n\n");
    }
    for (int i = 0; i < idx_1; i++)
        free(words_1[i]);
    free(words_1);
    for (int i = 0; i < idx_2; i++)
        free(words_2[i]);
    free(words_2);
    return 1;
}

int Diff(char *file1, char *file2, int begin_1, int end_1, int begin_2, int end_2)
{
    FILE *f_1 = fopen(file1, "r");
    char *name_1 = strrchr(file1, '/');
    if (name_1 != NULL) name_1++;
    else    name_1 = file1;

    FILE *f_2 = fopen(file2, "r");
    char *name_2 = strrchr(file2, '/');
    if (name_2 != NULL)
        name_2++;
    else
        name_2 = file2;

    char line_1[1000], line_2[1000];
    int sum_1 = 0, sum_2 = 0;

    while (fgets(line_1, sizeof(line_1), f_1))
        if (validline(line_1))
            sum_1++;
    if (end_1 > sum_1) end_1 = sum_1;  rewind(f_1);
    while (fgets(line_2, sizeof(line_2), f_2))
        if (validline(line_2))
            sum_2++;
    if (end_2 > sum_2) end_2 = sum_2;  rewind(f_2);

    for (int i = 1; i < begin_1; i++)
        do
            fgets(line_1, sizeof(line_1), f_1);
        while (! validline(line_1));
    int len_1 = end_1 - begin_1;
    for (int i = 1; i < begin_2; i++)
        do
            fgets(line_2, sizeof(line_2), f_2);
        while (! validline(line_2));
    int len_2 = end_2 - begin_2;
    bool flag=false;
    if (len_2 > len_1)
    {
        for (int i = begin_1; i <= end_1; i++)
        {
            do
                fgets(line_1, sizeof(line_1), f_1);
            while (! validline(line_1));
            do
                fgets(line_2, sizeof(line_2), f_2);
            while (!validline(line_2));
            different_line(line_1, line_2, name_1, name_2, i, i - begin_1 + begin_2);
        }
        printf("\nlines in second file that " _SGR_REDF "doesn't" _SGR_RESET " exist in first one:\n");
        while (fgets(line_2, sizeof(line_2), f_2))
        {   if( validline(line_2)){
            printf(_SGR_BLUEF "%d:\t%s" _SGR_RESET, end_2 - 1, line_2);
            end_2++;
            }
        }
        flag=true;
    }
    else if (len_2 < len_1)
    {
        for (int i = begin_2; i <= end_2; i++)
        {
            do
                fgets(line_1, sizeof(line_1), f_1);
            while (!validline(line_1));
            do
                fgets(line_2, sizeof(line_2), f_2);
            while (!validline(line_2));
            different_line(line_1, line_2, name_1, name_2, i - begin_2 + begin_1, i);
        }
        printf("\nlines in first file that " _SGR_REDF "doesn't" _SGR_RESET" exist in second one:\n");
        while (fgets(line_1, sizeof(line_1), f_1))
        {   if(validline(line_1)){
            printf(_SGR_BLUEF "%d:\t%s" _SGR_RESET, end_1 - 1, line_1);
            end_1++;
            }
        }
        flag=true;
    }
    else
    {
        for (int i = begin_1; i <= end_1; i++)
        {
            do
                fgets(line_1, sizeof(line_1), f_1);
            while (!validline(line_1));
            do
                fgets(line_2, sizeof(line_2), f_2);
            while (!validline(line_2));
           if(different_line(line_1, line_2, name_1, name_2, i, i - begin_1 + begin_2)) flag=true; 
        }
    }
    return flag;
}
//////////////////////////
void Diff_commit_one(char *adrs_1, char *adrs_2)
{
    DIR *cm = opendir(adrs_1);
    struct dirent *entry;
    while ((entry = readdir(cm)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            strcat(adrs_1, "/");
            strcat(adrs_1, entry->d_name);
            strcat(adrs_2, "/");
            strcat(adrs_2, entry->d_name);
            Diff_commit_one(adrs_1, adrs_2);
        }
        else if (entry->d_type != DT_DIR && strcmp(entry->d_name, "info.txt"))
        {
            char file_1[PATH_MAX], file_2[PATH_MAX];
            sprintf(file_1, "%s/%s", adrs_1, entry->d_name);
            sprintf(file_2, "%s/%s", adrs_2, entry->d_name);

            if (access(file_2, F_OK) != 0)
            {
                char *name_1 = strrchr(file_1, '/');
                if (name_1 != NULL)
                    name_1++;
                else
                    name_1 = file_1;
                printf(_SGR_MAGENF "%s :\n" _SGR_RESET, name_1);
                FILE *f = fopen(file_1, "r");
                char line[1100];
                while (fgets(line, sizeof(line), f) != NULL)
                    printf("%s", line);
                printf("\n\n");
                fclose(f);
            }
        }
    }
    while (adrs_1[strlen(adrs_1) - 1] != '/')
        adrs_1[strlen(adrs_1) - 1] = '\0';
    adrs_1[strlen(adrs_1) - 1] = '\0';
    while (adrs_2[strlen(adrs_2) - 1] != '/')
        adrs_2[strlen(adrs_2) - 1] = '\0';
    adrs_2[strlen(adrs_2) - 1] = '\0';
    closedir(cm);
    return;
}
void diff_commit(char* adrs1,char* adrs2,char* ProjPath){
    char c_1[PATH_MAX]; sprintf(c_1, "%s/.neogit/commits/%s", ProjPath, adrs1);
    char c_2[PATH_MAX]; sprintf(c_2, "%s/.neogit/commits/%s", ProjPath, adrs2);
    if (access(c_1, F_OK) != 0 || access(c_2, F_OK) != 0) {
        printf("commit ID not correct!\n");
        return;
    }
    printf(_SGR_ITALIC "files that are only in commit %s \n\n" _SGR_RESET, adrs1);
    Diff_commit_one(c_1, c_2);

    printf(_SGR_ITALIC "files that are only in commit %s\n\n" _SGR_RESET, adrs2);
    sprintf(c_1, "%s/.neogit/commits/%s", ProjPath, adrs1);
    sprintf(c_2, "%s/.neogit/commits/%s", ProjPath, adrs2);
    Diff_commit_one(c_2, c_1);

    printf(_SGR_ITALIC"files in both commits :\n\n" _SGR_RESET);
    sprintf(c_1, "%s/.neogit/commits/%s", ProjPath, adrs1);
    sprintf(c_2, "%s/.neogit/commits/%s", ProjPath, adrs2);
    Diff_commit_both(c_1, c_2);
}
void Diff_commit_both(char *adrs_1, char *adrs_2)
{
    DIR *cm = opendir(adrs_1);
    struct dirent *entry;
    while ((entry = readdir(cm)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            strcat(adrs_1, "/");
            strcat(adrs_1, entry->d_name);
            strcat(adrs_2, "/");
            strcat(adrs_2, entry->d_name);
            Diff_commit_both(adrs_1, adrs_2);
        }
        else if (entry->d_type != DT_DIR && strcmp(entry->d_name, "info.txt"))
        {
            char file_1[PATH_MAX], file_2[PATH_MAX];
            sprintf(file_1, "%s/%s", adrs_1, entry->d_name);
            sprintf(file_2, "%s/%s", adrs_2, entry->d_name);
            if (access(file_2, F_OK) == 0)
                Diff(file_1, file_2, 1, 10000, 1, 10000);
        }
    }
    while (adrs_1[strlen(adrs_1) - 1] != '/')
        adrs_1[strlen(adrs_1) - 1] = '\0';
    adrs_1[strlen(adrs_1) - 1] = '\0';
    while (adrs_2[strlen(adrs_2) - 1] != '/')
        adrs_2[strlen(adrs_2) - 1] = '\0';
    adrs_2[strlen(adrs_2) - 1] = '\0';
    closedir(cm);
    return;
}
//////////////////////////////////////
void Commit(char *ProjPath, char *message)
{
    run_pre(ProjPath,0);
    if(cherrt==false){
        printf("you are banned from commiting because of precommit fails\n");
        return;
    }

    char headadrs[PATH_MAX];
    sprintf(headadrs, "%s/.neogit/head.txt", ProjPath);
    FILE *head = fopen(headadrs, "r");
    char HEAD[100];
    fgets(HEAD, sizeof(HEAD), head);
    if (HEAD[strlen(HEAD) - 1] == '\n') HEAD[strlen(HEAD) - 1] = '\0';
    strcat(HEAD, "_");
    fclose(head);

    char branchadrs[PATH_MAX];
    sprintf(branchadrs, "%s/.neogit/branch.txt", ProjPath);
    FILE *Branch = fopen(branchadrs, "r");
    char b_Line[1000];
    fgets(b_Line, sizeof(b_Line), Branch);
    char *headID;
    while (1)
    {
        if (!strncmp(b_Line, HEAD, strlen(HEAD)))
        {
            strtok(b_Line, "_");
            strtok(NULL, "_");
            headID = strtok(NULL, "\n");
            break;
        }
        fgets(b_Line, sizeof(b_Line), Branch);
    }
    fclose(Branch);

    char commitIDAddress[PATH_MAX];
    sprintf(commitIDAddress, "%s/.neogit/current_IDs.txt", ProjPath);
    FILE *CurID = fopen(commitIDAddress, "r");
    char ComID[100];
    fgets(ComID, sizeof(ComID), CurID);
    fgets(ComID, sizeof(ComID), CurID);
    fgets(ComID, sizeof(ComID), CurID);
    if (ComID[strlen(ComID) - 1] == '\n') ComID[strlen(ComID) - 1] = '\0';
    fclose(CurID);
    if (strcmp(headID, ComID))
    {
        printf("You can only commit changes on HEAD.\n");
        return;
    }

    char StagePath[PATH_MAX];
    strcpy(StagePath, ProjPath);
    strcat(StagePath, "/.neogit/.staged");
    int count = 0;
    DIR *StageDir = opendir(StagePath);
    struct dirent *entry;
    while ((entry = readdir(StageDir)) != NULL)
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
            count++;
    if (count == 0 || count == -1){
        printf("nothing has been staged since last commit!\n");
        closedir(StageDir);
        return;
    }
    closedir(StageDir);
    char Username[100], Useremail[100], branch[100];
    char globalConfig[PATH_MAX], localConfig[PATH_MAX];
    sprintf(globalConfig, "/home/zahra/.neogitconfig/info.txt");
    sprintf(localConfig, "%s/.neogit/config.txt", ProjPath);
    FILE *config;
    switch (compareFileTimes(globalConfig, localConfig)) 
    {
    case 1:
        config = fopen(globalConfig, "r");
        break;
    case 2:
        config = fopen(localConfig, "r");
        break;
    }

    fgets(Username, sizeof(Username), config);
    strtok(Username, "=");
    char *Name = strtok(NULL, "\n");
    fgets(Useremail, sizeof(Useremail), config);
    strtok(Useremail, "=");
    char *Email = strtok(NULL, "\n");
    fclose(config);
    if (Name == NULL || Email == NULL){
        printf("You didn't set your config yet! Please set it and try again!\n");
        return;
    }

    int ID = get_ID(ProjPath,"commit");
    change_ID(ProjPath, '+',"commit");

    char commitPath[PATH_MAX];
    sprintf(commitPath, "%s/.neogit/commits", ProjPath);
    bash("copy",StagePath,commitPath);
    bash("remove",StagePath,NULL);


    if (access(StagePath, F_OK) != 0)
        mkdir(StagePath, 0755);

    sprintf(StagePath, "%s/.neogit/commits/.staged", ProjPath);
    sprintf(commitPath, "%s/.neogit/commits/%d", ProjPath, ID);
    rename(StagePath, commitPath);

    strcat(commitPath, "/info.txt");
    FILE* f = fopen(commitPath, "w");
    commitPath[strlen(commitPath) - 9] = '\0';

    char Head[PATH_MAX];
    sprintf(Head, "%s/.neogit/head.txt", ProjPath);
    config = fopen(Head, "r");
    fgets(branch, sizeof(branch), config);
    if (branch[strlen(branch) - 1] == '\n')
        branch[strlen(branch) - 1] = '\0';
    fclose(config);
    BranchUpdate(ProjPath, branch, ID);
    fprintf(f, "Username =%s\nUseremail =%s\nBranch =%s\nMessage =%s\n", Name, Email, branch, message);
    fclose(f);

    if (ID > 10000)
    {
        char Dest[PATH_MAX];
        sprintf(Dest, "%s/.neogit/commits/%d", ProjPath, ID);
        char Src[PATH_MAX];
        sprintf(Src, "%s/.neogit/commits/%d", ProjPath, ID - 1);
        makeln(ProjPath, Src, Dest);
    }
    struct stat folder;
    stat(commitPath, &folder);
    time_t crt = folder.st_ctime;
    printf("Committed successfully in time" _SGR_BLUEF " %s"_SGR_RESET _SGR_REDF "ID = %d" _SGR_RESET "\nMessage =" _SGR_YELLOWF " \"%s\"\n" _SGR_RESET, ctime(&crt), ID, message);
    
    CurID = fopen(commitIDAddress, "r");
    char addID[10], commitID[10];
    fgets(addID, 10, CurID);
    fgets(commitID, 10, CurID);
    fclose(CurID);
    CurID = fopen(commitIDAddress, "w");
    fputs(addID, CurID);
    fputs(commitID, CurID);
    fprintf(CurID,"%d\n",ID);
    fclose(CurID);


    return;
}
/////////////////////////////////////
