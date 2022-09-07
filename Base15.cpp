#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <thread>
#include <iostream>
#include <string.h> 

#include <opencv2/opencv.hpp>
#include <time.h>

#include <sys/socket.h>
#include <arpa/inet.h>

//EXIT_CODE 는 ARD_RAS_Newwork 프로그램을 종료시키는 프로그램입니다. 종료 프로세스는 아래와 같습니다.
//Bluetooth.py에서 Bluetooth_Disconnect() 함수를 실행시켜 블루투스 소켓을 close합니다.
//ARD_RAS_Newwork 에서 통신 기능(무한 루프)에서 빠져나오며 파이썬 인터프리터를 종료시킵니다.Py_Finalize()
#define EXIT_CODE 113
#define MAX_THREAD 4
#define PIPE_PATH_NET2BASE "/home/kh/Desktop/NullProject/pipe/pipe_Network_Base"
#define PIPE_PATH_BASE2NET "/home/kh/Desktop/NullProject/pipe/pipe_Base_Network"

#define IllegalTIme 10
#define MotionTime 3
//IPC(Inter Process Communication)통신 중 Named Pipe 통신법을 사용합니다. 이는 각 파이프가 단방향 통신으로 진행됩니다.
//따라서 두 프로세스가 있을 때 한 프로세스는 파이프에 write, 다른 프로세스는 read만 하여 통신 방향을 설정해야합니다.
//아래 프로그램에서는 파이프를 두개 설정하여 두 프로세스가 양방향 통신을 하도록 구현했습니다. 
using namespace std;
using namespace cv;

int buzzer_data = 0;
int pipe_data = 0;
int motion_data1 = 0;
int motion_data2 = 0;
int motion_data3 = 0;

/**
Pipe_Communication : Bluetooth.py 프로그램과 파이프 통신 기능을 하는 함수입니다. 
Module_OpenCV : openCV기반 영상 녹화 프로그램입니다. Bluetooth.py 파일과 파이프 통신을 통해 아두이노 보드 센서로부터 데이터를 받아 상황별 동작 감지, 영상 녹화 , 저장 기능을 합니다.
TCP_Read : 사용자 애플리케이션과  TCP/IP 소켓 통신으로 경고음 on/off 데이터를 수신합니다.
TCP_Write : 사용자 애플리케이션과  TCP/IP 소켓 통신으로  Bluetooth.py로부터 받은 아두이노 보드 센서 데이터를 송신합니다. 
*/
int Pipe_Communication();
int Module_OpenCV();
int TCP_Read();
int TCP_Write();

int main(int argc, char** argv) {
    int i;
    cout << "Linux Process Num" << thread::hardware_concurrency() << endl;

/**
파이프 통신, TCP/IP통신, openCV 영상 처리 함수를 멀티 스레드로 실행합니다. 
*/
    thread CVthread[MAX_THREAD];
    CVthread[0] = thread(Pipe_Communication);
    CVthread[1] = thread(Module_OpenCV);
    CVthread[2] = thread(TCP_Read);
    CVthread[3] = thread(TCP_Write);
    CVthread[0].join();
    CVthread[1].join();
    CVthread[2].join();
    CVthread[3].join();
    return 0;
}

int Pipe_Communication() {
    int Write_fd, Read_fd;
    int n_write, n_read;
    int& WriteTo_pipe_data = buzzer_data;
    int Readfrom_pipe_data = 0;
    int i;
    int Exit_Messgae = EXIT_CODE;

    mkfifo(PIPE_PATH_BASE2NET, 0666);

    if ((Read_fd = open(PIPE_PATH_NET2BASE, O_RDONLY, 0666)) == -1) {
        printf("pipe open error\n");
        return -1;
    }
    if ((Write_fd = open(PIPE_PATH_BASE2NET, O_WRONLY, 0666)) == -1) {
        printf("pipe open error\n");
        return -1;
    }

    while (true) {
        if ((n_write = write(Write_fd, &WriteTo_pipe_data, sizeof(WriteTo_pipe_data))) == -1) {
            printf("Write Error\n");
            return -1;
        }
        printf("Consumer Sent: %d\n", WriteTo_pipe_data);

        if ((n_read = read(Read_fd, &Readfrom_pipe_data, sizeof(Readfrom_pipe_data))) == -1) {
            printf("Read Error\n");
            return -1;
        }
        printf("Consumer Received: %d\n", Readfrom_pipe_data);
        pipe_data = Readfrom_pipe_data;
    }

    if ((n_write = write(Write_fd, &Exit_Messgae, sizeof(Exit_Messgae))) == -1) {
        printf("Write Error\n");
        return -1;
    }
    printf("Consumer Sent: %d\n", Exit_Messgae);
    if ((n_read = read(Read_fd, &Readfrom_pipe_data, sizeof(Readfrom_pipe_data))) == -1) {
        printf("Read Error\n");
        return -1;
    }

    printf("Consumer Received: %d\n", Readfrom_pipe_data);


    close(Read_fd);
    close(Write_fd);
    return 0;
}



