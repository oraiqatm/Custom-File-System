#include <iostream> 
#include "Filesys.h"
#include "Sdisk.cpp" //comment out for make file
#include <sstream>
#include <string>

using namespace std;

static vector<string> block(string,int);

Filesys::Filesys(string diskname, int numberofblocks, int blocksize) : Sdisk(diskname, numberofblocks, blocksize)
{
	
	this->rootsize = this->getblocksize() /10;
	this->fatsize = ((this->getnumberofblocks() *5) / this->getblocksize()) +1;
	cout << "This is rootsize " << this->rootsize << endl; //returns 12
	cout << "This is fatsize " << this->fatsize << endl; //returns 11
	this->buildRoot();
	this->fssynch();

}

int Filesys:: fsclose()
{
	return 1;
}

int Filesys:: fssynch()
{
	//write root to disk 
	ostringstream ostream; 
	for(int i=0; i < this->rootsize; i++)
		ostream << this->filename[i] << " " << this->firstblock[i] << " ";
	
	string buffer = ostream.str();
	vector<string> blocks = block(buffer, this->getblocksize());
	this->putblock(1,blocks[0]);

	//Write fat to sdisk
	ostringstream ostream2;
	for(int i=0; i< this->fat.size(); i++)
		ostream2 << this->fat[i] << " ";
	
	string buffer2 = ostream2.str();
	vector<string> blocks2 = block(buffer2, getblocksize());

	for(int i=0; i< blocks2.size(); i++)
		this->putblock(2+i, blocks2[i]); 
	
	return 1;
}

int Filesys::newfile(string file)
{
	//Checking if it exists
	for(int i= 0; i< this->filename.size(); i++)
	{
		if(filename[i] == file)
		{
			cout << "File exists"; 
			return 0;
		}
			
		
	}

	for(int i=0; i < filename.size(); i++)
	{
		if(this->filename[i] == "xxxx")
		{
			this->filename[i] = file; 
			this->firstblock[i] = 0;
			cout << "File created" << endl;
			this->fssynch();
			
			return 1;
		}
			
	}
	cout << "no room in root";
	return 0;
}

int Filesys:: rmfile(string file)
{
	int tempFirstBlock = this->getfirstblock(file);
	if(tempFirstBlock != 0)
		return 0;

	for(int i =0; i <this->filename.size(); i++)
	{
		if(this->filename[i] == file)
			this->filename[i] = "xxxx";
	}
	this->fssynch();
	return 1;
}

int Filesys::getfirstblock(string file)
{
	//returns first block of file: 0 if success, -1 if empty
	for(int i=0; i< this->filename.size(); i++)
	{
		if(this->filename[i] == file)
			return this->firstblock[i];
	}
	cout << "no such file found "<< endl;
	return -1;
}

int Filesys::addblock(string file, string block)
{
	int first = this->getfirstblock(file);
	if(first == -1)
	{
		cout << "No such file" << endl;
		return 0;
		
	}
	int allocate = fat[0];
	fat[0] = fat[fat[0]];
	fat[allocate] = 0;
	
	if(first == 0)
	{
		for(int i=0; i< filename.size(); i++)
		{
			if(filename[i] == file)
			{
				firstblock[i] == allocate;
			}
		}
	}
	else
	{
		int b = first; 
		while(fat[b] != 0)
			b=this->fat[b];
		
		this->fat[b] = allocate;
	}
	this->fssynch();
	this->putblock(allocate, block);
	return allocate;


}

int Filesys:: delblock(string file, int blocknumber)
{
	if(this->checkblock(file, blocknumber))
		return 0;
	int b = this->getfirstblock(file);
	if(blocknumber = b)
	{
		for(int i=0; i < this->filename.size(); i++)
		{
			if(this->filename[i] == file)
			{
				this->firstblock[i] = this->fat[blocknumber];
				break;	
			}
		}
	}
	else
	{
		while(this->fat[b] != blocknumber)
		{
			b = this->fat[b];
		}
		this->fat[b] = this->fat[blocknumber];
	}
	//update the free list
	this->fat[blocknumber] = this->fat[0];
	this->fat[0] = blocknumber;
	return this->fssynch();

}

