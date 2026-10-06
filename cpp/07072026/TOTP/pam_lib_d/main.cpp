#include <stdio.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 2225
#define BUFFER_SIZE 1024

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <exception>

#include <vector> 
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "stopService.h"
#include "startService.h"
#include "readConfiguration.h"
#include "writeConfiguration.h"
#include "isBinaryRunning.h"

//#include"apply2FASettings.h"

using namespace std;
extern bool isDLPValid;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 2225
#define BUFFER_SIZE 1024

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <fstream> // Include the necessary header file

#include <cstring> // Required for strcpy/memcpy
#include <cstdlib> // Required for malloc

using namespace std;
typedef unsigned int uint;

std::string AVAILABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

/*char *strrev(char *str)
  {
  char *p1, *p2;

  if (! str || ! *str)
  return str;
  for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
  {
 *p1 ^= *p2;
 *p2 ^= *p1;
 *p1 ^= *p2;
 }
 return str;
 }*/

void TwoFactorLog(int currentLogLevel,char *Logstring)
{
     int debugLevel = 1;
     char buff[50]="";
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf","auth:LogLevel",buff) == 0)
     {
          debugLevel = atoi(buff);
     }

     if(currentLogLevel <= debugLevel)
     {
          FILE *fp = fopen("/var/MicroWorld/var/log/TwoFactor.log", "a");
          if(fp == NULL)
          {
               system("mkdir -p /var/MicroWorld/var/log");
               FILE *fp = fopen("/var/MicroWorld/var/log/TwoFactor.log", "a");
               if(fp == NULL)
               {
                    //printf("logfile Open Failed");
                    return;
               }
          }

          {
               time_t currentTime = time(NULL);
               struct tm *rpt_time = localtime(&currentTime);
               char rtime[24];
               memset(rtime, 0, 24);
               strftime(rtime, 23, "[%d-%b-%Y@%H:%M:%S] ", rpt_time);

               fprintf( fp, "%s : %s\n",rtime,Logstring);
          }
          fclose(fp);
     }
     return;
}


int remove_file_from_policies()
{
             TwoFactorLog(1,"Start restoring file .");

     struct passwd *user;

     //    printf("List of users:\n");

     setpwent();
     while ((user = getpwent()) != NULL) 
     {
          //printf("%s\n", user->pw_name);
          char copy_path[1024]="";
          sprintf(copy_path,"/home/%s",user->pw_name);
          if(access(copy_path,F_OK)==0)
          {
               char cmd1[1024]="";
               sprintf(cmd1,"%s/%s",copy_path,".key_authenticator");
               //printf("%s/%s",copy_path,".key_authenticator\n");
               remove(cmd1);

               char cmd2[1024]="";
               sprintf(cmd2,"%s/%s",copy_path,".key_authenticator_1");
               //printf("%s/%s",copy_path,".key_authenticator_1\n");  
               remove(cmd2);
          }
     }
     endpwent();
     remove("/root/.key_authenticator");
     remove("/root/.key_authenticator_1");
     remove("/root/.key_authenticator_2");

     if(access("/opt/MicroWorld/backup/sshd_config_ORG_ESCAN",F_OK)==0)
     {    system("cp -rf /opt/MicroWorld/backup/sshd_config_ORG_ESCAN /etc/ssh/sshd_config"); }
     if(access("/opt/MicroWorld/backup/gdm-password_ORG_ESCAN",F_OK)==0)
     {    system("cp -rf /opt/MicroWorld/backup/gdm-password_ORG_ESCAN /etc/pam.d/gdm-password"); }
     if(access("/opt/MicroWorld/backup/lightdm_ORG_ESCAN",F_OK)==0)
     {    system("cp -rf /opt/MicroWorld/backup/lightdm_ORG_ESCAN /etc/pam.d/lightdm"); }
     if(access("/opt/MicroWorld/backup/sshd_ORG_ESCAN",F_OK)==0)
     {     system("cp -rf /opt/MicroWorld/backup/sshd_ORG_ESCAN /etc/pam.d/sshd");  }
     /* if(access("/opt/MicroWorld/etc/sudo_ORG_ESCAN",F_OK)==0)
        system("mv /opt/MicroWorld/etc/sudo_ORG_ESCAN /etc/pam.d/sudo");
        if(access("/opt/MicroWorld/etc/su_ORG_ESCAN",F_OK)==0)
        system("mv /opt/MicroWorld/etc/su_ORG_ESCAN /etc/pam.d/su");*/
     // system("systemctl restart sshd.service");
     system("systemctl restart sshd.service 2>/dev/null || systemctl restart ssh.service");

             TwoFactorLog(1,"Restoring file successfully.");
     return 0;
}

int change_permission(char *file_path)
{
     //const char *file_path = "/path/to/your/file";
     mode_t new_permissions = S_IRUSR | S_IWUSR; // 600 permissions

     if (chmod(file_path, new_permissions) == 0) 
     {
          //printf("File permissions changed to 600 successfully.\n");
     }
     else
     {
          perror("Failed to change file permissions");
     }
}
int change_ownership(char *file_path,uid_t uid)
{
     //    const char *file_path = "/path/to/your/file";
     uid_t new_owner_id = uid;  // UID of the new owner user
     gid_t new_group_id = uid;  // GID of the new owner group


     // Check if uid is root (0) and file_path starts with "/root/"
     if (uid != 0 && strncmp(file_path, "/root/", 6) == 0) 
     {
          //pw->pw_uid = 524288 and
          //   file_path = /root/.key_authenticator* to block this 

          return 1;   // or 0 for success
     } 
     else 
     {
          // Change the ownership of the file
          if (chown(file_path, new_owner_id, new_group_id) == 0) 
          {
               change_permission(file_path);
               //printf("File ownership changed successfully.\n"); 
          }
          else
          {
               perror("Failed to change file ownership");
               //printf("Failed  ownership changed .\n"); 
          }

     }
}

void stringRev( char *revOutput )
{
     char str[1024];
     memset(str,0,sizeof(str));
     strcpy( str, revOutput);

     int front=0, end = strlen(str)-1;
     char temp;
     for(front = 0 ; front < end; front++, end-- )
     {
          temp = str[front];
          str[front] = str[end];
          str[end] = temp;
     }

     strcpy(revOutput, str);
}

static std::string base64_encode(const std::string &in)
{
     std::string out;

     int val=0, valb=-6;
     for (uint jj = 0; jj < in.size(); jj++)
     {
          //char c = in[jj];
          char c = in[jj];
          val = (val<<8) + c;
          valb += 8;
          while (valb>=0)
          {
               out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
               valb-=6;
          }
     }

     if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
     while (out.size()%4) out.push_back('=');
     return(out);
}

static std::string base64_decode(const std::string &in)
{
     std::string out;

     std::vector<int> T(256,-1);
     for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 

     int val=0, valb=-8;
     for (uint jj = 0; jj < in.size(); jj++)
     {
          //char c = in[jj];
          char c = in[jj];
          if (T[c] == -1)
               break;

          val = (val<<6) + T[c];
          valb += 6;
          if (valb>=0)
          {
               out.push_back(char((val>>valb)&0xFF));
               valb-=8;
          }
     }

     return(out);
}

int index(char c)
{
     for(uint ii = 0; ii < AVAILABLE_CHARS.size(); ii++)
     {
          if (AVAILABLE_CHARS[ii] == c)
               return(ii);
     }

     return(-1);
}

std::string extend_key(std::string& msg, std::string& key)
{
     //generating new key
     int msgLen = msg.size();
     std::string newKey(msgLen, 'x');
     int keyLen = key.size(), i, j;
     for(i = 0, j = 0; i < msgLen; ++i, ++j)
     {
          if (j == keyLen)
               j = 0;
          newKey[i] = key[j];
     }

     newKey[i] = '\0';
     return(newKey);
}

std::string encrypt_wfx(std::string& msg, std::string& key)
{
     uint msgLen = msg.size(), i ; //keyLen = key.size();
     std::string encryptedMsg(msgLen, 'x');
     // char newKey[msgLen], encryptedMsg[msgLen], decryptedMsg[msgLen];

     std::string newKey = extend_key(msg, key);

     //encryption
     for(i = 0; i < msgLen; ++i)
     {
          // std::cout << msg[i] << " " << myisalnum(msg[i]) << std::endl;

          if (isalnum(msg[i]) || msg[i] == ' ')
          {
               encryptedMsg[i] = AVAILABLE_CHARS[((index(msg[i]) + index(newKey[i])) % AVAILABLE_CHARS.size())];
          }
          else
          {
               encryptedMsg[i] = msg[i];
          }
     }

     encryptedMsg[i] = '\0';
     return(encryptedMsg);
}

std::string decrypt_wfx(std::string& encryptedMsg, std::string& newKey)
{
     // decryption
     int msgLen = encryptedMsg.size();
     std::string decryptedMsg(msgLen, 'x');
     int i;
     for(i = 0; i < msgLen; ++i)
     {
          if (isalnum(encryptedMsg[i]) || encryptedMsg[i] == ' ')
          {
               decryptedMsg[i] = AVAILABLE_CHARS[(((index(encryptedMsg[i]) - index(newKey[i])) + AVAILABLE_CHARS.size()) % AVAILABLE_CHARS.size())];
          }
          else
          {
               decryptedMsg[i] = encryptedMsg[i];
          }
     }

     decryptedMsg[i] = '\0';
     return(decryptedMsg);
}

