#include <stdio.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "functions.h"
#include "views.h"

typedef struct {
  char perm[11];
  int hlink[4];
  char owner[128];
  char group[128];
  int size[32];
  char date[17];
  char name[512];
} results;

int hlinklen;
int ownerlen;
int grouplen;
int sizelen;
int namelen;

int ownstart;
int groupstart;
int sizestart;
int datestart;
int namestart;
;
int seglength(const void *seg, char *segname, int LEN)
{

  size_t longest, len;
  char hlinkstr[5], sizestr[32];

  results *dfseg = (results *)seg;


  if (segname == "owner") {
    longest = strlen(dfseg[0].owner);
  }
  else if (segname == "group") {
    longest = strlen(dfseg[0].group);
  }
  else if (segname == "hlink") {
    sprintf(hlinkstr, "%d", *dfseg[0].hlink);
    longest = strlen(hlinkstr);
  }
  else if (segname == "size") {
    sprintf(sizestr, "%d", *dfseg[0].size);
    longest = strlen(sizestr);
  }
  else if (segname == "name") {
    longest = strlen(dfseg[0].name);
  }
  else {
    longest = 0;
  }

  size_t j = 0;

  for(size_t i = 1; i < LEN; i++)
    {
      if (segname == "owner") {
        len = strlen(dfseg[i].owner);
      }
      else if (segname == "group") {
        len = strlen(dfseg[i].group);
      }
      else if (segname == "hlink") {
        sprintf(hlinkstr, "%d", *dfseg[i].hlink);
        len = strlen(hlinkstr);
      }
      else if (segname == "size") {
        sprintf(sizestr, "%d", *dfseg[i].size);
        len = strlen(sizestr);
      }
      else if (segname == "name") {
        len = strlen(dfseg[i].name);
      }
      else {
        len = 0;
      }

      if(longest < len)
        {
          longest = len;
          j = i;
        }
    }
  return longest;
}

int cmp_str(const void *lhs, const void *rhs)
{
  return strcmp(lhs, rhs);
}

int cmp_int(const void *lhs, const void *rhs)
{
  int aa, bb;

  aa = *(int *)lhs;
  bb = *(int *)rhs;

  return (aa - bb);
}

int cmp_dflist(const void *lhs, const void *rhs)
{

  //struct dfobject *lhs;
  //struct dfobject *rhs;

  results *dforderA = (results *)lhs;
  results *dforderB = (results *)rhs;

  return strcmp(dforderA->name, dforderB->name);

  //return (dforderA->size - dforderB->size);

}

