/*
 * main.cpp
 *
 *  Created on: 2018年3月2日
 *      Author: lebrance
 */

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include "ascii_fonts.h"
using Font::F8X16;
using std::string;
using std::fstream;
using std::cout;

using uint8_t = unsigned char;
string tolattice(char code)
{
	string dots;
	for(size_t i=0;i<8;i++)
	{
		dots  += (code&0x80)?"+":"_";
		code = code<<1;
	}
	dots[8] = '\0';
	return dots;
}
void word2lattice(char* word, fstream& fp)
{
	char* ptr = word;
	while(*ptr)
	{
		if(uint8_t(*ptr) > 127)
		{
			//是汉字
			auto code_region = uint8_t(*ptr++);
			auto code_locate = uint8_t(*ptr++);
			unsigned int offset = (94*(code_region-0xa0-1)+(code_locate-0xa0-1))*32;
			char dots[32] = {};
			fp.seekg(offset);
			fp.read(dots, 32);
			for(size_t i=0;i<16;i++)
				std::cout<<tolattice(dots[2*i])<<tolattice(dots[2*i+1])<<std::endl;
		}
		else
		{
			//是ascii码
			const unsigned char* ascii_map = F8X16[uint8_t(*ptr++ - ' ')];
			for(size_t i=0;i<16;i++)
				cout<< tolattice(ascii_map[i])<<std::endl;
		}
	}
}

int main()
{

	fstream fp("./res/HZK16", fp.binary|fp.in);
	if(!fp.is_open())
	{
		fp.open("../res/HZK16", fp.binary|fp.in);
		if(!fp.is_open())
		{
			std::cout<<"fail to open HZK16"<<std::endl;
			return -1;
		}
	}
	char statment[] = {(char)0xCF, (char)0xD6,(char)0xD4,(char)0xDA,(char)0xCA,(char)0xC7,(char)'2',(char)'2',(char)0x00};
	word2lattice(statment, fp);
	fp.close();
	return 0;
}


