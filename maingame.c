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
int a = 10;//���� ü��
int b = 3;//���� ü��
int i;
int bull;
int str_size;
int str_num;
int str_size;//��Ʈ ��Ʈ���� �迭 ������
int x = 5; int y = 4; //start point;
char keyState; //Ű ����(��, ��, ��, ��)
int fx; //snake �迭 �� x ��ǥ��
int fy; //snake �迭 �� y ��ǥ��
char getch;
int UpdatePause = 0;  //update �÷��� ����
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

}; //12 row , 9 col �� ���� �迭. �� �迭�� ��Ʈ ��Ʈ������ ��ȯ. 82�� ���� ��Ʈ ��Ʈ������ ���� �� �κ�
unsigned char player[5][10] = { // �÷��̾� ���
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c },//�⺻
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0e },//��1
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07 },//��2
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38 },//��1
		{ 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x70 }//��2
};
pthread_mutex_t mutex_lock;

void playermove() {
	read(push_dev, &push_sw_buff, buff_size);
	if (push_sw_buff[5] == 1) {//���ư ����
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
		unsigned char bull6ready[10] = {//���ʳ�2,6
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
	//�߰� 4
	sleep(1);
}

void enemybullet() {
	if (bull = 1) {
		unsigned char bull6[4][10] = {//���ʳ�2,6
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
		unsigned char bull6[4][10] = {//�߰�
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

void game() {// ���� ����

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
	push_dev = open("/dev/fpga_push_switch", O_RDWR); //push_switch ����̽� open
	if (push_dev < 0) {
		printf("Device open error\n");
		close(dev);
		return -1;
	}

	lcd_dev = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY); //lcd_device ����̽� open
	if (lcd_dev < 0) {
		printf("Device open error : %s\n", FPGA_TEXT_LCD_DEVICE);
		exit(1);
	}

	led_dev = open(FPGA_LED_DEVICE, O_WRONLY); //led_deviec ����̽� open
	if (led_dev < 0) {
		printf("Device open error : %s\n", FPGA_LED_DEVICE);
		exit(1);
	}

	void* tret = NULL;
	pthread_t thread, thread2, thread_lcd, thread_led;
	pthread_create(&thread, NULL, thread_inputUserKeymain, NULL); //����Ű �Է� ������
	pthread_create(&thread_lcd, NULL, thread_main, NULL); //lcd�� ���ھ� ���� ������  ����
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

void* thread_inputUserKeymain() //Ǫ�� ����ġ �Է�
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
void print_main() //���� �Ŵ� ���
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
void choice_game() //���� ����
{
	if (pushswitch == 0)
	{
		if (selectgame == 1)
		{
			switch (gamenmber)
			{
			case 1:
				str_size = sizeof(fpga_number); //��� ���Ͽ� �ִ� ��Ʈ ��Ʈ���� �迭�� �ִ� �迭 size�� ����
				write(dev, fpga_number, str_size); //�ʱ� ȭ�� write
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
void* thread_soul() //����� ���� led�� Ű��
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
void souldec() //��� ����
{
	soul--;
	pushswitch = 0;
	if (soul == 0)
	{
		gameover();
	}
	thread_soul();
}
void gameover() // ���ӿ��� ���
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
void init_fpga() {  //��Ʈ ��Ʈ���� �迭 �ʱ�ȭ �Լ�
	for (int i = 0; i < 10; i++) {
		fpga_number[i] = 0x00;
	}

}
int convert_Dot() { //snake �迭�� ��Ʈ ��Ʈ������ ����(��ȯ)�ϴ� �Լ�. 
	int i, j;
	unsigned char tmp1 = 0x00, tmp2 = 0x00; //tmp1�� �� 3�ڸ� ��Ʈ, tmp2�� �Ʒ� 4�ڸ� ��Ʈ
	for (i = 1; i < 11; i++) { //snake�迭�� 12*9 �迭�̹Ƿ� ��Ʈ ��Ʈ������ ��ȯ�ϱ� ���� ���� ������ 1~10������ �ݺ�
		for (j = 1; j < 8; j++) { //���������� ���� �����ϰ� 1���� 7������ �ݺ�
			if (snake[i][j] > 0) { //snake�迭���� �� ������ �ƴ� ��, �� �� �����̰ų� ������ ��� 
				if (j < 4) { //�� 3�ڸ�
					switch (j) {
					case 1: tmp1 += 0x40; break; //�ֻ��� ��Ʈ�� ��� +4
					case 2: tmp1 += 0x20; break; //�� ��° ��Ʈ�� ��� +2
					case 3: tmp1 += 0x10; break; //ù ��° ��Ʈ�� ��� +1
					}
				}
				else { //�Ʒ� 4�ڸ�
					switch (j) {
					case 4: tmp2 += 0x08; break; //�Ʒ� �ֻ���(4��°)��Ʈ�� ��� +8
					case 5: tmp2 += 0x04; break; //�� ��° ��Ʈ�� ��� +4
					case 6: tmp2 += 0x02; break; //�� ��° ��Ʈ�� ��� +2
					case 7: tmp2 += 0x01; break; //ù ��° ��Ʈ�� ��� +1
					}
				}
			}
		}

		fpga_number[i - 1] = tmp1 + tmp2; //��� ���Ͽ� �ִ� ��Ʈ ��Ʈ���� �迭�� tmp1+tmp2���� ����
		tmp1 = 0;//�ʱ�ȭ
		tmp2 = 0;
	}
}
void makeFood(int seed) {
	// in 11,8 -> make food random
	srand(seed);
	fx = (rand() % 10) + 1; //rand �Լ��� ������ ��ġ x��ǥ ����
	fy = (rand() % 7) + 1; //���������� y��ǥ ����
	if (snake[fx][fy] == 0) { //�� ��ǥ�� 0�� ��쿡��
		snake[fx][fy] = 77; //���� ����(���̴� �迭 �� 77�� ǥ�õ�)
	}
	else { makeFood(seed + 123); } //�� �ڸ��� �� ������ ���� ��� �ٽ� ����Ͽ� �ٸ� �ڸ��� ����

	//recursion -> make stack overflow, (gcc's stack mem = 2mb, if srand makes same result, then overflow)
}//���̸� ����� �Լ�
void startPoint() {
	snake[5][4] = 3; snake[6][4] = 2; snake[7][4] = 1;
	//head start point 5,4
	makeFood(SEED); //���̴� �������� ����
}//���� ����Ʈ ����(�׻� ����
void ThreadEnd() {//������ �����Ű�� �Լ�

	static int retval = 999;
	//sleep(10000);
	printf("thread is end");
	pthread_exit((void*)&retval);

}
void goToUp() { //�� �Ӹ��� ���� ������ �Լ�. �Ʒ� goToDown ���� �Լ��� ���� ������� ����(x,y��ǥ�� �ٸ��� ����)

	int noMinus = 1; //when snake eat food then arrary's -1 shouldn't happen

   //crash trigger
	if (snake[x - 1][y] > 77) //���� �΋H���� ���
	{
		printf("    Game over :: Wall Crash   \n");
		pthread_mutex_lock(&mutex_lock); //mutex
		stopHere = 1; //���� stop
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (0 < snake[x - 1][y] && snake[x - 1][y] < 70) //�ڽ��� ���뿡 �΋H���� ���
	{
		printf("  Game over :: Body Crash \n");
		pthread_mutex_lock(&mutex_lock);
		stopHere = 1; //���� stop
		pthread_mutex_unlock(&mutex_lock);
		souldec();
	}
	else if (snake[x - 1][y] == 77) //���̸� �Ծ��� ���
	{	//printf("find food\n");
		snake[x - 1][y] = snake[x][y] + 1; //���� �� �Ӹ� �� ���� ��ǥ�� +1
		x = x - 1;
		noMinus = 0; //���� ������ �̵����� ����.
		//makeLCD();
		makeFood(SEED); //�� ���� ����
		score += 100; //���ھ� + 100��
		update_score(score); //���ھ� lcd�� ������Ʈ
	}
	else {
		//does not happen crash
		snake[x - 1][y] = snake[x][y] + 1; //�׳� �̵����� ��� 
		x = x - 1; //���� ��ĭ �̵�(x��-1�� x�� ����)
	}


	if (noMinus == 1) { //�׳� �̵����� ���
		for (int i = 1; i < 11; i++) { //��� �迭�� ���ؼ�
			for (int j = 1; j < 8; j++) {
				if (snake[i][j] > 0) { //�� ������ �ƴϰ�
					if (snake[i][j] != 77)  //���̵� �ƴ� ��, �� �� ��ü�� ��
						snake[i][j] = snake[i][j] - 1; //�� ���뿡�� �迭 ���� 1�� ��. 
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
		snake[x + 1][y] = snake[x][y] + 1; //���� �� �Ӹ��� �Ʒ� x��ǥ�� +1
		x = x + 1;
		noMinus = 0;
		makeFood(SEED);
		score += 100;
		update_score(score);
	}
	else {
		snake[x + 1][y] = snake[x][y] + 1;
		x = x + 1; //���� ��ĭ �Ʒ��� ����
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
		snake[x][y - 1] = snake[x][y] + 1;//���� �� �Ӹ��� ���� y��ǥ�� +1
		y = y - 1;
		noMinus = 0;
		makeFood(SEED);
		score += 100;
		update_score(score);
	}
	else {
		snake[x][y - 1] = snake[x][y] + 1;
		y = y - 1;//���� ��ĭ �������� ����
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
		snake[x][y + 1] = snake[x][y] + 1;//���� �� �Ӹ��� ������ y��ǥ�� +1
		y = y + 1;
		noMinus = 0;
		makeFood(SEED);
		score += 100;
		update_score(score);
	}
	else {
		snake[x][y + 1] = snake[x][y] + 1;
		y = y + 1;//���� ��ĭ �������� ����
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
void Render() { //print game window �ܼ�â�� �迭 ����ϴ� �Լ�

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 9; j++) {
			printf("[%2d]", snake[i][j]);
		}

		printf("\n");
	}
}
void* thread_inputUserKey() { //����� �Է��� �޴� �Լ�(push_switch ���)
	buff_size = sizeof(push_sw_buff); //push_switch_buffer
	while (1) {
		usleep(50000);//�Է� ������. ���� ���� ������ ����� read���� ����.
		if (stopHere != 1) {
			read(push_dev, &push_sw_buff, buff_size); //push_switch ���� read��
		}
		if (push_sw_buff[1] == 1) { //push_switch ���� 1, �� 9�� �� 1�� 2�� ����ġ�� ���(����)
			pthread_mutex_lock(&mutex_lock);
			if (keyState == 's') { //���� ���� �Ʒ��� ���� ���� ��� ������ ����
				keyState = 's';
			}
			else { keyState = 'w'; } //�Ʒ��� ���� ���� �ʾ��� ��� �� ������ �������� ����

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[3] == 1) { //push_switch ���� 3, �� 9�� �� 2�� 1�� ����ġ�� ���(����)
			pthread_mutex_lock(&mutex_lock);
			if (keyState == 'd') { //���� ���� ���������� ���� ���� ��� ������ ����
				keyState = 'd';
			}
			else { keyState = 'a'; } //������ �������� ����

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[4] == 1) {//push_switch ���� 4, �� 9�� �� 2�� 2�� ����ġ�� ���(�Ʒ���)
			pthread_mutex_lock(&mutex_lock);
			if (keyState == 'w') { //���� ���� �������� ���� ���� ��� ������ ����
				keyState = 'w';
			}
			else { keyState = 's'; } //������ �Ʒ������� ����

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[5] == 1) { //push_switch ���� 5, �� 9�� �� 2�� 3�� ����ġ�� ���(������)

			pthread_mutex_lock(&mutex_lock);
			if (keyState == 'a') { //���� ���� �������� ���� ���� ��� ������ ����
				keyState = 'a';
			}
			else { keyState = 'd'; }  //������ ���������� ����

			pthread_mutex_unlock(&mutex_lock);
		}
		else if (push_sw_buff[8] == 1) { //3�� 3���� ����ġ�� ��� ���ӿ��� �Ǿ����� ���� ������� ���� ��ư���� ���
			push_sw_buff[8] = 0;
			pthread_exit((void*)2);
			pthread_mutex_lock(&mutex_lock);
			keyState = 'p';
			lcdThreadEnd = 1;
			pthread_mutex_unlock(&mutex_lock);
			break;
		}
		else if (push_sw_buff[6] == 1) { //���� ���� ��ư
			pthread_mutex_lock(&mutex_lock);
			endThisGame = 1;
		}
		usleep(50000);

	}
}
void* thread_whichWay() { //���� ���⿡ ���� ���� �����̰�, ��Ʈ ��Ʈ������ ��½�Ű�� ������
	while (1) {

		if (stopHere == 1) { //�׾��� ��� ����
			break;
		}

		switch (keyState) //Ű ���¿� ����(��,��,��,��)
		{
		case 'w': //����ġ �Է��� �� ������ ���
			do {
				goToUp(); //�� ���� ������
				system("clear"); //�ܼ� Ŭ����
				Render(); //�ֿܼ� �迭 �ٽ� ���
				convert_Dot(); //snake �迭�� ��Ʈ ��Ʈ������ ����
				write(dev, fpga_number, str_size); //��Ʈ ��Ʈ������ write
				usleep(400000); //0.4���� �������� ���� ������
			} while (keyState == 'w' && stopHere == 0); //���� �ʰų� Ű ���°� ������ ��쿡�� �ݺ�
			break;

		case 's': //����ġ �Է��� �Ʒ� ������ ���
			do {
				goToDown(); //�� �Ʒ��� ������
				system("clear");
				Render();
				convert_Dot(); //snake �迭�� ��Ʈ ��Ʈ������ ����
				write(dev, fpga_number, str_size); //��Ʈ ��Ʈ������ write
				usleep(400000); //0.4�� ����
			} while (keyState == 's' && stopHere == 0); //���� �ʰų� Ű ���°� �Ʒ��� ��쿡�� �ݺ�
			break;

		case 'a': //����ġ �Է��� ���� ������ ���
			do {
				goToLeft(); //�� �������� ������
				system("clear");
				Render();
				convert_Dot(); //snake �迭�� ��Ʈ ��Ʈ������ ����
				write(dev, fpga_number, str_size); //��Ʈ ��Ʈ������ write
				usleep(400000); //0.4�� ����
			} while (keyState == 'a' && stopHere == 0); //���� �ʰų� Ű ���°� ������ ��쿡�� �ݺ�
			break;
		case 'd': //����ġ �Է��� ������ ������ ���
			do {
				goToRight(); //�� ���������� ������
				system("clear");
				Render();
				convert_Dot(); //snake �迭�� ��Ʈ ��Ʈ������ ����
				write(dev, fpga_number, str_size); //��Ʈ ��Ʈ������ write
				usleep(400000); //0.4�� ����
			} while (keyState == 'd' && stopHere == 0); //���� �ʰų� Ű ���°� �������� ��쿡�� �ݺ�
			break;
		default:
			break;
		}


	}
	printf("\n Game over Restart??? \n"); //�׾��� ��쿡�� ��� �ݺ����� ��������

	//printf(" key state => : %c",keyState);

	pthread_mutex_lock(&mutex_lock);
	stopHere = 0;
	pthread_mutex_unlock(&mutex_lock);
	pthread_exit((void*)3);
}
void* thread_score() {
	update_score(0);
}
void update_score(int num) {	// score�� ������Ʈ �ϴ� �Լ�. ���� ���̸� ���� ������ ������Ʈ
	char buf1[16] = "snake game!";	//LCD ù ��° ��
	char buf2[16] = "score : ";		//LCD �� ��° ��
	char buf3[5] = "";
	int len;

	unsigned char string[32];
	memset(string, 0, sizeof(string));	// string �ʱ�ȭ
	sprintf(buf3, "%d", num);	// int�� ���� num�� ���ڿ��� ��ȯ�Ͽ� buf3�� ����
	strncat(buf2, buf3, sizeof(buf3));

	len = strlen(buf1);
	if (len > 0) {
		strncat(string, buf1, len);
		memset(string + len, ' ', LINE_BUFF - len);	// ù ��° ���� �������� ' '�� ä��
	}

	len = strlen(buf2);
	if (len > 0) {
		strncat(string, buf2, len);
		memset(string + LINE_BUFF + len, ' ', LINE_BUFF - len);	// �� ��° ���� �������� ' '�� ä��
	}

	write(lcd_dev, string, MAX_BUFF);
}
void StartGame() { //���� �����ϴ� �Լ�

	void* tret = NULL;
	score = 0;

	init(); //�迭 �ʱ�ȭ
	startPoint(); //���� ����
	Render();

	printf("xy : %d \n", snake[x][y]);

	pthread_t thread, thread2, thread_lcd;
	pthread_create(&thread, NULL, thread_inputUserKey, NULL); //����Ű �Է� ������
	pthread_create(&thread2, NULL, thread_whichWay, NULL); //��Ʈ ��Ʈ������ �� �����̴� ������
	pthread_create(&thread_lcd, NULL, thread_score, NULL); //lcd�� ���ھ� ���� ������  ����


	printf("Snake Game\n");

	pthread_join(thread2, &tret);
	pthread_join(thread, &tret);   //lcd end wait
	pthread_detach(thread_lcd);
	pthread_join(thread_lcd, &tret);
	//������ 3�� ��� ����(join)


	if (endThisGame != 1 || soul != 0) {
		StartGame(); //���� ������� ���� ����Լ� ȣ�� p�� ������ ��� �����尡 join(����)�ϸ鼭 �ٽ� StartGame�� ȣ���.
	}

	printf("all thread Stopped, game end c'u next time!");

}
void snakegame()
{
	dev = open(FPGA_DOT_DEVICE, O_WRONLY); //Dot_Matrix ����̽� open
	if (dev < 0) {
		printf("Device open error : %s\n", FPGA_DOT_DEVICE);
		exit(1);
	}
	str_size = sizeof(fpga_number); //��� ���Ͽ� �ִ� ��Ʈ ��Ʈ���� �迭�� �ִ� �迭 size�� ����
	write(dev, fpga_number, str_size); //�ʱ� ȭ�� write
	StartGame(); //���� ����
	gamenmber = 1, selectgame = 0, pushswitch = 1;
}