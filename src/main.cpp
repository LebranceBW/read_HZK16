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
	char statement[] = {(char)0xC7,(char)0xE0,(char)0xBB,(char)0xA8,'L','e','b','r','a','n','c','e',0x00};
//	wchar_t statement[] = L"Lebrance";
	auto a = statement_to_lattice(statement);
	draw_lattice(a);
	delete[] a->data;
	a->data=NULL;
	delete a;
	return 0;
}


