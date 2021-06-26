/* This module contains the necessary functions related 
 * to string */

int strLen(char s[]){
	int ln=0;
	while(s[ln])ln++;
	return ln;
}

void strClear(char s[]){
	int ln=strLen(s);
	for(int i=ln-1;i>=0;i--)s[i]='\0';
}

void strAssign(char to[],char from[]){
	int ln1=strLen(from),ln2=strLen(to);
	strClear(to);

	for(int i=0;i<ln1;i++)to[i]=from[i];
	for(int i=ln1;i<ln2;i++)to[i]='\0';
	to[ln1]='\0';
}

void strConcat(char s1[],char s2[]){
	int ln1=strLen(s1),ln2=strLen(s2);

	for(int i=0;i<ln2;i++)s1[ln1++]=s2[i];
	s1[ln1]='\0';
}


void strReverse(char s[]){
	int ln=strLen(s);
	char s2[ln+1];

	for(int i=0;i<ln;i++)s2[i]=s[ln-1-i];
	s2[ln]='\0';
	strAssign(s,s2);
}

void toString(int n,char s[]){
	int ln=0;
	while(n){
		s[ln++]='0'+n%10;
		n /= 10;
	}
	s[ln]='\0';
	strReverse(s);
}

int strCompare(char s1[],char s2[]){
	int ln1= strLen(s1),ln2= strLen(s2);

	for(int i=0;i<ln1;i++){
		if(i==ln2)return 1;
		if(s1[i]>s2[i])return 1;
		if(s2[i]>s1[i])return -1;
	}
	if(ln1==ln2)return 0;
	return 1;
}



void strSwap(char s1[],char s2[]){
	int ln1=strLen(s1),ln2=strLen(s2);
	char tmp[ln1];
	strAssign(tmp,s1);
	strAssign(s1,s2);
	strAssign(s2,tmp);
}
