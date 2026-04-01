
///Adds uninstalled software details to the software list.
/**
 * \return 0 On success
 * \return -1 On error
 */
int findUninstalledSoftwares()
{
     FILE *historyFile, *outputFile;
     int isEntryFound;
     char buffer[BUFFER_SIZE], oldPackageName[BUFFER_SIZE], newPackageName[BUFFER_SIZE], *cPtr = NULL;

     /**
      * The software.list, and sorted file of newly installed softwares software.list.tmp must be exist.
      */
          sprintf(buffer1, "[%s:%d %s] .\n\n", __FILE__, __LINE__, __FUNCTION__);
          log1(1, "%s", buffer1);
     historyFile = fopen(SOFTWARELIST_FILE, "r");
     if(historyFile == NULL)
     {
          return -1;
     }
     outputFile = fopen(SORTED_FILE, "r");
     if(outputFile == NULL)
     {
          fclose(historyFile);
          return -1;
     }
     /**
      * Each name from software.list file will be compared with the all names in the software.list.tmp file. Both files are sorted in this case so that it takes less coparisons when package name is exist in both files.
      */
     while(fgets(oldPackageName, BUFFER_SIZE, historyFile) != NULL)
     {

          char *oldAppName=strtok(oldPackageName,"|");
          if(oldAppName == NULL)
               continue;
    

          isEntryFound = 0;
          /**
           * To compare name in software.list with all names in software.list.tmp, pointer will be set to the beginning.
           */
          if(fseek(outputFile, 0, SEEK_SET) != 0)
          {
               fclose(outputFile);
               fclose(historyFile);
               return -1;
          }
          /**
           * The newline character in software.list and software.list.tmp file will be removed from lines before comparison.
           */
          while(fgets(newPackageName, BUFFER_SIZE, outputFile) != NULL)
          {
               char *newAppName=strtok(newPackageName,"|");
               if(newAppName == NULL)
                    continue;


                              if(strcmp(oldAppName, newAppName) == 0)
               {
                    /**
                     * If any name matches, comparison will be stopped and software name will not be added in list. If name is not found in new list, then only name will be added in linked list.
                     */
                    isEntryFound = 1;
                    break;
               }
          }
          if(isEntryFound == 0)
          {
               /**
                * The date information for uninstalled software would be '[00/000/0000] [00/00/0000]'.
                */

               // add uninstall packgae version to the list 
               oldAppName=strtok(NULL,"|");
               char *ptr=strstr(oldAppName,"\n");
               if(ptr!=NULL)
                    *ptr='\0';


               strcpy(buffer, "[00/000/0000] [00/00/0000]");
               if(addToNotificationList(oldPackageName, 1, buffer,oldAppName) != 0)
               {
               }
          }
     }
     fclose(historyFile);
     fclose(outputFile);
     return 0;
}