int Module_OpenCV(){

    //VideoCapture cap(0);
    int input = 0;
    int& arduino_data = pipe_data;
    int& motion_detected1 = motion_data1;
    int& motion_detected2 = motion_data2;
    int& motion_detected3 = motion_data3;
    
    VideoWriter writer11, writer21, writer31;
    VideoWriter writer12, writer22, writer32;

    Mat frameNew, frameNew_Area1, frameNew_Area2, frameNew_Area3;//두번째 프레임, 크롭된 두번째 프레임
    Mat frameNew_Area1Clone, frameNew_Area2Clone, frameNew_Area3CLone;//두번째 프레임, 크롭된 두번째 프레임
    Mat frameOld, frameOld_Area1, frameOld_Area2, frameOld_Area3;//첫번째 프레임, 크롭된 첫번째 프레임
    Mat frameDiff1, frameDiff2, frameDiff3;//두 프레임간의 차

    //  1인 상황일때 시간을 측정하여 10초 후에 영상촬영을 멈추는 알고리즘
    time_t Illegalstart1 = 0, Illegalcur1 = 0;
    time_t Illegalstart2 = 0, Illegalcur2 = 0;
    time_t Illegalstart3 = 0, Illegalcur3 = 0;
    bool isIllegalCaptured1 = 0, isIllegalCaptured2 = 0, isIllegalCaptured3 = 0;

    time_t MotionTimeStart1 = 0, MotionTimeStart2 = 0, MotionTimeStart3 = 0;
    time_t MotionTimeCur1 = 0, MotionTimeCur2 = 0, MotionTimeCur3 = 0;
    bool isMotionCaptured1 = 0, isMotionCaptured2 = 0, isMotionCaptured3 = 0;

    int Illegalcount1 = 0, Illegalcount2 = 0, Illegalcount3 = 0;
    int detectionCount1 = 0, detectionCount2 = 0, detectionCount3 = 0;



    Rect rect1(90, 34, 99, 125);
    Rect rect2(257, 36, 100, 114);
    Rect rect3(436, 38, 116, 117);

    Point corners1[2][4];
    corners1[0][0] = Point(0, 0);
    corners1[0][1] = Point(99, 0);
    corners1[0][2] = Point(99, 125);
    corners1[0][3] = Point(0, 125);

    corners1[1][0] = Point(24, 23);
    corners1[1][1] = Point(87, 22);
    corners1[1][2] = Point(78, 96);
    corners1[1][3] = Point(11, 96);
    const Point* corner1_list[2] = { corners1[0], corners1[1] };

    Point corners2[2][4];
    corners2[0][0] = Point(0, 0);
    corners2[0][1] = Point(100, 0);
    corners2[0][2] = Point(100, 114);
    corners2[0][3] = Point(0, 114);

    corners2[1][0] = Point(17, 22);
    corners2[1][1] = Point(90, 23);
    corners2[1][2] = Point(90, 98);
    corners2[1][3] = Point(13, 97);
    const Point* corner2_list[2] = { corners2[0], corners2[1] };

    Point corners3[2][4];
    corners3[0][0] = Point(0, 0);
    corners3[0][1] = Point(116, 0);
    corners3[0][2] = Point(116, 117);
    corners3[0][3] = Point(0, 117);
    
    corners3[1][0] = Point(13, 21);
    corners3[1][1] = Point(92, 23);
    corners3[1][2] = Point(100, 98);
    corners3[1][3] = Point(18, 97);
    const Point* corner3_list[2] = { corners3[0], corners3[1] };


    int num_points[] = { 4, 4 };
    int num_polygons = 2;
    int line_type = 8;


    int deviceID = 0;		//디폴트 카메라       // 매크로로 지정하기
    int apiID = CAP_ANY;	//디폴트 API           // 필요없음


    double min1, max1, min2, max2, min3, max3;
    int sensitivity = 70; //감도(그레이스케일 된 프레임의 색상차가 기준)
    VideoCapture cap(deviceID);

    //cap.open(deviceID, apiID);//카메라 사용
    cap.read(frameOld);	//첫번째 프레임을 읽어옴
    if (frameOld.empty()) return -1;



    double fps = 30.0;     // 1초에 30번 찍는다.
    //int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');
    //int codec = VideoWriter_fourcc('mp4v')
    int codec = VideoWriter::fourcc('m','p','4','v');
    pipe_data = 111;

    while (true)
    {
	motion_detected1 = 0;
	motion_detected2 = 0;
	motion_detected3 = 0;
        //cout << "openCV ardu:"<<arduino_data << endl;
        //cout << time(NULL) << endl;
        if (!cap.isOpened()) cap = VideoCapture(0);       // cap(0)
        cap.read(frameNew);//첫번째 프레임을 읽어옴
        if (frameNew.empty())
            break;

        if (!(arduino_data / 16 == 1) && writer11.isOpened() || isIllegalCaptured1) {
            writer11.release();

        }
        if (!((arduino_data / 4) % 4 == 1) && writer21.isOpened() || isIllegalCaptured2) {
            writer21.release();

        }
        if (!(arduino_data % 4 == 1) && writer31.isOpened() || isIllegalCaptured3) {
            writer31.release();

        }

        if (!(arduino_data / 16 == 2) && writer12.isOpened()) {
            writer12.release();
        }
        if (!((arduino_data / 4) % 4 == 2) && writer22.isOpened()) {
            writer22.release();
        }
        if (!(arduino_data % 4 == 2) && writer32.isOpened()) {
            writer32.release();
        }

        if ((arduino_data / 16 == 1) && !isIllegalCaptured1) {//1구역 불법주차시
            frameNew_Area1 = frameNew(rect1);
            fillPoly(frameNew_Area1, corner1_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);
            if (!writer11.isOpened()) {
                writer11 = VideoWriter(format("%sArea1 %d.mp4", "./Video /Area1 /Illegal motion/" , Illegalcount1), codec, fps, frameNew_Area1.size());
                Illegalstart1 = time(NULL);
            }
            //+frameNew_Area1 영상파일로 저장해주는 코드
            Illegalcur1 = time(NULL);
            if (Illegalcur1 <= Illegalstart1 + IllegalTIme) writer11.write(frameNew_Area1);
            else isIllegalCaptured1 = 1;
        }
        else if (arduino_data / 16 == 2) {//1구역 지정주차-동작감지
            frameNew_Area1 = frameNew(rect1);//첫번째, 두번째 프레임을 사각형 형태로 크롭
            frameOld_Area1 = frameOld(rect1);

            fillPoly(frameNew_Area1, corner1_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);//첫번째, 두번째 프레임에 같은 폴리곤을 masking하여 모션감지 예외범위를 지정
            fillPoly(frameOld_Area1, corner1_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);

            cvtColor(frameNew_Area1, frameNew_Area1Clone, COLOR_BGR2GRAY); //그레이스케일 변환
            cvtColor(frameOld_Area1, frameOld_Area1, COLOR_BGR2GRAY);
            absdiff(frameNew_Area1Clone, frameOld_Area1, frameDiff1);//두 프레임간의 차이를 비교하여 frameDiff에 출력
            minMaxLoc(frameDiff1, &min1, &max1); //frameDiff에서 최대 최소값(두 프레임의 채도차이)을 출력
           // printf("MAX 1 : %f\n", max1);

            if (max1 > sensitivity)	//정해놓은 민감도보다 채도차이가 클 경우
            {
				motion_detected1 = 16;
                if (!writer12.isOpened()) isMotionCaptured1 = 0;

                if (isMotionCaptured1 == 0) {          // = 0 이였음
                    MotionTimeStart1 = time(NULL);           // 시작 시간을 기록 VideoWrite 켠시간
                    isMotionCaptured1 = 1;
                    detectionCount1++;
		    
                }
                // VideoWriter 켬
                if (!writer12.isOpened()) writer12 = VideoWriter(format("%sArea1 Motion Detected %d.mp4", "./Video/Area1/Motion detected/", detectionCount1), codec, fps, frameNew_Area1.size());
            }
            // 현재 시간을 기록
            if (isMotionCaptured1) MotionTimeCur1 = time(NULL);
            if (max1 > sensitivity)//정해놓은 민감도보다 채도차이가 클 경우      // 모션 감지 될을때
            {
                if (isMotionCaptured1 == 1) {           // = 1 이였음
                    //+frameNew_Area1 //+frameNew_Area1 영상파일로 저장해주는 코드
                    writer12.write(frameNew_Area1);
                }
            }
            else if (MotionTimeCur1 >= MotionTimeStart1 + MotionTime) {         // 모션 감지 안됬을떄
                isMotionCaptured1 = 0;
                writer12.release();
            }
            else {
                writer12.write(frameNew_Area1);
            }
        }

        if ((arduino_data / 4) % 4 == 1 && !isIllegalCaptured2) {//2구역 불법주차시
            frameNew_Area2 = frameNew(rect2);
            fillPoly(frameNew_Area2, corner2_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);
            if (!writer21.isOpened()) {
                writer21 = VideoWriter(format("%sArea2 %d.mp4", "./Video/Area2/Illegal motion/", Illegalcount2), codec, fps, frameNew_Area2.size());
                Illegalstart2 = time(NULL);
            }

            //+frameNew_Area2 영상파일로 저장해주는 코드
            Illegalcur2 = time(NULL);
            if (Illegalcur2 <= Illegalstart2 + IllegalTIme) writer21.write(frameNew_Area2);
            else isIllegalCaptured2 = 1;
        }
        else if ((arduino_data / 4) % 4 == 2) {	//2구역 지정주차시
            frameNew_Area2 = frameNew(rect2);	//첫번째, 두번째 프레임을 사각형 형태로 크롭
            frameOld_Area2 = frameOld(rect2);

            fillPoly(frameNew_Area2, corner2_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);//첫번째, 두번째 프레임에 같은 폴리곤을 masking하여 모션감지 예외범위를 지정
            fillPoly(frameOld_Area2, corner2_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);
            cvtColor(frameNew_Area2, frameNew_Area2Clone, COLOR_BGR2GRAY);  //그레이스케일 변환
            cvtColor(frameOld_Area2, frameOld_Area2, COLOR_BGR2GRAY);
            absdiff(frameNew_Area2Clone, frameOld_Area2, frameDiff2); //두 프레임간의 차이를 비교하여 frameDiff에 출력
            minMaxLoc(frameDiff2, &min2, &max2); //frameDiff에서 최대 최소값(두 프레임의 채도차이)을 출력
            if (max2 > sensitivity)			//frameDiff에서 최대 최소값(두 프레임의 채도차이)을 출력
            {
				motion_detected2 = 4;
                if (!writer22.isOpened()) isMotionCaptured2 = 0;

                if (isMotionCaptured2 == 0) {            // = 0 이였음
                    MotionTimeStart2 = time(NULL);          // 시작 시간을 기록 VideoWrite 켠시간
                    isMotionCaptured2 = 1;
                    detectionCount2++;
                }
                // VideoWriter 켬
                if (!writer22.isOpened()) writer22 = VideoWriter(format("%sArea2 Motion Detected %d.mp4", "./Video/Area2/Motion detected/", detectionCount2), codec, fps, frameNew_Area2.size());
            }
            // 현재 시간을 기록
            if (isMotionCaptured2) MotionTimeCur2 = time(NULL);
            if (max2 > sensitivity)		//정해놓은 민감도보다 채도차이가 클 경우      // 모션 감지 될을때
            {
                if (isMotionCaptured2 == 1) {          // = 1 이였음
                    //+frameNew_Area2 영상파일로 저장해주는 코드
                    writer22.write(frameNew_Area2);
                }
            }
            else if (MotionTimeCur2 >= MotionTimeStart2 + MotionTime) {         // 모션 감지 안됬을떄
                isMotionCaptured2 = 0;
                writer22.release();
            }
            else {
                writer22.write(frameNew_Area2);
            }
        }
        if (arduino_data % 4 == 1 && !isIllegalCaptured3) {			//3구역 불법주차시
            frameNew_Area3 = frameNew(rect3);
            fillPoly(frameNew_Area3, corner3_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);
            if (!writer31.isOpened()) {
                writer31 = VideoWriter(format("%sArea3 %d.mp4", "./Video/Area3/Illegal motion/", Illegalcount3), codec, fps, frameNew_Area3.size());
                Illegalstart3 = time(NULL);
            }

            //+frameNew_Area3 영상파일로 저장해주는 코드
            Illegalcur3 = time(NULL);
            if (Illegalcur3 <= Illegalstart3 + IllegalTIme) writer31.write(frameNew_Area3);
            else isIllegalCaptured3 = 1;
        }
        else if (arduino_data % 4 == 2) {			//3구역 지정주차시
            frameNew_Area3 = frameNew(rect3);		//첫번째, 두번째 프레임을 사각형 형태로 크롭
            frameOld_Area3 = frameOld(rect3);

            fillPoly(frameNew_Area3, corner3_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);	//첫번째, 두번째 프레임에 같은 폴리곤을 masking하여 모션감지 예외범위를 지정
            fillPoly(frameOld_Area3, corner3_list, num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);
            cvtColor(frameNew_Area3, frameNew_Area3CLone, COLOR_BGR2GRAY); //그레이스케일 변환
            cvtColor(frameOld_Area3, frameOld_Area3, COLOR_BGR2GRAY);
            absdiff(frameNew_Area3CLone, frameOld_Area3, frameDiff3);  //두 프레임간의 차이를 비교하여 frameDiff에 출력
            minMaxLoc(frameDiff3, &min3, &max3);//frameDiff에서 최대 최소값(두 프레임의 채도차이)을 출력

            if (max3 > sensitivity)			//정해놓은 민감도보다 채도차이가 클 경우
            {
				motion_detected3 = 1;
                if (!writer32.isOpened()) isMotionCaptured3 = 0;

                if (isMotionCaptured3 == 0) {           // = 0 이였음
                    MotionTimeStart3 = time(NULL);          // 시작 시간을 기록 VideoWrite 켠시간
                    isMotionCaptured3 = 1;
                    detectionCount3++;
                }
                // VideoWriter 켬
                if (!writer32.isOpened()) writer32 = VideoWriter(format("%sArea3 Motion Detected %d.mp4", "./Video/Area3/Motion detected/", detectionCount3), codec, fps, frameNew_Area3.size());
            }
            // 현재 시간을 기록
            if (isMotionCaptured3) MotionTimeCur3 = time(NULL);
            if (max3 > sensitivity)			//정해놓은 민감도보다 채도차이가 클 경우      // 모션 감지 될을때
            {
                if (isMotionCaptured3 == 1) {          // = 1 이였음
                    //+frameNew_Area3 영상파일로 저장해주는 코드
                    writer32.write(frameNew_Area3);
                }
            }
            else if (MotionTimeCur3 >= MotionTimeStart3 + MotionTime) {         // 모션 감지 안됬을떄
                isMotionCaptured3 = 0;
                writer32.release();
            }
            else {
                writer32.write(frameNew_Area3);
            }
        }


        if (!(arduino_data / 16 == 1) && isIllegalCaptured1 == 1) {
            isIllegalCaptured1 = 0;
            Illegalcount1++;
        }
        if (!((arduino_data / 4) % 4 == 1) && isIllegalCaptured2 == 1) {
            isIllegalCaptured2 = 0;
            Illegalcount2++;
        }
        if (!(arduino_data % 4 == 1) && isIllegalCaptured3 == 1) {
            isIllegalCaptured3 = 0;
            Illegalcount3++;
        }

        if ((arduino_data / 16 == 1) || (arduino_data / 16 == 2)) cv::imshow("1", frameNew_Area1);
        if (((arduino_data / 4) % 4 == 1) || ((arduino_data / 4) % 4 == 2)) cv::imshow("2", frameNew_Area2);
        if ((arduino_data % 4 == 1) || (arduino_data % 4 == 2)) cv::imshow("NEW", frameNew_Area3);

        frameNew.copyTo(frameOld);

        input = waitKey(10);
        if (input == 27) break;
        else if (input == 13) {
            Illegalcount1++;
            detectionCount1++;
            Illegalcount2++;
            detectionCount2++;
            Illegalcount3++;
            detectionCount3++;
            writer11.release();
            writer21.release();
            writer31.release();
            writer12.release();
            writer22.release();
            writer32.release();
        }
    }
    cap.release();
    writer11.release();
    writer21.release();
    writer31.release();
    writer12.release();
    writer22.release();
    writer32.release();


    return 0;
}

