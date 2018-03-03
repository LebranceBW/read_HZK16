/*
 * word2lattice.cpp
 *
 *  Created on: 2018年3月3日
 *      Author: lebrance
 */

#include "word2lattice.h"

lattice_struct add_lattice_struct(const lattice_struct& ls1, const lattice_struct& ls2)
{
	//水平方向相加，如果垂直方向不一样大，则按照大的来
	/*计算需要多少空间，然后分配给他（记得释放）*/
	auto totalcolumns = ls1.columns + ls2.columns;
	auto totalrows = ls1.rows>ls2.rows?ls1.rows:ls2.rows;
	auto totallength_1 = ls1.rows * ls1.columns;
	auto totallength_2 = ls2.rows * ls2.columns;
	uint8_t* pt = new uint8_t[totalcolumns*totalrows/8]{0};
	shared_ptr<uint8_t> data(pt);
	/*内容拷贝*/
	for(size_t i=0;i<totalrows/8;i++)
	{
		for(size_t j1=0;j1<ls1.columns;j1++)
			if(i*ls1.columns+j1 < totallength_1)
			{
				*(pt++) = *ls1.data+i*ls1.columns+j1;
			}
		for(size_t j2=0;j2<ls2.columns;j2++)
			if(i*ls2.columns+j2 < totallength_2)
			{
				*(pt++) = *ls2.data+i*ls2.columns+j2;
			}
	}
	return lattice_struct{
		.data = data, .rows=totalrows, .columns=totalcolumns
	};

}

uint8_t* line2column(uint8_t* data)
{
	//输入八个逐行式数据转换为八个逐列式数据
	uint8_t* result = new uint8_t[8];
	uint8_t mask = 0x01;
	for(size_t i=0;i<8;i++)
	{
		//对八个输出循环
		for(size_t j=0;j<8;j++)
		{
		//对输入循环
			result[i] += data[j]&mask;
			result[i] <<= 1;
		}
		mask<<=1;
	}
	return result;
}
lattice_struct word2lattice(char* word, fstream& fp)
{
	char* ptr = word;
	lattice_struct ls{ // @suppress("Type cannot be resolved") // @suppress("Statement has no effect")
		.data = shared_ptr<uint8_t>(NULL),
		.rows = 0,
		.columns = 0
	};
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
			auto lpt = new uint8_t[32];
			shared_ptr<uint8_t> lp(lpt);

			// 逐行转换为逐列
			uint8_t selected_data[8];
			for(size_t base=0;base<32;base+=8)
			{
				for(size_t bias=0;bias<2;bias++)
				{
					for(size_t i=0;i<8;i++)
						selected_data[i] = (uint8_t)dots[i*2 + bias +base];
					auto result = line2column(selected_data);
					//将结果拷贝到结果数组里
					for(size_t i=0;i<8;i++)
						*(lpt+ i +bias*8+base) = result[i];
				}
			}
			ls = add_lattice_struct(ls, lattice_struct{.data=lp,.rows=16,.columns=16});

		}
		else
		{
			//是ascii码
			const unsigned char* ascii_map = F8X16[uint8_t(*ptr++ - ' ')];
			unsigned char* map = new uint8_t[16];
			for(int i =0;i<16;i++)
				map[i] = ascii_map[i];
			shared_ptr<uint8_t> lp(map);
			ls = add_lattice_struct(ls, lattice_struct{.data=lp,.rows=16,.columns=8});

		}
	}
	return ls;
}

string get_data(int row, int column, lattice_struct& ls)
{
	auto baserow = row/8;
	auto basecolumn = column;
	auto bias = row%8;
	int index = baserow * ls.rows + column;
	uint8_t byte= ls.data[index];
	uint8_t mask = 0x01;
	for(int i=0;i<bias;i++)
		mask<<=1;
	return byte&mask?"+":" ";
}
