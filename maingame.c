#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h> //for Render no delay
#include <pthread.h> //for use phtread
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "./fpga_dot_font.h"

#define SEED 300 
#define MAX_BUTTON 9
#define MAX_BUFF 32 
#define LINE_BUFF 16
#define FPGA_DOT_DEVICE "/dev/fpga_dot" //dot matrix device
#define FPGA_PUSH_SWITCH_DEVICE "/dev/fpga_push_switch" // push switch device
#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd" //lcd device
#define FPGA_LED_DEVICE "/dev/fpga_led" //led device

unsigned char push_sw_buff[MAX_BUTTON]; //push_switch buffer
int buff_size; //buffer size
int push_dev;
int lcd_dev;
int led_dev;
int gamenmber = 1, selectgame = 0, pushswitch = 0;
int dev;
int soul = 1, ledsoul = 16;
int m = 0;
int a = 10;//보스 체력
int b = 3;//유저 체력
int i;
int bull;
int str_size;
int str_num;
int str_size;//도트 매트릭스 배열 사이즈
int x = 5; int y = 4; //start point;
char keyState; //키 상태(상, 하, 좌, 우)
int fx; //snake 배열 상 x 좌표값
int fy; //snake 배열 상 y 좌표값
char getch;
int UpdatePause = 0;  //update 플래그 변수
int score;
int stopHere = 0;
int endThisGame = 0;
int lcdThreadEnd = 0;
int snake[12][9] = 
{
	 82,82,82,82,82,82,82,82,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0, 0, 0, 0, 0, 0, 0, 82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0, 82
	,82,0, 0, 0, 0, 0, 0, 0, 82
	,82,0 ,0 ,0 ,0 ,0 ,0 ,0 ,82
	,82,0, 0, 0, 0, 0, 0, 0, 82
	,82,82,82,82,82,82,82,82,82

}; //12 row , 9 col 뱀 게임 배열. 이 배열을 도트 매트릭스로 변환. 82는 벽을 도트 매트릭스에 없는 벽 부분
unsigned char player[5][10] = { // 플레이어 출력
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c },//기본
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0e },//우1
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07 },//우2
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38 },//좌1
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x70 }//좌2
};
pthread_mutex_t mutex_lock;

void playermove() {
	read(push_dev, &push_sw_buff, buff_size);
	if (push_sw_buff[5] == 1) {//우버튼 누름
		if (m = 0) {
			m = 1;
		}
		else if (m = 1) {
			m = 2;
		}
		else if (m = 3) {
			m = 0;
		}
		else if (m = 4) {
			m = 3;
		}
	}
	else if (push_sw_buff[3] == 1) {
		if (m = 0) {
			m = 3;
		}
		else if (m = 3) {
			m = 4;
		}
		else if (m = 1) {
			m = 0;
		}
		else if (m = 2) {
			m = 1;
		}
	}
	else {

	}
}


