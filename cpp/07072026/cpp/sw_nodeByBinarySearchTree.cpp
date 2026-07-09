
// this c++ code work binary search tree
#include <iostream> 	// cout , endl
#include <cstring>	// strcmp , strcpy

#include <signal.h> 	// for sig_atomic_t
#include <cstdio>     // fopen fclose fgets
volatile sig_atomic_t keep_running = 1;

using namespace std;	// std::
			//forward declaration for threads function 
void * sendswevent(void *);

struct sw_node
{
	char name[256];
	char version[128];
	char filename[256];
	char status;
	char date[30];

	int height; // binary search tree height // height used for balancing.

	sw_node *left; // left child
	sw_node *right;	// right child
};

void * sendswevent (void *)
{	
	//	while(keep_running)
	std::cout<<"searching ...\n\n\n";

	return NULL;
}

int Height(sw_node *node)
{
	if(node == NULL)
		return 0;

	return node->height;
}

int Max(int a, int b)
{
	return (a > b) ? a : b;
}

sw_node *CreateNode(const char *name, 		const char *version, 		const char *filename)
{
	sw_node *node = new sw_node;

	strcpy(node->name, name);
	strcpy(node->version, version);
	strcpy(node->filename, filename);

	node->status = 'A';

	strcpy(node->date, "09-Jul-2026");

	node->left = NULL;
	node->right = NULL;

	node->height = 1;

	return node;
}

int Balance(sw_node *node)
{
	if(node == NULL)
		return 0;

	return Height(node->left) - Height(node->right);
}

sw_node *RightRotate(sw_node *y)
{
	sw_node *x = y->left;

	sw_node *t2 = x->right;

	x->right = y;

	y->left = t2;

	y->height = Max(Height(y->left), Height(y->right)) + 1;

	x->height = Max(Height(x->left), Height(x->right)) + 1;

	return x;
}

sw_node *LeftRotate(sw_node *x)
{
	sw_node *y = x->right;

	sw_node *t2 = y->left;

	y->left = x;

	x->right = t2;

	x->height = Max(Height(x->left), Height(x->right)) + 1;

	y->height = Max(Height(y->left), Height(y->right)) + 1;

	return y;
}

sw_node *Insert(sw_node *root,		const char *name,		const char *version,		const char *filename)
{
	if(root == NULL)
	{
		return CreateNode(name,				version,				filename);
	}

	int cmp = strcmp(name, root->name);

	if(cmp < 0)
	{
		root->left = Insert(root->left,			name,				version,	filename);
	}
	else if(cmp > 0)
	{
		root->right = Insert(root->right,		name,			version,		filename);
	}
	else
	{
		strcpy(root->version, version);
		strcpy(root->filename, filename);

		return root;
	}

	root->height = Max(Height(root->left),	Height(root->right)) + 1;

	int balance = Balance(root);

	if(balance > 1 && strcmp(name, root->left->name) < 0)
	{
		return RightRotate(root);
	}

	if(balance < -1 && strcmp(name, root->right->name) > 0)
	{
		return LeftRotate(root);
	}

	if(balance > 1 && strcmp(name, root->left->name) > 0)
	{
		root->left = LeftRotate(root->left);

		return RightRotate(root);
	}

	if(balance < -1 && strcmp(name, root->right->name) < 0)
	{
		root->right = RightRotate(root->right);

		return LeftRotate(root);
	}

	return root;
}

sw_node *Search(sw_node *root,		const char *name)
{
	if(root == NULL)
		return NULL;

	int cmp = strcmp(name, root->name);

	if(cmp == 0)
		return root;

	if(cmp < 0)
		return Search(root->left, name);

	return Search(root->right, name);
}

void Print(sw_node *root)
{
	if(root == NULL)
		return;

	Print(root->left);

	cout << root->name
		<< " | "
		<< root->version
		<< " | "
		<< root->filename
		<< endl;

	Print(root->right);
}

int main()
{
	sw_node *root = NULL;
	char AppName [52] = "";
	char Version [25] = "";
	char Filepath [252] = "";
	const char *temp = "/tmpdata/cpp/07072026/cpp/softwares.list";
	FILE *fp = fopen(temp,"r");
	if (fp != NULL)
	{	
		char line[256]= "";
		//		if (fgets(line,sizeof(line),fp) != NULL)
		while(fgets(line,sizeof(line),fp) )
		{	
			// remove last newline
			//			line[strcspn(line , "\n" )] = '\0';  // \n replace by \0

			// remove last newline 
			size_t len = strlen(line);
			if( len > 0 && line[len-1] == '\n')
			{
				line[len-1] = '\0';
			}


			// skip empty line
			if ( line[0] == '\0' )
			{
				continue;
			}

			//tokenise
						char *token = strtok(line,"|");

						if (token != NULL)
						{
						strcpy(AppName,token);
						token = strtok(NULL,"|");
						if(token != NULL)
						{
						strcpy(Version,token);
						}
						}
			 



			// Split by '|'
/*			char *token = strtok(line, "|");
			if (!token)
				continue;
			strncpy(AppName, token, sizeof(AppName) - 1);
			AppName[sizeof(AppName) - 1] = '\0';

			token = strtok(NULL, "|");
			if (!token)
				continue;
			strncpy(Version, token, sizeof(Version) - 1);
			Version[sizeof(Version) - 1] = '\0';
*/
			root = Insert(root,AppName,Version,"/usr/share/applications/firefox.desktop");
		}
		fclose(fp);

	}

	root = Insert(root,
			"Firefox",
			"141.0",
			"/usr/share/applications/firefox.desktop");

	root = Insert(root,
			"Google Chrome",
			"138.0",
			"/usr/share/applications/google.desktop");

	root = Insert(root,
			"Opera(Snap)",
			"120",
			"/var/lib/snapd/desktop/applications/opera.desktop");

	root = Insert(root,
			"VLC",
			"3.0.21",
			"/usr/share/applications/vlc.desktop");

	root = Insert(root,
			"Chromium(Snap)",
			"150",
			"/var/lib/snapd/desktop/applications/chromium.desktop");

	cout << "Software List\n\n";

	Print(root);

	pthread_t swthreadID;

	if ( pthread_create(&swthreadID,NULL,sendswevent,(void*)NULL) < 0)
	{
		std::cout<<"Error create thread \n";
	}
	usleep(100);
	//	sw_node *node = Search(root, "Opera(Snap)");
	sw_node *node = Search(root, "Characters");

	if(node != NULL)
	{
		pthread_cancel( swthreadID );
		keep_running = 0;
		int ret = pthread_join( swthreadID , NULL ); // on found stop printing searching ...

		if (ret == 0)
		{
			std::cout<<"stop\n";
		}
		else if (ret != 0)
		{
			std::cout<<"error\n";
		}
		else
		{
			std::cout<<"else\n";
		}
		cout << "\n\nFound\n";

		cout << node->name << endl;

		cout << node->version << endl;

		cout << node->filename << endl;
	}
	else
	{
		cout << "Not Found\n";
	}

	return 0;
}

// pthread_join(threadID,NULL)
//	on success return 0,
//	on error return errno
