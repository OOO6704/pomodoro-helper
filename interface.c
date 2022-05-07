#include "interface.h"
#include "lcdtp.h"
#include "xpt2046.h"
#include "image.h"
#include "UartRingbuffer_multi.h"

static char charData[320];
static int16_t pointX;
static int16_t pointY;
extern UART_HandleTypeDef huart1;
extern void Uart_sendstring (const char *s, UART_HandleTypeDef *uart);

int button(uint16_t usC, uint16_t usP, int width, int height, const char *pstr, int wordCount, uint16_t color, uint16_t wordColor){
	LCD_Clear(usC,usP,width, height,color);
	LCD_DrawString_Color(usC+(width-8*wordCount)/2,usP+(height-16)/2,pstr,color,wordColor);
	return 0;
}

int testInterface(){
	int counter = 0;
	LCD_Clear ( 0, 0, 240, 320, BLUE );
	LCD_Clear ( 0, 0, 240, 320, WHITE );
	button(166,280,64,24,"Return",6,RED,YELLOW);
	while(1){
	LCD_DrawString(0,160,"Hello, this is the testing interface.");
	LCD_DrawString(60,200,"The point is:");
	LCD_DrawString(174,200,"XX");
	LCD_DrawString(190,200,":");
	LCD_DrawString(198,200,"YY");
	sprintf(charData,"%d",counter);
	LCD_DrawString(240,120,charData);
	LCD_DrawString(100,240,"   ");
	sprintf(charData,"%d",pointX);
	LCD_DrawString(100,240,charData);
	LCD_DrawString(100,280,"   ");
	sprintf(charData,"%d",pointY);
	LCD_DrawString(100,280,charData);
	//LCD_Clear(166,280,64,30,RED);
	//LCD_DrawString(174,287,"Return");
		
	if(myCheck()){
	counter++;
		XPT2046_ReadAdc_XY(&pointX, &pointY);
		touch2Display(&pointX, &pointY);
		HAL_Delay(50);
	}
	if(pointY>=280 && pointY<=280+8+16 && pointX>=166 && pointX<=166+8*6+4){
	pointX = 0;
	pointY = 0;
	return 1;
	}
	}
}

void touch2Display(int16_t *x, int16_t *y){
	float tmpX = *y;
	float tmpY = *x;
  tmpX = (tmpX-200)/3600*240;
	tmpY = (1-(tmpY-250)/3450)*320;
	*x = tmpX;
	*y = tmpY;
}

void loading(){
	LCD_Clear(0, 0, 240, 320, WHITE);
	LCD_drawImage(100, 120, 40, 40,pomodoro);
	LCD_DrawString(92,180,"Loading");
	LCD_Clear(40,200,160,30,BLUE);
	LCD_Clear(45,205,150,20,WHITE);
	for(int i = 0; i<15; i++){
  LCD_Clear(45+10*i,205,10,20,GREEN);
		if(i%5==4){
		LCD_DrawString(148+8*(i/5),180,".");
		}
	HAL_Delay(50);
}
	return;
}

int mainMenu(){
LCD_Clear(0,0,240,320,0xDB6E);
LCD_DrawString_Color(16,8,"Welcome to Pomodoro Timer!",0xDB6E,WHITE);
LCD_drawImage(100,100,40,40,pomodoro);

button(30,220,180,20,"Timer",5,WHITE,0xDB6E);
button(30,245,180,20,"Calibration",11,WHITE,0xDB6E);
button(30,270,180,20,"Reports",7,WHITE,0xDB6E);
button(30,295,180,20,"Option",6,WHITE,0xDB6E);
	
	while(1){
	if(myCheck()){
		XPT2046_ReadAdc_XY(&pointX, &pointY);
		touch2Display(&pointX, &pointY);
		if(pointX>=30 && pointX<=210){
		if(pointY>=220 && pointY<=240){
		return 1;
			//Timer Screen
		}
		else if(pointY>=245 && pointY<=265){
		return 2;
			//Calibration
		}
		else if(pointY>=270 && pointY<=290){
		return 3;
			//Report
		}
		else if(pointY>=295 && pointY<=315){
		return 4;
			//Option
		}
		}
	}
	}
}