int Filesys::readblock(string file, int blocknumber, string& buffer)
{
	if(this->checkblock(file, blocknumber))
	{
		this->getblock(blocknumber, buffer);
		return 1;
	}
	else
	{
		return 0;
	}

}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{

	if(this->checkblock(file, blocknumber))
	{
		this->putblock(blocknumber, buffer);
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;
}

int Filesys::nextblock(string file, int blocknumber)
{
	if(this->checkblock(file, blocknumber))
		return this->fat[blocknumber];
	
	else
	{
		return 0;
	}

}

bool Filesys::checkblock(string filename, int blocknumber)
{
	int b = this->getfirstblock(filename);
	if(b == -1)
		return false;
	
	if(b == blocknumber)
		return true;
	
	while(this->fat[b] != 0)
	{
		if(this->fat[b] == blocknumber)
			return true;
		b = this->fat[b];
	}
	return false;
		
}

void Filesys:: buildRoot()
{
	ostringstream ostream; 
	for(int i=0; i< this->rootsize; i++)
	{
		this->filename.push_back("xxxx");
		this->firstblock.push_back(0);
		ostream << "xxxx" << " " << 0 << " ";
	}

	string buffer = ostream.str();
	vector<string> blocks = block(buffer, this->getblocksize());
	this->putblock(1,blocks[0]);

	ostringstream ostream2; 
	this->fat.push_back(this->fatsize +2);
	this->fat.push_back(1);
	for(int i=0; i< this->fatsize; i++)
	{
		fat.push_back(1);
	}

	for (int i = this->fatsize+2; i < this->getnumberofblocks(); i++)
	{
		this->fat.push_back(i+1);
	}

	this->fat[this->fat.size() -1] = 0;
	for(int i=0; i<fat.size(); i++)
	{
		ostream2 << this->fat[i] << " ";
	}
	
	string buffer2 = ostream2.str();
	vector<string> blocks2 = block(buffer2, this->getblocksize());
	for(int i=0; i< blocks2.size(); i++)
	{
		putblock(2+i, blocks2[i]);
	}
}

void Filesys:: buildFat()
{
	istringstream istream;
	string buffer;
	this->getblock(1, buffer);
	istream.str(buffer);
	for(int i=0; i < this->rootsize; i++)
	{
		string s; //filename
		int t; 
		istream >> s;
		istream >> t;
	//	cout << "istream " << s << endl ;
		this->filename.push_back(s);
		this->firstblock.push_back(t);
	}

	istringstream istream2; 

	for(int i=0; i < this->fatsize; i++)
	{
		string b;
		this->getblock(i+2, b);
		buffer += b;
	}
	istream2.str(buffer);
	for(int i=0; i<this->getnumberofblocks(); i++)
	{
		int t; 
		istream2 >> t;
		this->fat.push_back(t);
	}

}

// This function is part of the Filesys class
// Prototype: vector<string> ls();

vector<string> Filesys::ls()
{ 
vector<string> flist;
  for (int i=0; i<filename.size(); i++)
      {
        if (filename[i] != "XXXXX")
           {
             flist.push_back(filename[i]);
           }
      }
  return flist;
}


vector<string> block (string buffer, int b)
{
	vector<string> blocks;
	int numberofblocks = 0;
	if(buffer.length() % b ==0)
	{
		numberofblocks = buffer.length()/b;
	}
	else
	{
		numberofblocks = buffer.length()/b +1;
	}

	string tempblock;
	for(int i=0; i< numberofblocks; i++)
	{
		tempblock = buffer.substr(b*i, b);
		blocks.push_back(tempblock);
	}

	int lastblock = blocks.size() -1; 
	for(int i = blocks[lastblock].length(); i<b; i++)
	{

		blocks[lastblock] += "#";
	}
	return blocks;

}

int Filesys::getFilesysLength(){
	for( int i=0; i<filename.size(); i++)
	{
		cout << "Name of file: "<< filename[i] << endl;
	}
	cout << "Filesystem Root size = "<< filename.size() <<  " Fat size: " << this->fat.size() << endl;
	return 1;
}
/*
int main()
{
  Sdisk disk1("disk",256,128);
  Filesys fsys("disk",256,128);
  
  fsys.newfile("file1");
  fsys.newfile("file2");
 fsys.getFilesysLength();

  string bfile1;
  string bfile2;

  for (int i=1; i<=1024; i++)
     {
       bfile1+="1";
     }

  vector<string> blocks= block(bfile1,128); 

  int blocknumber=0;

  for (int i=0; i< blocks.size(); i++)
     {
       blocknumber=fsys.addblock("file1",blocks[i]);
     }

  fsys.delblock("file1",fsys.getfirstblock("file1"));

  for (int i=1; i<=2048; i++)
     {
       bfile2+="2";
     }

  blocks=block(bfile2,128); 

  for (int i=0; i< blocks.size(); i++)
     {
       blocknumber=fsys.addblock("file2",blocks[i]);
     }

  fsys.delblock("file2",blocknumber);
   

}

*/
