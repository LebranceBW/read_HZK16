/*
 * main.cpp
 *
 *  Created on: 2018年3月2日
 *      Author: lebrance
 */

#include <iostream>
#include <fstream>
#include "word2lattice.h"
using std::fstream;
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
	auto a=word2lattice(statment, fp);
	fp.close();
	return 0;
}


