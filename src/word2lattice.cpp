/*
 * word2lattice.cpp
 *
 *  Created on: 2018年3月3日
 *      Author: lebrance
 */

#include "word2lattice.h"

extern "C"
lattice_str_ptr statement_to_lattice(char* statement)
{
	std::cout<<"开始转换..."<<std::endl;
	fstream fp("./HZK16",fp.binary|fp.in);
	if(!fp.is_open())
	{
			std::cout<<"无法打开字库"<<std::endl;
			return NULL;
	}
	lattice_str_ptr result = new lattice_struct(word2lattice(statement, fp));
	return result;
}

extern "C"
void draw_lattice(const lattice_str_ptr ls)
{
	for(unsigned i=0;i<ls->rows;i++)
	{
		for(unsigned j=0;j<ls->columns;j++)
				std::cout<<get_pixel(i, j, *ls);
		std::cout<<std::endl;
	}
}
extern "C"
lattice_struct add_lattice_struct(lattice_struct& ls1, lattice_struct&& ls2)
{
	//水平方向相加，如果垂直方向不一样大，则按照大的来
	/*计算需要多少空间，然后分配给他（记得释放）*/
	auto totalcolumns = ls1.columns + ls2.columns;
	auto totalrows = ls1.rows>ls2.rows?ls1.rows:ls2.rows;
	uint8_t* pt = new uint8_t[totalcolumns*totalrows/8]{0};
//
//	auto ls1_ptr = ls1.data;
//	auto ls2_ptr = ls2.data;
	/*内容拷贝*/
	//逐行由ls1和ls2拼接起来
	for(size_t prefix=0;prefix<totalrows/8;prefix++)
	{
		memmove(pt+prefix * totalcolumns, ls1.data+prefix*ls1.columns, ls1.columns);
		memmove(pt+prefix * totalcolumns+ls1.columns, ls2.data+prefix*ls2.columns, ls2.columns);
	}
	delete[] ls1.data;ls1.data=NULL;
	delete[] ls2.data;ls2.data=NULL;
	return lattice_struct{
		.data = pt, .rows=totalrows, .columns=totalcolumns
	};

}

extern "C"
uint8_t* line2column(uint8_t* zone)
{
	//输入8*8个逐行式数据区域，然后转换为八个逐列式数据
	//输入是逆序的，输出也是逆序的！
	uint8_t* result = new uint8_t[8]{0};
	for(size_t i=0;i<8;i++)
		//八个数据
		for(size_t j=0;j<8;j++)
		{
			//八位
			result[i] >>= 1;
			result[i] += (zone[j]<<i)&0x80;
		}
	return result;
}
extern "C"
lattice_struct word2lattice(char* word, fstream& fp)
{
	auto ptr = word;
	lattice_struct ls{
		.data = NULL,
		.rows = 0,
		.columns = 0
	};
	while(*ptr)
	{
		if((uint8_t)*ptr > 127)
		{
			//是汉字
			auto code_region = uint8_t(*ptr++);
			auto code_locate = uint8_t(*ptr++);
			unsigned int offset = (94*(code_region-0xa0-1)+(code_locate-0xa0-1))*32;
			char lattice_byline[32] = {};
			fp.seekg(offset);
			fp.read(lattice_byline, 32);
			auto lpt = new uint8_t[32];
			//将其划分为四个区域I II III IV 每个区域都是8X8，然后将各个区域内容转换后拷贝
			for(int zone=0;zone!=4;zone++)
			{
				//数据准备
				auto prefix = (zone / 2)* 16;
				auto bias = zone % 2;
				uint8_t zone_data[8];
				for(int i=0;i<8;i++)
					zone_data[i] = lattice_byline[2*i + bias + prefix];
				auto result = line2column(zone_data);
				memmove(lpt + zone*8, result, 8);
				delete[] result;
			}
			ls = add_lattice_struct(ls, lattice_struct{.data=lpt,.rows=16,.columns=16});
		}
		else
		{
			//是ascii码
			const unsigned char* ascii_map = F8X16[(*ptr++) - ' '];
			unsigned char* map = new uint8_t[16];
			for(int i =0;i<16;i++)
				map[i] = ascii_map[i];
			ls = add_lattice_struct(ls, lattice_struct{.data=map,.rows=16,.columns=8});
		}
	}
	fp.close();
	return ls;
}

inline string get_pixel(int row, int column, const lattice_struct& ls)
{
	auto baserow = row/8;
	auto basecolumn = column;
	auto bias = row%8;
	int index = baserow * ls.columns + basecolumn;
	uint8_t byte= ls.data[index];
	uint8_t mask = 0x01;
	for(int i=0;i<bias;i++)
		mask<<=1;
	bool flag = byte&mask;
	return flag?"● ":"○ ";
}