std::string encrypt1(std::string& msg, std::string& key)
{
     std::string b64_str = base64_encode(msg);
     std::string wfx_msg = encrypt_wfx(b64_str, key);
     // std::cout << wfx_msg << std::endl;
     return(wfx_msg);
}

std::string decrypt(std::string& encrypted_msg, std::string& key)
{
     std::string newKey = extend_key(encrypted_msg, key);
     std::string b64_encoded_str = decrypt_wfx(encrypted_msg, newKey);
     std::string b64_decode_str = base64_decode(b64_encoded_str);
     return(b64_decode_str);
}

//encrypt_flag = 0 for encrypt
//encrypt_flag = 1 for decrypt
void encstrWrap(char *Str, int encrypt_flag, char *Output)
{
     //if (argc != 4) {
     //    std::cout << "Usage: ./a.out message key (0:encrypt|1:decrypt)" << std::endl;
     //    std::cout << "./a.out \"Hello world\" MYPRIVATEKEY 0" << std::endl;
     //    std::cout << "./a.out ttz9JqxZHBClNtu= MYPRIVATEKEY 1" << std::endl;
     //    return -1;
     //}

     if (Output)
     {
          Output[0] = 0;
     }

     if (Str == NULL || Str[0] == 0)
     {
          return;
     }

     static char EncryptionKey1[]="baksiweoguialbbvbblhywraaubnakldsafoiuyyoq";
     static char EncryptionKey2[]="zymabvvuuywpoozcrreiykbbpbaqzweppoidcwxzcw";

     char *ec1 = (char*)EncryptionKey1;
     char *ec2 = (char*)EncryptionKey2;
     std::string key1 = ec1;
     std::string key2 = ec2;

     int len;
     int j;

     if (encrypt_flag == 0)
     {
          std::string msg = Str;

          std::string st1 = encrypt1(msg, key1);
          std::string st2 = encrypt1(st1, key2);
          //std::string st3 = encrypt(st2, key1);
          //std::string st4 = encrypt(st3, key2);

          strcpy(Output, st2.c_str());
          len = strlen(Output);

          for (j = len-1; j >= 0; j--)
          {
               if (Output[j] != '=')
               {
                    break;
               }
               Output[j] = '$';
          }
          //char *rev = strrev(Output);
          //strcpy(Output, rev);
          //std::reverse( string(Output).begin(), string(Output).end());
          stringRev(Output);

     }
     else
     {
          len = strlen(Str);
          char *Input = (char*)malloc(len + 1);

          if (Input == NULL)
          {
               return;
          }

          strcpy(Input, Str);

          //char *rev = strrev(Input);
          //strcpy(Input, rev);
          //std::reverse( string(Input).begin(), string(Input).end());
          stringRev(Input);


          for (j = len-1; j >= 0; j--)
          {
               if (Input[j] != '$')
               {
                    break;
               }
               Input[j] = '=';
          }

          std::string msg = Input;

          //std::string st4 = decrypt(msg, key2);
          //std::string st3 = decrypt(st4, key1);
          std::string st2 = decrypt(msg, key2);
          std::string st1 = decrypt(st2, key1);
          strcpy(Output, st1.c_str());
          free(Input);
     }
}

//encrypt_flag = 0 for encrypt
//encrypt_flag = 1 for decrypt
void encstr(char *Str, int encrypt_flag, char *Output)
{
     try
     {
          encstrWrap(Str, encrypt_flag, Output);

     }

     catch(...) // catch all
     {
          //writeLog(NULL, "Exception in encstr()...");
          cout << "Exception in encstr()..." << endl; 
     }
}




int find_the_mc_value(char *input,char *op)
{
     //	char input[] = "Welcome [C:1;P:0;IP:192.168.2.54][SRV] eScan RPC V11 [MC:901b0d95-c551-47ea-b7d4-47c20ad3b35e][NHX] Server 4.0.0.612 [Nishikant] [ES:11.0.2.659]";
     
     char *mc_start = strstr(input, "[MC:");
     if (mc_start == NULL) 
     {
          //printf("MC string not found.\n");
          return 1;
     }
     char mc_data[36];  // UUIDs typically have 36 characters
     int j=0;
     int start_concate=0;
     for(int i=0;i<strlen(mc_start);i++)
     {
          if(mc_start[i]==':')
          {
               start_concate=1;	
          }
          if(mc_start[i]==']')
          {
               //printf("MC data: %s\n", mc_data);
               strcpy(op,mc_data);
               start_concate=1;	
               return 0;
          }

          if(start_concate==1)
          {
               mc_data[j++]=mc_start[i];	
          }
     }


     // Print the extracted MC data

     return 0;

}

