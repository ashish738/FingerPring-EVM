#include <at89x52.h>
#include <8051_LCD_8_BIT.h>
#include <string.h>
#include <8051_Serial.h>

sbit red=P0^1;
sbit green=P0^0;
sbit buzzer=P0^2;


sbit SW_1=P1^0;
sbit SW_2=P1^1;
sbit SW_3=P1^2;
sbit SW_4=P1^3;
sbit SW_5=P1^4;
sbit SW_A=P1^5;
sbit SW_B=P1^6;
sbit SW_C=P1^7;


sbit Search=P3^2;
sbit Empty=P3^3;
sbit Add=P3^4;

bit User_1=0,User_2=0,User_3=0,User_4=0,User_5=0;
bit User_1_vote=0,User_2_vote=0,User_3_vote=0,User_4_vote=0,User_5_vote=0;
bit valid_user=0,invalid_user=0;
int system_mode=0,normal_mode=0;
int candidate_1=0,candidate_2=0,candidate_3=0;

char SM_ARU;

void vote_system_process();
void canditate_select_mode();
void warning_mode();
void vote_mode();
void system_mode_process();
void count_mode();

void receive_data()      interrupt 4   
{
	SM_ARU=SBUF;      //Read SBUF
	RI=0;
}

void main()
{
	P0=0x00;
	P1=0XFF;
	lcd_init();
	init_Serial();
	lcd_cmd(0x80);	
	lcd_string("Finger Print Based ");	
	lcd_cmd(0xC0);	
	lcd_string("  Voting System    ");
	delay(1000);
	red=1;
	green=0;
	buzzer=0;
//	EA=1;
//	IE=0x90;	
	lcd_cmd(0x01);	
	lcd_string("B> Sytm Mode");	
	lcd_cmd(0xc0);	
	lcd_string("C> Normal Mode");
	while(SW_C==1 && SW_B==1);
	
	if(SW_C==0){system_mode=0;normal_mode=1;lcd_cmd(0x01);lcd_string("Normal Mode");}
	if(SW_B==0){system_mode=1;normal_mode=0;lcd_cmd(0x01);lcd_string("System Mode");system_mode_process();}
	red=1;
	green=0;
	buzzer=1;
	delay(1000);
	buzzer=0;
	
	/*
			error -'/' 
			User 1 - 0;
	*/
	while(1)
	{
		vote_system_process();
		if(valid_user==1)
		{
			canditate_select_mode();
		}
	}
}	


void vote_system_process()
{
	lcd_cmd(0x01);	
	lcd_string("   Press 5 ");
	lcd_cmd(0xc0);	
	lcd_string("Scan Fingerprint");
	while(SW_5==1)
	{
	if(SW_A==0 && SW_B==0 && SW_C==0){count_mode();}
	
	}
	lcd_cmd(0x01);	
	lcd_string("Fingerprint");
	lcd_cmd(0xc0);	
	lcd_string("   Scan ");
	Search=0;delay(100);Search=1;delay(100);
	Rx_Char();
	SM_ARU=SBUF+0x30;
	lcd_cmd(0x8C);	
	lcd_data(SM_ARU);
	
	if(SM_ARU=='/')
	{lcd_cmd(0x01);	lcd_string("Invalid User...");red=0;green=1;valid_user=0;invalid_user=1;}
	
	if(SM_ARU=='0')
	{lcd_cmd(0x01);	lcd_string("Valid User 1");red=1;green=0;valid_user=1;invalid_user=0;
	if(User_1==0)User_1=1;
	if(User_1_vote==1)warning_mode();}
	
	if(SM_ARU=='1')
	{lcd_cmd(0x01);	lcd_string("Valid User 2");red=1;green=0;valid_user=1;invalid_user=0;
	if(User_2==0)User_2=1;
	if(User_2_vote==1)warning_mode();}
	
	if(SM_ARU=='2')
	{lcd_cmd(0x01);	lcd_string("Valid User 3");red=1;green=0;valid_user=1;invalid_user=0;
	if(User_3==0)User_3=1;
	if(User_3_vote==1)warning_mode();}
	
	if(SM_ARU=='3')
	{lcd_cmd(0x01);	lcd_string("Valid User 4");red=1;green=0;valid_user=1;invalid_user=0;
	if(User_4==0)User_4=1;
	if(User_4_vote==1)warning_mode();}
	
	if(SM_ARU=='4')
	{lcd_cmd(0x01);	lcd_string("Valid User 5");red=1;green=0;valid_user=1;invalid_user=0;
	if(User_5==0)User_5=1;
	if(User_5_vote==1)warning_mode();}
	
	
	delay(1000);
	buzzer=1;
	delay(1000);
	buzzer=0;
}

