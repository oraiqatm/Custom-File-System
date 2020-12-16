#include "shell.h"
#include <iostream>
#include <vector>
#include "Filesys.cpp" //comment out for make file



Shell::Shell(string diskname,int blocksize, int numberofblocks):Filesys(diskname, numberofblocks, blocksize)
{
	cout << "Shell is active" << endl;

}

int Shell::dir()
{ 
	vector<string> flist= ls();
  	for (int i=0; i<flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
}

int Shell::add(string file)
{
	int error = newfile(file);
	if(error < 1)
	{
		cout << "Sorry";
		return 0;
	}
	/*
	vector<string> blocks = block(buffer, getblocksize());
	For(int i=0; i< blocks.size(), i++)
	{
		Addblock(file,blocks[i]);
	}
	*/
	return 1;
}

int Shell::del(string file)
{
	int error = this->getfirstblock(file);
	if(error == -1)
	{
		cout << "Could not find file." << endl;
		return 0;
	}
	int newFirstblock;
	while((newFirstblock = this->getfirstblock(file)) > 0)
		//this->delblock(file, firstblock(file))
		this->delblock(file, newFirstblock);
		
	this->rmfile(file);
	return 1;
}

int Shell::type(string file)
{
	string fileBuffer = "";
	string blockBuffer = "";
	
	for(int current = this->getfirstblock(file); current !=0; current = this->nextblock(file, current))
	{
		this->readblock(file, current, blockBuffer);
		fileBuffer += blockBuffer;
	}
	
	fileBuffer = fileBuffer.substr(0, fileBuffer.find_first_of('#'));
	cout << fileBuffer << endl;
	return 1;
}

int Shell::copy(string file1, string file2)
{
	int errorFile1 = this->getfirstblock(file1);

	if(errorFile1 == -1)
	{
		cout<<"Could not find file."<<endl;
		return -11;
	}
	
	
	string copyBuffer = "";
	for(int current = this->getfirstblock(file1); current !=0; current =this->nextblock(file1, current))
	{
		this->readblock(file1, current, copyBuffer);
		this->addblock(file1, copyBuffer);
	}
	
}


string Shell::getfile(string filename)
{	
	string blockBuffer = "";
	string output = "";

	int error = this->getfirstblock(filename);
	if(error == -1)
	{
		cout << "could not open file: ";
		return output;
	}
	
	for(int current = this->getfirstblock(filename); current !=0; current =this->nextblock(filename, current))
	{
		this->readblock(filename, current, blockBuffer);
		output += blockBuffer;
	} 

	return output;
}









