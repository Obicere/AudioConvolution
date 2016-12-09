#ifndef WAV_H
#define WAV_H

#include <stdlib.h>

#define MAX_VALUE 32767

typedef struct {

  	char chunk_id[4];
  	int chunk_size;
  	char format[4];
  	char subchunk1_id[4];
  	int subchunk1_size;
  	short int audio_format;
  	short int num_channels;
  	int sample_rate;
  	int byte_rate;
  	short int block_align;
  	short int bits_per_sample;
  	char subchunk2_id[4];
  	int subchunk2_size;			// subchunk2_size specifies the number of samples.

} header;

class Wav {

private:
  header headerInfo;

  double * data;

  int length;

  void read(char * fileName);

public:

  Wav(Wav * headerCopy) : data(NULL), length(0) {
    headerInfo = headerCopy->headerInfo;

    // these two parameters are dependent on the size of the data
    // they should be updated with changes to length
    headerInfo.chunk_size = 4 + (8 + 16) + (8 + 0);
    headerInfo.subchunk2_size = 0;
  }

  Wav(char * fileName) {
    read(fileName);
  }

  ~Wav() {
    delete[] data;
    data = NULL;
  }

  double * getData() {
    return data;
  }

  int getLength() {
    return length;
  }

  void setData(double * newData, int length) {
    this->data = newData;
    this->length = length;
    headerInfo.subchunk2_size = 2 * length;
    headerInfo.chunk_size = 4 + (8 + headerInfo.subchunk1_size) + (8 + headerInfo.subchunk2_size);
  }

  void write(char * fileName);
};
#endif