void bulletmove() {
	read(push_dev, &push_sw_buff, buff_size);
	if (push_sw_buff[4] == 1) {
		if (m = 0) {
			unsigned char bull1[6][10] = {
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x1c },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x1c },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x1c },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x1c },
			{ 0x3e, 0x3e, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c },
			{ 0x3e, 0x3e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c }
			};
			for (i = 0; i < 6; i++) {
				str_size = sizeof(bull1[i]);
				write(dev, bull1[i], str_size);
				sleep(1);
			}
			a = a - 1;
		}
		else if (m = 1) {
			unsigned char bull2[6][10] = {
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x0e },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x0e },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 0x0e },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x0e },
			{ 0x3e, 0x3e, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0e },
			{ 0x3e, 0x3e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0e }
			};
			for (i = 0; i < 6; i++) {
				str_size = sizeof(bull2[i]);
				write(dev, bull2[i], str_size);
				sleep(1);
			}
			a = a - 1;
		}
		else if (m = 2) {
			unsigned char bull3[6][10] = {
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x07 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x07 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x02, 0x07 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x07 },
			{ 0x3e, 0x3e, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07 },
			{ 0x3e, 0x3e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07 }
			};
			for (i = 0; i < 6; i++) {
				str_size = sizeof(bull3[i]);
				write(dev, bull3[i], str_size);
				sleep(1);
			}
			a = a - 1;
		}
		else if (m = 3) {
			unsigned char bull4[6][10] = {
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x38 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x38 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x38 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x38 },
			{ 0x3e, 0x3e, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38 },
			{ 0x3e, 0x3e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38 }
			};
			for (i = 0; i < 6; i++) {
				str_size = sizeof(bull4[i]);
				write(dev, bull4[i], str_size);
				sleep(1);
			}
			a = a - 1;
		}
		else if (m = 4) {
			unsigned char bull5[6][10] = {
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x70 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x70 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x70 },
			{ 0x3e, 0x3e, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x70 },
	  { 0x3e, 0x3e, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70 },
					{ 0x3e, 0x3e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70 }
			};
			for (i = 0; i < 6; i++) {
				str_size = sizeof(bull5[i]);
				write(dev, bull5[i], str_size);
				sleep(1);
			}
			a = a - 1;
		}
	}

	else {

	}
}

void enemybulletready() {

	srand((unsigned int)time(NULL));
	bull = rand() % 2;
	if (bull = 1) {
		unsigned char bull6ready[10] = {//양쪽끝2,6
				0x3e, 0x3e ,0x22, 0x22,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00
		};
		str_size = sizeof(bull6ready);
		write(dev, bull6ready, str_size);
	}
	else if (bull = 2) {
		unsigned char bull7ready[10] = {
				0x3e, 0x3e ,0x08, 0x08,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00
		};
		str_size = sizeof(bull7ready);
		write(dev, bull7ready, str_size);
	}
	//중간 4
	sleep(1);
}

void enemybullet() {
	if (bull = 1) {
		unsigned char bull6[4][10] = {//양쪽끝2,6
				{0x3e, 0x3e ,0x00, 0x22,  0x22,  0x00,  0x00,  0x00,  0x00,  0x00},
				{ 0x3e, 0x3e ,0x00, 0x00,  0x22,  0x22,  0x00,  0x00,  0x00,  0x00 },
				{0x3e, 0x3e ,0x00, 0x00,  0x00,  0x22,  0x22,  0x00,  0x00,  0x00},
				{0x3e, 0x3e ,0x00, 0x00,  0x00,  0x00,  0x22,  0x22,  0x00,  0x00}
		};
		for (i = 0; i < 4; i++) {
			str_size = sizeof(bull6[i]);
			write(dev, bull6[i], str_size);
			sleep(1);
		}
		if (m = !0) {
			b = b - 1;
		}
	}
	else if (bull = 2) {
		unsigned char bull6[4][10] = {//중간
				{ 0x3e, 0x3e ,0x00, 0x08,  0x08,  0x00,  0x00,  0x00,  0x00,  0x00 },
				{ 0x3e, 0x3e ,0x00, 0x00,  0x08,  0x08,  0x00,  0x00,  0x00,  0x00 },
				{ 0x3e, 0x3e ,0x00, 0x00,  0x00,  0x08,  0x08,  0x00,  0x00,  0x00 },
				{ 0x3e, 0x3e ,0x00, 0x00,  0x00,  0x00,  0x08,  0x08,  0x00,  0x00 }
		};
		for (i = 0; i < 4; i++) {
			str_size = sizeof(bull6[i]);
			write(dev, bull6[i], str_size);
			sleep(1);
		}
		if (m == 2 || m == 4) {
			b = b - 1;
		}
	}
}