int timerScreen(){
	//returns a 5 digits number, the first 4 is the time indicator, the last one is the pageCounter
	LCD_Clear(0,0,240,320,WHITE);
	LCD_DrawString(40,8,"Set your timer here.");
	LCD_DrawString(46,40,"Min:");
	LCD_DrawString(162,40,"Sec:");
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	
	//Interface Init
	button(8,70,50,20,"^",1,BLUE,WHITE);
	button(66,70,50,20,"^",1,BLUE,WHITE);
	button(124,70,50,20,"^",1,BLUE,WHITE);
	button(182,70,50,20,"^",1,BLUE,WHITE);
	bigNumber(8,100,0);
	bigNumber(66,100,0);
	bigNumber(124,100,0);
	bigNumber(182,100,0);
	button(8,210,50,20,"v",1,BLUE,WHITE);
	button(66,210,50,20,"v",1,BLUE,WHITE);
	button(124,210,50,20,"v",1,BLUE,WHITE);
	button(182,210,50,20,"v",1,BLUE,WHITE);
	button(70,250,100,20,"Enter",5,CYAN,BLACK);
	button(70,280,100,20,"Return",6,CYAN,BLACK);
	
	while(1){
		pointX = 0;
		pointY = 0;
	if(myCheck()){
		XPT2046_ReadAdc_XY(&pointX, &pointY);
		touch2Display(&pointX, &pointY);
	}
	//First Digit Up down
	if(pointX>=8 && pointX<=58){
	if(pointY>=70&&pointY<=90){
		a++;
		if(a==10){a=9;}
		bigNumber(8,100,a);
		HAL_Delay(50);
	}
	else if(pointY>=210&&pointY<=230){
		a--;
		if(a<0){a=0;}
		bigNumber(8,100,a);
		HAL_Delay(50);
	}
	}
	//Second Digit Up down
	if(pointX>=66 && pointX<=116){
	if(pointY>=70&&pointY<=90){
		b++;
		if(b==10){b=9;}
		bigNumber(66,100,b);
		HAL_Delay(50);
	}
	else if(pointY>=210&&pointY<=230){
		b--;
		if(b<0){b=0;}
		bigNumber(66,100,b);
		HAL_Delay(50);
	}
	}
	//Third Digit Up down
	if(pointX>=124 && pointX<=174){
	if(pointY>=70&&pointY<=90){
		c++;
		if(c==6){c=5;}
		bigNumber(124,100,c);
		HAL_Delay(50);
	}
	else if(pointY>=210&&pointY<=230){
		c--;
		if(c<0){c=0;}
		bigNumber(124,100,c);
		HAL_Delay(50);
	}
	}
	//Fourth Digit Updown
	if(pointX>=182 && pointX<=232){
	if(pointY>=70&&pointY<=90){
		d++;
		if(d==10){d=9;}
		bigNumber(182,100,d);
		HAL_Delay(50);
	}
	else if(pointY>=210&&pointY<=230){
		d--;
		if(d<0){d=0;}
		bigNumber(182,100,d);
		HAL_Delay(50);
	}
	}
	
	else if(pointX >= 70 && pointX <= 170 && pointY >=250 && pointY <= 270){
	return a*10000+b*1000+c*100+d*10+5;
	//5 here indicates that the pageMode will go to 5, which should be the timer countdown page.
	}
	
	else if(pointX >= 70 && pointX <= 170 && pointY >= 280 && pointY <= 300){
	break;
	}
	//End while
	}
	
	return 0;
}

void bigNumber(uint16_t usC, uint16_t usP, int num){

	LCD_Clear(usC,usP,50,100,WHITE);
	int thick = 15;
	int sep = (100 - 3*thick) /2;
	switch(num){
		case 0:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC,usP,thick,100,BLUE);
			LCD_Clear(usC,usP+(100-thick),50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP,thick,100,BLUE);
			break;
		
		case 1:
			LCD_Clear(usC+(50-thick)/2,usP,thick,100,BLUE);
			break;
		
		case 2:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP,thick,thick+sep,BLUE);
			LCD_Clear(usC,usP+thick+sep,50,thick,BLUE);
			LCD_Clear(usC,usP+thick+sep,thick,thick+sep,BLUE);
			LCD_Clear(usC,usP+2*(thick+sep),50,thick,BLUE);
			break;
		
		case 3:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP,thick,100,BLUE);
			LCD_Clear(usC,usP+thick+sep,50,thick,BLUE);
			LCD_Clear(usC,usP+2*(thick+sep),50,thick,BLUE);
			break;
		
		case 4:
			LCD_Clear(usC,usP,thick,thick+sep,BLUE);
			LCD_Clear(usC,usP+thick+sep,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP,thick,100,BLUE);
			break;
		
		case 5:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC,usP,thick,thick+sep,BLUE);
			LCD_Clear(usC,usP+thick+sep,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP+thick+sep,thick,thick+sep,BLUE);
			LCD_Clear(usC,usP+2*(thick+sep),50,thick,BLUE);
			break;
		
		case 6:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC,usP,thick,100,BLUE);
			LCD_Clear(usC,usP+thick+sep,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP+thick+sep,thick,thick+sep,BLUE);
			LCD_Clear(usC,usP+2*(thick+sep),50,thick,BLUE);
			break;
		
		case 7:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP,thick,100,BLUE);
			break;
		
		case 8:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC,usP,thick,100,BLUE);
			LCD_Clear(usC,usP+thick+sep,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP,thick,100,BLUE);
			LCD_Clear(usC,usP+2*(thick+sep),50,thick,BLUE);
			break;
		
		case 9:
			LCD_Clear(usC,usP,50,thick,BLUE);
			LCD_Clear(usC,usP,thick,thick+sep,BLUE);
			LCD_Clear(usC,usP+thick+sep,50,thick,BLUE);
			LCD_Clear(usC+(50-thick),usP,thick,100,BLUE);
			LCD_Clear(usC,usP+2*(thick+sep),50,thick,BLUE);
			break;
		
		default:
			break;
	}
}


