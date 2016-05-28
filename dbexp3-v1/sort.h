#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extmem.h"
using namespace std;


void swap(unsigned int *x, unsigned int *y);
void bubbleSort(int n, unsigned int* p);//冒泡排序
int sortinit();//把R的各块分别排序，形成初始归并段
int mergeAll();