void game() {// 게임 실행

	buff_size = sizeof(push_sw_buff);

	while (1) {

		str_size = sizeof(player[m]);
		write(dev, player[m], str_size);
		sleep(1);
		playermove();
		sleep(1);
		str_size = sizeof(player[m]);
		write(dev, player[m], str_size);
		sleep(1);
		bulletmove();
		sleep(1);
		str_size = sizeof(player[m]);
		write(dev, player[m], str_size);
		sleep(1);
		enemybulletready();
		sleep(1);
		playermove();
		enemybullet();
		str_size = sizeof(player[m]);
		write(dev, player[m], str_size);
		sleep(1);
		bulletmove();
		str_size = sizeof(player[m]);
		write(dev, player[m], str_size);
		if (b = 0) {
			break;
		}
		else if (a = 0) {
			break;
		}
		sleep(1);
	}
}
void game2() {
	srand((unsigned)time(0));

	game();
	/*printMap();*/
}

int main(int argc, char** argv)
{
	int i;
	push_dev = open("/dev/fpga_push_switch", O_RDWR); //push_switch 디바이스 open
	if (push_dev < 0) {
		printf("Device open error\n");
		close(dev);
		return -1;
	}

	lcd_dev = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY); //lcd_device 디바이스 open
	if (lcd_dev < 0) {
		printf("Device open error : %s\n", FPGA_TEXT_LCD_DEVICE);
		exit(1);
	}

	led_dev = open(FPGA_LED_DEVICE, O_WRONLY); //led_deviec 디바이스 open
	if (led_dev < 0) {
		printf("Device open error : %s\n", FPGA_LED_DEVICE);
		exit(1);
	}

	void* tret = NULL;
	pthread_t thread, thread2, thread_lcd, thread_led;
	pthread_create(&thread, NULL, thread_inputUserKeymain, NULL); //방향키 입력 스레드
	pthread_create(&thread_lcd, NULL, thread_main, NULL); //lcd에 스코어 띄우는 스레드  생성
	pthread_create(&thread_led, NULL, thread_soul, NULL);
	pthread_join(thread, &tret);   //lcd end wait
	pthread_detach(thread_lcd);
	pthread_join(thread_lcd, &tret);
	while (1)
	{
		thread_soul();
		thread_inputUserKeymain();
	}

	return 0;
}

