#include <iostream>
#include <string>
#include "Shell.cpp" //comment out for make file

using namespace std;

int main()
{
 //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
 string s;
 string command="go";
 string op1,op2;
 
//---------Initialize the Filesystem so that there is something to test.
Shell *shell = new Shell("test1", 256,128);
shell->newfile("file1");
shell->newfile("file2");

string bfile1;
string bfile2;
int blocknumber=0;
for (int i=1; i<=1024; i++)
{
    bfile1+="1";
}
vector<string> blocks= block(bfile1,128); 
for (int i=0; i< blocks.size(); i++)
{
    blocknumber= shell->addblock("file1",blocks[i]);
}

for (int i=1; i<=2048; i++)
{
    bfile2+="2";
}

blocks=block(bfile2,128); 

for (int i=0; i< blocks.size(); i++)
{
	blocknumber=shell ->addblock("file2",blocks[i]);
}

//-------------------------END ----------------

 while (command != "quit")
     {
       command.clear();
       op1.clear();
       op2.clear();
       cout << "$";
       getline(cin,s);
       int firstblank=s.find(' ');
       
       if (firstblank < s.length()) s[firstblank]='#';
       int secondblank=s.find(' ');
       command=s.substr(0,firstblank);
       
       if (firstblank < s.length())
         op1=s.substr(firstblank+1,secondblank-firstblank-1);
         
       if (secondblank < s.length())
         op2=s.substr(secondblank+1);
         
       if (command=="dir")
          {
            // use the ls function
            shell->dir();
           }
       if (command=="add")
          {
            // The variable op1 is the new file
			shell->add(op1);
           }
       if (command=="del")
          {
            // The variable op1 is the file
            shell->del(op1);
           }
       if (command=="type")
          {
            // The variable op1 is the file
            shell->type(op1);
           }
       if (command=="copy")
          {
            // The variable op1 is the source file and the variable op2 is the destination file.
            shell->copy(op1, op2);
           }
       if (command=="search")
          {
            // This is the command for Project 4
            // The variable op1 is the date
           }
       
      }

 return 0;
}
