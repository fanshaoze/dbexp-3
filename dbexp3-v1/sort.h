#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extmem.h"
using namespace std;


void swap(unsigned int *x, unsigned int *y);
void bubbleSort(int n, unsigned int* p);//ð������
int sortinit();//��R�ĸ���ֱ������γɳ�ʼ�鲢��
int mergeSort(unsigned int addr1, int n1, unsigned int addr2, int n2, int base);
int mergeAll();
int binarysort(int array[],int val);