void* thread_inputUserKeymain() //푸쉬 스위치 입력
{
	unsigned char push_sw_buff[MAX_BUTTON]; //push_switch buffer
	int buff_size; //buffer size
	buff_size = sizeof(push_sw_buff);
	usleep(50000);
	read(push_dev, &push_sw_buff, buff_size);
	if (push_sw_buff[3] == 1)
	{
		push_sw_buff[3] = 0;
		gamenmber = gamenmber - 1;
		if (gamenmber == 0)
			gamenmber = 4;
		pushswitch = 0;
		thread_main();
	}
	else if (push_sw_buff[5] == 1)
	{
		push_sw_buff[5] = 0;
		gamenmber = gamenmber + 1;
		if (gamenmber == 5)
			gamenmber = 1;
		pushswitch = 0;
		thread_main();
	}
	else if (push_sw_buff[4] == 1)
	{
		push_sw_buff[4] = 0;
		selectgame = 1;
		pushswitch = 0;
		choice_game();
		selectgame = 0;
	}
	else if (push_sw_buff[0] == 1)
	{
		push_sw_buff[0] = 0;
		soul--;
		if (soul < 1)
		{
			soul = 1;
		}
		pushswitch = 0;
		thread_soul();
	}
	else if (push_sw_buff[2] == 1)
	{
		push_sw_buff[2] = 0;
		soul++;
		if (soul > 4)
		{
			soul = 4;
		}
		pushswitch = 0;
		thread_soul();
	}
	usleep(50000);
}
void print_main() //매인 매뉴 출력
{
	int len;
	char buf1[16] = "game 1";
	char buf2[16] = "game 2";
	char buf3[16] = "game 3";
	char buf4[16] = "game 4";
	unsigned char string[32];
	memset(string, 0, sizeof(string));
	if (pushswitch == 0)
	{
		switch (gamenmber)
		{
		case 1:

			len = strlen(buf1);
			if (len > 0)
			{
				strncat(string, buf1, len);
				memset(string + len, ' ', LINE_BUFF - len);
			}

			break;
		case 2:
			len = strlen(buf2);
			if (len > 0)
			{
				strncat(string, buf2, len);
				memset(string + len, ' ', LINE_BUFF - len);
			}
			break;
		case 3:
			len = strlen(buf3);
			if (len > 0)
			{
				strncat(string, buf3, len);
				memset(string + len, ' ', LINE_BUFF - len);
			}
			break;
		case 4:
			len = strlen(buf4);
			if (len > 0)
			{
				strncat(string, buf4, len);
				memset(string + len, ' ', LINE_BUFF - len);
			}
			break;
		default:
			break;
		}
		write(lcd_dev, string, MAX_BUFF);
		pushswitch = 1;
	}
}
void choice_game() //게임 선택
{
	if (pushswitch == 0)
	{
		if (selectgame == 1)
		{
			switch (gamenmber)
			{
			case 1:
				str_size = sizeof(fpga_number); //헤더 파일에 있는 도트 매트릭스 배열에 있는 배열 size를 구함
				write(dev, fpga_number, str_size); //초기 화면 write
				snakegame();
			case 2:
				game2();
				break;
			default:
				break;
			}
		}
		pushswitch = 1;
		selectgame = 0;
	}
}
void* thread_main() 
{
	print_main();
}
void* thread_soul() //목숨에 따라 led등 키기
{
	switch (soul)
	{
	case 0:
		ledsoul = 0;
		break;
	case 1:
		ledsoul = 16;
		break;
	case 2:
		ledsoul = 48;
		break;
	case 3:
		ledsoul = 112;
		break;
	case 4:
		ledsoul = 240;
		break;
	default:
		break;
	}
	write(led_dev, &ledsoul, 1);
	pushswitch = 1;
}
void souldec() //목숨 감소
{
	soul--;
	pushswitch = 0;
	if (soul == 0)
	{
		gameover();
	}
	thread_soul();
}
void gameover() // 게임오버 출력
{
	int len;
	char buf1[16] = "game over";
	unsigned char string[32];
	memset(string, 0, sizeof(string));
	len = strlen(buf1);
	if (len > 0)
	{
		strncat(string, buf1, len);
		memset(string + len, ' ', LINE_BUFF - len);
	}
	write(lcd_dev, string, MAX_BUFF);
	pushswitch = 1;
}

