#include "Sdisk.h"
#include <fstream>
#include <iostream>

using namespace std;

Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)
{
	this->diskname = diskname;
	this->numberofblocks = numberofblocks;
	this->blocksize = blocksize;

	fstream disk;

	disk.open(this->diskname.c_str(), ios::in);
	if(disk.fail())
	{
		cout<< "Creating new file called: " << this->diskname << endl;
		disk.open(this->diskname.c_str(), ios::out);
		if(disk.fail())
		{
			cout<<"failed to make new file" << endl;
		}else
		{
			for(int i =0; i < this->numberofblocks * this->blocksize; i++)
				disk.put('#');
		}
	
	}
	disk.close();


}

int Sdisk::getblock(int blocknumber, string& buffer)
{
	fstream dFile(this->diskname.c_str(), ios::in);
	if(dFile.fail())
	{
		cout << "Failed to open file: " << this->diskname << endl;
		return 0;
	}
	dFile.seekg(blocknumber * this->blocksize);
	buffer = "";

	for(int i=0; i< this->blocksize; i++)
		buffer+= dFile.get();

	dFile.close();
	return 1; // success

}

int Sdisk::putblock(int blocknumber, string buffer)
{
	fstream dFile(this->diskname.c_str(), ios::in | ios::out); 
	if(dFile.fail()) 
	{	
		cout<<" Failed to open File." << endl;
		return 0;
	}

	dFile.seekg(blocknumber * this->blocksize);
	for(int i=0; i< buffer.length() && i< this->blocksize; i++)
	{
		dFile.put(buffer[i]);
	}

	dFile.close();
	return 1;
}

int Sdisk::getnumberofblocks()
{
	return this->numberofblocks;
}

int Sdisk::getblocksize()
{
	return this->blocksize;
}

/*
int main ()
{
	Sdisk disk1("test1", 16, 32);
	string block1, block2, block3, block4;
	for(int i=1; i<=32; i++) block1 = block1+"1";
	for(int i=1; i<=32; i++) block2 = block2+"2";
	disk1.putblock(4,block1);
	disk1.getblock(4,block3);
	cout << "Should be 32 1s : ";
	cout << block3 << endl;
	disk1.putblock(8, block2);
	disk1.getblock(8,block4);
	cout << "Should be 32 2s : " ;
	cout << block4 << endl;
	


}
*/
