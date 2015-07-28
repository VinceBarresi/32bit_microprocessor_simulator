#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <cstdlib>
using namespace std;

void InstructionFetch(char IR[]);
void InstructionDecode(char IR[], int& A, int& B, int& Imm);
void InstructionExecute(char IR[], int A, int B, int Imm, int& ALUout, int& Cond);
void MemoryAccess(int B, int ALUout, int Cond, int& LMD);
void WriteBack(int ALUout, int LMD);

int StringCopyitoj(char IR[], int i, int j);

char* StringCopy(char IR[], int i, int j);

int ToDecimal (char bit[], int length);

void print();
int shift(char *param,int length);
int twoscomplement(char * param,int length);

ifstream PC;
int Regs[32];
int Memory[40]={0};

int OpCode=0, Rs=0, Rt=0, Rd=0, Shamt=0,Funct=0, Immediate=0, TargetAdd=0;

int main (void)
{
  char IR[33]="00000000000000000000000000000000";
  int A, B, Imm, ALUout, LMD, Cond;

  PC.open("BinInstructions.txt"); // open the input file.
  if (!PC) {cerr <<"File could not be opened\n";  exit(1); }
  while (PC>>IR)  //InstructionFetch(IR)
  {
    
    cout<<"\nIR:"<< IR; 
    InstructionDecode(IR, A, B, Imm);
    InstructionExecute(IR, A,B,Imm, ALUout, Cond);
    MemoryAccess(B, ALUout, Cond, LMD);
    WriteBack(ALUout, LMD);
    cout <<endl;  
  }
  print();
  return 1;
}

void InstructionDecode(char IR[], int& A, int& B, int& Imm)
{
  OpCode= StringCopyitoj(IR,0,5);
  Rs= StringCopyitoj (IR,6,10);
  A=Regs[Rs];
  Rt=StringCopyitoj (IR, 11,15);
  B=Regs[Rt];
  Rd= StringCopyitoj (IR, 16,20);
  Shamt= StringCopyitoj (IR, 21, 25);
  Funct= StringCopyitoj (IR, 26,31);
  Immediate= StringCopyitoj (IR, 16, 31);
  Imm=Immediate;
  TargetAdd= StringCopyitoj (IR, 6, 31);

}

void InstructionExecute(char IR[], int A, int B, int Imm, int& ALUout, int& Cond)
{
  if (OpCode== 8)
  {
    cout<<"\nInstruction: Addi";
    ALUout = A + Imm; 
    
  }
  if (OpCode == 43)
  {
    cout<<"\nInstruction: Sw";
    ALUout = A + Imm;
  }
  if (OpCode == 35)
  {
    cout<<"\n Lw";
    ALUout= A + Imm;
    
  }
  if (OpCode == 0 && Funct== 32)
  {
    cout<<"\nInstruction: Add";
    ALUout= A + B;
  }
  if (OpCode==5)
  {
    cout<<"\nInstruction: Bne";

    ALUout= ( (shift(StringCopy(IR, 16, 31),16) * 8) - 8 ); 
    if (Regs[Rs]!=Regs[Rt])
    {
      Cond= 1;  
    }
    if (Regs[Rs]==Regs[Rt])
    {
      Cond=0;
    }
  }
  if (OpCode == 0 && Funct == 34)
  {
    cout<<"\nInstruction: Sub";
    ALUout= A - B;
  }
  if (OpCode == 4)
  {
    cout<<"\nInstruction: Beq";
    //cout<<"\n bne address: "<< ( (shift(StringCopy(IR, 16, 31),16) * 8) - 8 );
    //ALUout=-168; // this should be extracted from binary and converted from -20 to -160-8 ( (-20*8) - 8 )
    ALUout= ( (shift(StringCopy(IR, 16, 31),16) * 8) - 8 );
    if (Regs[Rs]!=Regs[Rt])
    {
      Cond= 1;  
    }
    if (Regs[Rs]==Regs[Rt])
    {
      Cond=0;
    }
  }
}

