#ifndef __FILEPROCESS_H__
#define __FILEPROCESS_H__

#include "ansi_color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h> //
#include <sys/types.h>

#define DEBUG(x) printf("%s\n", x);
#define BUF_SIZE 65536
#define INVALID_CMD printf("Invalid Command!\n")
#define CHECK_IF_REPO(var)                                                     \
    if (!var)                                                                  \
    {                                                                          \
        printf("Not a neogit repository!\n"); \
        return 0;                                                              \
    }

int init();//done
int IsValid(char *command);//done
int IsAlias(char *command, char *ProjPath);//done
int repo_place(char *path);//done
void ChangeConfig(char *infoPath, char name[],char* type);//done
int Difference_file(char *path1, char *path2);//done
void Show_folder_n(char ProjPath[], char curAddress[], int depth, bool check);//done
void CreateFile(char FileName[], char Text[]);//done
int get_ID(char *ProjPath,char* type);//done
void change_ID(char *ProjPath, char sign,char* type);//done
void bash(char* type,char* file_src, char* file_dest);//done
void Staging(char *name, char *ProjPath);//done
void newRecords(char *ProjPath);//done
void redo(char *ProjPath, char *Staged);//done
void unstaging(char *name, char *ProjPath);//done
void resetundo(char *ProjPath);//done
int compareFileTimes(const char *file1, const char *file2);//done
void makeln(char ProjPath[], char Src[], char Dest[]);//done
void shortcut(char *message, char *shortcut, char *ProjPath);//done
mode_t similarmode(char *file1, char *file2);//done
void Log(char *ProjPath, int n);//done
void number_of_files(char *curadd, int *num);//done
void log_message(char *ProjPath, char *target);//done
void log_time(char *ProjPath, char *time, char sign);//done
int compare_time_log(const char *filename, const char *time);//done
///////////////////////////////
void Status(char *ProjPath);//done
void DFSCommit(char *ProjPath, char *address);//done
void DFSWD(char *ProjPath, char *address);//done
/////////////////////////////////
void show_branch(char *ProjPath);//done
void makebranch(char *name, char *ProjPath);//done
int WD_DFS_forcheckout(char *ProjPath, char *address, char *ID);//done
void Checkout(char *where, char *ProjPath);//done
void Checkout_ID(char* where,char* ProjPath);//done
void Checkout_branch(char* name,char* ProjPath);//done
void BranchUpdate(char *ProjPath, char *branch, int ID);//done
void CheckoutHead(char *where,char* ProjPath);//done
////////////////////////////////
int alphasort2 ( const struct dirent** a,const struct dirent** b);//done
void Show_acsending(char *ProjPath);//done
void Show_tag(char* TagName, char* ProjPath);//done
void tagging(char* Tagname, char* Message, char* CommitID, int state,char* ProjPath);//done
////////////////////////
void Applied_Hook(char* ProjPath);//done
void AddHook(char* name ,char* ProjPath);//done
void RemoveHook(char* name ,char* ProjPath);//done
void TODO_check(const char *filename,int mode); //done
void EOF_blank_space(char*Filename,int mode);//done
void balance_braces(char *filename, int mode);//done
void Format_check(char* Filename,int mode); //done but go back if time was left
void FileSize_check(char* Filename, int mode);//done
void Character_check(char* filename,int mode);//done
void RunHook(char* ProjPath,char* Filename,int mode);//done
void run_pre(char* ProjPath,int mode);//done
//////////////////////////////////////////
void grep(char *path, char *target, int n);//almost done
void grepCommit(char *commit_adrs, int n, char *target, char *file);//almost done
bool wildcard(char *pattern, char *str);//done
//////////////////////////////////////////
void stash_push(int state, char *Message, char *ProjPath);//done
void stash_list (char *ProjPath);//done
//////////////////////////////////
void Merge(char *branch1, char *branch2, char *ProjPath);//almost done
int DFSmerge(char *mainPath, char *curPath, char *secPath);
//////////////////////////////////
bool validline(char *line);
int Diff(char *file1, char *file2, int begin_1, int end_1, int begin_2, int end_2);
int different_line(char *line_1, char *line_2, char *file_1, char *file_2, int which_1, int which_2);
/////////////////////////////////////////
void diff_commit(char* adrs1,char* adrs2,char* ProjPath);
void Diff_commit_one(char *adrs_1, char *adrs_2);
void Diff_commit_both(char *adrs_1, char *adrs_2);
////////////////////////////////
void Commit(char *ProjPath, char *message);
void Logplus(char *ProjPath, char *TYPEt, char *WORDt);

#endif