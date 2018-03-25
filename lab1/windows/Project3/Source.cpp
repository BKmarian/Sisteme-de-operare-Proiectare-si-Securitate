#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp> 

//Am folosit 2 hashuri:unul<nume de familie , numar aparitii in fisier a acestui nume>
// al doilea <nume de familie, offsetul de la care apar numele in fisier>
//offseturile de la care se scriu numele de familie cresc din 5k in 5k , offsetul primului nume de familie fiind 64k
HANDLE hFile;
DWORD byteWrote;
std::map<std::string, int> hash;
std::map<std::string, int> aparitii;
int filePos = 0;
int offset = 64000; 
const char zero = '0';
const char space = ' ';
const char line = '\n';
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

char * removeZeros(char * buf) {
	char * temp = (char*)malloc(sizeof(char) * 50);
	int i;
	for (i = 0; i < 50; i++) {
		if (buf[i] == '0')
			break;
		temp[i] = buf[i];
	}
	temp[i] = '\0';
	return temp;
}
void readFromFile(std::string nume) {
	int pos = hash[nume];
	DWORD dwPtrLow = SetFilePointer(hFile,
		pos,
		NULL,
		FILE_BEGIN);

	char * buf = (char*)malloc(sizeof(char) * 50);
	DWORD byteRead, dw_bytes_written;
	for (int i = 0 ; i < aparitii[nume]; i++) {
		BOOL returnStatus = ReadFile(hFile, buf, 50, &byteRead, NULL);
		if (0 == byteRead)
		{
			break;
		}
		char *temp = removeZeros(buf);
		WriteFile(hStdOut, temp, strlen(temp), &dw_bytes_written, NULL);
	}
	WriteFile(hStdOut, &line, 1, &dw_bytes_written, NULL);
}
void readFromFile(std::string nume,std::string prenume) {
	int pos = hash[nume];
	DWORD dwPtrLow = SetFilePointer(hFile,
		pos,
		NULL,
		FILE_BEGIN);

	char buf[50];
	DWORD byteRead, dw_bytes_written;
	int nr = 0;
	
	for (int i = 0; i < aparitii[nume]; i++) {
		BOOL returnStatus = ReadFile(hFile, buf, 50, &byteRead, NULL);
		if (0 == byteRead)
		{
			break;
		}
		std::string temp(buf);
		std::vector<std::string> strs;
		boost::split(strs, temp, boost::is_any_of(" "));
		for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); ++it) {
			if (*it == prenume) {
				nr++;
				continue;
			}
		}
	}
	unsigned char nrChar = nr + '0'; // nr <= 100 deci e ok
	WriteFile(hStdOut, &nrChar, 1, &dw_bytes_written, NULL);
	WriteFile(hStdOut, &line, 1, &dw_bytes_written, NULL);
}

void writeNameAndOff(std::string nume, int off) {
	DWORD dwPtrLow = SetFilePointer(hFile,
		filePos,
		NULL,
		FILE_BEGIN);

	WriteFile(hFile, nume.c_str(), nume.length(), &byteWrote, NULL);
	WriteFile(hFile, &offset, sizeof(offset), &byteWrote, NULL);
	filePos += sizeof(int) + nume.length();
}
void writeToFile(std::string  nume, std::string  prenume) {
	int pos = 0;
	
	if (aparitii[nume] == NULL)
		aparitii[nume] = 1;
	else
		aparitii[nume] += 1;
	if (aparitii[nume] > 100)
		throw "Prea multe persoane cu acelasi nume de familie";
	
	if (hash[nume] == NULL) {
		hash[nume] = offset;
		writeNameAndOff(nume, offset);
		pos = offset;
		offset += 5000;
	}
	else {
		pos = hash[nume] + 50 * (aparitii[nume] - 1);
	}

	DWORD dwPtrLow = SetFilePointer(hFile,
		pos,
		NULL,
		FILE_BEGIN);

	WriteFile(hFile, nume.c_str(), nume.length(), &byteWrote, NULL);
	WriteFile(hFile, &space, 1, &byteWrote, NULL);
	WriteFile(hFile, prenume.c_str(), prenume.length(), &byteWrote, NULL);
	WriteFile(hFile, &space, 1, &byteWrote, NULL);
	for (int i = 0; i < 50 - nume.length() - prenume.length() - 2; i++)
		WriteFile(hFile, &zero, 1, &byteWrote, NULL);
}

int main(int arc, TCHAR * argv[])
{
	LPDWORD temp = NULL;
	hFile = CreateFile(argv[1],
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	if (hFile == INVALID_HANDLE_VALUE)
		WriteFile(hStdOut, "Error opening the file", sizeof("Error opening the file"), temp, NULL);


	std::string operation;
	while (1 != 0) {
		std::getline(std::cin, operation);
		if (operation == "exit")
			break;
		if (operation.find("insert") == 0) {
			std::vector<std::string> strs;
			boost::split(strs, operation, boost::is_any_of(" "));
			writeToFile(strs.at(1), strs.at(2));
		}
		if (operation.find("get") == 0) {
			std::vector<std::string> strs;
			boost::split(strs, operation, boost::is_any_of(" "));
			if (strs.size() == 2)
				readFromFile(strs.at(1));
			if (strs.size() == 3)
				readFromFile(strs.at(1), strs.at(2));
		}
	}

	CloseHandle(hFile);
	return 0;

}