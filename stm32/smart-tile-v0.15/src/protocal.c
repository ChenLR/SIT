#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "protocal.h"

// Buffer for store received chars
static uint8_t recv_buffer[MAX_BUFFER_LEN];
int RECV_CNT = 0;

uint8_t checkSum(uint8_t recv_idx, uint8_t target) {
	uint8_t i, sum = recv_idx;
	for(i=0; i<recv_idx; i++) {
		sum += recv_buffer[i];
	}
	return sum == target;
}


void recvByteHandler(uint8_t byte) {
	static uint8_t recv_state = 0;
	static uint8_t recv_last_byte = 0x00;
	static uint8_t recv_length = 0;
	static uint8_t recv_idx = 0;

	switch(recv_state) {
	case 0:
		if (recv_last_byte == PROT_FIRST_BYTE && byte == PROT_SECOND_BYTE) {
			recv_state = 1;
			recv_idx = 0;
		}
		else {
			recv_state = 0;
			recv_idx = 0;
		}
		break;
	case 1:
		recv_state = 2;
		recv_idx = 0;
		recv_length = byte;
		break;
	case 2:
		if (recv_last_byte == PROT_FIRST_BYTE && byte == PROT_SECOND_BYTE) {
			recv_state = 1;
			recv_idx = 0;
		}
		else {
			if (recv_idx < recv_length) {
				if (recv_idx < MAX_BUFFER_LEN) {
					recv_buffer[recv_idx] = byte;
					recv_idx++;
				}
				else {
					recv_state = 0;
					recv_idx = 0;
				}
			}
			else {
				if (recv_idx == recv_length && checkSum(recv_idx, byte)) {
					// remove padding and process
					RECV_CNT++;
				}
				recv_state = 0;
				recv_idx = 0;
			}
		}
		break;
	default:
		// idle state
		recv_state = 0;
		recv_idx = 0;
	}
	recv_last_byte = byte;
}
