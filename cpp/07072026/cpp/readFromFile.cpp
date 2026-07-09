#include <iostream>
#include <cstring>
#include <cstdio>

int main()
{
	const char *filename = "/tmpdata/cpp/07072026/cpp/softwares.list";
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return 1;
	}

	char line[256];
	char AppName[52];
	char Version[25];

	while (fgets(line, sizeof(line), fp))
	{
		// Remove trailing newline
		size_t len = strlen(line);
		if (len > 0 && line[len-1] == '\n')
			line[len-1] = '\0';

		// Skip empty lines
		if (line[0] == '\0')
			continue;

		// Split by '|'
		char *token = strtok(line, "|");
		if (!token)
			continue;
		strncpy(AppName, token, sizeof(AppName) - 1);
		AppName[sizeof(AppName) - 1] = '\0';

		token = strtok(NULL, "|");
		if (!token)
			continue;
		strncpy(Version, token, sizeof(Version) - 1);
		Version[sizeof(Version) - 1] = '\0';

		// Process the entry (here we just print it)
		std::cout << "App: " << AppName << ", Version: " << Version << std::endl;
	}

	fclose(fp);
	return 0;
}
