#include "Table.h"
#include <iostream>
#include <vector>
#include <string>
#include "Shell.cpp"

using namespace std;


Table::Table(string diskname, int blocksize, int numberofblocks, string flatfile, string indexfile):Filesys(diskname,numberofblocks,blocksize) 
{
	this->flatfile = flatfile;
	this->indexfile = indexfile;
	this->newfile(flatfile);
	this->newfile(indexfile);

}
/*
int Table::Build_Table(string input_file)
{
	ostringstream indexstream, flatstream;
	ifstream infile; 
	infile.open(input_file.c_str());
	if(infile.bad())
	{
		return 0;
	}
	
	string s = getfile(input_file.c_str());
	while(infile.good())
	{
		//Process line // write the record to the flat file and add the index record
		string key = s.substr(0,5);
		vector <string> blocks = block(s,getblocksize);
		int b = addblock(flatfile, blocks[0]);
		indexstream << key << " " << b << " ";
		numrec ++;
		
		s= getline(infile); 
	}
	
	Process the indexfile;
	String buffer = indexstream.string();
	Vector <string> blocks = block(buffer, getblocksize());
	For(int i=0; i< blocks.size(); i++)
	{
		Int  b = addblock(indexfile, blocks[i]);
	}

}
*/
int Table::Search(string value)
{
	int b = this->IndexSearch(value);
	if(b<=0)
	{
		return -1;
	}
	
	string buffer;
	b = readblock(flatfile,b,buffer);
	cout << buffer;
	return b;
}

int Table::IndexSearch(string value)
{
	istringstream instream;
	string bigbuffer;
	int numrec =0;
	//int b = this->firstblock(indexfile);
	int b = 1;
	if(b <=0)
		return 0;
	while (b!=0)
	{
		string buffer;
		int error = readblock(indexfile, b, buffer);
		bigbuffer += buffer;
		b = nextblock(indexfile,b);
	}
	
	instream.str(bigbuffer);
	for(int i=0; i<numrec; i++)
	{
		string key;
		int block;
		instream>>key>>block;
		if(key == value)
		{
			return block;
		}
		return -1;
	}
	
}









