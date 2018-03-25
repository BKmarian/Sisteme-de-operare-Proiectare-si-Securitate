#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

//Am folosit 2 hashuri:unul<nume de familie , numar aparitii in fisier a acestui nume>
// al doilea <nume de familie, offsetul de la care apar numele in fisier>
//offseturile de la care se scriu numele de familie cresc din 5k in 5k , offsetul primului nume de familie fiind 64k
//pentru operatii de parse pe stringuri am folosit boost
int fd;
std::map<std::string, int> hash;
std::map<std::string, int> aparitii;
int filePos = 0;
int offset = 64000;
const char zero = '0';
const char space = ' ';
const char line = '\n';

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
	lseek(fd, pos, SEEK_SET);

	char * buf = (char*)malloc(sizeof(char) * 50);
	for (int i = 0 ; i < aparitii[nume]; i++) {
		ssize_t byteRead = read (fd, buf, 50);

        if(-1 == byteRead) {
            printf ("%s", strerror (errno));
            break;
        }

		if (0 == byteRead) {
			break;
		}
		char *temp = removeZeros(buf);
		write (1, temp, strlen(temp));
	}
	write (1, &line, 1);
}

void readFromFile(std::string nume,std::string prenume) {
	int pos = hash[nume];
	lseek(fd, pos, SEEK_SET);

	char buf[50];
	ssize_t byteRead;
	int nr = 0;

	for (int i = 0; i < aparitii[nume]; i++) {
		byteRead = read (fd, buf, 50);
        if(-1 == byteRead) {
            printf ("%s", strerror (errno));
            break;
        }
		if (0 == byteRead) {
			break;
		}
		std::string temp(buf);
        std::vector<std::string> strs;
        boost::split(strs, temp, boost::is_any_of(" "));
        for (std::vector<std::string>::iterator it = strs.begin() ; it != strs.end(); ++it) {
            if(*it == prenume) {
                nr ++;
                continue;
            }
        }
	}
	unsigned char nrChar = nr + '0'; // nr <= 100 deci e ok
	write (1, &nrChar, 1);
	write (1, &line, 1);
}

void writeNameAndOff(std::string nume, int off) {
	lseek(fd, off, SEEK_SET);

	write (fd, nume.c_str(), nume.length());
	write (fd, &offset, sizeof(offset));
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
		writeNameAndOff(nume, offset); //Cazul in care numele de familie nu a mai fost inserat
		pos = offset;
		offset += 5000;
	}
	else {
		pos = hash[nume] + 50 * (aparitii[nume] - 1);
	}

	lseek(fd, pos, SEEK_SET);

	write (fd, nume.c_str(), nume.length());
    write (fd, &space, 1);
	write (fd, prenume.c_str(), prenume.length());
	write (fd, &space, 1);

	for (int i = 0; i < 50 - nume.length() - prenume.length() - 2; i++)
		write (fd, &zero, 1);
}

int main(int arc, char * argv[])
{
	fd = open (argv[1], O_RDWR);

    if (fd == -1){
      printf ("%s", strerror (errno));
      return -1;
    }

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

	close(fd);
	return 0;

}