#include <math.h>
#include <time.h>
#include <cstdlib>

#include "HelloWorldScene.h"
#include "NeuralNetworkAlgorithm.h"

NeuralNetworkAlgorithm::NeuralNetworkAlgorithm()
{
	// memory allocation
	_weight_s = (double**)malloc(UNIT_NEURON * sizeof(double*));
	for (int i = 0; i < UNIT_NEURON; i++)
		_weight_s[i] = (double*)malloc(BOX_X * BOX_Y * sizeof(double));

	_weight_f = (double**)malloc(UNIT_NUM * sizeof(double*));
	for (int i = 0; i < UNIT_NUM; i++)
		_weight_f[i] = (double*)malloc(UNIT_NEURON  *sizeof(double));

	_output_hidden = (double*)malloc(UNIT_NEURON * sizeof(double));
	_output_fdata = (double*)malloc(UNIT_NUM * sizeof(double));
}

NeuralNetworkAlgorithm::~NeuralNetworkAlgorithm()
{
}


void NeuralNetworkAlgorithm::init_weight() {//weight value making
	// random seed initialize
	srand((unsigned)time(NULL));

	// randomize value
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 100; j++)
			_weight_s[i][j] = (double)(rand() % (9999 * 2) + (-9999)) / 10000.0;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 25; j++)
			_weight_f[i][j] = (double)(rand() % (9999 * 2) + (-9999)) / 10000.0;
	}
}

double NeuralNetworkAlgorithm::act_ftn(double in) {//activition function
	return (1.0 / (1.0 + exp(-1.0*in)));
}

int NeuralNetworkAlgorithm::getNumber(int *arr) {//number recognition

	double output = 0;
	double temp = 0;

	for (int j = 0; j < UNIT_NEURON; j++) {
		for (int i = 0; i < BOX_X  * BOX_Y; i++) {
			temp = arr[i] * _weight_s[j][i];
			output = output + temp;
		}
		_output_hidden[j] = act_ftn(output);
		output = 0;
	}

	for (int j = 0; j < UNIT_NUM; j++) {
		for (int i = 0; i < UNIT_NEURON; i++) {
			temp = _output_hidden[i] * _weight_f[j][i];
			output = output + temp;
		}
		_output_fdata[j] = act_ftn(output);
		output = 0;
	}
	temp = 0;

	for (int i = 0; i < UNIT_NUM; i++) {
		if (temp < _output_fdata[i]) {
			temp = _output_fdata[i];
			output = i;
		}
	}
	return (int)(output);
}

void NeuralNetworkAlgorithm::getWeight(int *arr, int num) {//change weight

	double output = 0; double temp = 0;
	double eta = 1; // %0.9;  %learning rate

	double error_f[UNIT_NUM];
	double error_s[UNIT_NEURON];
	double target[10] = { 0, };

	target[num] = 1;

	for (int i = 0; i < UNIT_NUM; i++) {
		error_f[i] = _output_fdata[i] * (1 - _output_fdata[i]) * (target[i] - _output_fdata[i]);
	}

	for (int i = 0; i < UNIT_NUM; i++) {
		for (int j = 0; j < UNIT_NEURON; j++) {
			_weight_f[i][j] = _weight_f[i][j] + eta * _output_hidden[j] * error_f[i];
		}
	}

	for (int i = 0; i < UNIT_NEURON; i++) {
		for (int k = 0; k < UNIT_NUM; k++) {
			temp = _weight_f[k][i] * error_f[k];
			output = output + temp;
		}
		error_s[i] = _output_hidden[i] * (1 - _output_hidden[i]) * (output);
		output = 0;
	}

	for (int i = 0; i < UNIT_NEURON; i++) {
		for (int j = 0; j < BOX_X*BOX_Y; j++) {
			_weight_s[i][j] = _weight_s[i][j] + eta * arr[j] * error_s[i];
		}
	}

}

void NeuralNetworkAlgorithm::basicLearn() {//기본 모델 학습(선택사항)

	int ar0[15] = { 1,1,1,1,0,1,1,0,1,1,0,1,1,1,1 };
	int ar1[15] = { 0,1,0,0,1,0,0,1,0,0,1,0,0,1,0 };
	int ar2[15] = { 1,1,1,0,0,1,1,1,1,1,0,0,1,1,1 };
	int ar3[15] = { 1,1,1,0,0,1,1,1,1,0,0,1,1,1,1 };
	int ar4[15] = { 1,0,1,1,0,1,1,1,1,0,0,1,0,0,1 };
	int ar5[15] = { 1,1,1,1,0,0,1,1,1,0,0,1,1,1,1 };
	int ar6[15] = { 1,1,1,1,0,0,1,1,1,1,0,1,1,1,1 };
	int ar7[15] = { 1,1,1,1,0,1,1,0,1,0,0,1,0,0,1 };
	int ar8[15] = { 1,1,1,1,0,1,1,1,1,1,0,1,1,1,1 };
	int ar9[15] = { 1,1,1,1,0,1,1,1,1,0,0,1,1,1,1 };

	for (int j = 0; j < 100; j++) {
		getNumber(ar0);
		getWeight(ar0, 0);
		getNumber(ar1);
		getWeight(ar1, 1);
		getNumber(ar2);
		getWeight(ar2, 2);
		getNumber(ar3);
		getWeight(ar3, 3);
		getNumber(ar4);
		getWeight(ar4, 4);
		getNumber(ar5);
		getWeight(ar5, 5);
		getNumber(ar6);
		getWeight(ar6, 6);
		getNumber(ar7);
		getWeight(ar7, 7);
		getNumber(ar8);
		getWeight(ar8, 8);
		getNumber(ar9);
		getWeight(ar9, 9);
	}
}

/*
int main() {//예상 시나리오
	int ar[15] = { 1,1,0,
		0,0,1,
		0,1,0,
		0,0,1,
		1,1,0 };//이런 input이 오면

	init_weight();//weight을 초기화후
	basicLearn();//기본 모델을 할껀지 말껀지 선택사항

	printf("%d", getNumber(ar, 3, 5));//어떤 숫자인지 구분하고

	getWeight(ar, 3);//학습 시키고

	printf("%d", getNumber(ar, 3, 5));//잘 학습했는지 확인, 끝

	return 0;
}
*/