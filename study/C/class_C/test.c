/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月07日 星期二 19时28分41秒


	输入一个数据，输出逆序数
 ************************************************************************/

#include<stdio.h>


int main()
{
	//输入的数据	
	int num1,i;
	int num2[3];

	scanf("%d",&num1);

	num2[0]=num1/100;
	num2[1]=num1%100/10;
	num2[2]=num1%10;

	for(i=0;i<3;i++){

		if(num2[0]==0)
			continue;
		else{
		
			if(num2[1]==0)
				continue;
		}
		printf("%d \n",num2[i]);
	}

	return 0;
}
