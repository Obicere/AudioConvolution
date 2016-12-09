#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "wav.h"

void Wav::read(char * fileName) {

  FILE * in = fopen(fileName, "rb");
  if (in == NULL) {
    std::cerr << "Cannot open file: " << fileName << std::endl;
    return;
  }

  // magic
  fread(&headerInfo, 36, 1, in);
  if(headerInfo.subchunk1_size == 18) {
    short empty;
    fread(&empty, sizeof(empty), 1, in);
  }
  fread(&headerInfo.subchunk2_id, sizeof(headerInfo.subchunk2_id), 1, in);
  fread(&headerInfo.subchunk2_size, sizeof(headerInfo.subchunk2_size), 1, in);

  if(headerInfo.bits_per_sample != 16) {
    std::cerr << "Only working with 16 bits per sample. Given: " << headerInfo.bits_per_sample << std::endl;
    return;
  }

  int length = headerInfo.subchunk2_size / 2; // 2 bytes per sample
  double * data = new double[length];

  int readCount = 0;
  short readValue;
  while(fread(&readValue, sizeof(short), 1, in) == 1) {
    data[readCount++] = (double) readValue / MAX_VALUE;
  }
  setData(data, length);
  fclose(in);
}

void Wav::write(char * fileName) {

  FILE * out = fopen(fileName, "wb");
  if (out == NULL) {
    std::cerr << "Cannot open file: " << fileName << std::endl;
    return;
  }

  // magic
  fwrite(&headerInfo, 36, 1, out);
  if(headerInfo.subchunk1_size == 18) {
    short empty = 0;
    fwrite(&empty, sizeof(empty), 1, out);
  }
  fwrite(&headerInfo.subchunk2_id, sizeof(headerInfo.subchunk2_id), 1, out);
  fwrite(&headerInfo.subchunk2_size, sizeof(headerInfo.subchunk2_size), 1, out);

  if(length == 0) {
    return;
  }

  double max = data[0];
  for (int i = 1; i < length; i++) {
    if (data[i] > max) {
      max = data[i];
    }
  }

  short sample;
  for(int i = 0; i < length; i++){
    sample = (short) (data[i] * MAX_VALUE / max);

    fwrite(&sample, sizeof(short), 1, out);
  }
  fclose(out);
}