void MemoryAccess(int B, int ALUout, int Cond, int& LMD)
{

  if (OpCode== 8)
  {
    //cout<<"\n Add1";
    
  }
  if (OpCode == 43)
  {
    Memory[ALUout] = B;
  }
  if (OpCode == 35)
  {
    LMD = Memory [ALUout];
  }
  if (OpCode == 0 && Funct== 32)
  {
    //cout<<"\n Add";
  }
  if (OpCode==5)
  {
    if (Cond == 1)
    {
      PC.seekg(ALUout ,std::ios::cur);
    }
  }
  if (OpCode == 0 && Funct == 34)
  {
    //cout<<"\nsub";
  }
  if (OpCode == 4)
  {
    if (Cond == 1)
    {
      PC.seekg(ALUout ,std::ios::cur);
    }
  }
}
void WriteBack(int ALUout, int LMD)
{
  if (OpCode== 8)
  {
    Regs[Rt] = ALUout;
    
  }
  if (OpCode == 43)
  {
   
  }
  if (OpCode == 35)
  {
    Regs[Rt] = LMD;
  }
  if (OpCode == 0 && Funct== 32)
  {
    Regs[Rd] = ALUout;
  }
  if (OpCode==5)
  {
  }
  if (OpCode == 0 && Funct == 34)
  {
    Regs[Rd] = ALUout;
  }
  if (OpCode==4)
  {
  
  }
}

int ToDecimal (char bit[], int length)
{
  int decimal = 0;
  for (int i=0; i<length; i++)
  {
    if (bit[i]== '1')
      decimal += (int)pow((float)2,length-1-i);
  }
    return decimal;
}

int StringCopyitoj(char IR[], int i, int j)
{
  char temp[100];
  int k=0;
  int length= (j-i) + 1;
  for (;i<=j;i++)
  {
    temp[k]=IR[i];
    k++;
  }
  temp[k]='\0';
  return ToDecimal(temp, length);
}

char* StringCopy(char IR[], int i, int j)
{
  char static temp[100];
  int k=0;
  int length= (j-i) + 1;
  for (;i<=j;i++)
  {
    temp[k]=IR[i];
    k++;
  }
  temp[k]='\0';
  return temp;
}
void print()
{
  cout<<"\n---------------------------------------------------------";
  cout<<"\n\t Memory Contents";
  cout<<"\n---------------------------------------------------------";
  for (int i=0;i<30;i=i+4)
  {
    cout<<"\tMemory["<<i<<"]: "<<Memory[i];
  }
  cout<<"\n---------------------------------------------------------";
  cout<<"\n\t Register Contents";
  cout<<"\n---------------------------------------------------------";
  for (int i=0;i<5;i++)
  {
    cout<<"\tRegs["<<i<<"]: "<<Regs[i];
  }
  cout<<endl;
}

int twoscomplement(char * param,int length)
{
  char temp[length];
  for (int i=0;i<length;i++)
  {
    if (param[i]=='0')
      temp[i]='1';
    else
      temp[i]='0';
  }
  //cout<<"\ntemp 2 complement: "<<temp;
  // now adding 1
  for (int i=length-1;i>=0;i--)
  {
    if (temp[i]=='1')
    {
      temp[i]='0';
      continue;
    }
    if (temp[i] == '0')
    {
      temp[i]='1';
      break;
    }
  }
  return (-1*ToDecimal(temp,32));
}
int shift(char *param,int length)
{
  char temp[20];
  char temp2[32];
  int location;
  for (int i=0;i<length;i++)
  {
    location=((i-2))%length;
    if (location < 0)
    {
      location=location+length;
      temp[location]='0';
    }
    else
    {
      temp[location]=param[i];
    }
  }
  temp[length]='\0';
  strcpy(temp2,"1111111111111111"); //padding extra bits
  strcat(temp2,temp);

  return twoscomplement(temp2,32);
}
