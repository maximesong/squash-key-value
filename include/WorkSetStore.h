#ifndef WORKSETSTORE_H_
#define WORKSETSTORE_H_

#include "store.h"
#include "Clock.h"
#include "Data.h"
#include "SpookyV2.h"
#include "lz4.h"

#include <map>
#include <string>
#include <fstream>
using namespace std;

#include <cstdlib>
#include <assert.h>
#include <string.h>
#include "stdio.h"

class WorkSetStore: public Store
{
public:
	WorkSetStore()
	{
		store = std::map<std::string,Data*>();
		uncompressed_entry_num = 0;
		store_clock = Clock();

		/*Configured parameter from configured file*/
		T = 1000;
		MAX_UNCOMPRESSED_ENTRY_NUM = 5000;
	}
	~ WorkSetStore()
	{
		std::map<std::string,Data*>::iterator iter = store.begin();
		while (iter != store.end())
		{
			delete iter->second;
			iter++;
		}
	}

	/*wrapper function*/
	int get(const char* key, int key_size, char *dest)
	{
		std::string key_string = std::string(key, key_size);
		std::string value_string = "";
		int result = this->get(key_string,value_string);
		strcpy(dest, value_string.c_str());
		return result;
	}

	/*wrapper function*/
	int put(const char* key, int key_size, const char *value, int size)
	{
		std::string key_string = std::string(key, key_size);
		assert(value[size] == '\0');
		std::string value_string = std::string(value);
		int result = this->put(key_string,value_string);
		return result;
	}

	/**
	* @return the size of the value, or -1 if the value it not available
	 */
	int get(string key, string& dataContent)
	{
		int uncompressed_size = -1;

		if (store.count(key))
		{
			store_clock.increaseTime();
			Data* p_data = store[key];
			uncompressed_size = 0;//uncompressed size
			if (p_data->compressed_state == COMPRESSED_STATE::UNCOMPRESSED_IN_MEM)//uncompressed in memory
			{
				dataContent = p_data->getData();
				uncompressed_size = dataContent.length();
				p_data->access_flag = 1;
				p_data->last_access_time = store_clock.getCurrentTime();
				goto finished;
			}
			else if (p_data->compressed_state == COMPRESSED_STATE::COMPRESSED_IN_MEM)//compressed in memory
			{
				/*get data and set access flag and last_access_time*/
				std::string compressed_dataContent = p_data->getData();
				dataContent = this->decompress(compressed_dataContent);
				uncompressed_size = dataContent.length();
				p_data->access_flag = 1;
				p_data->last_access_time = store_clock.getCurrentTime();

				/*Check if an uncompressed entry needed to be swapped out*/
				if (this->uncompressed_entry_num+1 > this->MAX_UNCOMPRESSED_ENTRY_NUM)//do not need to retrieve page if <=
				{
					/*meaning that retrieve_page process will encounter with entry compressed in MEM with access_flag == 1*/
					if (!this->retrieve_page())
					{
						this->MAX_UNCOMPRESSED_ENTRY_NUM++;//need to add a new uncompressed position if all_access_1 
					}
					else
					{
						this->uncompressed_entry_num--;//bug found
					}
					/*Set the access flag again.Since it has been erased in retrieve_page op*/
					p_data->access_flag = 1;
				}

				/*Make the entry visited uncompressed*/
				p_data->compressed_state = COMPRESSED_STATE::UNCOMPRESSED_IN_MEM;
				p_data->setData(dataContent);
				this->uncompressed_entry_num++;
		
				goto finished;
			}
		}
		else
			goto finished;
	finished:
		return uncompressed_size;
	}

	/**
	 * @return 0 if put is success, or -1 otherwise
	 */
	int put(string key, string dataContent)
	{
		store_clock.increaseTime();
		if (store.count(key))
		{
			Data* p_data = store[key];

			/*Set the access_flag*/
			p_data->access_flag = 1;
			p_data->last_access_time = store_clock.getCurrentTime();

			/*If it used to be an compressed entry
			 * (1)Run the retrieve process
			 * (2)Make the entry in a uncompressed state
			 * */
			if (p_data->compressed_state == COMPRESSED_STATE::COMPRESSED_IN_MEM)//compressed in memory
			{
				/*Check if an uncompressed entry needed to be swapped out*/
				if (this->uncompressed_entry_num+1 > this->MAX_UNCOMPRESSED_ENTRY_NUM)//do not need to retrieve page if <=
				{
					if (!this->retrieve_page())
					{
						this->MAX_UNCOMPRESSED_ENTRY_NUM++;
					}
					else
					{
						this->uncompressed_entry_num--;//added when debug
					}
					/*Set the access flag again.Since it has been erased in retrieve_page op*/
					p_data->access_flag = 1;
				}

				p_data->compressed_state = COMPRESSED_STATE::UNCOMPRESSED_IN_MEM;
				this->uncompressed_entry_num++;
			}
			
			/*With access_flag == 1 ,last_access_time set and compressed_state == UNCOMPRESSED_IN_MEM we now can put the data*/
			p_data->setData(dataContent);
			goto finished;
		}
		else
		{
			store[key] = new Data();
			Data* p_data = store[key];

			/*Save the data and set the access_flag and last access time*/
			p_data->setData(dataContent);
			p_data->access_flag = 1;
			p_data->compressed_state = COMPRESSED_STATE::UNCOMPRESSED_IN_MEM;
			p_data->last_access_time = store_clock.getCurrentTime();

			/*Check if an uncompressed entry needed to be swapped out*/
			if (this->uncompressed_entry_num+1 > this->MAX_UNCOMPRESSED_ENTRY_NUM)//do not need to retrieve page if <=
			{
				if (!this->retrieve_page())
				{
					this->MAX_UNCOMPRESSED_ENTRY_NUM++;
				}
				else
				{
					this->uncompressed_entry_num--;//added when debug
				}
				/*Set the access flag again.Since it has been erased in retrieve_page op*/
				p_data->access_flag = 1;
			}
			this->uncompressed_entry_num++;
			goto finished;
		}
	finished:
		return 0;
	}

//protected:
public:
	std::map<std::string,Data*> store;
	int uncompressed_entry_num;
	Clock store_clock;//record the virtual access time. Auto increment when get and put is called

