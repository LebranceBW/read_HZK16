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
#include <fstream>
#include <cstdint>
#include "ascii_fonts.h"
#include <memory>

using std::shared_ptr; // @suppress("Symbol is not resolved")
using Font::F8X16;
using std::string;
using std::fstream;
using uint8_t = unsigned char;

typedef struct
{
	shared_ptr<uint8_t> data;//列式点阵数据 // @suppress("Type cannot be resolved")
	unsigned int rows;//多少行
	unsigned int columns;//多少列
}lattice_struct;
extern lattice_struct word2lattice(char* word, fstream& fp);
extern string get_data(int row, int column, lattice_struct& ls);
#endif /* SRC_WORD2LATTICE_H_ */
