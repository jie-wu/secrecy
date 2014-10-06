/*
 *  main.c 
 *  concertino 
 *
 *  Created by adow on 2014-08-05 20:03:20 
 *  Copyright (C) 20014 adow, All rights reserved.
 *
 *  gcc lib/polarssl/base64.c lib/polarssl/aes.c lib/polarssl/sha1.c secrecy.c main.c -o secrecy
 *
 *  加密文件
 *  secrecy encrypt -i README.md -o README.secrecy.md -k key -h <hints>
 *  解密文件 
 *  secrecy decrypt -i README.secrecy.md -o README.decrypt.md -k key
 *
 *  -i 输入文件
 *  -o 输出文件，如果不指定 -o,那就覆盖输入的文件
 *  -k 密钥
 *  -h 密钥提示
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

extern int secrecy_encrypt_file(const char *input_filename,
		const char *output_filename,
		const unsigned char *key,
		const char *hints
		);
extern int secrecy_decrypt_file(const char *input_filename,
		const char *output_filename,
		const unsigned char *key,
		int check_sha
		);
extern void secrecy_self_test();

///输出帮助信息
void _output_help(){
	const char *help="SECRECY COMMANDS:\n"
		"* secrecy encrypt -i <inputfile> -o <outputfile> -k <secret> -h <hints>\n"
		"\t Encrypt a File,-h could be ignored, -k should be less than 256 in length;\n"
		"* secrecy decrypt -i <inputfile> -o <outputfile> -k <secret>\n"
		"\t Decrypt a file, -k should be less than 256 in length;\n"
		"* secrecy test\n"
		"\t Test encrypt and decrypt lipsum.md;\n"
		"* secrecy help\n"
		"\t Help;";
	printf("%s\n",help);
}
///处理参数，执行加密或者解密
int _execute_cmd(int arg_c,char *arg_v[]){
	if(arg_c<=1){
		printf("need a command\n");
		_output_help();
		return -1;	
	}
	char *cmd=arg_v[1];
	printf("cmd:%s\n",cmd);
	///第一个参数是encrypt/decrpt,所以处理的参数列表要往后面移
	arg_v+=1;
	arg_c-=1;
	char input_filename[PATH_MAX]={'\0'};
	char output_filename[PATH_MAX]={'\0'};
	char hints[PATH_MAX]={'\0'};
	unsigned char key[256]={'\0'};
	char format[]="k:i:o::h::";
	int ch;
	while((ch=getopt(arg_c,arg_v,format))!=-1){
		//printf("%c\n",ch);
		switch(ch){
			case 'i':
				strcpy(input_filename,optarg);
				break;
			case 'o':
				strcpy(output_filename,optarg);
				break;
			case 'k':
				strcpy((char *)key,optarg);
				break;
			case 'h':
				strcpy(hints,optarg);
				break;
		}
	}
	if (!strcmp(cmd,"help")){
		_output_help();
		return 0;
	}
	else if (!strcmp(cmd,"test")){
		secrecy_self_test();
		return 0;
	}
	else {
		if (!strlen((char *)key)){
			printf("key required\n");
			_output_help();
			return -1;
		}
		if (!strlen(input_filename)){
			printf("no input filename\n");
			_output_help();
			return -1;
		}
		if (!strlen(output_filename)){
			strcpy(output_filename,input_filename);	
		}
		printf("key:%s\n",key);
		printf("input_filename:%s\n",input_filename);
		printf("output_filename:%s\n",output_filename);
		printf("hints:%s\n",hints);
		if (!strcmp(cmd,"encrypt")){
			if (secrecy_encrypt_file(input_filename,output_filename,key,hints)){
				printf("encrypt error\n");
				return -1;
			}
			return 0;
		}
		else if (!strcmp(cmd,"decrypt")){
			if (secrecy_decrypt_file(input_filename,output_filename,key,1)){
				printf("decrypt error\n");
				return -1;
			}
			return 0;	
		}
		else{
			printf("command unknown");
			_output_help();
			return -1;
		}

	}	
	return 0;
}

int main(int arg_c,char *arg_v[]){
	//secrecy_self_test();
	_execute_cmd(arg_c,arg_v);
	return 0;
}
