/*
 * word2lattice.h
 *
 *  Created on: 2018年3月3日
 *      Author: lebrance
 */

#ifndef SRC_WORD2LATTICE_H_
#define SRC_WORD2LATTICE_H_

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>
#include "ascii_fonts.h"
#include <memory>

//using std::shared_ptr; //
using Font::F8X16;
using std::string;
using std::fstream;
using std::memmove;
using uint8_t = unsigned char;

typedef struct
{
	uint8_t* data;//列式点阵数据 //
	unsigned int rows;//多少行
	unsigned int columns;//多少列
}lattice_struct;
extern lattice_struct word2lattice(char* word, fstream& fp);
extern string get_pixel(int row, int column, const lattice_struct& ls);
#endif /* SRC_WORD2LATTICE_H_ */
