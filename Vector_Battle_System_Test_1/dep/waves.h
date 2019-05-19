/*This header file contains classes and methods relating to the chord, note, and wave logic of this game*/
#pragma once
#ifndef __waves_h__
#define __waves_h__
#include <cmath>
#include <vector>
//#include "geometry.h"
#include "fgrutils.h"

using namespace std;
using namespace fgr;

//Simply stores an amplitude and frequency. Can evaluate itself.
class sinusoid {
public:
	float amplitude; //Amplitude
	float frequency; //Frequency, NOT ANGULAR FREQUENCY

	float eval(float x) {
		return amplitude * sin(2 * PI* frequency * x);
	}

	sinusoid() {
		amplitude = 1;
		frequency = 1;
	}

	sinusoid(float A, float f) {
		amplitude = A;
		frequency = f;
	}
};

/*Remember that the Fourier sinusoid's frequency or amplitude member can be multiplied by that of the desired modulated 
wave to get the corrected value. Default amplitude is 1 and default frequency is 1/(2*PI) */

///Duuude that wikipedia page is super helpful https://en.wikipedia.org/wiki/Piano_key_frequencies lol

//The actual square wave function
inline float squ(float x) { return (sin(x) / abs(sin(x))); }

//Returns the nth sinusoid for the modulation of a square wave with amplitude 1 and frequency 1
inline sinusoid squHarm(int term, float amplitude, float frequency) {
	sinusoid ret;
	ret.amplitude = (4.0f / PI) / (2.0f * term - 1.0f) * amplitude;
	ret.frequency = float((2 * term - 1)) * frequency;
	return ret;
}

//Returns a vector of sinusoids representing the fourier sine series for a square wave
inline vector<sinusoid> squSeries(int terms, float amplitude, float frequency) {
	vector<sinusoid> rets(0); //DP: What's the 0 for?
	for (int n = 1; n <= terms; n++) {
		sinusoid component = squHarm(n, amplitude, frequency);
		rets.push_back(component);
	}
	return rets;
}

//The actual saw wave function
inline float saw(float x) { return fmod((x / PI), 1.0f); }

//Returns the nth sinusoid for the modulation of a saw wave with amplitude 1 and frequency 1
inline sinusoid sawHarm(int term, float amplitude, float frequency) {
	sinusoid ret;
	ret.amplitude = -(2.0f / PI) * pow(-1.0f, term) / term * amplitude;
	ret.frequency = term * frequency;
	return ret;
}

//Returns a vector of sinusoids representing the fourier sine series for a saw wave
inline vector<sinusoid> sawSeries(int terms, float amplitude, float frequency) {
	vector<sinusoid> rets(0);
	for (int n = 1; n <= terms; n++) {
		sinusoid component = sawHarm(n, amplitude, frequency);
		rets.push_back(component);
	}
	return rets;
}

//The actual triangle wave function
inline float tri(float x) { return abs(saw(2.0f * x)); } //Triangle wave function

//Returns the nth sinusoid for the modulation of a triangle wave with amplitude 1 and frequency 1
inline sinusoid tri_harm(int term, float amplitude, float frequency) {
	sinusoid ret;
	ret.amplitude = powf(-1.0f, term + 1.0f) / powf((2.0f * term - 1.0f), 2.0f) * amplitude;
	ret.frequency = (2 * term - 1) * frequency; ///Not so sure this is correct
	return ret;
}

//Returns a vector of sinusoids representing the fourier sine series for a triangle wave
inline vector<sinusoid> tri_series(int terms, float amplitude, float frequency) {
	vector<sinusoid> rets(0);
	for (int n = 1; n <= terms; n++) {
		sinusoid component = tri_harm(n, amplitude, frequency);
		rets.push_back(component);
	}
	return rets;
}

//Evaluate a series (vector) of sinusoids at a given x
inline float evalSeries(vector<sinusoid>& series, float x) {
	float ret = 0.0f;
	for (unsigned int n = 0; n < series.size(); n++) {
		ret += series[n].eval(x);
	}
	return ret;
}

//Returns the note frequency of a piano note given its index
inline float noteFrequency(int key) { return pow(pow(2.0f, 1.0f / 12.0f), float(key - 49)) * 440.0f; }

//Returns the piano note key index corresponding to a particular frequency
inline int pianoKey(float frequency) { return int(round(12 * log2(frequency / 440.0f) + 49)); }

//To-do list: Make some GLUT rendering functions for all this stuff


#endif