int list_dir(char *pwd)
{
  size_t count = 0;
  size_t list_count = 0;
  size_t file_count = 0;
  struct dirent *res;
  struct stat sb;
  struct group *gr;
  struct passwd *pw;
  const char *path = pwd;
  struct stat buffer;
  int         status;
  char filedatetime[17];
  char perms[11] = {0};
  // char result[11][4][128][128][32][17][512];

  //  struct dfobject {
  //    char perm[11];
  //    int hlink[4];
  //    char owner[128];
  //    char group[128];
  //    int size[32];
  //    char date[17];
  //    char name[512];
  //  };

  results *ob = malloc(1024 * sizeof(results)); // Needs to be dynamic

  if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)){
    DIR *folder = opendir ( path );

    if (access ( path, F_OK ) != -1 ){
      if ( folder ){
        //while (( res = readdir( folder )) ) {
        //    file_count++;
        //}
        while ( ( res = readdir ( folder ) ) ){
          //if ( strcmp( res->d_name, "." ) && strcmp( res->d_name, ".." ) ){
          lstat(res->d_name, &sb);
          struct passwd *pw = getpwuid(sb.st_uid);
          struct group *gr = getgrgid(sb.st_gid);
          status = lstat(res->d_name, &buffer);
          strftime(filedatetime, 20, "%Y-%m-%d %H:%M", localtime(&(buffer.st_mtime)));


          if ( buffer.st_mode & S_IFDIR ) {
            perms[0] = 'd';
          } else if ( S_ISLNK(buffer.st_mode) ) {
            perms[0] = 'l';
          } else {
            perms[0] = '-';
          }
          perms[1] = buffer.st_mode & S_IRUSR? 'r': '-';
          perms[2] = buffer.st_mode & S_IWUSR? 'w': '-';
          perms[3] = buffer.st_mode & S_IXUSR? 'x': '-';
          perms[4] = buffer.st_mode & S_IRGRP? 'r': '-';
          perms[5] = buffer.st_mode & S_IWGRP? 'w': '-';
          perms[6] = buffer.st_mode & S_IXGRP? 'x': '-';
          perms[7] = buffer.st_mode & S_IROTH? 'r': '-';
          perms[8] = buffer.st_mode & S_IWOTH? 'w': '-';
          perms[9] = buffer.st_mode & S_IXOTH? 'x': '-';

          // Writing our structure
          strcpy(ob[count].perm, perms);
          *ob[count].hlink = buffer.st_nlink;
          strcpy(ob[count].owner, pw->pw_name);
          strcpy(ob[count].group, gr->gr_name);
          *ob[count].size = buffer.st_size;
          strcpy(ob[count].date, filedatetime);
          strcpy(ob[count].name, res->d_name);

          // grp = getgrgid(res->d_ino);
          //mvprintw(4 + count, 4,"%s %i  %s %s      %i  %s  %s\n",ob[count].perm,buffer.st_nlink,pw->pw_name,ob[count].group,buffer.st_size,ob[count].date,ob[count].name);
          // mvprintw(4 + count, 4,"%s",ob[count].perm);
          // mvprintw(4 + count, 15,"%i",*ob[count].hlink);
          // mvprintw(4 + count, 18,"%s",ob[count].owner);
          // mvprintw(4 + count, 22,"%s",ob[count].group);
          // mvprintw(4 + count, 35,"%i",*ob[count].size);
          // mvprintw(4 + count, 42,"%s",ob[count].date);
          // mvprintw(4 + count, 60,"%s",ob[count].name);

          count++;
            //}
        }

        qsort(ob, count, sizeof(results), cmp_dflist);

        for(list_count; list_count < count; ){
          //TEMP Emulate listed item
          if (list_count == 4) {
            attron(A_BOLD);
            attron(COLOR_PAIR(4));
          }

          hlinklen = seglength(ob, "hlink", count);
          ownerlen = seglength(ob, "owner", count);
          grouplen = seglength(ob, "group", count);
          sizelen = seglength(ob, "size", count);
          namelen = seglength(ob, "name", count);

          ownstart = 15 + hlinklen + 1;
          groupstart = ownstart + ownerlen + 1;
          if (ownerlen + 1 + grouplen < 16) {
            sizestart = ownstart + 16;
          } else {
            sizestart = groupstart + grouplen + 1;
          }
          datestart = sizestart + sizelen + 1;
          namestart = datestart + 18;

          mvprintw(4 + list_count, 4,"%s",ob[list_count].perm);
          mvprintw(4 + list_count, 15,"%i",*ob[list_count].hlink);
          mvprintw(4 + list_count, ownstart,"%s",ob[list_count].owner);
          mvprintw(4 + list_count, groupstart,"%s",ob[list_count].group);
          mvprintw(4 + list_count, sizestart,"%i",*ob[list_count].size);
          mvprintw(4 + list_count, datestart,"%s",ob[list_count].date);
          mvprintw(4 + list_count, namestart,"%s",ob[list_count].name);
          //TEMP Emulate listed item
          if (list_count == 4) {
            attron(COLOR_PAIR(1));
            attroff(A_BOLD);
          }
          list_count++;
         }


        // mvprintw(4 + count + 2, 4,"Hlink: %i",hlinklen);
        // mvprintw(4 + count + 3, 4,"Owner: %i",ownerlen);
        // mvprintw(4 + count + 4, 4,"Group: %i",grouplen);
        // mvprintw(4 + count + 5, 4,"Size:  %i",sizelen);
        // mvprintw(4 + count + 6, 4,"Name:  %i",namelen);

        attron(COLOR_PAIR(2));
        mvprintw(1, 2, "%s", pwd);
        mvprintw(2, 2, "%i Objects   00000 Used 00000000 Available", count); // Parcial Placeholder for PWD info
        mvprintw(3, 4, "---Attrs----");
        mvprintw(3, ownstart, "-Owner & Group-");
        mvprintw(3, sizestart, "-Size-");
        mvprintw(3, datestart, "---Date & Time---");
        mvprintw(3, namestart, "----Name----");
        //mvprintw(3, 4, "----Attrs---- -Owner & Group-  -Size- ---Date & Time--- ----Name----"); // Header
        attron(COLOR_PAIR(1));

        closedir ( folder );
        free(ob); // Freeing memory
      }else{
        perror ( "Could not open the directory" );
        return 1;
      }
    }

  }else{
    printf("The %s it cannot be opened or is not a directory\n", path);
    return 1;
  }
}