void canditate_select_mode()
{
	if(User_1==1 && User_1_vote==0)
	{
		lcd_cmd(0x01);
		lcd_string("User 1");
		lcd_cmd(0xc0);
		lcd_string("Please Give Vote");
		vote_mode();
	  User_1_vote=1;
	}
	if(User_2==1 && User_2_vote==0)
	{
		lcd_cmd(0x01);
		lcd_string("User 2");
		lcd_cmd(0xc0);
		lcd_string("Please Give Vote");
		vote_mode();
	  User_2_vote=1;
	}
	if(User_3==1 && User_3_vote==0)
	{
		lcd_cmd(0x01);
		lcd_string("User 3");
		lcd_cmd(0xc0);
		lcd_string("Please Give Vote");
		vote_mode();
	  User_3_vote=1;
	}
	if(User_4==1 && User_4_vote==0)
	{
		lcd_cmd(0x01);
		lcd_string("User 4");
		lcd_cmd(0xc0);
		lcd_string("Please Give Vote");
		vote_mode();
	  User_4_vote=1;
	}
	if(User_5==1 && User_5_vote==0)
	{
		lcd_cmd(0x01);
		lcd_string("User 5");
		lcd_cmd(0xc0);
		lcd_string("Please Give Vote");
		vote_mode();
	  User_5_vote=1;
	}

}

void warning_mode()
{
	lcd_cmd(0x01);
	lcd_string("Warning");
	lcd_cmd(0xc0);
	lcd_string("Vote Given");
	delay(1000);
	buzzer=1;
	delay(1000);
	buzzer=0;
}

	void vote_mode()
{
	int vote=0;
	
	while(vote==0)
	{
	if(SW_1==0 && SW_2==1 && SW_3==1){candidate_1++;vote=1;}
	if(SW_1==1 && SW_2==0 && SW_3==1){candidate_2++;vote=1;}
	if(SW_1==1 && SW_2==1 && SW_3==0){candidate_3++;vote=1;}
	}
	
	lcd_cmd(0x01);
	lcd_string("Thank You...");
	lcd_cmd(0xC0);
	lcd_string("Vote Register...");
	buzzer=1;red=1;green=0;
	delay(800);
	buzzer=0;red=1;green=1;
	delay(2000);
}

