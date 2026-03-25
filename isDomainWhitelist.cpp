#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <utility>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include "log1.h"

#define MAX_PID_TRACK 1024

using namespace std;
bool isBlock = false;
struct CompareByLastSend {
     bool operator()(const std::pair<std::string, int> &a,
               const std::pair<std::string, int> &b) const {
          return a.second < b.second;
     }
};

struct PidTimestamp_1 {
     pid_t pid;
     time_t last_time;
     bool last_result;
     std::string last_dest_ip;  // Added to track destination IP
};

PidTimestamp_1 pid_timestamps[MAX_PID_TRACK];
int pid_count_1 = 0;

int find_pid_index(pid_t pid) {
     for (int i = 0; i < pid_count_1; i++) {
          if (pid_timestamps[i].pid == pid)
               return i;
     }
     return -1;
}

// Function to clear expired cache entries
void clear_expired_cache() {
     time_t now = time(NULL);
     int write_index = 0;
     
     for (int read_index = 0; read_index < pid_count_1; read_index++) {
          // Keep entries that are still within the 2-second window
          if ((now - pid_timestamps[read_index].last_time) < 2) {
               if (write_index != read_index) {
                    pid_timestamps[write_index] = pid_timestamps[read_index];
               }
               write_index++;
          }
          // Expired entries are automatically discarded by not copying them
     }
     
     pid_count_1 = write_index;
     std::cout << "[CACHE] Cleared expired entries. Active entries: " << pid_count_1 << "\n";
}

// Modified to check destination IP as well
bool should_skip_dump(pid_t pid, std::string& current_dest_ip, bool &cached_result) {
     time_t now = time(NULL);
     int index = find_pid_index(pid);
     if (index != -1) {
          // Check if time is within 2 seconds AND destination IP is the same
          if ((now - pid_timestamps[index].last_time) < 10 && 
              pid_timestamps[index].last_dest_ip == current_dest_ip) {
               cached_result = pid_timestamps[index].last_result;
               return true;  // Use cached result, skip full process
          } else {
               // Don't update timestamp here; full process will update after finishing
               return false; // Not skipping, run full process (time expired or different IP)
          }
     }

     // PID not found, add it to cache with default values
     if (pid_count_1 < MAX_PID_TRACK) {
          pid_timestamps[pid_count_1].pid = pid;
          pid_timestamps[pid_count_1].last_time = 0;       // Force full processing next time
          pid_timestamps[pid_count_1].last_result = false; // Default result
          pid_timestamps[pid_count_1].last_dest_ip = "";   // Empty IP initially
          pid_count_1++;
     } else {
          std::cout << "[CACHE] Warning: Cache is full, cannot add new PID\n";
     }
     return false; // New PID, run full process
}

// Modified to update both result and destination IP
void update_cached_result(pid_t pid, bool result, const std::string& dest_ip) {
     time_t now = time(NULL);
     int index = find_pid_index(pid);
     if (index != -1) {
          pid_timestamps[index].last_result = result;
          pid_timestamps[index].last_time = now;  // Update timestamp after full process
          pid_timestamps[index].last_dest_ip = dest_ip;  // Update destination IP
     }
}

bool checkCDNExists(const std::string& rawDomain, const std::string& valueToCheck, const std::string& filename,string& dstIP) {

     printf("checkdomain----------------> %s\n",rawDomain.c_str());
     printf("check_CDN----------------> %s\n",valueToCheck.c_str());


     /*if(strstr(rawDomain.c_str(),"zippyshare") !=NULL && strstr(valueToCheck.c_str(),"google-analytics")!=NULL)
     {
     std::ostringstream oss;
     oss << "File Attach for URL : " << valueToCheck <<"["<<rawDomain <<"]"<< " and IP : " << dstIP << " [Minimum LastSend IP]";
     log1(1, oss.str().c_str());
     cout << "Mathch desired sni: " << rawDomain << "Using its CDN : " << valueToCheck << endl;
     return true;
     }*/
     std::ifstream file(filename);
     if (!file.is_open()) {
          std::cerr << "❌ Could not open file: " << filename << std::endl;
          return false;
     }

     std::string line;
     std::string currentSection;
     std::string domain = rawDomain;

     // Strip extension (e.g., "wetransfer.com" -> "wetransfer")
     size_t dotPos = domain.find('.');
     if (dotPos != std::string::npos)
          domain = domain.substr(0, dotPos);

     bool sectionFound = false;

     while (std::getline(file, line)) {
          // Trim line
          line.erase(0, line.find_first_not_of(" \t\r\n"));
          line.erase(line.find_last_not_of(" \t\r\n") + 1);

          if (line.empty() || line[0] == '#')
               continue;

          // Detect section header
          if (line.front() == '[' && line.back() == ']') {
               currentSection = line.substr(1, line.length() - 2);
               sectionFound = (currentSection.find(domain) != std::string::npos); // Match whole section like "wesendit.com"
               continue;
          }

          if (sectionFound) {
               size_t arrowPos = line.find("➝");
               if (arrowPos != std::string::npos) {
                    std::string host = line.substr(0, arrowPos);
                    host.erase(0, host.find_first_not_of(" \t\r\n"));
                    host.erase(host.find_last_not_of(" \t\r\n") + 1);
                    if(strstr(valueToCheck.c_str(),"google.com") != NULL)
                    {
                        // cout << "host in cdn :" << host <<endl;
                        // cout << "value in cdn :" << valueToCheck <<endl;
                         if (host == valueToCheck) {
                              return true;
                         }
                    }
                    else
                    {
                         if (host.find(valueToCheck) != std::string::npos) {
                              return true;
                         }
                    }
               }
          }
     }

     return false;
}

