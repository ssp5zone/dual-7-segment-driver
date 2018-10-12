// the variables that store current display context
// for individual segements. You can use a single variable too.
unsigned char Disp1=0, Disp2=0;

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
  if (TMR2IF_bit) 
  {
    // toggle the active display 
    PORTA.B0=~PORTA.B0;
    PORTA.B1=~PORTA.B1;

    // based on which one is active 
    // drive that value
    if(PORTA.B0)
    {
       drive(Disp1);
    }
    else
    {
       drive(Disp2);
    }

    // clear TMR2IF flag so that interrupts again
    TMR2IF_bit = 0;

    // reset timer
    TMR2 = 0;
  }
}

// main macro to call for displaying
// Notice it does not do anything other than splitting the data. Thats it!
void display(unsigned char i)
{
  // Split the 8 bits (eg.86 = 1000 1010) into 8 and 6
  // 6 = 0000 1010 -> for Display 1
  Disp1=(i)&(0x0F); // masking top 4 bits
  // pushing higher bits to right, you can shift 4 times too
  Disp2=swap(i);
  // 8 = 0000 1000 -> for Display 2
  Disp2=(Disp2)&(0x0F); // masking top 4 bits
}

// main program start here
void main()
{
  unsigned char i;
  
  // Make PortB output
  TRISB=0;
  PORTB=255;

  // Make 2 additional ports as output
  // To each we connect the common pin of a display
  // intead of 8th pin of driver. Here lies the magic, using this
  // we only activate one at a time with a particular drive value.
  TRISA.B0=0;
  TRISA.B1=0;
  PORTA.B0=1;
  PORTA.B1=0;
  Delay_ms(1000);
  
  // Now we need a mechanism that repeatedly swicthes the active display
  // and changes the drive value needed for that display. 
  // We use a timer.
  T2CON = 0x8F;               // Timer2 settings
  TMR2  =   0;                // Initialize Timer2 register

  // If we were to poll the timer flag, we will have to
  // check it periodically while doing other tasks. That sucks!!

  // Solution: Tell the timer to trigger a global interrupt once it reaches 0.
  // And forget about it.
  TMR2IE_bit = 1;             // enable interupt
  INTCON = 0xC0;              // Set GIE, PEIE

  // do what ever task you want
  while(1)
  {
    // loop 8-bit sequence
    for(i=0;i<256;i++)
    {
      display(i);
      Delay_ms(1000);
    }
  }
}
