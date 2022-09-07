#include <Python.h>
//아래 헤더파일들은 <Python.h>에 포함되어있습니다.88
//#include <stdio.h>
//#include <string.h>
//#include <unistd.h>
//#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//EXIT_CODE 는 ARD_RAS_Newwork 프로그램을 종료시키는 프로그램입니다. 종료 프로세스는 아래와 같습니다.
//Bluetooth.py에서 Bluetooth_Disconnect() 함수를 실행시켜 블루투스 소켓을 close합니다.
//ARD_RAS_Newwork 에서 통신 기능(무한 루프)에서 빠져나오며 파이썬 인터프리터를 종료시킵니다.Py_Finalize()
#define EXIT_CODE 113
#define PIPE_PATH_NET2BASE "/home/kh/Desktop/NullProject/pipe/pipe_Network_Base"
#define PIPE_PATH_BASE2NET "/home/kh/Desktop/NullProject/pipe/pipe_Base_Network"

int main(int argc,char** argv){
	//파이썬 임베딩 변수
	PyObject *pName,*pModule,*pFunc,*pValue,*pArg;
	//파이프 통신 변수
	int Write_fd,Read_fd;
	int n_write,n_read;
	int WriteTo_pipe_data,Readfrom_pipe_data;
	int i;
	
	mkfifo(PIPE_PATH_NET2BASE,0666);
	
	if((Write_fd = open(PIPE_PATH_NET2BASE,O_WRONLY,0666))==-1){
			printf("pipe open error\n");
			return -1;
		}
	if((Read_fd = open(PIPE_PATH_BASE2NET,O_RDONLY,0666))==-1){
			printf("pipe open error\n");
			return -1;
		}
	
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(r'/home/kh/Desktop/NullProject')");
	pName = PyUnicode_FromString("Bluetooth");
	pModule = PyImport_Import(pName);
	pFunc = PyObject_GetAttrString(pModule,"Bluetooth_Connect");
	pValue = PyObject_CallObject(pFunc,NULL);
	while(true){
		pFunc = PyObject_GetAttrString(pModule,"DataReceived");
		pValue = PyObject_CallObject(pFunc,NULL);
		PyArg_Parse(pValue,"i",&WriteTo_pipe_data);
		
		if((WriteTo_pipe_data == EXIT_CODE || Readfrom_pipe_data == EXIT_CODE)){ 
				pFunc = PyObject_GetAttrString(pModule,"Bluetooth_Disconnect");
				pValue = PyObject_CallObject(pFunc,NULL);
				break;
			}
			
		if((n_write = write(Write_fd,&WriteTo_pipe_data,sizeof(WriteTo_pipe_data))) == -1){
				printf("Write Error\n");
				return -1;
			}
			printf("Producer Sent: %d\n",WriteTo_pipe_data);
				
		if((n_read = read(Read_fd,&Readfrom_pipe_data,sizeof(Readfrom_pipe_data))) == -1){
				printf("Read Error\n");
				return -1;
			}
			printf("Producer Received: %d\n",Readfrom_pipe_data);

		switch(Readfrom_pipe_data){
			case 0:
				pFunc = PyObject_GetAttrString(pModule,"DataSend_OFF");
				pValue = PyObject_CallObject(pFunc,NULL);
				break;
			case 1:
				pFunc = PyObject_GetAttrString(pModule,"DataSend_ON1");
				pValue = PyObject_CallObject(pFunc,NULL);
				break;
			case 2:
				pFunc = PyObject_GetAttrString(pModule,"DataSend_ON2");
				pValue = PyObject_CallObject(pFunc,NULL);
				break;
			case 3:
				pFunc = PyObject_GetAttrString(pModule,"DataSend_ON3");
				pValue = PyObject_CallObject(pFunc,NULL);
				break;
			default:
				break;
			
			}
	}
	Py_Finalize();
	
	close(Write_fd);
	close(Read_fd);
	return 0;
}
