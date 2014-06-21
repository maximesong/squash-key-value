/*
 * Data.h
 *
 *  Created on: May 15, 2014
 *      Author: whj-linux
 */

#ifndef DATA_H_
#define DATA_H_

#include <string>

enum COMPRESSED_STATE{UNCOMPRESSED_IN_MEM,COMPRESSED_IN_MEM};

class Data
{
public:
	Data()
	{
		saved_data = "";
		compressed_state = COMPRESSED_STATE::UNCOMPRESSED_IN_MEM;
		access_flag = 0;
		last_access_time = 0;
	}

	std::string getData()//return size of data, -1 if null.
	{
		return (this->saved_data);
	}
	int setData(std::string dataContent)//return size of data, -1 if null.
	{
		this->saved_data = dataContent;
		return 0;
	}
	int getSize()//return size of data, -1 if null.
	{
		return (this->saved_data.length());
	}

	COMPRESSED_STATE compressed_state;/*UNCOMPRESSED_IN_MEM,COMPRESSED_IN_MEM*/
	int access_flag;/*Record if the data has been accessed recently either by get or put*/
	unsigned int last_access_time;/*Virtual time with auto-increment,record the last access time by get and put*/
protected:
	std::string saved_data;
};

#endif /* DATA_H_ */