int timerCount(int timer){
	//Init
	LCD_Clear(0,0,240,320,WHITE);
	int min = timer/100;
	int sec = timer%100;
	int time = 60*min+sec;
	if(timer == 2500){
	LCD_DrawString(36,8,"Now its time to work!");}
	else if(timer == 500){
	LCD_DrawString(24,8,"Let's take a good break!");
	}
	LCD_DrawString(52,49,"Min:");
	LCD_DrawString(178,49,"Sec:");
	bigNumber(5,70,min/10);
	bigNumber(60,70,min%10);
	bigNumber(130,70,sec/10);
	bigNumber(185,70,sec%10);
	LCD_Clear(118,106,4,4,BLUE);
	LCD_Clear(118,130,4,4,BLUE);
	button(10,280,100,20,"Pause",5,CYAN,BLACK);
	button(130,280,100,20,"Stop!",5,RED,WHITE);
	
		while(time>=0){
		//update big number time
		bigNumber(5,70,time/60/10);
		bigNumber(60,70,time/60%10);
		bigNumber(130,70,time%60/10);
		bigNumber(185,70,time%60%10);
		
		//update the two dots
		if(time%2==1){
		LCD_Clear(118,106,4,4,BLUE);
		LCD_Clear(118,130,4,4,BLUE);
		}
		else if(time%2==0){
		LCD_Clear(118,106,4,4,WHITE);
		LCD_Clear(118,130,4,4,WHITE);
		}
		time--;
		
		for(int i=0; i<10; i++){
		if(i == 1){
		// on the first 0.1 second, update the brainstruct
		
		//Uart send the brainstruct
		//test function
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
		sprintf(charData,"%d",time);
		LCD_DrawString(120,200,charData);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
		Uart_sendstring("Hello",&huart1);	
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
		}
		if(checkButton(1)==1){
		button(10,280,100,20,"Continue",8,CYAN,BLACK);
		button(130,280,100,20,"Stop!",5,GREY,WHITE);
		HAL_Delay(100);
		while(1){
		if(checkButton(1)==1){
		
		break;
		}
		}
		button(10,280,100,20,"Pause",5,CYAN,BLACK);
		button(130,280,100,20,"Stop!",5,RED,WHITE);
		}
		else if(checkButton(1)==2){
		time = 0;
		}
		HAL_Delay(100);
		}
	}
	if(timer == 2500){
	LCD_Clear(0,0,240,320,0xDB6E);
	LCD_DrawString_Color(60,144,"Focus time ends!",0xDB6E,WHITE);
	LCD_DrawString_Color(12,164,"Time to take a short break!",0xDB6E,WHITE);
	//Timer ends!
	return 6;}
	else if(timer == 500){
	LCD_Clear(0,0,240,320,0x4D18);
	LCD_DrawString_Color(52,144,"Short break ends!",0x4D18,WHITE);
	LCD_DrawString_Color(20,164,"Time to get back to work!",0x4D18,WHITE);
	return 7;
	}
}

//Check button from different cases
int checkButton(int mode){
	//timerCountDown Screen
	
	if(myCheck()){
		XPT2046_ReadAdc_XY(&pointX, &pointY);
		touch2Display(&pointX, &pointY);
	}
	else {return 0;}
	
	//TimerCountdown Screen
	if(mode == 1){
	if(pointY>=280 && pointY<=300){
	if(pointX>=10 && pointX<=110){
		// pause
		return 1;
	}
	else if(pointX>=130 && pointX<=230){
		// Stop
		return 2;
	}
	}
	}
	
	
}