int find_out_the_telnet_string(char *ip,char *op)
{
     int sockfd, n;
     struct sockaddr_in serv_addr;
     struct hostent *server;
     char buffer[BUFFER_SIZE];

     // Create a socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
     {
          perror("Error opening socket");
          return -1;
          exit(1);
     }

     // Get the server's IP address
     //server = gethostbyname("192.168.0.165");
     server = gethostbyname(ip);
     if (server == NULL) 
     {
          //fprintf(stderr, "Error, no such host\n");
          TwoFactorLog(2,"Error No such Host Present");
          return -1;
          exit(1);
     }

     // Initialize the server address structure
     memset(&serv_addr, 0, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
     serv_addr.sin_port = htons(PORT);

     // Connect to the server
     if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
     {
          perror("Error connecting");
          return -1;
          exit(1);
     }

     memset(buffer, 0, sizeof(buffer));
     n = read(sockfd, buffer, sizeof(buffer)-1);
     if (n < 0) 
     {
          perror("Error reading from socket");
          TwoFactorLog(1,"Error Reading from socket");
          return -1;
          exit(1);
     }

     strcpy(op,buffer);

     // Close socket
     close(sockfd);

     return 0;

}


static const char *getUserName(uid_t uid) 
{
     struct passwd pwbuf, *pw;
     char *buf = NULL;
     char *user = NULL;

#ifdef _SC_GETPW_R_SIZE_MAX
     long len = sysconf(_SC_GETPW_R_SIZE_MAX);
     if (len <= 0) 
     {
          len = 4096;
     }
#else
     long len = 4096;
#endif

     buf =(char *) malloc(len);
     if (buf == NULL) 
     {
          perror("malloc()");
          return NULL;
          exit(1);
     }

     if (getpwuid_r(uid, &pwbuf, buf, len, &pw) != 0 || pw == NULL) 
     {
          // Error getting username, fallback to UID
          user = (char *)malloc(32);
          if (user == NULL) 
          {
               perror("malloc()");
               return NULL;
               exit(1);
          }
          snprintf(user, 32, "%d", (int)uid);
     }
     else
     {
          // Username found
          user = strdup(pw->pw_name);
          if (user == NULL) 
          {
               perror("strdup()");
               return NULL;
               exit(1);
          }
     }

     free(buf);
     return user;
}

char* GetFirst16Char(const char* password) 
{

     char* encoded16 = (char*)malloc(17 * sizeof(char));
     int encodedIndex = 0;
     for (int i = 0; i < strlen(password) && encodedIndex < 16; i++) 
     {
          if ((password[i] >= 'a' && password[i] <= 'z') || (password[i] >= 'A' && password[i] <= 'Z') || (password[i] >= '0' && password[i] <= '9')) 
          {
               // Excluding characters ' ', '8', '9', '0', 'i', 'I', 'O', 'o', '1'
               // [' ', '8', '9', '0', 'i', 'I', 'O', 'o', '1'] and replace with respective following characters
               // ['A', 'b', 'C', 'd', 'E', 'f', 'G', 'h', 'J']
               if (password[i] == ' ' || password[i] == '8' || password[i] == '9' || password[i] == '0' || password[i] == 'i' || password[i] == 'I' || password[i] == 'O' || password[i] == 'o' || password[i] == '1') 
               {
                    switch(password[i]) 
                              {
                              case ' ':
                                   encoded16[encodedIndex++] = 'A';
                                   break;
                              case '8':
                                   encoded16[encodedIndex++] = 'b';
                                   break;
                              case '9':
                                   encoded16[encodedIndex++] = 'C';
                                   break;
                              case '0':
                                   encoded16[encodedIndex++] = 'd';
                                   break;
                              case 'i':
                                   encoded16[encodedIndex++] = 'E';
                                   break;
                              case 'I':
                                   encoded16[encodedIndex++] = 'f';
                                   break;
                              case 'O':
                                   encoded16[encodedIndex++] = 'G';
                                   break;
                              case 'o':
                                   encoded16[encodedIndex++] = 'h';
                                   break;
                              case '1':
                                   encoded16[encodedIndex++] = 'J';
                                   break;

                              }
               }
               else
               {
                    encoded16[encodedIndex++] = password[i];
               }
          }
     }
     encoded16[encodedIndex] = '\0';

     return encoded16;
}


char* GetFirstCharIgnore(const char* password) 
{
     char* encoded = (char*)malloc(strlen(password) +1 * sizeof(char));
     int encodedIndex = 0;
     
     for (int i = 0; i < strlen(password) ; i++) 
     {
          if ((password[i] >= 'a' && password[i] <= 'z') || (password[i] >= 'A' && password[i] <= 'Z') || (password[i] >= '0' && password[i] <= '9')) 
          {
               // Excluding characters ' ', '8', '9', '0', 'i', 'I', 'O', 'o', '1'
               if (password[i] == ' ' || password[i] == '8' || password[i] == '9' || password[i] == '0' || password[i] == 'i' || password[i] == 'I' || password[i] == 'O' || password[i] == 'o' || password[i] == '1') 
               {
                    continue;
               }
               encoded[encodedIndex++] = password[i];
          }
     }
     encoded[encodedIndex] = '\0';

     return encoded;
}


char* GetFirstCharIgnoreAllUsers(const char* password) 
{
     char* encoded = (char*)malloc(17 * sizeof(char));
     int encodedIndex = 0;
     
     for (int i = 0; i < strlen(password) && encodedIndex < 16; i++) 
     {
          if ((password[i] >= 'a' && password[i] <= 'z') || (password[i] >= 'A' && password[i] <= 'Z') || (password[i] >= '0' && password[i] <= '9')) 
          {
               // Excluding characters ' ', '8', '9', '0', 'i', 'I', 'O', 'o', '1'
               if (password[i] == ' ' || password[i] == '8' || password[i] == '9' || password[i] == '0' || password[i] == 'i' || password[i] == 'I' || password[i] == 'O' || password[i] == 'o' || password[i] == '1') 
               {
                    continue;
               }
               encoded[encodedIndex++] = password[i];
          }
     }
     encoded[encodedIndex] = '\0';

     return encoded;
}


char* extract_server_address_from_conf() 
{
     FILE* file = fopen("/opt/MicroWorld/etc/winclient.conf", "r");
     if (!file) 
     {
          return NULL;
     }

     char line[256];
     int eserv_section = 0;

     // Allocate memory for server address
     char* server_address = (char*)malloc(256 * sizeof(char));
     if (!server_address) 
     {
          fclose(file);
          return NULL;
     }

     // Iterate through each line in the file
     while (fgets(line, sizeof(line), file)) 
     {
          // Check if the line contains '[eserv]'
          if (strstr(line, "[eserv]") != NULL) 
          {
               eserv_section = 1;
               continue;
          }

          if (eserv_section) 
          {
               // Look for the 'Server=' key
               char* server_pos = strstr(line, "Server=");
               if (server_pos != NULL) 
               {
                    // Extract and return the server address
                    sscanf(server_pos, "Server=%s", server_address);
                    fclose(file);
                    return server_address;
               }

               // Break the loop if we encounter a new section or an empty line
               if (line[0] == '[' || line[0] == '\n') 
               {
                    break;
               }
          }
     }

     fclose(file);
     // Free memory if server address is not found
     free(server_address);
     return NULL;
}


void toUpperCase(char *str) 
{
     while (*str) 
     {
          *str = toupper(*str);
          str++;
     }
}


int configure_sshfile_for_authenticator()
{
     FILE *source_file, *destination_file;
     char buffer[1024]="";



     char cPtr[100]="";
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf", (char *)"auth:Status", cPtr)== 0)
     {
          if(atoi(cPtr)==1 || atoi(cPtr)==2)
          {
               /*   if(access("/etc/ssh/sshd_config",F_OK)==0 && access("/opt/MicroWorld/backup/sshd_config_ORG_ESCAN",F_OK)==0)
                    {
                    return 0;
                    }*/
          }
          //      return -1;
     }


     if(access("/etc/ssh/sshd_config",F_OK)!=0)
     {
          //printf("sshd file not present\n");
          return 0;
     }

     // Open the source file for reading
     if(access("/opt/MicroWorld/backup/sshd_config_ORG_ESCAN",F_OK)!=0)
     {
          system("cp -rf /etc/ssh/sshd_config /opt/MicroWorld/backup/sshd_config_ORG_ESCAN");
     }
     source_file = fopen("/etc/ssh/sshd_config", "r");
     if (source_file == NULL) {
          perror("Error opening source file");
          return 1;
     }

     // Open the destination file for writing
     destination_file = fopen("/etc/ssh/sshd_config_new", "w");
     if (destination_file == NULL) 
     {
          perror("Error opening destination file");
          fclose(source_file);  // Close the source file
          return 1;
     }

     while (fgets(buffer, 2024, source_file) != NULL) 
     {
          if(strstr(buffer,"UsePAM")!=NULL) //please add proper string and fprintf statement
          {
               fprintf(destination_file, "%s", "UsePAM yes\n");
               continue;
          }
          if(strstr(buffer,"KbdInteractiveAuthentication")!=NULL) //please add proper string and fprintf statement
          {
               fprintf(destination_file, "%s", "KbdInteractiveAuthentication yes\n");
               continue;
          }
          if(strstr(buffer,"ChallengeResponseAuthentication")!=NULL && strcasestr(buffer,"no")!=NULL && strcasestr(buffer,"#")==NULL) //please add proper string and fprintf statement
          {
               fprintf(destination_file, "%s", "ChallengeResponseAuthentication yes\n");
               continue;
          }
          fprintf(destination_file, "%s", buffer);
     }


     // Close both files
     fclose(source_file);
     fclose(destination_file);
     rename("/etc/ssh/sshd_config_new","/etc/ssh/sshd_config");
     //printf("File copied successfully.\n");

     return 0;

}


//ketan 
char *getUserString(int status)
{  

     static char op[2048]="";  // Make it static so it persists after function returns
     char str[2048] = "";
     const char* delim = ",";
     char* token;
     std::string s1 = "";

     // Clear the output buffer
     memset(op, 0, sizeof(op));

     /* get the first token */
     /* walk through other tokens */
     //ketan status     if(status == 2)
     if(status == 2 || status == 1) // as per vaibahv 1 ya 2 both will come.
     {
          if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf" , (char *)"auth:Users", str) != 0)
          {   
              //printf("unable to read users name \n");
             //return NULL;
             // if status = 1 then return this
               s1 = "auth required /opt/MicroWorld/lib/key_authenticator.so";
               strncpy(op,s1.c_str(),sizeof(op) - 1);

     // printf("op = '%s'\n",op);
               return op; 
          }
          token = strtok(str, delim);

          while( token != NULL ) 
          {
               char final_str[2024]="";
               //sprintf(final_str,"auth [success=1 default=ignore] pam_succeed_if.so user != %s \nauth required /opt/MicroWorld/lib/key_authenticator.so\n auth [success=1 default=ignore] pam_succeed_if.so user != root \nauth required /opt/MicroWorld/lib/key_authenticator.so\n",token);
               sprintf(final_str,"auth [success=1 default=ignore] pam_succeed_if.so user != %s \nauth required /opt/MicroWorld/lib/key_authenticator.so\n",token);
               s1+=final_str;
               //      printf( " %s\n", token );
               // printf("inside 2 =%d\n",status);
               token = strtok(NULL, delim);
          }
     }
     else if (status == 1)
     {
          status = 1;
          //printf("inside 1");
          s1 = "auth required /opt/MicroWorld/lib/key_authenticator.so";
     }
     else
     {

     }

     //printf("%s\n",s1.c_str());
     strncpy(op,s1.c_str(),sizeof(op) - 1);

//printf("op = '%s'\n",op);
     return op; 

}

int gdm_password_forGUI(int status)
{
     FILE *source_file, *destination_file;
     char buffer[1024]="";
     char *new_string="";
     char cPtr[100]="";

     const char *file_path = "/etc/pam.d/gdm-password";
     //const char *new_string = "auth required /opt/MicroWorld/lib/key_authenticator.so";
     new_string = getUserString(status);
     //printf("string=%s\n",new_string);
     // Open the file in append mode
     FILE *file = fopen(file_path, "a+");
     if (file == NULL) 
     {
          perror("Error opening file");
          return 1;
     }

     // Move to the end of the file
     fseek(file, 0, SEEK_END);

     // Find the start of the last line
     long pos = ftell(file);
     while (pos > 0 && fgetc(file) != '\n') 
     {
          fseek(file, --pos, SEEK_SET);
     }

     // Append the new string
     fprintf(file, "%s\n", new_string);

     // Close the file
     fclose(file);

     //printf("String appended to the last line of the file.\n");

     // Open the source file for reading
     /*    source_file = fopen("/etc/pam.d/gdm-password", "r");
           if (source_file == NULL) {
           perror("Error opening source file");
           return 1;
           }

     // Open the destination file for writing
     destination_file = fopen("/etc/pam.d/gdm-password_new", "w");
     if (destination_file == NULL) {
     perror("Error opening destination file");
     fclose(source_file);  // Close the source file
     return 1;
     }

     while (fgets(buffer, 2024, source_file) != NULL) {
     if(strstr(buffer,"include common-auth")!=NULL)
     {
     fprintf(destination_file, "%s", buffer);
     fprintf(destination_file, "\n%s\n","auth required /opt/MicroWorld/lib/key_authenticator.so" );
     }
     else
     {
     fprintf(destination_file, "%s", buffer);
     }
     }


     // Close both files
     fclose(source_file);
     fclose(destination_file);
     rename("/etc/pam.d/gdm-password_new","/etc/pam.d/gdm-password");

     printf("File copied successfully.\n");
     */
     return 0;


}


int lightdm_forGUI(int status)
{

     FILE *source_file, *destination_file;
     char buffer[1024]="";
     char *new_string="";
     char cPtr[100]="";

     const char *file_path = "/etc/pam.d/lightdm";
     //const char *new_string = "auth required /opt/MicroWorld/lib/key_authenticator.so";
     new_string = getUserString(status);
     //printf("string=%s\n",new_string);
     // Open the file in append mode
     FILE *file = fopen(file_path, "a+");
     if (file == NULL) 
     {
          perror("Error opening file");
          return 1;
     }

     // Move to the end of the file
     fseek(file, 0, SEEK_END);

     // Find the start of the last line
     long pos = ftell(file);
     while (pos > 0 && fgetc(file) != '\n') 
     {
          fseek(file, --pos, SEEK_SET);
     }

     // Append the new string
     fprintf(file, "%s\n", new_string);

     // Close the file
     fclose(file);

     //printf("String appended to the last line of the file.\n");

     return 0 ;

}


int pamd_file_for_authenticator_forGUI(int status)
{
     FILE *source_file, *destination_file;
     char buffer[1024]="";
     char *new_string="";
     char cPtr[100]="";
     int ret = 0;
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf" , (char *)"auth:Status", cPtr)== 0)
     {
          if(atoi(cPtr)==1 || atoi(cPtr)==2)
          {
               /* if(access("/etc/pam.d/gdm-password",F_OK)==0 && access("/opt/MicroWorld/backup/gdm-password_ORG_ESCAN",F_OK)==0)
                  {
                  return 0;
                  }*/
          }
          //      return -1;
     }

     if( (access("/etc/pam.d/gdm-password",F_OK)!=0) && (access("/etc/pam.d/lightdm",F_OK) !=0) )
     {
          //printf("gdm and lightdm file not present\n");
          return 0;
     }
     if(access("/opt/MicroWorld/backup/gdm-password_ORG_ESCAN",F_OK)!=0)
     {
          system("cp -rf /etc/pam.d/gdm-password /opt/MicroWorld/backup/gdm-password_ORG_ESCAN >/dev/null 2>&1");
     }

     if(access("/opt/MicroWorld/backup/lightdm_ORG_ESCAN",F_OK) !=0)
     {    system("cp -rf /etc/pam.d/lightdm  /opt/MicroWorld/backup/lightdm_ORG_ESCAN  >/dev/null 2>&1"); }

   if( (access("/etc/pam.d/lightdm",F_OK) ==0) )
     {
          ret = lightdm_forGUI(status);
     }


     if( (access("/etc/pam.d/gdm-password",F_OK) ==0) )
     {
          ret = gdm_password_forGUI(status);
     }

     return ret;
}


void toLowerCase(char *str) 
{
     int i = 0;
     while (str[i] != '\0') 
     {
          str[i] = tolower(str[i]); // Convert each character to lowercase
          i++;
     }
}

void concat_until_16(char *dest,char *src) 
{
     int dest_len = 0;
     int src_len = strlen(src);

     // Loop to append the string until the total length reaches 16
     while (dest_len < 16) 
     {
          // Append the source string or as much as can fit without exceeding 16 characters
          int to_copy = (16 - dest_len) < src_len ? (16 - dest_len) : src_len;

          // Copy the substring of `src` to `dest`
          strncat(dest, src, to_copy);

          // Update the length of `dest`
          dest_len += to_copy;
     }
}






char* generateAdminString(const std::string& username, const std::string& op)
{
    // Custom alphabet (all possible characters)
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int alphabet_len = alphabet.length();

    std::string result = "";

    // For each character in op, find its index in alphabet
    std::vector<int> indices;
    for (char c : op)
    {
        size_t pos = alphabet.find(c);
        if (pos != std::string::npos)
        {
            indices.push_back(pos);
        }
        else
        {
            // fallback: use ASCII value mod alphabet_len
            indices.push_back((int)c % alphabet_len);
        }
    }

    int current_index = 0;
    int start_char_index = 0;
    // Even username: skip 2 chars (take every 3rd)
    // Odd username: skip 1 char (take every 2nd)
    int skip_count = (username.length() % 2 == 0) ? 3 : 2;

    while (result.length() < 16)
    {
        // Get the character from alphabet using the mapped index
        int idx = indices[current_index % indices.size()];
        result += alphabet[idx];

        int next_index = current_index + skip_count;
        if (next_index >= (int)indices.size())
        {
            start_char_index++;
            current_index = start_char_index % indices.size();
        }
        else
        {
            current_index = next_index;
        }
    }

    char* re1 = (char*)malloc(17 * sizeof(char));
    if (re1 == nullptr) return nullptr;

    std::memcpy(re1, result.c_str(), 16);
    re1[16] = '\0';

    return re1;
}


int calculate_secret_key_for_particular_user2(char *name,char *mc,char *op)
{
     char final_op[1024]="";
     char secret_key_generation[2024]="";
     toLowerCase(name);
     sprintf(secret_key_generation,"%s%s%s%s%s%s",name,mc,name,mc,name,mc);
    
    
     strcpy(final_op,GetFirst16Char(secret_key_generation));
   
   
   
     char op6[1024]="";
     encstr(final_op,0,op6);


     char final_op1[1024]="";
     strcpy(final_op1,GetFirstCharIgnore(op6));
     
     

     strcpy(op,generateAdminString(name,final_op1));

     return 0;
}

int calculate_secret_key_for_particular_user(char *name,char *mc,char *op)
{
     char final_op[1024]="";
     char secret_key_generation[2024]="";
     toLowerCase(name);
    
    
     char dest[17]="";
     concat_until_16(dest,mc);
     sprintf(secret_key_generation,"%s%s",name,dest,name,dest);
     strcpy(final_op,GetFirst16Char(secret_key_generation));
     
     
     
     char op6[1024]="";
     encstr(final_op,0,op6);
     
     
     char final_op1[1024]="";
     strcpy(final_op1,GetFirstCharIgnore(op6));
     
     

     strcpy(op,generateAdminString(name,final_op1));

     
     return 0;
}


/*
int calculate_secret_key_for_particular_user2(char *name,char *mc,char *op)
{
     char final_op[1024]="";
     char secret_key_generation[2024]="";
     toLowerCase(name);
     sprintf(secret_key_generation,"%s%s%s%s%s%s",name,mc,name,mc,name,mc);
     strcpy(final_op,GetFirst16Char(secret_key_generation));
     char op6[1024]="";
     //toUpperCase(final_op);
     encstr(final_op,0,op6);
     char final_op1[1024]="";
     strcpy(final_op1,GetFirst16CharIgnore(op6));
 //    strcpy(final_op1,GetFirst16Char(op6));
     char FinalString[1024] = "";
     strcpy(FinalString,final_op1);

     strcpy(op,generateAdminString(name,FinalString));




*/
     /*        char google_auth_path[1024]="";
               sprintf(google_auth_path,"%s/%s",filepath,".key_authenticator");
               FILE *fp;
               fp=fopen(google_auth_path,"w");
               if(fp==NULL)
               return -1;
               else
               {
               fprintf(fp,"%s \n\" TOTP_AUTH\n",final_op1);
               fclose(fp);
               }
               change_ownership(google_auth_path,pw->pw_uid);
               */
/*     return 0;
}


int calculate_secret_key_for_particular_user(char *name,char *mc,char *op)
{
     char final_op[1024]="";
     char secret_key_generation[2024]="";
     toLowerCase(name);
     char dest[17]="";
     concat_until_16(dest,mc);
     sprintf(secret_key_generation,"%s%s",name,dest,name,dest);
     //toLowerCase(secret_key_generation);  
     //printf("secreteKey=%s\n",secret_key_generation); 
     strcpy(final_op,GetFirst16Char(secret_key_generation));
     char op6[1024]="";
     //toUpperCase(final_op);
     encstr(final_op,0,op6);
     char final_op1[1024]="";
     strcpy(final_op1,GetFirst16Char(op6));
     strcpy(op,final_op1);
     return 0;
}

*/
int pamd_file_for_authenticator_forsu(int status)
{
     FILE *source_file, *destination_file;
     char buffer[1024]="";
     char *new_string="";
     char cPtr[100]="";
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf" , (char *)"auth:Status", cPtr)== 0)
     {
          if(atoi(cPtr)==1 || atoi(cPtr)==2)
          {
               if(access("/etc/pam.d/su",F_OK)==0 && access("/opt/MicroWorld/etc/su_ORG_ESCAN",F_OK)==0)
               {
                    return 0;
               }
          }
          //      return -1;
     }

     if(access("/etc/pam.d/su",F_OK)!=0)
     {
          //printf("su file not present\n");
          return 0;
     }
     if(access("/opt/MicroWorld/etc/su_ORG_ESCAN",F_OK)!=0)
     {
          system("cp -rf /etc/pam.d/su /opt/MicroWorld/etc/su_ORG_ESCAN");
     }


     const char *file_path = "/etc/pam.d/su";
     //const char *new_string = "auth required /opt/MicroWorld/lib/key_authenticator.so";
     new_string = getUserString(status);
     //printf("string=%s\n",new_string);
     // Open the file in append mode
     FILE *file = fopen(file_path, "a+");
     if (file == NULL) 
     {
          perror("Error opening file");
          return 1;
     }

     // Move to the end of the file
     fseek(file, 0, SEEK_END);

     // Find the start of the last line
     long pos = ftell(file);
     while (pos > 0 && fgetc(file) != '\n') 
     {
          fseek(file, --pos, SEEK_SET);
     }

     // Append the new string
     fprintf(file, "%s\n", new_string);

     // Close the file
     fclose(file);


}


int pamd_file_for_authenticator_forsudo(int status)
{
     FILE *source_file, *destination_file;
     char buffer[1024]="";
     char *new_string="";
     char cPtr[100]="";
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf" , (char *)"auth:Status", cPtr)== 0)
     {
          if(atoi(cPtr)==1 || atoi(cPtr)==2)
          {
               if(access("/etc/pam.d/sudo",F_OK)==0 && access("/opt/MicroWorld/etc/sudo_ORG_ESCAN",F_OK)==0)
               {
                    return 0;
               }
          }
          //      return -1;
     }

     if(access("/etc/pam.d/sudo",F_OK)!=0)
     {
          //printf("sudo file not present\n");
          return 0;
     }
     if(access("/opt/MicroWorld/etc/sudo_ORG_ESCAN",F_OK)!=0)
     {
          system("cp -rf /etc/pam.d/sudo /opt/MicroWorld/etc/sudo_ORG_ESCAN");
     }


     const char *file_path = "/etc/pam.d/sudo";
     //const char *new_string = "auth required /opt/MicroWorld/lib/key_authenticator.so";
     new_string = getUserString(status);
     //printf("string=%s\n",new_string);
     // Open the file in append mode
     FILE *file = fopen(file_path, "a+");
     if (file == NULL) 
     {
          perror("Error opening file");
          return 1;
     }

     // Move to the end of the file
     fseek(file, 0, SEEK_END);

     // Find the start of the last line
     long pos = ftell(file);
     while (pos > 0 && fgetc(file) != '\n') 
     {
          fseek(file, --pos, SEEK_SET);
     }

     // Append the new string
     fprintf(file, "%s\n", new_string);

     // Close the file
     fclose(file);

}



int pamd_file_for_authenticator_forssh(int status)
{
     FILE *source_file, *destination_file;
     char buffer[1024]="";
     char *new_str="";
     char cPtr[100]="";
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf" , (char *)"auth:Status", cPtr)== 0)
     {
          if(atoi(cPtr)==1 || atoi(cPtr)==2)
          {
               /*  if(access("/etc/pam.d/sshd",F_OK)==0 && access("/opt/MicroWorld/backup/sshd_ORG_ESCAN",F_OK)==0)
                   {
                   return 0;
                   }*/
          }
          //        return -1;
     }


     if(access("/etc/pam.d/sshd",F_OK)!=0)
     {
          //printf("file not present\n");
          return 0;
     }
     if(access("/opt/MicroWorld/backup/sshd_ORG_ESCAN",F_OK)!=0)
     {
          system("cp -rf /etc/pam.d/sshd /opt/MicroWorld/backup/sshd_ORG_ESCAN");
     }



     // Open the source file for reading
     source_file = fopen("/etc/pam.d/sshd", "r");
     if (source_file == NULL) 
     {
          perror("Error opening source file");
          return 1;
     }

     // Open the destination file for writing
     destination_file = fopen("/etc/pam.d/sshd_new", "w");
     if (destination_file == NULL) 
     {
          perror("Error opening destination file");
          fclose(source_file);  // Close the source file
          return 1;
     }

     while (fgets(buffer, 2024, source_file) != NULL) 
     {
          if(strcasestr(buffer,"account")!=NULL && strcasestr(buffer,"required")!=NULL && strcasestr(buffer,"pam_nologin")!=NULL)          {
               //printf("READ: %s", buffer);
               fprintf(destination_file, "%s", buffer);
               new_str = getUserString(status);
               //printf("Injected line: %s\n", new_str);
               //fprintf(destination_file, "\n%s\n","auth required /opt/MicroWorld/lib/key_authenticator.so" );
               //printf("sshString=%s\n",new_str);
               fprintf(destination_file, "\n%s\n",new_str);
          }
          else
          {
               fprintf(destination_file, "%s", buffer);
          }
     }


     // Close both files
     fclose(source_file);
     fclose(destination_file);
     if (rename("/etc/pam.d/sshd_new","/etc/pam.d/sshd") != 0) 
     {
          perror("Rename failed");
     }
     //printf("File copied successfully.\n");

     return 0;

}
int stopService(const char *serviceName)
{
     bool isSuccess = false;
     int status = -1;

     string fileName = string("/etc/init.d/") + string(serviceName);
     /**If initctl is available, service will be stopped using initctl.*/
     if(access("/etc/init/", F_OK) == 0)
     {
          string command = string("initctl stop ") + string(serviceName) + string(" >/dev/null 2>&1");
          status = system(command.c_str());
          if(WEXITSTATUS(status) == 0)
          {
               isSuccess = true;
          }
     }
     /**If systemctl available, and service is not stopped by initctl, then service will be stopped using systemctl.*/
     if(!isSuccess && access("/bin/systemctl", F_OK) == 0)
     {
          string command = string("/bin/systemctl stop ") + string(serviceName) + string(".service >/dev/null 2>&1 &") ;
          status = system(command.c_str());
          if(WEXITSTATUS(status) == 0)
          {
               isSuccess = true;
          }
     }
     /**If invoke-rc.d is available, and service is not stopped by initctl or systemctl, then service will be stopped by invoke-rc.d.*/
     if(!isSuccess && access("/usr/sbin/invoke-rc.d", F_OK) == 0)
     {
          string command = string("invoke-rc.d  ") + string(serviceName) + string(" stop >/dev/null 2>&1");
          status = system(command.c_str());
          if(WEXITSTATUS(status) == 0)
          {
               isSuccess = true;
          }
     }
     /**If service is not stopped by initctl, systemctl, and invoke-rc.d then service will be stopped by script in '/etc/init.d'.*/
     if(!isSuccess && access(fileName.c_str(), F_OK) == 0)
     {
          string command = fileName + string(" stop > /dev/null 2>&1");
          status = system(command.c_str());
          if(WEXITSTATUS(status) == 0)
          {
               isSuccess = true;
          }
     }
     if(isSuccess)
     {
          return 0;
     }
     return -1;

}

int startService(const char *serviceName)
{
     int status;
     bool isStartupCreated = false;
     bool isSuccess = false;
     {
          string fileName = string("/etc/init.d/") + string(serviceName);
          /**If link of script for specified service is not exist in '/etc/init.d' directory, it will create a link with file in installation path.*/ 
          if(access(fileName.c_str(), F_OK) != 0)
          { 
               isStartupCreated = true;
               string fileName2 = string("/opt/MicroWorld/etc/init.d/") + string(serviceName) ;	
               if(access(fileName2.c_str(), F_OK) == 0)
               {
                    int status = symlink(fileName2.c_str(), fileName.c_str());
                    if(WEXITSTATUS(status) != 0)
                    {
                         status = -1;
                    }
               }
          }
     }

     if(access("/bin/systemctl", F_OK) == 0)
     {
          string file1 = string("/etc/systemd/system/") + string(serviceName) + string(".service");
          string file2 = string("/lib/systemd/system/") + string(serviceName) + string(".service");
          /**If systemctl is available, and '.service' file for systemctl daemon is not exit, then it will copy file in '/etc/systemd/system/' or '/lib/systemd/system/' directory from installation path. The systemctl daemon will be reloaded, and service will be enabled.*/
          //if(access(file1.c_str(), F_OK) != 0 && access(file2.c_str(), F_OK) != 0)
          {
               ifstream source;
               ofstream dest;
               isStartupCreated = true;
               file1 = string("/opt/MicroWorld/etc/init.d/") + string(serviceName) + string(".service");
               if(access(file1.c_str(), F_OK) == 0)
               {
                    source.open(file1.c_str(), ios::binary);
               }
               if(access("/etc/systemd/system/", F_OK) == 0)
               {
                    file2 = string("/etc/systemd/system/") + string(serviceName) + string(".service");
                    dest.open(file2.c_str(), ios::binary);
               }
               else if(access("/lib/systemd/system/", F_OK) == 0)
               {
                    file2 = string("/lib/systemd/system/") + string(serviceName) + string(".service");
                    dest.open(file2.c_str(), ios::binary);
               }
               if(source.is_open() &&  dest.is_open())
               {
                    dest << source.rdbuf();
                    source.close();
                    dest.close();
               }
               system("/bin/systemctl --system daemon-reload   >/dev/null 2>&1");
               string command = string("/bin/systemctl enable ") + string(serviceName) + string(".service >/dev/null 2>&1") ;
               system(command.c_str());
          }
          /*{
            string command = string("/bin/systemctl start ") + string(serviceName) + string(".service >/dev/null 2>&1 &") ;
            status = system(command.c_str());
            }*/
     }
     else if(access("/etc/init/", F_OK) == 0 && (access("/sbin/initctl", F_OK) == 0 || access("/usr/sbin/initctl", F_OK) == 0))
     {
          /**If systemctl does not exist, but initctl is exist, it will create link for script at installation path to '/etc/init/'. The initctl daemon will be reloaded.*/
          string file1 = string("/etc/init/") + string(serviceName) + string(".conf");
          if(access(file1.c_str(), F_OK) != 0)
          {
               isStartupCreated = true;
               string file2 = string("/opt/MicroWorld/etc/init/") + string(serviceName) + string(".conf");
               if(access(file2.c_str(), F_OK) == 0)
               {
                    symlink(file2.c_str(), file1.c_str());
               }
               //systemStartUpPath = string("/etc/init/") + string(serviceName) + string(".conf");
               if(access(file1.c_str(), F_OK) == 0)
               {
                    status = system("initctl reload-configuration >/dev/null 2>&1 ");
                    if(WEXITSTATUS(status) != 0)
                    {
                         return -1;
                    }
               }
          }
          /*string command = string("initctl restart ") + string(serviceName) + string(" >/dev/null 2>&1");
            status = system(command.c_str());
            if(WEXITSTATUS(status) != 0)
            {
            system("initctl reload-configuration >/dev/null 2>&1 ");
            command = string("initctl restart ") + string(serviceName) + string(" >/dev/null 2>&1 &");
            status = system(command.c_str());
            }
            */
     }
     /*else
       {
       string file1 = string("/etc/init.d/") + string(serviceName);
       if(access(file1.c_str(), F_OK) != 0)
       {
       isStartupCreated = true;
       string file2 = string("/opt/MicroWorld/etc/init.d/") + string(serviceName);
       if(access(file2.c_str(), F_OK) == 0)
       {
       status = symlink(file2.c_str(), file1.c_str());
       }
       }
     //string command = file1 + string(" restart 2 > /dev/null 2>&1 &");
     //status = system(command.c_str());
     }*/


     //if(isStartupCreated)
     {
          string fileName = string("/etc/init.d/") + string(serviceName);
          /**If chkconfig is available, service will be added to start at run level 3, 4, 5, and set enabled.*/
          if(access("/sbin/chkconfig", F_OK) == 0)
          {
               string command = string("/sbin/chkconfig --add ") + string(serviceName) + string(" >/dev/null 2>&1");
               system(command.c_str());
               command = string("/sbin/chkconfig --level 345 ") + string(serviceName) + string(" >/dev/null 2>&1");
               system(command.c_str());
               command = string("/sbin/chkconfig --set ") + string(serviceName) + string(" on >/dev/null 2>&1");
               system(command.c_str());
          }
          /**If update-rc.d is available, service will be added to start at run level 2, 3, 4, 5.*/
          if(access("/usr/sbin/update-rc.d", F_OK) == 0)
          {
               string command = string("/usr/sbin/update-rc.d ") + string(serviceName) + string(" start 99 2 3 4 5 . >/dev/null 2>&1");
               system(command.c_str());
          }
          /*if(access("/usr/sbin/invoke-rc.d", F_OK) == 0)
            {
            string command = string("invoke-rc.d  ") + string(serviceName) + string(" start >/dev/null 2>&1");
            status = system(command.c_str());
            }*/
     }			

     {
          string fileName = string("/etc/init.d/") + string(serviceName);
          /*if(access("/etc/init/", F_OK) == 0)
            {
            string command = string("initctl start ") + string(serviceName) + string(" >/dev/null 2>&1");
            status = system(command.c_str());
            if(WEXITSTATUS(status) != 0)
            {
            if(access(fileName.c_str(), F_OK) == 0)
            {
            system("initctl reload-configuration >/dev/null 2>&1 ");
            status = system(command.c_str());
            }
            }
            }*/

          /**If initctl is available, service will be stated using initctl daemon.*/
          if(access("/etc/init/", F_OK) == 0 && (access("/sbin/initctl", F_OK) == 0 || access("/usr/sbin/initctl", F_OK) == 0))
          {
               string command = string("initctl start ") + string(serviceName) + string(" >/dev/null 2>&1");
               status = system(command.c_str());
               if(WEXITSTATUS(status) == 0)
               {
                    isSuccess = true;
               }
          }
          /**If initctl failed or not available, and systemctl is available, then service will be started using systemctl daemon.*/
          if(!isSuccess && access("/bin/systemctl", F_OK) == 0)
          {
               string command = string("/bin/systemctl start ") + string(serviceName) + string(".service >/dev/null 2>&1 &") ;
               status = system(command.c_str());
               if(WEXITSTATUS(status) == 0)
               {
                    isSuccess = true;
               }
          }
          /**If initctl and system failed to start or not exist, and invoke-rc.d available, then service will be started using invoke-rc.d.*/
          if(!isSuccess && access("/usr/sbin/invoke-rc.d", F_OK) == 0)
          {
               string command = string("invoke-rc.d  ") + string(serviceName) + string(" start >/dev/null 2>&1");
               status = system(command.c_str());
               if(WEXITSTATUS(status) == 0)
               {
                    isSuccess = true;
               }
          }
          /**If systemctl, initctl, invoke-rc.d are not exist or failed to start the service, then service will be restarted using script in '/etc/init.d/' directory.*/
          if(!isSuccess && access(fileName.c_str(), F_OK) == 0)
          {
               string command = fileName + string(" restart 2 > /dev/null 2>&1");
               status = system(command.c_str());
               if(WEXITSTATUS(status) == 0)
               {
                    isSuccess = true;
               }
          }
     }
     if(isSuccess)
     {
          return 0;
     }
     return -1;
}
int finding_users()
{

}

//ketan
int getSlkOptions()
{

     char val[50] = "";
     int opt = 0;
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf","auth:SLKOPTION",val) !=0)
     {
          //printf("Failed to read slk_options \n");
     }
     else
     {
          opt = atoi(val);
     }
     return opt;
}

//int copy_file_from_policies(char *filename,char *filenamepath)
int copy_file_from_policies(int status)
{
     /*	struct passwd *user;

          //printf("List of users:\n");

     // Open the password file
     setpwent();
     while ((user = getpwent()) != NULL) {
     //printf("%s\n", user->pw_name);
     char copy_path[1024]="";
     sprintf(copy_path,"/home/%s",user->pw_name);
     if(access(copy_path,F_OK)==0)
     {
     char cmd1[1024]="";
     sprintf(cmd1,"cp -rf %s %s/.%s",filenamepath,copy_path,filename);	
     }
     }
     endpwent();*/
     int option = getSlkOptions();
     if(status==1 || status == 2 )
     {   
          if(status==1)
          {
               TwoFactorLog(2,"2Fa Applied for the All Users");
          }
          else if(status == 2)
          {  
               TwoFactorLog(2,"2Fa Applied for the Particular Users");
          }

          if(option == 3)
          {
               if(access("/opt/MicroWorld/backup/gdm-password_ORG_ESCAN",F_OK)==0)
               {     system("cp -rf /opt/MicroWorld/backup/gdm-password_ORG_ESCAN /etc/pam.d/gdm-password");}

               if(access("/opt/MicroWorld/backup/lightdm_ORG_ESCAN",F_OK)==0)
               {     system("cp -rf /opt/MicroWorld/backup/lightdm_ORG_ESCAN /etc/pam.d/lightdm");}


               if(access("/opt/MicroWorld/backup/sshd_ORG_ESCAN",F_OK)==0)
               {     system("cp -rf /opt/MicroWorld/backup/sshd_ORG_ESCAN /etc/pam.d/sshd"); }
               /*if(access("/opt/MicroWorld/etc/sudo_ORG_ESCAN",F_OK)==0)
                 system("mv /opt/MicroWorld/etc/sudo_ORG_ESCAN /etc/pam.d/sudo");
                 if(access("/opt/MicroWorld/etc/su_ORG_ESCAN",F_OK)==0)
                 system("mv /opt/MicroWorld/etc/su_ORG_ESCAN /etc/pam.d/su");*/
               configure_sshfile_for_authenticator();
               pamd_file_for_authenticator_forGUI(status);
               pamd_file_for_authenticator_forssh(status);
               // pamd_file_for_authenticator_forsudo(status);
               // pamd_file_for_authenticator_forsu(status);
               //            system("systemctl restart sshd.service");
               system("systemctl restart sshd.service 2>/dev/null || systemctl restart ssh.service");
          }
          else if(option == 2)
          {
               if(access("/opt/MicroWorld/backup/sshd_ORG_ESCAN",F_OK)==0)
                    system("cp -rf /opt/MicroWorld/backup/sshd_ORG_ESCAN /etc/pam.d/sshd");
               /*if(access("/opt/MicroWorld/etc/sudo_ORG_ESCAN",F_OK)==0)
                 system("mv /opt/MicroWorld/etc/sudo_ORG_ESCAN /etc/pam.d/sudo");
                 if(access("/opt/MicroWorld/etc/su_ORG_ESCAN",F_OK)==0)
                 system("mv /opt/MicroWorld/etc/su_ORG_ESCAN /etc/pam.d/su");*/
               configure_sshfile_for_authenticator();
               pamd_file_for_authenticator_forGUI(status);
               //            system("systemctl restart sshd.service");
               system("systemctl restart sshd.service 2>/dev/null || systemctl restart ssh.service");
          }
          else if(option == 1)
          {
               if(access("/opt/MicroWorld/backup/gdm-password_ORG_ESCAN",F_OK)==0)
               {    system("cp -rf /opt/MicroWorld/backup/gdm-password_ORG_ESCAN /etc/pam.d/gdm-password"); }

               if(access("/opt/MicroWorld/backup/lightdm_ORG_ESCAN",F_OK)==0)
               {    system("cp -rf /opt/MicroWorld/backup/lightdm_ORG_ESCAN /etc/pam.d/lightdm"); }

               configure_sshfile_for_authenticator();
               pamd_file_for_authenticator_forssh(status);
               //pamd_file_for_authenticator_forsudo(status);
               //pamd_file_for_authenticator_forsu(status);
               //            system("systemctl restart sshd.service");
               system("systemctl restart sshd.service 2>/dev/null || systemctl restart ssh.service");
          }
     }
     /*    else if(status==2)
           {
           if(option == 5)
           {
           remove_file_from_policies();
           configure_sshfile_for_authenticator();
           pamd_file_for_authenticator_forGUI(status);
           pamd_file_for_authenticator_forssh(status);
           pamd_file_for_authenticator_forsudo(status);
           pamd_file_for_authenticator_forsu(status);
           system("systemctl restart sshd.service");
           }
           else if(option == 4)
           {
           remove_file_from_policies();
           configure_sshfile_for_authenticator();
           pamd_file_for_authenticator_forsu(status);
           system("systemctl restart sshd.service");
           }
           else if(option == 3)
           {
           remove_file_from_policies();
           configure_sshfile_for_authenticator();
           pamd_file_for_authenticator_forsudo(status);
           system("systemctl restart sshd.service");
           }
           else if(option == 2)
           {
           remove_file_from_policies();
           configure_sshfile_for_authenticator();
           pamd_file_for_authenticator_forGUI(status);
           system("systemctl restart sshd.service");
           }
           else if(option == 1)
           {
           remove_file_from_policies();
           configure_sshfile_for_authenticator();
           pamd_file_for_authenticator_forssh(status);
           system("systemctl restart sshd.service");
           }
           }*/
     //    system("sudo systemctl disable ssh.service");
     //    sleep(1);
     //    system("sudo systemctl enable ssh.service");
     //    sleep(1);
     //    stopService("ssh");
     //    sleep(1);
     //    startService("ssh");
}
//int remove_file_from_policies(char *filename,char *filenamepath)
/*int remove_file_from_policies()
  {
  struct passwd *user;

//    //printf("List of users:\n");

setpwent();
while ((user = getpwent()) != NULL) {
//printf("%s\n", user->pw_name);
char copy_path[1024]="";
sprintf(copy_path,"/home/%s",user->pw_name);
if(access(copy_path,F_OK)==0)
{
char cmd1[1024]="";
sprintf(cmd1,"%s/%s",copy_path,".key_authenticator");
remove(cmd1);

char cmd2[1024]="";
sprintf(cmd2,"%s/%s",copy_path,".key_authenticator_1");
remove(cmd2);



}
}
endpwent();
remove("/root/.key_authenticator");
remove("/root/.key_authenticator_1");
remove("/root/.key_authenticator_2");

if(access("/etc/ssh/sshd_config_ORG_ESCAN",F_OK)==0)
system("mv /etc/ssh/sshd_config_ORG_ESCAN /etc/ssh/sshd_config");
if(access("/etc/pam.d/gdm-password_ORG_ESCAN",F_OK)==0)
system("mv /etc/pam.d/gdm-password_ORG_ESCAN /etc/pam.d/gdm-password");
if(access("/etc/pam.d/sshd_ORG_ESCAN",F_OK)==0)
system("mv /etc/pam.d/sshd_ORG_ESCAN /etc/pam.d/sshd");
if(access("/etc/pam.d/sudo_ORG_ESCAN",F_OK)==0)
system("mv /etc/pam.d/sudo_ORG_ESCAN /etc/pam.d/sudo");
system("systemctl restart sshd.service");
//    system("systemctl daemon-reload");
//    system("sudo systemctl disable ssh.service");
//    sleep(1);
//    system("sudo systemctl enable ssh.service");
//    sleep(1);

//  system("sudo systemctl unmask ssh.service");
//  stopService("ssh");
//   sleep(1);
//   startService("ssh");

}*/
int apply_setting_for_all()
{
     uid_t uid = getuid(); // Get current user ID, you can change this to any desired user ID
     const char *username = getUserName(uid);
     free((void *)username);
     char final_op[1024]="";
     // Free the memory allocated for the username
     char server_address[1024]="";
     char local_Prehost[1024]="";
     char isCloud[50]="";
     if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf","Cloud:LocalPreHost",local_Prehost)!=0)
     {
          if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf","Cloud:IsCloud",isCloud) != 0 || atoi(isCloud) == 0)
          {
               if(readConfiguration("/opt/MicroWorld/etc/winclient.conf", "eserv:Server", server_address) != 0)
               {
                    //   return 0;
               }
               char mc_code1[1024]="";
               if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf", "Config:serverUUID", mc_code1) == 0)
               {
                    //strcpy(final_op,GetFirst16Char(mc_code1));
                    strcpy(final_op,GetFirstCharIgnoreAllUsers(mc_code1));
                    toUpperCase(final_op);


               }
               else
               {

                    char op1[1024]="";
                    if(server_address != NULL) 
                    {
                         if(find_out_the_telnet_string(server_address,op1) == 0)
                         {

                              char op2[1024]="";
                              find_the_mc_value(op1,op2);
                              //char final_op[1024]="";

                              //strcpy(final_op,GetFirst16Char(op2));
                              strcpy(final_op,GetFirstCharIgnoreAllUsers(op2));
                              toUpperCase(final_op);

                         }
                         else
                         {
                              strcpy(final_op,"BAKAGBBAFAMAGANB");
                         }

                    }    
                    else 
                    {
                         return 0;
                    }
               }
          }
          else
          {
               char companyId[256]="";
               if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf","Cloud:CompanyId",companyId) ==0)
               {
                    char op[1024]="";
                    sprintf(op,"%s%s%s%s%s",companyId,companyId,companyId,companyId,companyId);
                    //strcpy(final_op,GetFirst16Char(op));
                    strcpy(final_op,GetFirstCharIgnoreAllUsers(op));
               }
               else
               {
                    //printf("Failed to read CompanyId\n"); //og
               }
          }
     }
     else
     {   
          if(readConfiguration("/opt/MicroWorld/etc/winclient.conf", "eserv:Server", server_address) != 0)
          {
               //   return 0;
          }
          char mc_code1[1024]="";
          if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf", "Config:serverUUID", mc_code1) == 0)
          {
               //strcpy(final_op,GetFirst16Char(mc_code1));
               strcpy(final_op,GetFirstCharIgnoreAllUsers(mc_code1));
               toUpperCase(final_op);


          }
          else
          {

               char op1[1024]="";
               if(server_address != NULL)
               {
                    if(find_out_the_telnet_string(server_address,op1) == 0)
                    {

                         char op2[1024]="";
                         find_the_mc_value(op1,op2);
                         //char final_op[1024]="";

                         //strcpy(final_op,GetFirst16Char(op2));
                         strcpy(final_op,GetFirstCharIgnoreAllUsers(op2));
                         toUpperCase(final_op);

                    }
                    else
                    {
                         strcpy(final_op,"BAKAGBBAFAMAGANB");
                    }

               }
               else
               {
                    return 0;
               }
          }
     }

     struct passwd *pw;

     // Loop through all user entries using getpwent()
     setpwent(); // Start at the beginning of the password database

     while ((pw = getpwent()) != NULL) 
     {
          char filepath[1024]="";
          sprintf(filepath,"/home/%s",pw->pw_name);
    
    
          if(strstr(pw->pw_name,"root")!=NULL)
          {
               FILE *fp;
               fp=fopen("/root/.key_authenticator","w");
               if(fp==NULL)
               {
                    return -1;
               }
               else
               {
                    fprintf(fp,"%s \n\" TOTP_AUTH\n",final_op);
                    fclose(fp);
               }
               change_ownership("/root/.key_authenticator",pw->pw_uid);




               FILE *fp1;
               fp1=fopen("/root/.key_authenticator_1","w");
               if(fp1==NULL)
               {
                    return -1;
               }
               else
               {
                    fprintf(fp1,"BAKAGBBAFAMAGANB \n\" TOTP_AUTH\n",final_op);
                    fclose(fp1);
               }
               change_ownership("/root/.key_authenticator_1",pw->pw_uid);

          }

          struct stat st;
          if (stat(filepath, &st) == 0) 
          {
               if (S_ISDIR(st.st_mode)) 
               {
                    char google_auth_path[1024]="";
                    sprintf(google_auth_path,"%s/%s",filepath,".key_authenticator");
                    FILE *fp;
                    fp=fopen(google_auth_path,"w");
                    if(fp==NULL)
                    {
                         return -1;
                    }
                    else
                    {
                         fprintf(fp,"%s \n\" TOTP_AUTH\n",final_op);
                         fclose(fp);
                    }
                    change_ownership(google_auth_path,pw->pw_uid);


                    char google_auth_path1[1024]="";
                    sprintf(google_auth_path1,"%s/%s",filepath,".key_authenticator_1");
                    FILE *fp1;
                    fp1=fopen(google_auth_path1,"w");
                    if(fp1==NULL)
                    {
                         return -1;
                    }
                    else
                    {
                         fprintf(fp1,"BAKAGBBAFAMAGANB \n\" TOTP_AUTH\n",final_op);
                         fclose(fp1);
                    }
                    change_ownership(google_auth_path1,pw->pw_uid);

               } 
               else 
               {

               } 
          }

     }
     copy_file_from_policies(1);                
     return 0;
}

int apply_setting_for_users()
{
     uid_t uid = getuid(); // Get current user ID, you can change this to any desired user ID
     const char *username = getUserName(uid);
     char companyid[1024]="";
     char status[100]="";
     char local_Prehost[1024]="";
     int opt = 0;
     char final_op[1024]="";
     char mc_code1[1024]="";
     char op2[1024]="";
     char op1[1024]="";
     int val_setter= 1;
     
     free((void *)username); // Free the memory allocated for the username

     char server_address[1024]="";
     if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf","Cloud:LocalPreHost",local_Prehost) != 0)
     {
          if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf", "Cloud:IsCloud", status) != 0 || atoi(status) == 0)
          {
               if(readConfiguration("/opt/MicroWorld/etc/winclient.conf", "eserv:Server", server_address) != 0)
               {
                    ////printf("Unable to read the Server");
                    return 0;
               }
               else
               {
                    if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf", "Config:serverUUID", mc_code1) == 0)
                    {
                         //strcpy(final_op,GetFirst16Char(mc_code1));
                         //  toUpperCase(final_op);
                         strcpy(op2,mc_code1);
                        val_setter= 0;

                    }
                    else
                    {
                         if (server_address != NULL)
                         {
                              val_setter = find_out_the_telnet_string(server_address,op1);
                              //       free(server_address);
                         }
                         else
                         {
                              //printf("Server address not found in the configuration file.\n");
                              return 0;
                         }

                         find_the_mc_value(op1,op2);
                    }


               }
          }
          else
          {
               opt = atoi(status);
               if(opt == 1)
               {
                    if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf","Cloud:CompanyId",companyid) != 0)
                    {
                         //printf("Unable to read the company Id\n");
                    }
                    else
                    {
                        val_setter= 0;
                    }
               }
          }
     }
     else
     {
          if(readConfiguration("/opt/MicroWorld/etc/winclient.conf", "eserv:Server", server_address) != 0)
          {
               printf("Unable to read the Server");
               return 0;
          }
          else
          {
               if(readConfiguration("/opt/MicroWorld/etc/commonsettings.conf", "Config:serverUUID", mc_code1) == 0)
               {
                    //strcpy(final_op,GetFirst16Char(mc_code1));
                    //  toUpperCase(final_op);
                    strcpy(op2,mc_code1);
                        val_setter= 0;
               }
               else
               {
                    if (server_address != NULL)
                    {
                         val_setter = find_out_the_telnet_string(server_address,op1);
                         //       free(server_address);
                    }
                    else
                    {
                         //printf("Server address not found in the configuration file.\n");
                         return 0;
                    }

                    find_the_mc_value(op1,op2);
               }


          }

     }

     struct passwd *pw;

     setpwent(); // Start at the beginning of the password database
     while ((pw = getpwent()) != NULL) 
     {
          char filepath[1024]="";
          sprintf(filepath,"/home/%s",pw->pw_name);
          struct stat st;

          if(strcmp(pw->pw_name,"root")==0)
          {
               char op3[1024]="";
               if(opt == 1)
               {
                    //calculate_secret_key_for_particular_user(pw->pw_name,companyid,op3);
                    calculate_secret_key_for_particular_user(pw->pw_name,companyid,op3);
               }
               else
               {
                    calculate_secret_key_for_particular_user2(pw->pw_name,op2,op3);
               }
               char google_auth_path[1024]="";
               sprintf(google_auth_path,"/root/%s",".key_authenticator");
               FILE *fp;
               fp=fopen(google_auth_path,"w");
               if(fp==NULL)
               {
                    return -1;
               }
               else
               {
                    //fprintf(fp,"%s \n\" TOTP_AUTH\n",final_op1);
                    if(val_setter != 0)
                    {
                         fprintf(fp,"%s \n\" TOTP_AUTH\n","BAKAGBBAFAMAGANB");
                         fclose(fp);
                    }
                    else
                    {
                         fprintf(fp,"%s \n\" TOTP_AUTH\n",op3);
                         fclose(fp);
                    }
               }
               change_ownership(google_auth_path,pw->pw_uid);



               char google_auth_path1[11024]="";
               sprintf(google_auth_path1,"/root/%s",".key_authenticator_1");
               FILE *fp1;
               fp1=fopen(google_auth_path1,"w");
               if(fp1==NULL)
               {
                    return -1;
               }
               else
               {
                    fprintf(fp1,"BAKAGBBAFAMAGANB \n\" TOTP_AUTH\n");
                    fclose(fp1);
               }
               change_ownership(google_auth_path1,pw->pw_uid);


          }



          if (stat(filepath, &st) == 0) 
          {
               if (S_ISDIR(st.st_mode)) 
               {
                    /*    char secret_key_generation[1024]="";
                          sprintf(secret_key_generation,"%s%s",pw->pw_name,op2);
                          strcpy(final_op,GetFirst16Char(secret_key_generation));
                          char op6[1024]="";
                          toUpperCase(final_op);
                          encstr(final_op,0,op6);
                          char final_op1[1024]="";
                          strcpy(final_op1,GetFirst16Char(op6));
                          */
                    char op3[1024]="";
                    //   calculate_secret_key_for_particular_user(pw->pw_name,op2,op3);
                    if(opt == 1)
                    {
                         // calculate_secret_key_for_particular_user(pw->pw_name,companyid,op3);
                         calculate_secret_key_for_particular_user(pw->pw_name,companyid,op3);
                    }
                    else
                    {
                         calculate_secret_key_for_particular_user2(pw->pw_name,op2,op3);
                    }
                    char google_auth_path[1024]="";
                    sprintf(google_auth_path,"%s/%s",filepath,".key_authenticator");
                    FILE *fp;
                    fp=fopen(google_auth_path,"w");
                    if(fp==NULL)
                    {
                         return -1;
                    }
                    else
                    {
                         fprintf(fp,"%s \n\" TOTP_AUTH\n",op3);
                         fclose(fp);
                    }
                    change_ownership(google_auth_path,pw->pw_uid);



                    char google_auth_path1[11024]="";
                    sprintf(google_auth_path1,"%s/%s",filepath,".key_authenticator_1");
                    FILE *fp1;
                    fp1=fopen(google_auth_path1,"w");
                    if(fp1==NULL)
                         return -1;
                    else
                    {
                         fprintf(fp1,"BAKAGBBAFAMAGANB \n\" TOTP_AUTH\n");
                         fclose(fp1);
                    }
                    change_ownership(google_auth_path1,pw->pw_uid);



               } 
               else 
               {
                    //printf("%s is not a directory.\n", filepath); 
               }
          } 
          else 
          {
          }
     }

     endpwent(); // Close the password database


     copy_file_from_policies(2);                

     return 0;
}

int apply2FASettings(const char *policyname)
{
     bool isSuccess=true;
     int totalFiles=0;
     if(policyname==NULL)
     {
          //cout<<"policyname == NULL. \n ";
          TwoFactorLog(1,"policyname == NULL. ");
          return -1;
     }
     //    string policyFilename= "/opt/MicroWorld/var/avsupd/" +string(policyname);

     if (access(policyname, F_OK) != 0 )
     {
          //cout<<"conf file not present. \n ";
          TwoFactorLog(1,"conf file not present. ");
          return -1;
     }

     char Status[1024]="";
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf", (char *)"auth:Status", Status) != 0) 
     {
          //cout<<"Failed to read 2fa Status \n ";
          TwoFactorLog(1,"Failed to read 2fa Status ");
          return -1;
     
     }
     else
     {
          if(atoi(Status) == 0)
          {
               remove_file_from_policies();

               return 0;          
          }

     }

     char cPtr[1024]="";
     if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf", (char *)"auth:SLKOPTION", cPtr) != 0)
     {
                    //cout<<"Failed to get 2fa auth:SLKOPTION\n";
                    TwoFactorLog(1,"Failed to get 2fa auth:SLKOPTION");
                    return -1;
     }
     else
     {
          if(atoi(cPtr)==1)
          {
               remove_file_from_policies();
               char Users[1024]="";
               if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf", (char *)"auth:Users", Users) != 0)
               {
                            // cout<<"Failed to get 2fa Users, All users\n";
               }
     
               if(strcmp(Users,"") == 0)
               {
                    apply_setting_for_all();
               }
               else
               {
                    apply_setting_for_users();
               
               }
          }
          else if(atoi(cPtr)==2)
          {
               remove_file_from_policies();
               char Users[1024]="";
               if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf", (char *)"auth:Users", Users) != 0)
               {
                             //cout<<"Failed to get 2fa Users, All users\n";
               }

               if(strcmp(Users,"") == 0)
               {
                    apply_setting_for_all();
               }
               else
               {
                    apply_setting_for_users();

               }

          }
          else if(atoi(cPtr)==3)
          {
               remove_file_from_policies();
               char Users[1024]="";
               if(readConfiguration("/opt/MicroWorld/etc/keyauth.conf", (char *)"auth:Users", Users) != 0)
               {
                             //cout<<"Failed to get 2fa Users,  All users\n";
               }

               if(strcmp(Users,"") == 0)
               {
                    apply_setting_for_all();
               }
               else
               {
                    apply_setting_for_users();

               }
          }
          else if(atoi(cPtr)==0)
          {
               remove_file_from_policies();
          }



     }

     return 0;
}
int main()
{
     apply2FASettings("/opt/MicroWorld/etc/keyauth.conf");    
}