	/*Configured parameter*/
	int T;//if a page was visited before store_clock-T, it will be swapped out.
	int MAX_UNCOMPRESSED_ENTRY_NUM;

	/*assistant method*/
	std::string compress(std::string contentToBeCompressed)
	{
		//return ("Compressed:"+contentToBeCompressed);
		
		char compressed[1024];
		int source_size = strlen(contentToBeCompressed.c_str());
		int compressed_size = LZ4_compress(contentToBeCompressed.c_str(), compressed, source_size);
		compressed[compressed_size] = '\0';
		//cout<<"Original size:"<<contentToBeCompressed.length()<<endl;
		//cout<<"Compressed size:"<<compressed_size<<endl;
		return (std::string(compressed));
		
	}
	std::string decompress(std::string contentToBeDecompressed)
	{
		//return contentToBeDecompressed.substr(11);
		
		char decompressed[1024];
		int compressed_size = strlen(contentToBeDecompressed.c_str());
		int decompressed_size = LZ4_decompress_safe(contentToBeDecompressed.c_str(), decompressed,
								    compressed_size, 1024);
		decompressed[decompressed_size] = '\0';
		return (std::string(decompressed));
		
	}

	/*Retrieving page operation:
	* We have to make at least one another less visited uncompressed page compressed
	* (1)Try to retrieve at least one page.
	* (2)If pages with access flag 0 exceed a certain proportion, we may retrieve more than one page to keep the statistics under certain proportion
	* (3)If the access_flags of all the pages are 1,then comes the need to add a new physical page so we need to return false
	* Return true if at least one page has been retrieved
	* Return false if no page has been retrieved, indicating that the MAX_UNCOMPRESSED_ENTRY_NUM needed to add one.
	* */
	bool retrieve_page()
	{
		map<string,Data*>::iterator iter = this->store.begin();//this->get_begin_iter();
		map<string,Data*>::iterator iter_oldest_access_time = this->store.begin();//this->get_begin_iter();
		unsigned int oldest_access_time = iter_oldest_access_time->second->last_access_time;
		
		bool last_access_time_all_within_T = true;
		bool swapped_entry_found = false;

		while (iter != this->store.end()/*this->get_end_iter()*/)
		{
			/*Record the oldest last access time and the entry*/
			if (iter->second->last_access_time < iter_oldest_access_time->second->last_access_time)
			{
				iter_oldest_access_time = iter;
				oldest_access_time = iter_oldest_access_time->second->last_access_time;
			}

			if (iter->second->compressed_state == COMPRESSED_STATE::UNCOMPRESSED_IN_MEM && //bug found
				iter->second->last_access_time < (store_clock.getCurrentTime()-T))
			{
				last_access_time_all_within_T = false;
				swapped_entry_found = true;
				break;
			}

			iter++;//iter = this->get_next_iter(iter);
		}
		//this->set_last_checked_iterator(iter);//set last_checked_iter when we get out of the loop

		if (last_access_time_all_within_T && !(swapped_entry_found))
			return false;
		else
		{
			Data* p_data = iter->second;
			p_data->access_flag = 0;
			p_data->compressed_state = COMPRESSED_STATE::COMPRESSED_IN_MEM;
			p_data->setData(this->compress(p_data->getData()));
			return true;
		}
	}	

	/*Used in clock wise work set algorithm*/
	map<string,Data*>::iterator last_checked_iterator;
	map<string,Data*>::iterator get_next_iter(map<string,Data*>::iterator iter_now)
	{
		iter_now++;
		if (iter_now == store.end())
		{
			iter_now = store.begin();
		}
		return iter_now;
	}
	map<string,Data*>::iterator get_begin_iter()
	{
		map<string,Data*>::iterator temp_iter = last_checked_iterator;
		return get_next_iter(temp_iter);
	}
	map<string,Data*>::iterator get_end_iter()
	{
		map<string,Data*>::iterator temp_iter = last_checked_iterator;
		return get_next_iter(temp_iter);
	}
	bool set_last_checked_iterator(map<string,Data*>::iterator iter_now)
	{
		last_checked_iterator = iter_now;
		return true;
	}
};

#endif /* WORKSETSTORE_H_ */