bool isDomainWhitelist(char *domain, int pid,char *path,char *url,char *ip)
{
     string filepath = path;
     // Clear expired cache entries at the start of each call
     clear_expired_cache();
     
     std::string domainPath = "/var/MicroWorld/var/run/domain.txt";
/*     std::string socketPath = "/var/MicroWorld/var/run/socketInfo.log";
     
     // Step 1: Find the single lowest LastSend IP
     std::ifstream socketFile(socketPath.c_str());
     if (!socketFile) {
          std::cerr << "Failed to open socketInfo.log\n";
          return false;
     }

     std::string line;
     std::regex socketRegex(R"(^\s*([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)\s+\d+\s+ESTABLISHED\s+\S+\s+\d+\s+(\d+))");
     std::vector<std::pair<std::string, int>> ipSendList;

     while (std::getline(socketFile, line)) {
          std::smatch match;
          if (std::regex_search(line, match, socketRegex)) {
               std::string remoteIP = match[1].str();
               int lastSend = std::stoi(match[2].str());
               ipSendList.push_back(std::make_pair(remoteIP, lastSend));
          }
     }
     socketFile.close();

     if (ipSendList.empty()) {
          std::cerr << "No valid RemoteIP found in socketInfo.log\n";
          return false;
     }

     // Sort to get the minimum LastSend IP
     std::sort(ipSendList.begin(), ipSendList.end(),
               [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
               return a.second < b.second;
               });

     // Get only the single lowest IP
     std::string lowestIP = ipSendList[0].first;
     std::cout << "Selected IP (Min LastSend): " << lowestIP << "\n";*/
     //system("/tmpdata/active-monitor-1.sh > /tmpdata/most_active_ip.txt");

     // Step 2: Load domainList from `domain` argument
     std::vector<std::string> domainList;
     std::stringstream ss(domain);
     std::string token;
     while (std::getline(ss, token, ',')) {
          size_t dotPos = token.find('.');
          if (dotPos != std::string::npos) {
               domainList.push_back(token.substr(0, dotPos));
          } else {
               domainList.push_back(token);
          }
     }

     std::cout << "Extracted domain names:\n";
     for (size_t i = 0; i < domainList.size(); ++i) {
          std::cout << "  - " << domainList[i] << "\n";
     }



     
     // Step 3: Check the single lowest IP against domain.txt
     std::string sni;
     std::string hostname;
     std::string dstIP;
     bool ipMatched = false;

     std::ifstream domainFile(domainPath.c_str());
     if (!domainFile) {
          std::cerr << "Failed to open domain.txt\n";
          update_cached_result(pid, false, filepath);
          return false;
     }
    
       //system("/tmpdata/start");
//     system("/tmpdata/active-monitor-1.sh > /tmpdata/most_active_ip.txt");
L:
     std::string activeIpFile = "/var/MicroWorld/var/run/active_ip.txt";
     std::ifstream activeFile(activeIpFile.c_str());
     if (!activeFile) {
          std::cerr << "Failed to open " << activeIpFile << "\n";
          return false;
     }

     std::string line;
     std::string lowestIP;

     if (std::getline(activeFile, line)) {
          std::string prefix = "Most recently active IP: ";
          size_t pos = line.find(prefix);
          if (pos != std::string::npos) {
               lowestIP = line.substr(pos + prefix.size());
               // Trim any trailing whitespace
               lowestIP.erase(lowestIP.find_last_not_of(" \t\n\r\f\v") + 1);
          } else {
               std::cerr << "IP prefix not found in line\n";
               return false;
          }
     } else {
          std::cerr << "File is empty or unreadable\n";
          return false;
     }

     activeFile.close();

     if (lowestIP.empty()) {
          std::cerr << "No IP found in the file\n";
          return false;
     }

     std::cout << "Selected IP (from most_active_ip.txt): " << lowestIP << "\n";
      char event_log1[1024]={0};
                                   std::sprintf(event_log1, "292 Selected IP (from most_active_ip.txt): %s",lowestIP.c_str());
     log1(1,event_log1); 
     std::regex domainRegex(R"(SNI:\s*([^|]+)\s*\|hostname:\s*([^|]+)\s*\|.*?dstIP:\s*([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+))");

     if(strcasestr(domain,lowestIP.c_str())!=NULL)
     {
          cout << "IP match found in whitelisting = " <<lowestIP <<endl;
          
          strcpy(ip,lowestIP.c_str());
          char event_log[1024]={0};
          std::sprintf(event_log, "304 File Attach for IP : %s ",lowestIP.c_str());
          log1(1, event_log);
          return true;
     }
     else
     {

          while (std::getline(domainFile, line)) {
               std::smatch match;
               if (std::regex_search(line, match, domainRegex)) {
                    sni = match[1].str();
                    hostname = match[2].str();
                    dstIP = match[3].str();
                    //printf("line====%s=%s=%s\n",line.c_str(),dstIP.c_str(),lowestIP.c_str());


                    if (dstIP == lowestIP) {
                         ipMatched = true;
                         std::cout << "[INFO] Matched IP: " << lowestIP << "\n";

                         if(sni == "bat.bing-int.com" || sni == "logs.browser-intake-datadoghq.eu" || sni == "mtalk.google.com" || sni == "ogads-pa.clients6.google.com" || sni == "android.clients.google.com" || sni == "www.googleapis.com" || sni == "rum.browser-intake-datadoghq.eu")
                         {
                              log1(1,"skipping this ip and proceding for new one");
                              goto L;
                         }

                         // Check cache now that we know the destination IP
                         bool cached_result = false;
                         /* if (should_skip_dump(pid, filepath, cached_result)) {
                            std::cout << "[SKIP] Using cached result for PID " << pid << " with IP " << dstIP << ": " << cached_result << "\n";
                            return cached_result;
                            }*/

                         for (std::string& d : domainList) {
                              cout << "d---->" << d <<endl;
                              if(d.find("gmail") != std::string::npos)
                                   d = "mail";
                              if (sni.find(d) != std::string::npos || sni.find("escanav") != std::string::npos || sni.find("outlook") != std::string::npos ||  sni.find("microsoft") != std::string::npos) {
                                   std::cout << "SNI match, IP match: SNI = " << sni << ", IP = " << dstIP << "\n";
                                   char event_log[1024]={0};
                                   std::sprintf(event_log, "File Attach for URL : %s and IP : %s ", sni.c_str(), dstIP.c_str());
                                   log1(1, event_log);
                                   strcpy(url,sni.c_str());
                                   printf("LOG1 ----> %s\n", event_log);
                                   // update_cached_result(pid, true, filepath);
                                   return true;  // ✅ Exit early if successful
                              }
                              else
                              {
                                   if(hostname == "google.com" || hostname == " gstatic.com")
                                   {
                                        cout << "sni -->" << sni <<endl;
                                        if (checkCDNExists(d,sni, "/var/MicroWorld/var/run/cdn_output.txt",dstIP)) {
                                             std::cout << "✅ Found a match for :" << sni << std::endl;
                                             std::ostringstream oss;
                                             oss << "File Attach for CDN : " << sni <<"["<<d<<"]"<< " and IP : " << dstIP;
                                             log1(1, oss.str().c_str());
                                             strcpy(url,sni.c_str());
                                             //      update_cached_result(pid, true, filepath);
                                             return true;
                                        } else {
                                             isBlock = true;
                                             strcpy(url,sni.c_str());
                                             std::cout << "❌ No match found.\n";
                                        }
                                   }
                                   else
                                   {
                                        if (checkCDNExists(d, hostname, "/var/MicroWorld/var/run/cdn_output.txt",dstIP)) {
                                             std::cout << "✅ Found a match for :" << hostname << std::endl;
                                             std::ostringstream oss;
                                             oss << "File Attach for CDN : " << hostname <<"["<<d<<"]"<< " and IP : " << dstIP ;
                                             log1(1, oss.str().c_str());
                                             strcpy(url,hostname.c_str());
                                             //      update_cached_result(pid, true, filepath);
                                             return true;
                                        } else {
                                             isBlock = true;
                                             strcpy(url,hostname.c_str());
                                             std::cout << "❌ No match found.\n";
                                        }
                                   }

                              }

                         }

                         // SNI didn't match
                         std::cout << "SNI not match, but IP matched: SNI = " << sni << ", IP = " << dstIP << "\n";
                         char event_log[1024]={0};
                         std::sprintf(event_log, "394 SNI not match, but IP matched IP:%s SNI:%s", sni.c_str(), dstIP.c_str());
               log1(1, event_log);
                         isBlock = true;
                         break;  // Exit the while loop since we found the IP match
                    }
               }
          }
          domainFile.close();

          if (!ipMatched) {
               isBlock = true;
               std::cout << "[INFO] No match for IP: " << lowestIP << " in domain.txt\n";
               char event_log[1024]={0};
               std::sprintf(event_log, "407No match fo IP :%s",lowestIP.c_str());
               log1(1, event_log);
          }
          if(isBlock)
          {
               char event_log[1024]={0};
               std::sprintf(event_log, "413 File block for URL : %s and IP : %s [Minimum LastSend IP]", url, dstIP.c_str());
               log1(1, event_log);
               strcpy(url,sni.c_str());
               printf("LOG2 ----> %s\n", event_log);
               return false;
          }
     }

     return true;
}
