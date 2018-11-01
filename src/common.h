#define PROGRAM_DESC "\nDF-SHOW: An interactive directory/file browser written for Unix-like systems.\nBased on the applications from the PC-DOS DF-EDIT suite by Larry Kroeker.\n"

void printVersion(char* programName);
void printMenu(int line, int col, char *menustring);
void topLineMessage(const char *message);
int readline(char *buffer, int buflen, char *oldbuf);
int check_dir(char *pwd);
int check_file(char *file);
void clear_workspace();
int check_exec(const char *object);
int check_last_char(const char *str, const char *chk);
int check_first_char(const char *str, const char *chk);
int check_numbers_only(const char *s);
char *str_replace(char *orig, char *rep, char *with);
char * read_line(FILE *fin);
void showManPage(const char * prog);
int can_run_command(const char *cmd);
int exittoshell();