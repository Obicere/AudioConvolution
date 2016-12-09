#ifndef CONVOLVE_H
#define CONVOLVE_H

#define INPUT_SIDE 0
#define FFT 1
#define FFT_OPTIMIZED 0

#include "wav.h"

class Convolve {

private:

  Wav * input;
  Wav * ir;
  Wav * output;

public:

  Convolve(Wav * input, Wav * ir) : input(input), ir(ir) {
    // pass in input and copy the header
    output = new Wav(input);
  }

  ~Convolve() {
    delete output;
    output = NULL;
  }

  Wav * getOutput() {
    return output;
  }

  void convolve();

};

#endif
