#include <iostream>
#include <fstream>
#include <string>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <sstream>

using namespace std;
typedef unsigned char BYTE;
using std::string;

int btoi(string r)
{
   int output = 0;
   string t="1",t1;

   for(int i = 0; i<r.length(); i++)
   {
     t1 = r.substr(i,1);
     if(t.compare(t1)==0)
       {
   	output = output+pow(2,r.length()-i-1);
       }
   }
   return (output);
}

long getFileSize(FILE *file)
{
    long lCurPos, lEndPos;
    lCurPos = ftell(file);
    fseek(file, 0, 2);
    lEndPos = ftell(file);
    fseek(file, lCurPos, 0);
    return lEndPos;
}

string dec2bin(unsigned n)
{
    const size_t size = sizeof(n) * 8;
    char result[size];

    unsigned index = size;
    do
    {
        result[--index] = '0' + (n & 1);
    } 
    while (n >>= 1);

    return std::string(result + index, result + size);
}

int main(int argc, char* argv[])
{
    int pc = 584, im = 0,rt,rs,rd, f=0,reg[32],reg_inst[32],add_space[20],i,k,z,l,cycle=1,rs_ctr=0,rob_ctr=0,add_ctr=0,done=0,add_space_ctr=0,iq_ctr=0,iq_ctr2=0,btb_ctr=0,start,end,done1,m,n,print_flag;
    string line, add="000000",addi="001000", addiu="001001", beq="000100", bgez="000001";
    string bgtz="000111", bne="000101", blez="000110", j="000010", lw="100011", sw="101011";
    string slti="001010";
    string opcode, r1, r2, r3, r4 ,ed, temp;
    ofstream fout;
    ifstream fin;
    BYTE *fileBuf;
    FILE *file = NULL;
	
	if(argc==3)
	{
		print_flag=0;
	}
	else if(argc==4)
	{
		print_flag=1;
		string myString = argv[3];
		string m1=myString.substr(2,myString.find_first_of(":")-2);
		string n1=myString.substr(myString.find_first_of(":")+1,myString.length()-myString.find_first_of(":")-1);
		istringstream buffer(m1);
		istringstream buffer1(n1);
		buffer >> m;
		buffer1 >> n;
	}//cout<<m<<" "<<n;
    struct BTB
    {
	int address, destination, predictor,iq;
    }btb[10]; 	         
        
    struct RS
    {
	int cycle,print_valid,rd,rs,rt,im,done,iq;
	string inst;
    }rs1[100];

    struct ROB
    {
	int cycle,valid,rs_no,rd,rt,rs,im,done;
	string inst;
    }rob[100];

    struct IQ
    {
	string inst;
	int rs,rd,rt,im;
    }iq[32];
    fout.open("temp1.txt");
    if ((file = fopen(argv[1], "rb")) == NULL)
	cout << "Could not open specified file" << endl;
    long fileSize = getFileSize(file);
    fileBuf = new BYTE[fileSize];
    fread(fileBuf, fileSize, 1, file);
    for (i = 0; i < fileSize; i++)
    {
        string temp = dec2bin(static_cast<int>(fileBuf[i])), temp2="0";
	l=temp.length();
	if (l < 8)
	{
		for(z=0;z<(8-l);z++)
		{
			temp = temp2 + temp;
		}
	}
	fout<< temp;
	if (i%4 == 3)
	{
		fout<<"\n";
	}
    }
    fin.close();   
    fout.close();
    fin.open("temp1.txt");
    while(!fin.eof())
    {
	getline(fin,line);	
	if (fin.eof())
	  break;
        opcode = line.substr(0,6);
        r1 = line.substr(6,5);
        r2 = line.substr(11,5);
        r3 = line.substr(16,5);
        r4 = line.substr(21,5);
        ed = line.substr(26,6);
        if (f==1 && pc>=700)
	{
	    im=btoi(line);
	    if (im>pow(2,31)-1)
		im=im-pow(2,32);
	    add_space[add_space_ctr++]=im;
            pc = pc + 4;
	}
   else {
        if (opcode.compare(add) == 0)
        {
	  if (r4.compare("00000") == 0)
            {
	      if (ed.compare("100000") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
		    iq[iq_ctr].inst="ADD";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("100001") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
		    iq[iq_ctr].inst="ADDU";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("100100") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="AND";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("000000") == 0)
                {
                    iq[iq_ctr].inst="NOP";
                }
	      else if (ed.compare("100111") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="NOR";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("100101") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="OR";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("100110") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="XOR";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("100010") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="SUB";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("100011") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="SUBU";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("101010") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="SLT";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("101011") == 0)
                {
                    rt = btoi(r1);
                    rs = btoi(r2);
                    rd = btoi(r3);
                    iq[iq_ctr].inst="SLTU";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].rs=rs;
                }
	      else if (ed.compare("001101") == 0)
		{
		  iq[iq_ctr].inst="BREAK";
		  f=1;
		}
            }
	  else if (r1.compare("00000") == 0)
            {
	      if (ed.compare("000010") == 0)
                {
        	    rt = btoi(r2);
        	    rd = btoi(r3);
        	    im = btoi(r4);
		    iq[iq_ctr].inst="SRL";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].im=im;
                }
	      else if (ed.compare("000011") == 0)
                {
                	rt = btoi(r2);
                	rd = btoi(r3);
                	im = btoi(r4);
                    iq[iq_ctr].inst="SRA";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].im=im;
                }
	      else if (ed.compare("000000") == 0)
                {
                	rt = btoi(r2);
                	rd = btoi(r3);
                	im = btoi(r4);
                	iq[iq_ctr].inst="SLL";
		    iq[iq_ctr].rd=rd;
		    iq[iq_ctr].rt=rt;
		    iq[iq_ctr].im=im;
                }
            }
        }
        else if (opcode.compare(addi) == 0)
        {
            rs = btoi(r1);
            rt = btoi(r2);
            temp = line.substr(16,16);
            im = btoi(temp);
	    if (im>32767)
		im=im-65536;
	    iq[iq_ctr].inst="ADDI";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].rt=rt;
	    iq[iq_ctr].im=im;
        }
        else if (opcode.compare(addiu) == 0)
        {
            rs = btoi(r1);
            rt = btoi(r2);
            temp = line.substr(16,16);
            im = btoi(temp);
	    if (im>32767)
		im=im-65536;
            iq[iq_ctr].inst="ADDIU";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].rt=rt;
	    iq[iq_ctr].im=im;
        }
        else if (opcode.compare(beq) == 0)
        {
            rs = btoi(r1);
            rt = btoi(r2);
            temp = line.substr(16,16);
	    temp = temp + "00";
            im = btoi(temp);
	    if (im>pow(2,17))
		im=im-pow(2,18);
            iq[iq_ctr].inst="BEQ";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].rt=rt;
	    iq[iq_ctr].im=im;
	    btb[btb_ctr].address=pc;
	    btb[btb_ctr].destination=pc+4+iq[iq_ctr].im;
	    btb[btb_ctr].iq=iq_ctr;
	    btb[btb_ctr++].predictor=3;
        }
        else if (opcode.compare(bgez) == 0 && r2.compare("00001") == 0)
        {
            rs = btoi(r1);
            temp = line.substr(16,16);
	    temp = temp + "00";
            im = btoi(temp);
	    if (im>pow(2,17))
		im=im-pow(2,18);
	    iq[iq_ctr].inst="BGEZ";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].im=im;
	    btb[btb_ctr].address=pc;
	    btb[btb_ctr].destination=pc+4+iq[iq_ctr].im;
	    btb[btb_ctr].iq=iq_ctr;
	    btb[btb_ctr++].predictor=3;
        }
        else if (opcode.compare(bgtz) == 0 && r2.compare("00000") == 0)
        {
            rs = btoi(r1);
            temp = line.substr(16,16);
	    temp = temp + "00";
            im = btoi(temp);
	    if (im>pow(2,17))
		im=im-pow(2,18);
	    iq[iq_ctr].inst="BGTZ";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].im=im;
	    btb[btb_ctr].address=pc;
	    btb[btb_ctr].destination=pc+4+iq[iq_ctr].im;
	    btb[btb_ctr].iq=iq_ctr;
	    btb[btb_ctr++].predictor=3;
        }
        else if (opcode.compare(bne) == 0)
        {
            rs = btoi(r1);
            rt = btoi(r2);
            temp = line.substr(16,16);
	    temp = temp + "00";
            im = btoi(temp);
	    if (im>pow(2,17))
		im=im-pow(2,18);
	    iq[iq_ctr].inst="BNE";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].rt=rt;
	    iq[iq_ctr].im=im;
	    btb[btb_ctr].address=pc;
	    btb[btb_ctr].destination=pc+4+iq[iq_ctr].im;
	    btb[btb_ctr].iq=iq_ctr;
	    btb[btb_ctr++].predictor=3;
        }
        else if (opcode.compare(blez) == 0 && r2.compare("00000") == 0)
        {
            rs = btoi(r1);
            temp = line.substr(16,16);
	    temp = temp + "00";
            im = btoi(temp);
	    if (im>pow(2,17))
		im=im-pow(2,18);
            iq[iq_ctr].inst="BLEZ";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].im=im;
	    btb[btb_ctr].address=pc;
	    btb[btb_ctr].destination=pc+4+iq[iq_ctr].im;
	    btb[btb_ctr].iq=iq_ctr;
	    btb[btb_ctr++].predictor=3;
        }
	else if (opcode.compare("000001") == 0 && r2.compare("00000") == 0)
        {
            rs = btoi(r1);
            temp = line.substr(16,16);
	    temp = temp + "00";
            im = btoi(temp);
	    if (im>pow(2,17))
		im=im-pow(2,18);
            iq[iq_ctr].inst="BLTZ";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].im=im;
	    btb[btb_ctr].address=pc;
	    btb[btb_ctr].destination=pc+4+iq[iq_ctr].im;
	    btb[btb_ctr].iq=iq_ctr;
	    btb[btb_ctr++].predictor=3;
        }
        else if (opcode.compare(j) == 0)
        {
	    temp = line.substr(6,26);
	    temp = temp + "00";
	    im = btoi(temp);
	    iq[iq_ctr].inst="J";
	    iq[iq_ctr].im=im;
	    btb[btb_ctr].address=pc;
	    btb[btb_ctr].destination=iq[iq_ctr].im;
	    btb[btb_ctr].iq=iq_ctr;
	    btb[btb_ctr++].predictor=3;
        }
        else if (opcode.compare(lw) == 0)
        {
            rs = btoi(r1);
            rt = btoi(r2);
            temp = line.substr(16,16);
            im = btoi(temp);
	    if (im>32767)
		im=im-65536;
	    iq[iq_ctr].inst="LW";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].rt=rt;
	    iq[iq_ctr].im=im;
        }
        else if (opcode.compare(sw) == 0)
        {
            rs = btoi(r1);
            rt = btoi(r2);
            temp = line.substr(16,16);
            im = btoi(temp);
	    if (im>32767)
		im=im-65536;
            iq[iq_ctr].inst="SW";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].rt=rt;
	    iq[iq_ctr].im=im;
        }
        else if (opcode.compare(slti) == 0)
        {
            rs = btoi(r1);
            rt = btoi(r2);
            temp = line.substr(16,16);
            im = btoi(temp);
	    if (im>32767)
		im=im-65536;
	    iq[iq_ctr].inst="ADDI";
	    iq[iq_ctr].rs=rs;
	    iq[iq_ctr].rt=rt;
	    iq[iq_ctr].im=im;
        }
        pc = pc + 4;
		iq_ctr++;
    }
   }
   fin.close();
	for(i=iq_ctr-1;i>=0;i--)
	{
		if(iq[i].inst=="BREAK")
			break;
		else iq[i].inst="";
	}
   for (i=0;i<100;i++)
    {
	if(i<32) {
	reg[i]=0;
	reg_inst[i]=0;}
	rs1[i].print_valid=0;
	rob[i].valid=0;
	rs1[i].rt=0;
	rs1[i].rd=0;
	rs1[i].rs=0;
	rs1[i].done=0;
	rs1[i].im=0;
	rob[i].rt=0;
	rob[i].rd=0;
	rob[i].rs=0;
	rob[i].im=0;
	rob[i].done=0;
    }
   fout.open(argv[2]);
	start=0;
   while(done!=1)
   {	
	rob[rob_ctr].inst=iq[iq_ctr2].inst;
	rob[rob_ctr].rs=iq[iq_ctr2].rs;
	rob[rob_ctr].rt=iq[iq_ctr2].rt;
	rob[rob_ctr].rd=iq[iq_ctr2].rd;
	rob[rob_ctr].im=iq[iq_ctr2].im;
	rob[rob_ctr].valid=cycle+1;
	rob[rob_ctr].done=0;
	if(iq[iq_ctr2].inst=="LW")
		rob[rob_ctr].cycle=cycle+3;
	else rob[rob_ctr].cycle=cycle+4;
	rs1[rs_ctr].inst=iq[iq_ctr2].inst;
	rs1[rs_ctr].rs=iq[iq_ctr2].rs;
	rs1[rs_ctr].rt=iq[iq_ctr2].rt;
	rs1[rs_ctr].rd=iq[iq_ctr2].rd;
	rs1[rs_ctr].im=iq[iq_ctr2].im;
	rs1[rs_ctr].print_valid=cycle+1;
	rs1[rs_ctr].done=0;rs1[rs_ctr].iq=iq_ctr2;
	if(iq[iq_ctr2].inst=="LW")
		rs1[rs_ctr].cycle=cycle+2;
	else if(iq[iq_ctr2].inst=="NOP")
		rs1[rs_ctr].cycle=cycle+1;	
	else if(iq[iq_ctr2].inst=="BREAK")
		rs1[rs_ctr].cycle=cycle+2;
	else rs1[rs_ctr].cycle=cycle+4;
	if(rob[rob_ctr].inst=="BNE" || rob[rob_ctr].inst=="BEQ" || rob[rob_ctr].inst=="BGEZ" || rob[rob_ctr].inst=="BGTZ" || rob[rob_ctr].inst=="BLEZ" || rob[rob_ctr].inst=="BLTZ" || rob[rob_ctr].inst=="J")
	{
		for(i=0;i<btb_ctr;i++)
		{
			if(btb[i].predictor==3 && btb[i].iq==iq_ctr2)
				btb[i].predictor=2;
		}
	}
	if(print_flag==0 || (print_flag==1 && cycle>=m && cycle<=n))
	{
	if(iq[iq_ctr2].inst=="")
		fout<<"Cycle <"<<cycle<<">:\nIQ:\n";
	else{
	fout<<"Cycle <"<<cycle<<">:\nIQ:\n[";	
	fout<<iq[iq_ctr2].inst;
	if(iq[iq_ctr2].inst=="ADD" || iq[iq_ctr2].inst=="ADDU" || iq[iq_ctr2].inst=="AND" || iq[iq_ctr2].inst=="NOR" || iq[iq_ctr2].inst=="OR" || iq[iq_ctr2].inst=="XOR" || iq[iq_ctr2].inst=="SUB" || iq[iq_ctr2].inst=="SUBU" || iq[iq_ctr2].inst=="SLT" || iq[iq_ctr2].inst=="SLTU")
		fout<<" R"<<iq[iq_ctr2].rd<<", R"<<iq[iq_ctr2].rt<<", R"<<iq[iq_ctr2].rs<<"]\n";
	else if(iq[iq_ctr2].inst=="SRL" || iq[iq_ctr2].inst=="SRA" || iq[iq_ctr2].inst=="SLL")
		fout<<" R"<<iq[iq_ctr2].rd<<", R"<<iq[iq_ctr2].rt<<", #"<<iq[iq_ctr2].im<<"]\n";
	else if(iq[iq_ctr2].inst=="ADDI" || iq[iq_ctr2].inst=="ADDIU" || iq[iq_ctr2].inst=="BEQ" || iq[iq_ctr2].inst=="BNE")
		fout<<" R"<<iq[iq_ctr2].rt<<", R"<<iq[iq_ctr2].rs<<", #"<<iq[iq_ctr2].im<<"]\n";
	else if(iq[iq_ctr2].inst=="BGEZ" || iq[iq_ctr2].inst=="BGTZ" || iq[iq_ctr2].inst=="BLEZ" || iq[iq_ctr2].inst=="BLTZ")
		fout<<" R"<<iq[iq_ctr2].rs<<", #"<<iq[iq_ctr2].im<<"]\n";
	else if(iq[iq_ctr2].inst=="LW" || iq[iq_ctr2].inst=="SW")
		fout<<iq[iq_ctr2].rt<<", "<<iq[iq_ctr2].im<<"(R"<<iq[iq_ctr2].rs<<")]\n";
	else if(iq[iq_ctr2].inst=="J")
		fout<<"#"<<iq[iq_ctr2].im<<"]\n";
	else fout<<"]\n";}
	fout<<"RS:\n";
	for(k=0;k<rs_ctr;k++)
	{
		if(rs1[k].print_valid!=0 && rs1[k].done==0)
		{
			fout<<"["<<rs1[k].inst;
			if(rs1[k].inst=="ADD" || rs1[k].inst=="ADDU" || rs1[k].inst=="AND" || rs1[k].inst=="NOR" || rs1[k].inst=="OR" || rs1[k].inst=="XOR" || rs1[k].inst=="SUB" || rs1[k].inst=="SUBU" || rs1[k].inst=="SLT" || rs1[k].inst=="SLTU")
				fout<<" R"<<rs1[k].rd<<", R"<<rs1[k].rt<<", R"<<rs1[k].rs<<"]\n";
			else if(rs1[k].inst=="SRL" || rs1[k].inst=="SRA" || rs1[k].inst=="SLL")
				fout<<" R"<<rs1[k].rd<<", R"<<rs1[k].rt<<", #"<<rs1[k].im<<"]\n";
			else if(rs1[k].inst=="ADDI" || rs1[k].inst=="ADDIU" || rs1[k].inst=="BEQ" || rs1[k].inst=="BNE")
				fout<<" R"<<rs1[k].rt<<", R"<<rs1[k].rs<<", #"<<rs1[k].im<<"]\n";
			else if(rs1[k].inst=="BGEZ" || rs1[k].inst=="BGTZ" || rs1[k].inst=="BLEZ" || rs1[k].inst=="BLTZ")
				fout<<" R"<<rs1[k].rs<<", #"<<rs1[k].im<<"]\n";
			else if(rs1[k].inst=="LW" || rs1[k].inst=="SW")
				fout<<" R"<<rs1[k].rt<<", "<<rs1[k].im<<"(R"<<rs1[k].rs<<")]\n";
			else if(rs1[k].inst=="J")
				fout<<" #"<<rs1[k].im<<"]\n";
			else fout<<"]\n";
		}
	}
	fout<<"ROB:\n";
	for(k=0;k<rob_ctr;k++)
	{
		if(cycle>=rob[k].valid && rob[k].done==0)
		{
			if(rob[k].cycle==999)
				rob[k].cycle=1000;
			else if(rob[k].cycle==1000)
				rob[k].done=1;
			fout<<"["<<rob[k].inst;
			if(rob[k].inst=="ADD" || rob[k].inst=="ADDU" || rob[k].inst=="AND" || rob[k].inst=="NOR" || rob[k].inst=="OR" || rob[k].inst=="XOR" || rob[k].inst=="SUB" || rob[k].inst=="SUBU" || rob[k].inst=="SLT" || rob[k].inst=="SLTU")
				fout<<" R"<<rob[k].rd<<", R"<<rob[k].rt<<", R"<<rob[k].rs<<"]\n";
			else if(rob[k].inst=="SRL" || rob[k].inst=="SRA" || rob[k].inst=="SLL")
				fout<<" R"<<rob[k].rd<<", R"<<rob[k].rt<<", #"<<rob[k].im<<"]\n";
			else if(rob[k].inst=="ADDI" || rob[k].inst=="ADDIU" || rob[k].inst=="BEQ" || rob[k].inst=="BNE")
				fout<<" R"<<rob[k].rt<<", R"<<rob[k].rs<<", #"<<rob[k].im<<"]\n";
			else if(rob[k].inst=="BGEZ" || rob[k].inst=="BGTZ" || rob[k].inst=="BLEZ" || rob[k].inst=="BLTZ")
				fout<<" R"<<rob[k].rs<<", #"<<rob[k].im<<"]\n";
			else if(rob[k].inst=="LW" || rob[k].inst=="SW")
				fout<<" R"<<rob[k].rt<<", "<<rob[k].im<<"(R"<<rob[k].rs<<")]\n";
			else if(rob[k].inst=="J")
				fout<<" #"<<rob[k].im<<"]\n";
			else fout<<"]\n";
		}
	}
	fout<<"BTB:\n";
	for(k=0;k<btb_ctr;k++)
	{
		if(btb[k].predictor!=3 && btb[k].predictor==2)
			fout<<"[Entry "<<k+1<<"]:<"<<btb[k].address<<","<<btb[k].destination<<","<<"Not Set>\n";
		else if(btb[k].predictor<2)
			fout<<"[Entry "<<k+1<<"]:<"<<btb[k].address<<","<<btb[k].destination<<","<<btb[k].predictor<<">\n";
	}
	fout<<"Registers:";
	for(k=0;k<32;k++)
	{
		if(k%8==0)
		{
			if(k<10)
				fout<<"\nR0"<<k<<":";
			else fout<<"\nR"<<k<<":";
		}
		if(reg[k]>99)
			fout<<"  "<<reg[k];
		else if(reg[k]>9 && reg[k]<100)
			fout<<"   "<<reg[k];
		else fout<<"    "<<reg[k];
	}
	fout<<"\nData Segment:\n700:";
	for(k=0;k<add_space_ctr;k++)
	{
		if(add_space[k]>99)
			fout<<"  "<<add_space[k];
		else if(add_space[k]>9 && add_space[k]<100)
			fout<<"   "<<add_space[k];
		else fout<<"    "<<add_space[k];	
	}
	fout<<"\n";}
	rob_ctr++;
	rs_ctr++;
	if(iq_ctr2<=iq_ctr)
		iq_ctr2++;
	cycle++;
	for(i=start;i<rs_ctr;i++)
	{done1=0;int temp_1=i;
		if(rs1[i].cycle<=cycle && rs1[i].done==0)
		{
			if((rob[i].inst=="ADD" || rob[i].inst=="ADDU")  && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rd]=reg[rs1[i].rt]+reg[rs1[i].rs];
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					reg_inst[rs1[i].rd]==1;
					reg_inst[rs1[i].rt]==0;
					reg_inst[rs1[i].rs]==0;
					rs1[i].done=1;
					start++;
					done1=1;
				}
			}
			else if(rob[i].inst=="AND"  && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rd]=reg[rs1[i].rt] & reg[rs1[i].rs];
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					reg_inst[rs1[i].rd]==0;
					reg_inst[rs1[i].rt]==0;
					reg_inst[rs1[i].rs]==0;
					rs1[i].done=1;
					start++;done1=1;
				}
			}
			else if(rob[i].inst=="OR" && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rd]=reg[rs1[i].rt] | reg[rs1[i].rs];
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;
					start++;done1=1;
				}
			}
			else if(rob[i].inst=="NOR" && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rd]=~(reg[rs1[i].rt] | reg[rs1[i].rs]);
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					start++;
					rs1[i].done=1;done1=1;
				}
			}		
			else if(rob[i].inst=="XOR" && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rd]=reg[rs1[i].rt] ^ reg[rs1[i].rs];
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if(rob[i].inst=="SLT" || rob[i].inst=="SLTU" && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					if(reg[rs1[i].rt] < reg[rs1[i].rs])
						reg[rs1[i].rd]=1;
					else reg[rs1[i].rd]=0;
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if(rob[i].inst=="SUB" || rob[i].inst=="SUBU" && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rd]=reg[rs1[i].rt] - reg[rs1[i].rs];
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if((rob[i].inst=="SRA" || rob[i].inst=="SRL") && done1==0)
			{
				if(reg_inst[rs1[i].rd]==0 && reg_inst[rs1[i].rt]==0)
				{
					reg[rs1[i].rd]=reg[rs1[i].rt] << rs1[i].im;
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if((rob[i].inst=="ADDI" || rob[i].inst=="ADDIU") && done1==0)
			{
				if(reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rt]=reg[rs1[i].rs] + rs1[i].im;
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if(rob[i].inst=="BEQ" && done1==0)
			{
				if(reg_inst[rs1[i].rs]==0 && reg_inst[rs1[i].rt]==0)
				{
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
					if(reg[rs1[i].rt]==reg[rs1[i].rs])
					{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=1;
						}
						iq_ctr2=rs1[i].iq+(rs1[i].im/4)+1;
						//rob_ctr=rob_ctr-(rs_ctr-i);
						i++;
						while(i<=rs_ctr)
						{
							rs1[i].print_valid=0;
							rs1[i].done=1;
							rob[i].cycle=1000;
							i++;
						}
						//rs_ctr=rob_ctr;
					}
					else{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=0;
						}
					}
				}
			}
			else if(rob[i].inst=="BNE" && done1==0)
			{
				if(reg_inst[rs1[i].rs]==0 && reg_inst[rs1[i].rt]==0)
				{
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
					if(reg[rs1[i].rt]!=reg[rs1[i].rs])
					{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=1;
						}
						iq_ctr2=rs1[i].iq+(rs1[i].im/4)+1;
						rob_ctr=rob_ctr-(rs_ctr-i);
						i++;
						while(i<=rs_ctr)
						{
							rs1[i].print_valid=0;
							rob[i].cycle=1000;
							i++;
						}
						rs_ctr=rob_ctr;
					}
					else{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=0;
						}	
					}
				}
			}
			else if(rob[i].inst=="SLTI" && done1==0)
			{
				if(reg_inst[rs1[i].rs]==0 && reg_inst[rs1[i].rt]==0)
				{
					if(reg[rs1[i].rs]<reg[rs1[i].im])
						rs1[i].rt=1;
					else rs1[i].rs=0;
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if(rob[i].inst=="BGEZ" && done1==0)
			{
				if(reg_inst[rs1[i].rs]==0)
				{
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
					if(reg[rs1[i].rs]>=0)
					{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=1;
						}
						iq_ctr2=rs1[i].iq+(rs1[i].im/4)+1;
						rob_ctr=rob_ctr-(rs_ctr-i);
						i++;
						while(i<=rs_ctr)
						{
							rs1[i].print_valid=0;
							rob[i].cycle=1000;
							i++;
						}
						rs_ctr=rob_ctr;
					}
					else{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=0;
						}	
					}
				}
			}
			else if(rob[i].inst=="BGTZ" && done1==0)
			{
				if(reg_inst[rs1[i].rs]==0)
				{
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
					if(reg[rs1[i].rs]>0)
					{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=1;
						}
						iq_ctr2=rs1[i].iq+(rs1[i].im/4)+1;
						rob_ctr=rob_ctr-(rs_ctr-i);
						i++;
						while(i<=rs_ctr)
						{
							rs1[i].print_valid=0;
							rob[i].cycle=1000;
							i++;
						}
						rs_ctr=rob_ctr;
					}
					else{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=0;
						}	
					}
				}
			}
			else if(rob[i].inst=="BLEZ" && done1==0)
			{
				if(reg_inst[rs1[i].rs]==0)
				{
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
					if(reg[rs1[i].rs]<=0)
					{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=1;
						}
						iq_ctr2=rs1[i].iq+(rs1[i].im/4)+1;
						rob_ctr=rob_ctr-(rs_ctr-i);
						i++;
						while(i<=rs_ctr)
						{
							rs1[i].print_valid=0;
							rob[i].cycle=1000;
							i++;
						}
						rs_ctr=rob_ctr;
					}
					else{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=0;
						}	
					}
				}
			}
			else if(rob[i].inst=="BLTZ" && done1==0)
			{
				if(reg_inst[rs1[i].rs]==0)
				{
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
					if(reg[rs1[i].rs]<0)
					{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=1;
						}
						iq_ctr2=rs1[i].iq+(rs1[i].im/4)+1;
						rob_ctr=rob_ctr-(rs_ctr-i);
						i++;
						while(i<=rs_ctr)
						{
							rs1[i].print_valid=0;
							rob[i].cycle=1000;
							i++;
						}
						rs_ctr=rob_ctr;
					}
					else{
						for(k=0;k<btb_ctr;k++)
						{
							if(btb[k].iq==rs1[i].iq)
								btb[k].predictor=0;
						}	
					}
				}
			}
			else if(rob[i].inst=="J" && done1==0)
			{
				pc=pc+rs1[i].im;
				iq_ctr2=((rs1[i].im-584)/4)-1;
				rs1[i].print_valid=0;
				rob[i].cycle=999;
				for(k=0;k<btb_ctr;k++)
				{
					if(btb[k].iq==rs1[i].iq)
						btb[k].predictor=1;
				}
				rob_ctr=rob_ctr-(rs_ctr-i);
				i++;
				while(i<=rs_ctr)
				{
					rs1[i].print_valid=0;
					rob[i].cycle=1000;
					rs1[i].done=1;
					i++;
				}
				rs_ctr=rob_ctr;
				rs1[i].done=1;start++;done1=1;
			}
			else if(rob[i].inst=="LW" && done1==0)
			{
				if(reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					reg[rs1[i].rt]=add_space[(reg[rs1[i].rs]+rs1[i].im-700)/4];
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if(rob[i].inst=="SW" && done1==0)
			{
				if(reg_inst[rs1[i].rt]==0 && reg_inst[rs1[i].rs]==0)
				{
					add_space[(reg[rs1[i].rs]+rs1[i].im-700)/4]=reg[rs1[i].rt];
					rs1[i].print_valid=0;
					rob[i].cycle=999;
					rs1[i].done=1;start++;done1=1;
				}
			}
			else if(rob[i].inst=="NOP" && done1==0)
			{
				rob[i].cycle=999;
				rs1[i].print_valid=0;
				rs1[i].done=1;start++;done1=1;
			}
			else if(rob[i].inst=="BREAK"  && done1==0)
			{
				done=1;
				rob[i].cycle=999;
				rs1[i].print_valid=0;
				rs1[i].done=1;done1=1;
			}
		}
		if(done1==1)
		{
			temp_1=temp_1-4;
			while(temp_1>=0)
			{
				rs1[temp_1].print_valid=0;
				rs1[temp_1].done=1;
				rob[temp_1].cycle=1000;
				temp_1--;
			}
		}
   	}//cycle++;
   }
	
	fout<<"Final Cycle <"<<cycle-1<<">:\nIQ:\nRS:\nROB:\n";
	fout<<"BTB:\n";
	for(k=0;k<btb_ctr;k++)
	{
		if(btb[k].predictor!=3 && btb[k].predictor==2)
			fout<<"[Entry "<<k+1<<"]:<"<<btb[k].address<<","<<btb[k].destination<<","<<"Not Set>\n";
		else if(btb[k].predictor<2)
			fout<<"[Entry "<<k+1<<"]:<"<<btb[k].address<<","<<btb[k].destination<<","<<btb[k].predictor<<">\n";
	}
	fout<<"Registers:";
	for(k=0;k<32;k++)
	{
		if(k%8==0)
		{
			if(k<10)
				fout<<"\nR0"<<k<<":";
			else fout<<"\nR"<<k<<":";
		}
		if(reg[k]>99)
			fout<<"  "<<reg[k];
		else if(reg[k]>9 && reg[k]<100)
			fout<<"   "<<reg[k];
		else fout<<"    "<<reg[k];
	}
	fout<<"\nData Segment:\n700:";
	for(k=0;k<add_space_ctr;k++)
	{
		if(add_space[k]>99)
			fout<<"  "<<add_space[k];
		else if(add_space[k]>9 && add_space[k]<100)
			fout<<"   "<<add_space[k];
		else fout<<"    "<<add_space[k];	
	}	
    fout.close();
    return (0);
}