void init() { //make snake's way 0
	x = 5; y = 4;
	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 8; j++) {
			snake[i][j] = 0;
		}
	}
}
void init_fpga() {  //도트 매트릭스 배열 초기화 함수
	for (int i = 0; i < 10; i++) {
		fpga_number[i] = 0x00;
	}

}
int convert_Dot() { //snake 배열을 도트 매트릭스로 매핑(변환)하는 함수. 
	int i, j;
	unsigned char tmp1 = 0x00, tmp2 = 0x00; //tmp1은 위 3자리 도트, tmp2는 아래 4자리 도트
	for (i = 1; i < 11; i++) { //snake배열은 12*9 배열이므로 도트 매트릭스로 변환하기 위해 벽을 제외한 1~10까지만 반복
		for (j = 1; j < 8; j++) { //마찬가지로 벽을 제외하고 1에서 7까지만 반복
			if (snake[i][j] > 0) { //snake배열에서 빈 공간이 아닐 때, 즉 뱀 몸통이거나 먹이일 경우 
				if (j < 4) { //위 3자리
					switch (j) {
					case 1: tmp1 += 0x40; break; //최상위 비트일 경우 +4
					case 2: tmp1 += 0x20; break; //두 번째 비트일 경우 +2
					case 3: tmp1 += 0x10; break; //첫 번째 비트일 경우 +1
					}
				}
				else { //아래 4자리
					switch (j) {
					case 4: tmp2 += 0x08; break; //아래 최상위(4번째)비트일 경우 +8
					case 5: tmp2 += 0x04; break; //두 번째 비트일 경우 +4
					case 6: tmp2 += 0x02; break; //세 번째 비트일 경우 +2
					case 7: tmp2 += 0x01; break; //첫 번째 비트일 경우 +1
					}
				}
			}
		}

		fpga_number[i - 1] = tmp1 + tmp2; //헤더 파일에 있는 도트 매트릭스 배열에 tmp1+tmp2값을 대입
		tmp1 = 0;//초기화
		tmp2 = 0;
	}
}
void makeFood(int seed) {
	// in 11,8 -> make food random
	srand(seed);
	fx = (rand() % 10) + 1; //rand 함수로 랜덤한 위치 x좌표 생성
	fy = (rand() % 7) + 1; //마찬가지로 y좌표 생성
	if (snake[fx][fy] == 0) { //그 좌표가 0일 경우에만
		snake[fx][fy] = 77; //먹이 생성(먹이는 배열 상 77로 표시됨)
	}
	else { makeFood(seed + 123); } //그 자리에 뱀 몸통이 있을 경우 다시 재귀하여 다른 자리에 생성

	//recursion -> make stack overflow, (gcc's stack mem = 2mb, if srand makes same result, then overflow)
}//먹이를 만드는 함수
void startPoint() {
	snake[5][4] = 3; snake[6][4] = 2; snake[7][4] = 1;
	//head start point 5,4
	makeFood(SEED); //먹이는 랜덤으로 생성
}//시작 포인트 정의(항상 고정
void ThreadEnd() {//스레드 종료시키는 함수

	static int retval = 999;
	//sleep(10000);
	printf("thread is end");
	pthread_exit((void*)&retval);

}
void goToUp() { //뱀 머리를 위로 보내는 함수. 아래 goToDown 등의 함수들 같은 방식으로 동작(x,y좌표만 다르게 변경)

	int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen

   //crash trigger
	if (snake[x - 1][y] > 77) //벽에 부딫혔을 경우
	{
		printf("    Game over :: Wall Crash   \n");
		pthread_mutex_lock(&mutex_lock); //mutex
		stopHere = 1; //게임 stop
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (0 < snake[x - 1][y] && snake[x - 1][y] < 70) //자신의 몸통에 부딫혔을 경우
	{
		printf("  Game over :: Body Crash \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1; //게임 stop
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (snake[x - 1][y] == 77) //먹이를 먹었을 경우
	{	//printf("find food\n");
		snake[x - 1][y] = snake[x][y] + 1; //기존 뱀 머리 값 위의 좌표에 +1
		x = x - 1;
		noMinus = 0; //기존 꼬리가 이동하지 않음.
		//makeLCD();
		makeFood(SEED); //새 먹이 생성
		score += 100; //스코어 + 100점
		update_score(score); //스코어 lcd에 업데이트
	}
	else {
		//does not happen crash
		snake[x - 1][y] = snake[x][y] + 1; //그냥 이동했을 경우 
		x = x - 1; //위로 한칸 이동(x축-1을 x에 대입)
	}


	if (noMinus == 1) { //그냥 이동했을 경우
		for (int i = 1; i < 11; i++) { //모든 배열에 대해서
			for (int j = 1; j < 8; j++) {
				if (snake[i][j] > 0) { //빈 공간이 아니고
					if (snake[i][j] != 77)  //먹이도 아닐 때, 즉 뱀 객체일 때
						snake[i][j] = snake[i][j] - 1; //뱀 몸통에서 배열 값을 1씩 뺌. 
				}

			}
		}
	}

	noMinus = 1;
	//crash trigger. 

}
void goToDown() {
	int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen

  //crash trigger
	if (snake[x + 1][y] > 77)
	{
		printf("    Game over :: Wall Crash   \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (0 < snake[x + 1][y] && snake[x + 1][y] < 70)
	{
		printf("  Game over :: Body Crash \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (snake[x + 1][y] == 77)
	{	//printf("find food");
		snake[x + 1][y] = snake[x][y] + 1; //기존 뱀 머리값 아래 x좌표에 +1
		x = x + 1;
		noMinus = 0;
		makeFood(SEED);
		score += 100;
		update_score(score);
	}
	else {
		snake[x + 1][y] = snake[x][y] + 1;
		x = x + 1; //뱀을 한칸 아래로 보냄
	}

	if (noMinus == 1) {
		for (int i = 1; i < 11; i++) {
			for (int j = 1; j < 8; j++) {
				if (snake[i][j] > 0) {
					if (snake[i][j] != 77)
						snake[i][j] = snake[i][j] - 1;
				}

			}
		}
	}

	noMinus = 1;
	//crash trigger. 

}
void goToLeft() {
	int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen

  //crash trigger
	if (snake[x][y - 1] > 77)
	{
		printf("    Game over :: Wall Crash   \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (0 < snake[x][y - 1] && snake[x][y - 1] < 70)
	{
		printf("  Game over :: Body Crash \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (snake[x][y - 1] == 77)
	{	//printf("find food");
		snake[x][y - 1] = snake[x][y] + 1;//기존 뱀 머리값 왼쪽 y좌표에 +1
		y = y - 1;
		noMinus = 0;
		makeFood(SEED);
		score += 100;
		update_score(score);
	}
	else {
		snake[x][y - 1] = snake[x][y] + 1;
		y = y - 1;//뱀을 한칸 왼쪽으로 보냄
	}

	if (noMinus == 1) {
		for (int i = 1; i < 11; i++) {
			for (int j = 1; j < 8; j++) {
				if (snake[i][j] > 0) {
					if (snake[i][j] != 77)
						snake[i][j] = snake[i][j] - 1;
				}

			}
		}
	}

	noMinus = 1;
	//crash trigger. 

}
void goToRight() {
	int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen

	//crash trigger
	if (snake[x][y + 1] > 77)
	{
		printf("    Game over :: Wall Crash   \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);
		souldec();
		//gameoverTrigger();
	}
	else if (0 < snake[x][y + 1] && snake[x][y + 1] < 70)
	{
		printf("  Game over :: Body Crash \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1;
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (snake[x][y + 1] == 77)
	{	//printf("find food");
		snake[x][y + 1] = snake[x][y] + 1;//기존 뱀 머리값 오른쪽 y좌표에 +1
		y = y + 1;
		noMinus = 0;
		makeFood(SEED);
		score += 100;
		update_score(score);
	}
	else {
		snake[x][y + 1] = snake[x][y] + 1;
		y = y + 1;//뱀을 한칸 왼쪽으로 보냄
	}

	if (noMinus == 1) {
		for (int i = 1; i < 11; i++) {
			for (int j = 1; j < 8; j++) {
				if (snake[i][j] > 0) {
					if (snake[i][j] != 77)
						snake[i][j] = snake[i][j] - 1;
				}

			}
		}
		noMinus = 0;
	}

	noMinus = 1;
}
void Render() { //print game window 콘솔창에 배열 출력하는 함수

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 9; j++) {
			printf("[%2d]", snake[i][j]);
		}

		printf("\n");
	}
}
void* thread_inputUserKey() { //사용자 입력을 받는 함수(push_switch 사용)
	buff_size = sizeof(push_sw_buff); //push_switch_buffer
	while (1) {
		usleep(50000);//입력 딜레이. 텀을 주지 않으면 제대로 read하지 못함.
		if (stopHere != 1) {
			read(push_dev, &push_sw_buff, buff_size); //push_switch 값을 read함
		}
		if (push_sw_buff[1] == 1) { //push_switch 값이 1, 즉 9개 중 1행 2열 스위치일 경우(위쪽)
			pthread_mutex_lock(&mutex_lock);
			if (keyState == 's') { //기존 뱀이 아래로 가고 있을 경우 변하지 않음
				keyState = 's';
			}
			else { keyState = 'w'; } //아래로 가고 있지 않았을 경우 뱀 방향을 위쪽으로 변경

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[3] == 1) { //push_switch 값이 3, 즉 9개 중 2행 1열 스위치일 경우(왼쪽)
			pthread_mutex_lock(&mutex_lock);
			if (keyState == 'd') { //기존 뱀이 오른쪽으로 가고 있을 경우 변하지 않음
				keyState = 'd';
			}
			else { keyState = 'a'; } //방향을 왼쪽으로 변경

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[4] == 1) {//push_switch 값이 4, 즉 9개 중 2행 2열 스위치일 경우(아래쪽)
			pthread_mutex_lock(&mutex_lock);
			if (keyState == 'w') { //기존 뱀이 위쪽으로 가고 있을 경우 변하지 않음
				keyState = 'w';
			}
			else { keyState = 's'; } //방향을 아래쪽으로 변경

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[5] == 1) { //push_switch 값이 5, 즉 9개 중 2행 3열 스위치일 경우(오른쪽)

			pthread_mutex_lock(&mutex_lock);
			if (keyState == 'a') { //기존 뱀이 왼쪽으로 가고 있을 경우 변하지 않음
				keyState = 'a';
			}
			else { keyState = 'd'; }  //방향을 오른쪽으로 변경

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[8] == 1) { //3행 3열의 스위치일 경우 게임오버 되었을때 게임 재시작을 위한 버튼으로 사용
			push_sw_buff[8] = 0;
			pthread_exit((void*)2);
			pthread_mutex_lock(&mutex_lock);
			keyState = 'p';
			lcdThreadEnd = 1;
			pthread_mutex_unlock(&mutex_lock);
			break;
		}
		else if (push_sw_buff[6] == 1) { //게임 종료 버튼
			pthread_mutex_lock(&mutex_lock);
			endThisGame = 1;
		}
		usleep(50000);

	}
}
void* thread_whichWay() { //뱀의 방향에 따라 뱀을 움직이고, 도트 매트릭스에 출력시키는 스레드
	while (1) {

		if (stopHere == 1) { //죽었을 경우 멈춤
			break;
		}

		switch (keyState) //키 상태에 따라서(상,하,좌,우)
		{
		case 'w': //스위치 입력이 위 방향일 경우
			do {
				goToUp(); //뱀 위로 움직임
				system("clear"); //콘솔 클리어
				Render(); //콘솔에 배열 다시 출력
				convert_Dot(); //snake 배열을 도트 매트릭스로 매핑
				write(dev, fpga_number, str_size); //도트 매트릭스에 write
				usleep(400000); //0.4초의 간격으로 뱀이 움직임
			} while (keyState == 'w' && stopHere == 0); //죽지 않거나 키 상태가 위쪽일 경우에만 반복
			break;

		case 's': //스위치 입력이 아래 방향일 경우
			do {
				goToDown(); //뱀 아래로 움직임
				system("clear");
				Render();
				convert_Dot(); //snake 배열을 도트 매트릭스로 매핑
				write(dev, fpga_number, str_size); //도트 매트릭스에 write
				usleep(400000); //0.4초 간격
			} while (keyState == 's' && stopHere == 0); //죽지 않거나 키 상태가 아래일 경우에만 반복
			break;

		case 'a': //스위치 입력이 왼쪽 방향일 경우
			do {
				goToLeft(); //뱀 왼쪽으로 움직임
				system("clear");
				Render();
				convert_Dot(); //snake 배열을 도트 매트릭스로 매핑
				write(dev, fpga_number, str_size); //도트 매트릭스에 write
				usleep(400000); //0.4초 간격
			} while (keyState == 'a' && stopHere == 0); //죽지 않거나 키 상태가 왼쪽일 경우에만 반복
			break;
		case 'd': //스위치 입력이 오른쪽 방향일 경우
			do {
				goToRight(); //뱀 오른쪽으로 움직임
				system("clear");
				Render();
				convert_Dot(); //snake 배열을 도트 매트릭스로 매핑
				write(dev, fpga_number, str_size); //도트 매트릭스에 write
				usleep(400000); //0.4초 간격
			} while (keyState == 'd' && stopHere == 0); //죽지 않거나 키 상태가 오른쪽일 경우에만 반복
			break;
		default:
			break;
		}


	}
	printf("\n Game over Restart??? \n"); //죽었을 경우에는 모든 반복문을 빠져나옴

	//printf(" key state => : %c",keyState);

	pthread_mutex_lock(&mutex_lock);
	stopHere = 0;
	pthread_mutex_unlock(&mutex_lock);
	pthread_exit((void*)3);
}
void* thread_score() {
	update_score(0);
}
void update_score(int num) {	// score를 업데이트 하는 함수. 뱀이 먹이를 먹을 때마다 업데이트
	char buf1[16] = "snake game!";	//LCD 첫 번째 줄
	char buf2[16] = "score : ";		//LCD 두 번째 줄
	char buf3[5] = "";
	int len;

	unsigned char string[32];
	memset(string, 0, sizeof(string));	// string 초기화
	sprintf(buf3, "%d", num);	// int형 변수 num을 문자열로 변환하여 buf3에 저장
	strncat(buf2, buf3, sizeof(buf3));

	len = strlen(buf1);
	if (len > 0) {
		strncat(string, buf1, len);
		memset(string + len, ' ', LINE_BUFF - len);	// 첫 번째 줄의 나머지를 ' '로 채움
	}

	len = strlen(buf2);
	if (len > 0) {
		strncat(string, buf2, len);
		memset(string + LINE_BUFF + len, ' ', LINE_BUFF - len);	// 두 번째 줄의 나머지를 ' '로 채움
	}

	write(lcd_dev, string, MAX_BUFF);
}
void StartGame() { //게임 시작하는 함수

	void* tret = NULL;
	score = 0;

	init(); //배열 초기화
	startPoint(); //시작 지점
	Render();

	printf("xy : %d \n", snake[x][y]);

	pthread_t thread, thread2, thread_lcd;
	pthread_create(&thread, NULL, thread_inputUserKey, NULL); //방향키 입력 스레드
	pthread_create(&thread2, NULL, thread_whichWay, NULL); //도트 매트릭스에 뱀 움직이는 스레드
	pthread_create(&thread_lcd, NULL, thread_score, NULL); //lcd에 스코어 띄우는 스레드  생성


	printf("Snake Game\n");

	pthread_join(thread2, &tret);
	pthread_join(thread, &tret);   //lcd end wait
	pthread_detach(thread_lcd);
	pthread_join(thread_lcd, &tret);
	//스레드 3개 모두 동작(join)


	if (endThisGame != 1 || soul != 0) {
		StartGame(); //게임 재시작을 위한 재귀함수 호출 p를 누르면 모든 스레드가 join(종료)하면서 다시 StartGame이 호출됨.
	}

	printf("all thread Stopped, game end c'u next time!");

}
void snakegame()
{
	dev = open(FPGA_DOT_DEVICE, O_WRONLY); //Dot_Matrix 디바이스 open
	if (dev < 0) {
		printf("Device open error : %s\n", FPGA_DOT_DEVICE);
		exit(1);
	}
	str_size = sizeof(fpga_number); //헤더 파일에 있는 도트 매트릭스 배열에 있는 배열 size를 구함
	write(dev, fpga_number, str_size); //초기 화면 write
	StartGame(); //게임 시작
	gamenmber = 1, selectgame = 0, pushswitch = 1;
}