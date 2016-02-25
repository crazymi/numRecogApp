#ifndef __NEURAL_NETWORK_ALGORITHM_H__
#define __NEURAL_NETWORK_ALGORITHM_H__

#define UNIT_NEURON 25
#define UNIT_NUM 10

class NeuralNetworkAlgorithm
{
private:
	double** _weight_s;
	double** _weight_f;
	// f->hidden->g : hidden Array
	double* _output_hidden;
	// getNumber resultArray
	double* _output_fdata;

	/*
	double weight_s[25][100] = { 0, };
	double weight_f[10][25] = { 0, };
	double output_hidden1[25] = { 0, };
	double output_fdata[10] = { 0, };
	*/

public:
	NeuralNetworkAlgorithm();
	~NeuralNetworkAlgorithm();

	void init_weight();
	double act_ftn(double in);
	int getNumber(int* arr);
	void getWeight(int* arr, int num);
	void basicLearn();
};

#endif __NEURAL_NETWORK_ALGORITHM_H__