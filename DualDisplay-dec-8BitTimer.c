// the variables that store current display context
// for individual segements. You can use a single variable too.
unsigned char Disp1, Disp2;

// the block that drives the 8 segment display
void drive(unsigned char i)
{
  // An 7 segment display is typically used at-most 
  // to display hex chars i.e. 0-9, a-f(10-15)
   switch(i)
   {
    // The 7 segments are connected to 7 pins of a given port (portB here) and
    // the 8th pin is connected to common anode or cathode pin of display.
    // Depending on how you connect the wires (which bit maps to which segment),
    // figure out which pins should be high/low to display a certain character
    case 0: {PORTB=0x02; break;}
    case 1: {PORTB=0x5F; break;}
    case 2: {PORTB=0x84; break;}
    case 3: {PORTB=0x0C; break;}
    case 4: {PORTB=0x58; break;}
    case 5: {PORTB=0x28; break;}
    case 6: {PORTB=0x20; break;}
    case 7: {PORTB=0x1F; break;}
    case 8: {PORTB=0x00; break;}
    case 9: {PORTB=0x08; break;}
    case 10: {PORTB=0x10; break;}
    case 11: {PORTB=0x60; break;}
    case 12: {PORTB=0xA2; break;}
    case 13: {PORTB=0x44; break;}
    case 14: {PORTB=0xA0; break;}
    case 15: {PORTB=0xB0; break;}
  }
}

// The common macro that gets called on any interrupt.
// This is a reserved keyword only used internally and is completly independent
// to the main call.
void interrupt()
{
  // indentify if the interrupt is relevant to us.
  if (TMR0IF_bit)    //IMPORTANT: Display uses Timer0
  {
    
    // based on which one is active 
    // drive that value
    if(PORTA.B0)
    {  
      PORTA.B0=0;  // switching off the other display 1st to reduce load.
      drive(Disp2);
      PORTA.B1=1;
    }
    else
    {
      PORTA.B1=0;  // switching off the other display 1st to reduce load.
      drive(Disp1);
      PORTA.B0=1;
    }

    // clear TMR2IF flag so that interrupts again
    TMR0IF_bit = 0;

    // reset timer           
    TMR0 = 0;
  }
}

// main macro to call for displaying decimal values
void displayDecimal(unsigned char i)
{
  // input was intented as a decimal 0-99. Convert it to hex char
   i = Dec2Bcd(i);
   displayHex(i);

}

// main macro to call for displaying hexa-decimal values
// Notice it does not do anything other than splitting the data. Thats it!
void displayHex(unsigned char i) {
  // Split the 8 bits (eg.86 = 1000 1010) into 8 and 6
  // 6 = 0000 1010 -> for Display 1
  Disp1=(i)&(0x0F); // masking top 4 bits
  // pushing higher bits to right, you can shift 4 times too
  Disp2=swap(i);
  // 8 = 0000 1000 -> for Display 2
  Disp2=(Disp2)&(0x0F); // masking top 4 bits
}

// Initialize the display
void InitDisplay(void)
{
  // Make ALL PINS OF PortA Digital
  ADCON1=0X06;               
  
  // Set initial display value to 00.
  Disp1=0;
  Disp2=0;
  
  // Make driver port out
  TRISB=0;
  PORTB=0xFF;
  
  // Make selection pins out
  TRISA.B0=0;
  TRISA.B1=0;
  PORTA.B0=0;
  PORTA.B1=0;
  
  // Configure Timer 0 and init interrupts.
  OPTION_REG = 0x83;       // Assign prescaler to TMR0
  TMR0  = 0;               // Timer0 initial value
  INTCON = 0xA0;           // Enable TMRO interrupt
}

void main()
{
  // Just init display
  InitDisplay();

  // do your task
  unsigned char i;
  while(1)
  {
    for(i=99;i>0;i--)
    {
      displayDecimal(i);
      Delay_ms(1000);
    }
  }

}
