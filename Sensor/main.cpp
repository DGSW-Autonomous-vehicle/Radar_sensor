#include <iostream>
#include <wiringPi.h>

using namespace std;

#define Radar_pin 8
#define IR_pin 7

int GPIO_init(){
	if(wiringPiSetup() == -1){
		cout << "WirginPi Serup Error" << endl;
		return -1;
	}

	pinMode(Radar_pin, INPUT);
	pinMode(IR_pin, INPUT);
	return 0;
}

int Radar_input(){
	int tem = digitalRead(Radar_pin);
	return tem;
}

int IR_input(){
	int tem = digitalRead( + IR_pin);
	return tem;
}



int main(){
	if(GPIO_init()){
		cout << "GPIO_Error" << endl;
		return -1;
	}

	while(1){
		cout << "Radar : " << Radar_input() << endl;
		//cout << "IR : " << IR_input() << endl;
		delay(200);
	}


}
