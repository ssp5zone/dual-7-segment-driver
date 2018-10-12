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

// Program starts here
void main()
{
  unsigned char i;

  // Make PortB output
  TRISB=0;
  PORTB=255;

  Delay_ms(1000);

  // do forever
  while(1)
  {
    // test all characters one by one
   for(i=0;i<16;i++)
   {
     drive(i);
     Delay_ms(1000);
   }
  }
}
