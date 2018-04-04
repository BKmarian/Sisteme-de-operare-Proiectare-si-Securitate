#include<stdio.h>
#include<windows.h>

void dirListFiles(char* startDir, int depth)
{
	HANDLE hFind;
	WIN32_FIND_DATA content;
	char path[MAX_PATH];

	sprintf_s(path, "%s\\*", startDir);


	if ((hFind = FindFirstFile(path, &content)) == INVALID_HANDLE_VALUE)
	{
		abort();
	}

	BOOL cont = TRUE;
	while (cont == TRUE)
	{
		if ((strcmp(".", content.cFileName) != 0) && (strcmp("..", content.cFileName) != 0))
		{
			if (content.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				for (int i = 0; i < depth; ++i)
					printf("\t");
				fprintf(stdout, "%s\n", content.cFileName);
				sprintf_s(path, "%s\\%s", startDir, content.cFileName);
				dirListFiles(path, depth + 1);
			}
			else
			{
				for (int i = 0; i < depth; ++i)
					printf("\t");
				fprintf(stdout, "%s\n", content.cFileName);
			}
		}
		cont = FindNextFile(hFind, &content);
	}
}

int main(int argc, char** argv)
{
	char* startDir = "c:\\work";

	dirListFiles(startDir, 0);

	system("pause");
	return 0;
}
