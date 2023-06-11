#pragma mapinc("regf","QTEMP/REGF(REGFMT) ","both","p", ,"")
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <recio.h>
#include "regf"

bool patmatch(char text[], char pat[]);

void writetofile(char p_filename[],char p_extname[],int p_subep);

void remove_spaces(char* s);

void traverse(char *fn, char *filtyp, char *patt)
{
  DIR *dir;
  struct dirent *entry;
  int count,subsp,subep, totlen, extlen;
  char path[1025]; /*** EXTRA STORAGE MAY BE NEEDED ***/
  struct stat info;
  char *fileext;
  char fnascarr[641];
  char feascarr[5];
  char filename[640];
  char extname[5];
  bool extmatch,filmatch;
  char p_filn[21],p_filtyp[5],p_patt[66];

   memset(p_filn,'\0', sizeof(p_filn));
   memset(p_filtyp,'\0', sizeof(p_filtyp));
   memset(p_patt,'\0', sizeof(p_patt));
   strcpy(p_filn,fn);
   strcpy(p_filtyp,filtyp);
   strcpy(p_patt,patt);
   /* Open the directory to read its contents */
  if ((dir = opendir(p_filn)) == NULL)
     perror("opendir() error");
  else
     {
      /* Read entire directory */
     while ((entry = readdir(dir)) != NULL)
     {
      /* Skip current and up folder links */
     if (entry->d_name[0] != '.')
         {
         /* copy pointer contents into work variables and concatenate*/
         strcpy(path, fn);
         strcat(path, "/");
         strcat(path, entry->d_name);
         /* on error print error number on stream */
         if (stat(path, &info) != 0)
           fprintf(stderr, "stat() error on %s: %s\n", path,
                   strerror(errno));
         /* If encountered a directory then invoke self */
         else if (S_ISDIR(info.st_mode))
            traverse(path,p_filtyp,p_patt);
         /* If encounted a file go thru below logic*/
         else if (S_ISREG(info.st_mode))
            /* find the last occurrence of dot to extension*/
            fileext = strrchr(entry->d_name,'.');
            /* copy pointer contents into work variables*/
            memset(feascarr, '\0',sizeof(feascarr));
            strcpy(feascarr,fileext);
            /* substring to get ext name */
            /*strcpy(extname," ");*/
            /*extname[0]='\0';*/
            memset(extname, '\0',sizeof(extname));
            extlen=strlen(feascarr);
            int extc = 1 ;
            while (extc < extlen)
            {
               extname[extc-1] = feascarr[extc];
               extc++;
            }
            extname[extc]='\0';
            /* If file type matched go thru below logic */
            /*int ret =strcmp(extname,p_filt);*/
            if ((strcmp(extname,p_filtyp))==0)
            {
               /* copy file name from pointer */
               strcpy(fnascarr,entry->d_name);
               /* calculate lenghts to do substring operation */
               totlen=strlen(fnascarr);
              /* fnascarr[totlen+1] = '\0'; */
               subep = totlen - strlen(feascarr);
               /* substring to get file name */
               strncpy(filename,fnascarr,subep);
               /* invoke function to check if pattern is matching*/
               filmatch = patmatch(filename,p_patt);
               /* if pattern is matching write to file for displaying it */
               if (filmatch == true)
               {
                  writetofile(filename,extname,subep);
               }
            }
         }
     }
     /* Close down the directory */
     closedir(dir);
     }
}

void writetofile(char p_filename[],char p_extname[], int p_subep)
{
   char w_filename[65];
   char w_extname[4];
   strncpy(w_filename,p_filename,p_subep);
   /*strcpy(w_filename,p_filename);*/
   strcpy(w_extname,p_extname);
   int w_filelen = strlen(w_filename);
   int w_extenlen = strlen(w_extname);
   REGFMT_both_t buf1;
   _RFILE *fp;
   memset(&buf1,' ',sizeof(buf1));
   memcpy(buf1.FILENAME,w_filename,w_filelen);
   memcpy(buf1.FILETYPE,w_extname,w_extenlen);
   if ((fp = _Ropen("QTEMP/REGF", "ar+")) == NULL)
   {
      printf("Cannot open QTEMP/REGF file\n");
   }
   _Rwrite(fp, (void *)&buf1, sizeof(buf1));
   _Rclose(fp);
}

bool patmatch(char text[], char pat[])
{
   int a,b;

   a = strlen(pat);
   b = strlen(text);

   for (int i = 0; i <= b - a; i++)
   {
      int j;

      for (j = 0; j < a; j++)
         if (text[i + j] != pat[j])
            break;
      if (j == a)
         {
         /*   printf("Pattern found at position %d \n", i+1); */
            return true;
         }

   }
}

void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

void main(int argc, char *argv[])
{
   char fn[21];
   char filtyp[5];
   char patt[66];
   int fnlen=0, counter=0, ftlen, ptlen;
   strcpy(fn,argv[1]);
   strcpy(filtyp,argv[2]);
   strcpy(patt,argv[3]);
   fnlen = strlen(fn);
   ftlen = strlen(filtyp);
   ptlen = strlen(patt);
   while (counter < fnlen)
   {
      if (fn[counter] == ' ')
         fn[counter] = '\0';
         counter++;
   }
   counter =0 ;
   while (counter < ftlen)
   {
      if (filtyp[counter] == ' ')
         filtyp[counter] = '\0';
         counter++;
   }
   counter =0 ;
   while (counter < ptlen)
   {
      if (patt[counter] == ' ')
         patt[counter] = '\0';
         counter++;
   }
   traverse(fn, filtyp, patt);
}
