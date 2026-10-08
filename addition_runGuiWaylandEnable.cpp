
/*************************************************************************/


#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <pthread.h>

static int   g_stderr_pipe[2];
static pthread_t g_stderr_thread;

static void* stderrFilterThread(void*)
{
    char buf[4096];
    std::string line;
    FILE* in = fdopen(g_stderr_pipe[0], "r");
    if (!in) return NULL;

    while (fgets(buf, sizeof(buf), in)) {
        line = buf;
        // Drop lines that look like the GTK pk-gtk-module warning.
        if (line.find("Gtk-Message:") != std::string::npos &&
            line.find("pk-gtk-module") != std::string::npos) {
            continue;
        }
        fputs(line.c_str(), stderr);
    }
    return NULL;
}

// Call this at the very top of main(), before anything else.
static void silenceGtkPkModuleWarning()
{
    if (pipe(g_stderr_pipe) != 0) return;

    // Redirect fd 2 into the write end of our pipe.
    dup2(g_stderr_pipe[1], 2);
    close(g_stderr_pipe[1]);

    // Background thread reads the read end, filters, and re-emits to the real stderr.
    pthread_create(&g_stderr_thread, NULL, stderrFilterThread, NULL);
    pthread_detach(g_stderr_thread);
}



//runGuiWaylandEnable() func start

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>

// ---------------------------------------------------------------------------
// Helper: is the given path readable by this process?
// ---------------------------------------------------------------------------
static bool isReadable(const std::string& path)
{
    return access(path.c_str(), R_OK) == 0;
}

// ---------------------------------------------------------------------------
// Helper: glob() wrapper that returns a sorted list of matches.
// ---------------------------------------------------------------------------
static std::vector<std::string> globPaths(const char* pattern)
{
    std::vector<std::string> out;
    glob_t g;
    std::memset(&g, 0, sizeof(g));
    if (glob(pattern, GLOB_NOSORT, NULL, &g) == 0) {
        for (size_t i = 0; i < g.gl_pathc; ++i) {
            out.push_back(g.gl_pathv[i]);
        }
    }
    globfree(&g);
    return out;
}

// ---------------------------------------------------------------------------
// runGuiWaylandEnable()
//
// Equivalent of the wrapper shell script:
//
//   export LD_LIBRARY_PATH=/opt/MicroWorld/lib:$LD_LIBRARY_PATH
//   export QT_X11_NO_MITSHM=1
//   export QT_GRAPHICSSYSTEM=raster
//   export GTK_MODULES=""
//   export GTK2_MODULES=""
//   export GTK_PATH=/opt/MicroWorld/lib
//
//   if [ "$(id -u)" = "0" ] && [ -z "$XAUTHORITY" ]; then
//       for xa in /run/user/*/gdm/Xauthority /home/*/.Xauthority; do
//           [ -r "$xa" ] && export XAUTHORITY="$xa" && break
//       done
//   fi
//   [ -n "$DISPLAY" ] || export DISPLAY=:0
// ---------------------------------------------------------------------------
void runGuiWaylandEnable()
{
    // --- 1. Prepend /opt/MicroWorld/lib to LD_LIBRARY_PATH ----------------
    {
        const char* old = getenv("LD_LIBRARY_PATH");
        std::string val = "/opt/MicroWorld/lib";
        if (old && *old) {
            val += ":";
            val += old;
        }
        setenv("LD_LIBRARY_PATH", val.c_str(), 1 /*overwrite*/);
    }

    // --- 2. Qt / GTK environment ------------------------------------------
    setenv("QT_X11_NO_MITSHM", "1", 1);
    setenv("QT_GRAPHICSSYSTEM", "raster", 1);

    // These are intentionally set to empty strings, matching the shell script.
    setenv("GTK_MODULES",   "", 1);
    setenv("GTK2_MODULES",  "", 1);

    setenv("GTK_PATH", "/opt/MicroWorld/lib", 1);

    // --- 3. Root + no XAUTHORITY: try to find a usable cookie -------------
    uid_t uid = getuid();
    const char* xauthEnv = getenv("XAUTHORITY");

    if (uid == 0 && (!xauthEnv || !*xauthEnv)) {
        // Candidates in the same order as the shell script.
        const char* patterns[] = {
            "/run/user/*/gdm/Xauthority",
            "/home/*/.Xauthority"
        };

        bool found = false;
        for (size_t p = 0; p < sizeof(patterns)/sizeof(patterns[0]) && !found; ++p) {
            std::vector<std::string> matches = globPaths(patterns[p]);
            for (size_t i = 0; i < matches.size(); ++i) {
                if (isReadable(matches[i])) {
                    setenv("XAUTHORITY", matches[i].c_str(), 1);
                    found = true;
                    break;
                }
            }
        }
    }

    // --- 4. Fallback DISPLAY ---------------------------------------------
    const char* disp = getenv("DISPLAY");
    if (!disp || !*disp) {
        setenv("DISPLAY", ":0", 1);
    }
}


//runGuiWaylandEnable() func end
/********************************************************************************************/





int main()
{




	        silenceGtkPkModuleWarning();   // <-- very first line
        runGuiWaylandEnable();	// MIT_SHM error show white blankScreenGui





}
