#include "complex_store.h"
#include <sys/time.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include "lz4.h"

#define CHECK_TIME 1
#define COLD_TEMP 80
#define INITIAL_TEMP 100.0
#define A 0.192
#define N 20.0
#define SIZE 300000

using namespace std;

ComplexStore::ComplexStore():store([](const SimpleDataBlock *a, const SimpleDataBlock *b) {
                    int result = a->compare(b);

                                  return result < 0;
                                           }) {}

int ComplexStore::get(const char* key, int key_size, char *value) {
	SimpleDataBlock *key_block = new SimpleDataBlock{key,key_size};
    int r = -1;
    if (store.count(key_block)) {

        const char *source;

        if(store[key_block].compressed == true){
            cout<<"decompressing..."<<endl;
            char decompressed[SIZE];
            int compressed_size = store[key_block].compressed_size;
	        int decompressed_size = LZ4_decompress_safe(store[key_block].value, decompressed,compressed_size,SIZE);
         //   cout<<"compressed_size:"<<compressed_size<<endl;
           // cout<<"decompressed_size:"<<decompressed_size<<endl;
            delete [] store[key_block].value;
            store[key_block].value = new char[decompressed_size+1];
            decompressed[decompressed_size] = '\0';
            memcpy(store[key_block].value, decompressed, decompressed_size);
            store[key_block].value[decompressed_size] = '\0';
            store[key_block].compressed = false;
            hot_list.push_front(key_block);
        }
		source = store[key_block].value;
		int size = strlen(source);
		memcpy(value, source, size);		

        //calc current temperature
        int time = (this->getTime() - store[key_block].last_atime)/1000;
        double temp = this->calcTemp(store[key_block].temp, time);

        //modify new time and temperature
        store[key_block].temp = temp + N;
        store[key_block].last_atime = this->getTime();

        //if last_compressed_time = 0, initial it; otherwise now - last_modified_time > CHECK_TIME(10s), compress cold data
        if(last_compressed_time == 0){
            last_compressed_time = this->getTime();
        }else{
            int during_time = (this->getTime() - last_compressed_time)/1000;
            cout<<"during_time:"<<during_time<<endl;
            if (during_time > CHECK_TIME){
                this->compressing();
                last_compressed_time = this->getTime();
            }
        }
		r = size;
	} else {
		r = -1;
	}
    return r;
}

int ComplexStore::put(const char* key, int key_size, const char *value, int value_size) {
    SimpleDataBlock *key_block = new SimpleDataBlock{key,key_size};
    
    char compressed[SIZE]; 
    //cout<<"raw_value="<<value<<endl;
    int source_size = strlen(value);
    int compressed_size = LZ4_compress(value, compressed, source_size);

    if(store.count(key_block)){
        info *p_info;
        p_info = &store[key_block];
        (*p_info).compressed = true;
        (*p_info).temp = INITIAL_TEMP;
        (*p_info).last_atime = this -> getTime();
        delete [] (*p_info).value;
        (*p_info).value = new char[compressed_size];
        memcpy((*p_info).value,compressed,compressed_size);
       (*p_info).compressed_size = compressed_size;
        cout<<"p_info"<<endl;
    }else{
        info new_info;
        new_info.compressed = true;
        new_info.temp = INITIAL_TEMP;
        new_info.last_atime =  this->getTime();
        new_info.value = new char[compressed_size];
        memcpy(new_info.value, compressed, compressed_size);
        new_info.compressed_size = compressed_size;
	    store[key_block] = new_info;
        cout<<"new_info"<<endl;
    }
	return 0;
}

unsigned long long ComplexStore::getTime(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

double ComplexStore::calcTemp(double temp, int second){    
    return round(temp*exp(-A*second));
}

void ComplexStore::compressing(void){
   // cout<<"start compressing .... "<<endl;
    list<SimpleDataBlock*>::iterator i= hot_list.begin();
    while(i != hot_list.end()){
        //calc current temperature
        SimpleDataBlock *key_block = *i;
        int time = (this->getTime() - store[key_block].last_atime)/1000;
        double temp = this->calcTemp(store[key_block].temp, time);
        if(temp < COLD_TEMP){
            store[key_block].compressed = true;
            store[key_block].last_atime = this->getTime();
            store[key_block].temp = INITIAL_TEMP;
            char compressed[SIZE];
            cout<<store[key_block].value<<endl;
            char *value = store[key_block].value;
            cout<<value<<endl;
            int source_size = strlen(value);
            cout<<source_size<<endl;
            int compressed_size = LZ4_compress(value, compressed, source_size);
         //   cout<<"compressed text: "<<compressed<<endl;
            delete [] store[key_block].value;
            store[key_block].value = new char[compressed_size];
            memcpy(store[key_block].value, compressed, compressed_size);
            hot_list.erase(i++);
        }else{
            ++i;
        }
        //delete key_block;
    }
    return;
}
