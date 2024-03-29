#include "starpirate.h"

void multMatrices4x4(const GLfloat *A, GLfloat *B)
{
	float temp[16];

	temp[0] =A[0]*B[0] + A[4]*B[1] + A[8] *B[2] + A[12]*B[3];
	temp[1] =A[1]*B[0] + A[5]*B[1] + A[9] *B[2] + A[13]*B[3];
	temp[2] =A[2]*B[0] + A[6]*B[1] + A[10]*B[2] + A[14]*B[3];
	temp[3] =A[3]*B[0] + A[7]*B[1] + A[11]*B[2] + A[15]*B[3];

	temp[4] =A[0]*B[4] + A[4]*B[5] + A[8] *B[6] + A[12]*B[7];
	temp[5] =A[1]*B[4] + A[5]*B[5] + A[9] *B[6] + A[13]*B[7];
	temp[6] =A[2]*B[4] + A[6]*B[5] + A[10]*B[6] + A[14]*B[7];
	temp[7] =A[3]*B[4] + A[7]*B[5] + A[11]*B[6] + A[15]*B[7];

	temp[8] =A[0]*B[8] + A[4]*B[9] + A[8] *B[10] + A[12]*B[11];
	temp[9] =A[1]*B[8] + A[5]*B[9] + A[9] *B[10] + A[13]*B[11];
	temp[10]=A[2]*B[8] + A[6]*B[9] + A[10]*B[10] + A[14]*B[11];
	temp[11]=A[3]*B[8] + A[7]*B[9] + A[11]*B[10] + A[15]*B[11];
	
	temp[12]=A[0]*B[12] + A[4]*B[13] + A[8] *B[14] + A[12]*B[15];
	temp[13]=A[1]*B[12] + A[5]*B[13] + A[9] *B[14] + A[13]*B[15];
	temp[14]=A[2]*B[12] + A[6]*B[13] + A[10]*B[14] + A[14]*B[15];
	temp[15]=A[3]*B[12] + A[7]*B[13] + A[11]*B[14] + A[15]*B[15];

	for(int i=0; i< 16; i++)
		B[i]=temp[i];
}

void swapMatrices4x4(GLfloat *A, GLfloat *B)
{
	float temp[16];
	for (int i=0; i<16; i++)
	{
		temp[i]=A[i];
		A[i]=B[i];
		B[i]=temp[i];
	}
}
