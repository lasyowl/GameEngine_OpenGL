#ifndef __MODEL_LIST_H
#define __MODEL_LIST_H

#include <vector>
#include <list>
#include <string>

struct FileInfo {
	std::string fileName;
	std::string filePath;
	std::string fullPath;
	int additionalAnims;
	std::list<std::string> animList;
};

class Model_List {

public:
	Model_List();
	~Model_List();

	void ReadFile(std::string);

	std::vector<FileInfo> modelList;

private:

};
#endif