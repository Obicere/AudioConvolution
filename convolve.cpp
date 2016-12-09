#include "convolve.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

#if INPUT_SIDE

void Convolve::convolve() {
  int inputLength = input->getLength();
  int irLength = ir->getLength();

  int outputLength = inputLength + irLength - 1;
  double * outputData = new double[outputLength];
  memset(outputData, 0, sizeof(double) * outputLength);

  double * inputData = input->getData();
  double * irData = ir->getData();

  for (int i = 0; i < inputLength; i++) {
    for(int j = 0; j < irLength; j++) {
      outputData[i + j] += inputData[i] * irData[j];
    }
  }
  output->setData(outputData, outputLength);
}

#endif
