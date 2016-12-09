#include <iostream>
#include <ctime>
#include "convolve.h"
#include "wav.h"

int main(int argc, char * argv[]) {
  if(argc != 4) {
    std::cerr << "Invalid arguments: convolve input irfile output" << std::endl;
    return 1;
  }

  clock_t t = clock();
  Wav * input = new Wav(argv[1]);
  Wav * ir = new Wav(argv[2]);

  Convolve conv(input, ir);

  conv.convolve();

  Wav * output = conv.getOutput();

  output->write(argv[3]);

  t = clock() - t;

  std::cout << "Program took " << ((float) t) / CLOCKS_PER_SEC << "s to run.";

  delete input;
  delete ir;

  return 0;
}
