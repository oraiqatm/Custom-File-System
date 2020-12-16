#ifndef FILESYS_H
#define FILESYS_H

#include "Sdisk.h"

#include <string>
#include <vector>



class Filesys: public Sdisk{
	public:
		Filesys(string diskname, int numberofblocks, int blocksize);
		int fsclose();
		int fssynch();
		int newfile(string file);
		int rmfile(string file);
		int getfirstblock(string file);
		int addblock(string file, string block);
		int delblock(string file, int blocknumber);
		int readblock(string file, int blocknumber, string& buffer);
		int writeblock(string file, int blocknumber, string buffer);
		int nextblock(string file, int blocknumber);
		int getFilesysLength();
		vector<string> ls();
		string getling(string file);
		
	//	static vector<string> block(string,int);	
	private:
		void buildRoot();
		void buildFat();
		bool checkblock(string filename, int blocknumber);
		int rootsize; //Maximum number of entries in ROOT
		int fatsize; //Number of blocks occupied by FAT
		vector<string> filename;  //files in root
		vector<int> firstblock; //firstblocks in root
		vector<int> fat; //fat

};


#endif