int TCP_Read(){
	//socket 디스크립터 정의
	int serverSocket;
	socklen_t clientLen;
	struct sockaddr_in serverAddr, clientAddr;
	char mesg[BUFSIZ];
	int recv_data = 0;

	//서버 소켓 생성
	if((serverSocket = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket()");
		return -1;
	}else {
		cout<<"서버 소켓 open 완료"<<endl;
		}
	//주소 구조체에 주소 지정
	memset(&serverAddr, 0 , sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi("5400")); //사용할 포트 지정
	//bind함수를 사용하여 서버 소켓의 주소 설정
	if(bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0){
			perror("bind()");
			return -1;
		} else{
			cout<<"서버 소켓 bind 완료"<<endl;
			}
	//동시에 접속하는 클라이언트의 처리를 위한 대기 큐를 설정
	if(listen(serverSocket,8) < 0){
			perror("listen()");
			return -1;
		} else{
			cout<<"서버 소켓 listen, 대기열 큐 = 8"<<endl;
			}
	
	clientLen = sizeof(clientAddr);
	do{
		//클라이언트가 접속하면 접속을 허용하고 클라이언트 소켓 생성
		int n, clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
		//네트워크 주소를 문자열로 변경
		inet_ntop(AF_INET, &clientAddr.sin_addr,mesg,BUFSIZ);
		printf("Client is connected : %s\n", mesg);
		
		if((n = read(clientSocket,&recv_data,sizeof(int*))) <= 0)
			perror("read()");
            
        buzzer_data = recv_data;
		printf("Received data : %d\n",recv_data);
			
		close(clientSocket);
	
	} while(true);
	
	close(serverSocket);
		
	return 0;
}

int TCP_Write(){
	//socket 디스크립터 정의
	int serverSocket;
	socklen_t clientLen;
	struct sockaddr_in serverAddr, clientAddr;
	char mesg[BUFSIZ];
	int &write_data = pipe_data;
	int &write_motion1 = motion_data1;
	int &write_motion2 = motion_data2;
	int &write_motion3 = motion_data3;
	int sum_data = 0;
	//서버 소켓 생성
	if((serverSocket = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket()");
		return -1;
	}else {
		cout<<"서버 소켓 open 완료"<<endl;
		}
	//주소 구조체에 주소 지정
	memset(&serverAddr, 0 , sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi("5401")); //사용할 포트 지정
	//bind함수를 사용하여 서버 소켓의 주소 설정
	if(bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0){
			perror("bind()");
			return -1;
		} else{
			cout<<"서버 소켓 bind 완료"<<endl;
			}
	//동시에 접속하는 클라이언트의 처리를 위한 대기 큐를 설정
	if(listen(serverSocket,8) < 0){
			perror("listen()");
			return -1;
		} else{
			cout<<"서버 소켓 listen, 대기열 큐 = 8"<<endl;
			}
	
	clientLen = sizeof(clientAddr);
    
    
	do{
		//클라이언트가 접속하면 접속을 허용하고 클라이언트 소켓 생성
		int n, clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
		//네트워크 주소를 문자열로 변경
		inet_ntop(AF_INET, &clientAddr.sin_addr,mesg,BUFSIZ);
		printf("Client is connected : %s\n", mesg);
		sum_data = write_data + write_motion1 + write_motion2 + write_motion3;
		if((n = write(clientSocket,&sum_data,sizeof(int))) <= 0)
			perror("write()");
			
		printf("TCP_Write data : %d\n",sum_data);
			
		close(clientSocket);
		if(write_data == 113) break;
	} while(true);
	
	close(serverSocket);
		
	return 0;
}
