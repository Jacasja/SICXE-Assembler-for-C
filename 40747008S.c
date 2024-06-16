#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<inttypes.h>
int checkre(char a){
	int b;
	if(a=='A'){
		b=0;
	}
	else if(a=='X'){
		b=1;
	}
	else if(a=='L'){
		b=2;
	}
	else if(a=='S'){
		b=4;
	}
	else if(a=='T'){
		b=5;
	}
	return b;
}
int tcom(int a){
	a=-a;
	int br[12],n=0;
	int tdi=a;
	for(int i=11;i>-1;i--){
		br[i]=tdi%2;
		tdi=tdi/2;
	}
	for(int i=0;i<12;i++){
		if(br[i]==0){
			br[i]=1;
		}
		else{
			br[i]=0;
		}
	}
	for(int i=11;i>-1;i--){
		n=n+br[i]*pow(2,11-i);
	}
	n++;
	return n;
}
int main(){
	int adr,line=1,tst;
	char name[10],label[10],mnemonic[10],operand[10],inf[50],gli[200],tgli[3][200];
	char opname[59][10]={"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF",
						"DIVR","FIX","FLOAT","HIO","J","JEQ","JGT","JLT","JSUB", "LDA",
						"LDB","LDCH","LDF","LDL","LDS","LDT","LDX", "LPS", "MUL", "MULF",
						"MULR", "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR","SIO", "SSK",
						"STA", "STB", "STCH", "STF", "STI", "STL", "STS", "STSW", "STT", "STX",
						"SUB", "SUBF","SUBR", "SVC", "TD", "TIO", "TIX", "TIXR", "WD"};
	int opcode[59]={ 0x18, 0x58, 0x90, 0x40, 0xB4, 0x28, 0x88, 0xA0, 0x24, 0x64,
					 0x9C, 0xC4, 0xC0, 0xF4, 0x3C, 0x30, 0x34, 0x38, 0x48, 0x00,
					 0x68, 0x50, 0x70, 0x08, 0x6C, 0x74, 0x04, 0xD0, 0x20, 0x60,
					 0x98, 0xC8, 0x44, 0xD8, 0xAC, 0x4C, 0xA4, 0xA8, 0xF0, 0xEC,
					 0x0C, 0x78, 0x54, 0x80, 0xD4, 0x14, 0x7C, 0xE8, 0x84, 0x10,
					 0x1C, 0x5C, 0x94, 0xB0, 0xE0, 0xF8, 0x2C, 0xB8, 0xDC};
	FILE *in = NULL, *in_1 = NULL , *in_2 = NULL , *syt = NULL;
	int adrr[200];

	//generate LOC_file

	printf("please enter the file name(with file extension):");
	scanf("%s",inf);
	in=fopen(inf,"r+");
	in_1=fopen("loc.txt","w");
	fgets(gli,200,in);
	char *d=" \t\n";
	char *temp11;
	temp11 = strtok(gli,d);
	tst=0;
	while (temp11 != NULL) {
		strcpy(tgli[tst],temp11);
        temp11 = strtok(NULL, d);	
		tst++;
    }
	strcpy(name,tgli[0]);
	strcpy(mnemonic,tgli[1]);
	adr=atoi(tgli[2]);
	fprintf(in_1,"%d\t%.4X\t%s\t%s\t%d\n",line,adr,name,mnemonic,adr);
	adrr[0]=adr;
	while(strcmp(mnemonic,"END")!=0){
		//reset string
		tst=0;
		for(int i=0;i<3;i++){
			strcpy(tgli[i],"");
		}
		strcpy(label,"");
		strcpy(operand,"");
		strcpy(mnemonic,"");
		//get line&split
		fgets(gli,200,in);
		temp11 = strtok(gli,d);
		while (temp11 != NULL){
			strcpy(tgli[tst],temp11);
        	temp11 = strtok(NULL, d);	
			tst++;
    	}
		line++;
		if(strlen(tgli[2])==0){
			if(strlen(tgli[1])==0){
				strcpy(tgli[1],tgli[0]);
				strcpy(tgli[0],"--");
				strcpy(tgli[2],"--");
			}
			else{
				strcpy(tgli[2],tgli[1]);
				strcpy(tgli[1],tgli[0]);
				strcpy(tgli[0],"--");
			}
		}
		strcpy(label,tgli[0]);
		strcpy(mnemonic,tgli[1]);
		strcpy(operand,tgli[2]);
		fprintf(in_1,"%d\t%.4X\t%s\t%s\t%s\n",line,adr,label,mnemonic,operand);
		if(strcmp(mnemonic,"RESW")==0){
			adr+=(atoi(operand)*3);
		}
		else if(strcmp(mnemonic,"RESB")==0){
			adr+=(atoi(operand));
		}
		else if(mnemonic[0]=='+'){
			adr+=4;
		}
		else if(strcmp(mnemonic,"BASE")==0){}
		else if((strcmp(mnemonic,"CLEAR")==0)||(strcmp(mnemonic,"COMPR")==0)||(strcmp(mnemonic,"TIXR")==0)){
			adr+=2;
		}
		else if(strcmp(mnemonic,"BYTE")==0){
            int leng=0;
            leng=strlen(operand);
            leng-=3;
            if(operand[0]=='C'){
				adr+=leng;
			}
            else{
                leng=(leng/2);
                adr+=leng;
            }
		}
		else{
			adr+=3;
		}
		adrr[line-2]=adr;
	}
	fclose(in);
	fclose(in_1);
	strcpy(label,"");
	strcpy(mnemonic,"");
	strcpy(operand,"");
	printf("loc file done.\n");
	
	//generate symbol table

	in_1=fopen("loc.txt","r+");
	syt=fopen("symtab.txt","w");
	int sym_num=0;
	while(strcmp(mnemonic,"END")!=0){
		strcpy(label,"");
		strcpy(mnemonic,"");
		strcpy(operand,"");
		fscanf(in_1,"%d%X%s%s%s",&line,&adr,&label,&mnemonic,&operand);
        if(strcmp(label,"--")!=0){
            fprintf(syt,"%s\t%.4X\n",label,adr);
            sym_num++;
        }
	}
	fclose(in_1);
	fclose(syt);
	printf("symbol table done.\n");

	//data load from symtab

    syt=fopen("symtab.txt","r");
	char sytla[sym_num][10];
	int sytadr[sym_num];
	for(int i=0;i<sym_num;i++){
		fscanf(syt,"%s%X",&sytla[i],&sytadr[i]);
	}
	fclose(syt);

	//search BA
	int BA=0;
	in_1=fopen("loc.txt","r");
	fscanf(in_1,"%d%X%s%s%s",&line,&adr,&label,&mnemonic,&operand);
	while(strcmp(mnemonic,"END")!=0){
		fscanf(in_1,"%d%X%s%s%s",&line,&adr,&label,&mnemonic,&operand);
		if(strcmp(mnemonic,"BASE")==0){
			for(int i=0;i<sym_num;i++){
				if(strcmp(operand,sytla[i])==0){
					BA=sytadr[i];
				}
			}
		}
	}
	fclose(in_1);

	//generate opcode	

	in_1=fopen("loc.txt","r");
	in_2=fopen("object_code.txt","w");
	fscanf(in_1,"%d%X%s%s%s",&line,&adr,&label,&mnemonic,&operand);
	fprintf(in_2,"%d\t%.4X\t%s\t%s\t%s\t\t--\n",line,adr,label,mnemonic,operand);
	while(strcmp(mnemonic,"END")!=0){
		fscanf(in_1,"%d%X%s%s%s",&line,&adr,&label,&mnemonic,&operand);
		fprintf(in_2,"%d\t%.4X\t%s\t%s\t%s\t",line,adr,label,mnemonic,operand);
		if(strcmp(mnemonic,"BYTE")==0){
			int leng=0;
            leng=strlen(operand);
            leng-=3;
			char tby[leng];
			fprintf(in_2,"\t");
			if(operand[0]=='C'){
				for(int i=0;i<leng;i++){
					fprintf(in_2,"%X",operand[2+i]);
				}
				fprintf(in_2,"\n");			
			}
			else{
				for(int i=0;i<leng;i++){
					fprintf(in_2,"%c",operand[2+i]);
				}
				fprintf(in_2,"\n");
			}
		}
		else if((strcmp(mnemonic,"END")==0)||(strcmp(mnemonic,"RESB")==0)||(strcmp(mnemonic,"RESW")==0)||(strcmp(mnemonic,"BASE")==0)){
			fprintf(in_2,"\t--\n");
		}
		else if(strcmp(mnemonic,"CLEAR")==0){
			fprintf(in_2,"\t%X%d0\n",opcode[4],checkre(operand[0]));
		}
		else if(strcmp(mnemonic,"TIXR")==0){
			fprintf(in_2,"\t%X%d0\n",opcode[57],checkre(operand[0]));
		}
		else if(strcmp(mnemonic,"COMPR")==0){
			fprintf(in_2,"\t%X%d%d\n",opcode[7],checkre(operand[0]),checkre(operand[2]));
		}
		else if(strcmp(mnemonic,"RSUB")==0){
			int opc0=0,opc1=0;
			opc0=opcode[35]/16;
			opc1=opcode[35]%16+3;
			fprintf(in_2,"\t%X%X0000\n",opc0,opc1);
		}
		else if(mnemonic[0]=='+'){
			int x,b,p,e=1,disp=0x0;
			int opc0=0,opc1=0;
			char tmn[4];
			for(int j=0;j<5;j++){
				tmn[j]=mnemonic[j+1];
			}
			for(int j=0;j<59;j++){
				if(strcmp(tmn,opname[j])==0){
					opc0=opcode[j]/16;
					opc1=opcode[j]%16;
				}
			}
			if(operand[0]=='#'){
				opc1+=1;
				x=0;
				b=0;
				p=0;
				int sp=x*8+b*4+p*2+e;
				char lc[5];
				for(int j=1;j<strlen(operand)+1;j++){
					lc[j-1]=operand[j];
				}
				disp=atoi(lc);
				fprintf(in_2,"\t%X%X%X%.5X\n",opc0,opc1,sp,disp);
			}
			else{
				opc1+=3;
				x=0;
				b=0;
				p=0;
				int sp=x*8+b*4+p*2+e;
				int lc=0x0;
				for(int j=0;j<sym_num;j++){
					if(strcmp(operand,sytla[j])==0){
						lc=sytadr[j];
					}
				}
				fprintf(in_2,"\t%X%X%X%.5X\n",opc0,opc1,sp,lc);
			}
		}
		else{
			int x,b,p,e=0,disp=0;
			int opc0=0,opc1=0;
			for(int i=0;i<59;i++){
				if(strcmp(mnemonic,opname[i])==0){
					opc0=opcode[i]/16;
					opc1=opcode[i]%16;
				}
			}
			if(operand[0]=='#'){
				int lc=strlen(operand);
				char top[20];
				for(int i=0;i<lc-1;i++){
					top[i]=operand[i+1];
				}
				top[lc-1]='\0';
				int top1=0x0,tn=0;
				for(int j=0;j<sym_num;j++){
					if(strcmp(top,sytla[j])==0){
						top1=sytadr[j];
						tn=1;
					}
				}
				opc1++;
				x=0;
				if(tn==1){
					disp=top1-adrr[line-2];
					if((disp>-2049)&&(disp<2048)){
						b=0;
						p=1;
						int sp=x*8+b*4+p*2+e;
						fprintf(in_2,"\t%X%X%X%.3X\n",opc0,opc1,sp,disp);
					}
					else{
						b=1;
						p=0;
						disp=top1-BA;
						int sp=x*8+b*4+p*2+e;
						fprintf(in_2,"%X%X%X%.3X\n",opc0,opc1,disp);
					}
				}
				else{
					fprintf(in_2,"\t%X%X%.4X\n",opc0,opc1,atoi(top));
				}
			}
			else if(operand[0]=='@'){
				opc1+=2;
				x=0;
				int lc=strlen(operand);
				char top[20];
				for(int i=0;i<lc-1;i++){
					top[i]=operand[i+1];
				}
				top[lc-1]='\0';
				int top1=0x0,tn=0;
				for(int j=0;j<sym_num;j++){
					if(strcmp(top,sytla[j])==0){
						top1=sytadr[j];
					}
				}
				disp=top1-adrr[line-2];
				if((disp>-2049)&&(disp<2048)){
					b=0;
					p=1;
					int sp=x*8+b*4+p*2+e;
					if(disp<0){
						disp=tcom(disp);
					}
					fprintf(in_2,"\t%X%X%X%.3X\n",opc0,opc1,sp,disp);
				}
				else{
					b=1;
					p=0;
					disp=top1-BA;
					int sp=x*8+b*4+p*2+e;
					fprintf(in_2,"\t%X%X%X%.3X\n",opc0,opc1,sp,disp);
				}
			}
			else if((strcmp(mnemonic,"STCH")==0)||(strcmp(mnemonic,"LDCH")==0)){
				opc1+=3;
				x=1;
				int i=0,top1=0x0;
				char top[20];
				int lo=strlen(operand);
				for(int i=0;i<lo-2;i++){
					top[i]=operand[i];
				}
				top[lo-2]='\0';
				for(int j=0;j<sym_num;j++){
					if(strcmp(top,sytla[j])==0){
						top1=sytadr[j];
					}
				}
				disp=top1-adrr[line-2];
				if((disp>-2049)&&(disp<2048)){
					b=0;
					p=1;
					int sp=x*8+b*4+p*2+e;
					if(disp<0){
						disp=tcom(disp);
					}
					fprintf(in_2,"%X%X%X%.3X\n",opc0,opc1,sp,disp);
				}
				else{
					b=1;
					p=0;
					disp=top1-BA;
					int sp=x*8+b*4+p*2+e;
					fprintf(in_2,"%X%X%X%.3X\n",opc0,opc1,sp,disp);
				}
			}
			else{
				opc1+=3;
				x=0;
				int top1=0;
				for(int j=0;j<sym_num;j++){
					if(strcmp(operand,sytla[j])==0){
						top1=sytadr[j];
					}
				}
				int disp=top1-adrr[line-2];
				if((disp>-2049)&&(disp<2048)){
					b=0;
					p=1;
					int sp=x*8+b*4+p*2+e;
					if(disp<0){
						disp=tcom(disp);
					}
					fprintf(in_2,"\t%X%X%X%.3X\n",opc0,opc1,sp,disp);
				}
				else{
					b=1;
					p=0;
					disp=top1-BA;
					int sp=x*8+b*4+p*2+e;
					fprintf(in_2,"\t%X%X%X%.3X\n",opc0,opc1,sp,disp);
				}
			}
		}
	}
	printf("object code done.");
    return 0;
}