void modify_key_menu_inputs();
void modify_owner_input();
void modify_group_input();
void modify_permissions_input();
void show_directory_input();
void sort_view();
void directory_view_menu_inputs0();
void directory_view_menu_inputs1();
void directory_change_menu_inputs();
void show_directory_inputs();
void copy_file_input(char *file);
void replace_file_confirm(char *filename);
int replace_file_confirm_input();
void rename_file_input(char *file);
void edit_file_input();
void make_directory_input();
void delete_file_confirm();
void delete_multi_file_confirm(const char *filename);
void delete_file_confirm_input(char *file);
void delete_multi_file_confirm_input(results* ob);
void copy_multi_file_input(results* ob, char *input);
void copy_multi_file(results* ob, char *dest);
void rename_multi_file_input(results* ob, char *input);
void topLineMessage(const char *message);
void printMenu(int line, int col, char *menustring);
