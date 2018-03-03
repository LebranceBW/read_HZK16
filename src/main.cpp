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
	char statement[] = {(char)0xCF, (char)0xD6,(char)0xD4,(char)0xDA,(char)0xCA,(char)0xC7,(char)'2',(char)'2',(char)0x00};
//	char statement[] = {(char)0xD5, (char)0xFD, (char)0x00};
//	char statment[] = {'2','2',(char)0x00};
//	char statement[] = "helloworld";
	auto a=word2lattice(statement, fp);
	for(unsigned i=0;i<a.rows;i++)
	{
		for(unsigned j=0;j<a.columns;j++)
			std::cout<<get_pixel(i, j, a);
		std::cout<<std::endl;
	}
	fp.close();
	delete[] a.data;
	return 0;
}


