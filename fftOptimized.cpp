#include "convolve.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if FFT_OPTIMIZED
#define SWAP(a,b)  tempr=(a);(a)=(b);(b)=tempr
  //  The four1 FFT from Numerical Recipes in C,
  //  p. 507 - 508.
  //  Note:  changed float data types to double.
  //  nn must be a power of 2, and use +1 for
  //  isign for an FFT, and -1 for the Inverse FFT.
  //  The data is complex, so the array size must be
  //  nn*2. This code assumes the array starts
  //  at index 1, not 0, so subtract 1 when
  //  calling the routine (see main() below).

  void four1(double data[], int nn, int isign)
  {
  	unsigned long n, mmax, m, j, istep, i;
  	double wtemp, wr, wpr, wpi, wi, theta;
  	double tempr, tempi;

  	n = nn << 1;
  	j = 1;

  	for (i = 1; i < n; i += 2) {
  		if (j > i) {
  			SWAP(data[j], data[i]);
  			SWAP(data[j + 1], data[i + 1]);
  		}
  		m = nn;
  		while (m >= 2 && j > m) {
  			j -= m;
  			m >>= 1;
  		}
  		j += m;
  	}

  	mmax = 2;
  	while (n > mmax) {
  		istep = mmax << 1;
  		theta = isign * (6.28318530717959 / mmax);
  		wtemp = sin(0.5 * theta);
  		wpr = -2.0 * wtemp * wtemp;
  		wpi = sin(theta);
  		wr = 1.0;
  		wi = 0.0;
  		for (m = 1; m < mmax; m += 2) {
  			for (i = m; i <= n; i += istep) {
  				j = i + mmax;
  				tempr = wr * data[j] - wi * data[j + 1];
  				tempi = wr * data[j + 1] + wi * data[j];
  				data[j] = data[i] - tempr;
  				data[j + 1] = data[i + 1] - tempi;
  				data[i] += tempr;
  				data[i + 1] += tempi;
  			}
  			wr = (wtemp = wr) * wpr - wi * wpi + wr;
  			wi = wi * wpr + wtemp * wpi + wi;
  		}
  		mmax = istep;
  	}
  }

  int upper_power_of_two(int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
  }


  void Convolve::convolve() {
    int inputLength = input->getLength();
    double * inputData = input->getData();

    int irLength = ir->getLength();
    double * irData = ir->getData();

    // optimization 1
    int n = upper_power_of_two(inputLength);
    // optimization 2
    int n2 = n << 1;

    double * x = new double[n2];
    double * fr = new double[n2];

    for (int i = 0; i < inputLength; i++) {
      x[i << 1] = inputData[i];
      // optimization 5
      x[(i << 1) + 1] = 0;
    }
    four1(x - 1, n, 1);

    for (int i = 0; i < irLength; i++) {
      fr[i << 1] = irData[i];
      // optimization 5
      fr[(i << 1) + 1] = 0;
    }
    four1(fr - 1, n, 1);

    double rex, imgx, refr, imgfr;
    for (int i = 0; i < n2; i += 2) {
      // optimization 4
      rex = x[i];
      imgx = x[i + 1];
      refr = fr[i];
      imgfr = fr[i + 1];
      x[i] = rex * refr - imgx * imgfr;
      x[i + 1] = rex * imgfr + imgx * refr;
    }
    four1(x - 1, n, -1);

    int outputLength = inputLength + irLength - 1;
    double * outputData = new double[outputLength];

    for (int i = 0; i < outputLength; i++) {
      outputData[i] = x[i << 1];
    }

    delete[] fr;
    delete[] x;

    output->setData(outputData, outputLength);
  }

#endif