void system_mode_process()
{
	lcd_cmd(0x01);
	lcd_string("Data clear...");
	buzzer=1;red=1;green=0;
	Empty=0;delay(100);Empty=1;
	delay(2000);
	buzzer=0;red=1;green=1;Rx_Char();
	lcd_cmd(0x01);
	lcd_string(" Voter ID  1 ");
	lcd_cmd(0xC0);
	lcd_string("Registration");
	while(SM_ARU!='0')
	{ 
		lcd_cmd(0x8F);	
		lcd_data(' ');
		if(SW_A==0)
		{
			Add=0;delay(100);Add=1;delay(100);
			Rx_Char();
			SM_ARU=SBUF+0x30;
			lcd_cmd(0x8F);	
			lcd_data(SM_ARU);
			delay(1000);
		}
	}
	lcd_cmd(0x01);
	lcd_string("Register...");
	buzzer=1;red=1;green=0;
	delay(1000);
	buzzer=0;red=1;green=1;
	
	
	lcd_cmd(0x01);
	lcd_string(" Voter ID  2 ");
	lcd_cmd(0xC0);
	lcd_string("Registration");
	while(SM_ARU!='1')
	{
		lcd_cmd(0x8F);	
		lcd_data(' ');
		if(SW_A==0)
		{
			Add=0;delay(100);Add=1;delay(100);
			Rx_Char();
			SM_ARU=SBUF+0x30;
			lcd_cmd(0x8F);	
			lcd_data(SM_ARU);
			delay(1000);
		}
	}
	lcd_cmd(0x01);
	lcd_string("Register...");
	buzzer=1;red=1;green=0;
	delay(1000);
	buzzer=0;red=1;green=1;
	
	
	lcd_cmd(0x01);
	lcd_string(" Voter ID  3 ");
	lcd_cmd(0xC0);
	lcd_string("Registration");
	while(SM_ARU!='2')
	{
		lcd_cmd(0x8F);	
		lcd_data(' ');
		if(SW_A==0)
		{
			Add=0;delay(100);Add=1;delay(100);
			Rx_Char();
			SM_ARU=SBUF+0x30;
			lcd_cmd(0x8F);	
			lcd_data(SM_ARU);
			delay(1000);
		}
	}
	
	lcd_cmd(0x01);
	lcd_string("Register...");
	buzzer=1;red=1;green=0;
	delay(1000);
	buzzer=0;red=1;green=1;
	
	
	lcd_cmd(0x01);
	lcd_string(" Voter ID  4 ");
	lcd_cmd(0xC0);
	lcd_string("Registration");
	while(SM_ARU!='3')
	{
		lcd_cmd(0x8F);	
		lcd_data(' ');
		if(SW_A==0)
		{
			Add=0;delay(100);Add=1;delay(100);
			Rx_Char();
			SM_ARU=SBUF+0x30;
			lcd_cmd(0x8F);	
			lcd_data(SM_ARU);
			delay(1000);
		}
	}
	lcd_cmd(0x01);
	lcd_string("Register...");
	buzzer=1;red=1;green=0;
	delay(1000);
	buzzer=0;red=1;green=1;
	
	
	lcd_cmd(0x01);
	lcd_string(" Voter ID  5");
	lcd_cmd(0xC0);
	lcd_string("Registration");
	while(SM_ARU!='4')
	{
		lcd_cmd(0x8F);	
		lcd_data(' ');
		if(SW_A==0)
		{
			Add=0;delay(100);Add=1;delay(100);
			Rx_Char();
			SM_ARU=SBUF+0x30;
			lcd_cmd(0x8F);	
			lcd_data(SM_ARU);
			delay(1000);
		}
	}
	lcd_cmd(0x01);
	lcd_string("Register...");
	buzzer=1;red=1;green=0;
	delay(1000);
	buzzer=0;red=1;green=1;
}


void count_mode()
{
	buzzer=1;red=1;green=0;
	delay(1000);
	buzzer=0;red=1;green=1;
	lcd_cmd(0x01);
	lcd_string("  Total Count");
	lcd_cmd(0xC0);
	lcd_string("A: ");
	lcd_data(candidate_1+0x30);
	lcd_string(" B: ");
	lcd_data(candidate_2+0x30);
	lcd_string(" C: ");
	lcd_data(candidate_3+0x30);
	delay(3000);
	buzzer=1;red=1;green=0;
	delay(1000);
	buzzer=0;red=1;green=0;
	lcd_cmd(0x01);
	lcd_string("Winner:- ");
	
	if((candidate_1==candidate_2) && (candidate_2==candidate_3)){lcd_string("A B C");}
	else if((candidate_1==candidate_2) && candidate_2!=0 && (candidate_1 > candidate_3) ){lcd_string("A & B");}
	else if((candidate_1==candidate_3) && candidate_3!=0  && (candidate_1 > candidate_2) ){lcd_string(" A & C");}
	else if((candidate_3==candidate_2) && candidate_2!=0  && (candidate_2 > candidate_1) ){lcd_string(" B & C");}
	else if(candidate_1 > candidate_2)
	{
		if(candidate_1 > candidate_3){lcd_string(" A");}
		else{lcd_string(" C");}
	}
	else if(candidate_2 > candidate_3)
	{
		lcd_string(" B");
	}
	else{lcd_string(" C");}
	
	
	while(1);
}