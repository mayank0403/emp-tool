#ifndef CIRCUIT_FILE
#define CIRCUIT_FILE

#define AND_GATE 0
#define XOR_GATE 1
#define NOT_GATE 2
#include "garble_circuit.h"
#include "backend.h"
#include <stdio.h>
#include "block.h"
#include "bit.h"
class CircuitFile { public:
	int num_gate, num_wire, n1, n2, n3;
	int *gates;
	block * wires;
	int tmp, tmp2;
	CircuitFile(const char * file) {
		FILE * f = fopen(file, "r");
		tmp2=fscanf(f, "%d%d\n", &num_gate, &num_wire);
		tmp2=fscanf(f, "%d%d%d\n", &n1, &n2, &n3);
		tmp2=fscanf(f, "\n");
		char str[10];
		gates = new int[num_gate*4];
		wires = new block[num_wire];
		for(int i = 0; i < num_gate; ++i) {
			tmp2=fscanf(f, "%d", &tmp);
			if (tmp == 2) {
				tmp2=fscanf(f, "%d%d%d%d%s", &tmp, &gates[4*i], &gates[4*i+1], &gates[4*i+2], str);
				if (str[0] == 'A') gates[4*i+3] = AND_GATE;
				else if (str[0] == 'X') gates[4*i+3] = XOR_GATE;
			}
			else if (tmp == 1) {
				tmp2=fscanf(f, "%d%d%d%s", &tmp, &gates[4*i], &gates[4*i+2], str);
				gates[4*i+3] = NOT_GATE;
			}
		}
		fclose(f);
	}

	~CircuitFile(){
		delete[] gates;
		delete[] wires;
	}
	int table_size() const{
		return num_gate*4;
	}

	void compute(block * out, block * in1, block * in2) {
		memcpy(wires, in1, n1*sizeof(block));
		memcpy(wires+n1, in2, n2*sizeof(block));
		for(int i = 0; i < num_gate; ++i) {
			if(gates[4*i+3] == AND_GATE) {
				wires[gates[4*i+2]] = local_gc->gc_and(
wires[gates[4*i]], wires[gates[4*i+1]]);
			}
			else if (gates[4*i+3] == XOR_GATE) {
				wires[gates[4*i+2]] = local_gc->gc_xor(
wires[gates[4*i]], wires[gates[4*i+1]]);
			}
			else  
				wires[gates[4*i+2]] = local_gc->gc_not(wires[gates[4*i]]);
		}
		memcpy(out, &wires[num_wire-n3], n3*sizeof(block));
	}
};
#endif// CIRCUIT_FILE
