# simHubByArduino
simHub hub by arduino fro racing car game

total design 
hardware base on ATmega32U4 nrf24l01(JDY-40 is Serial)

steering wheel osh(arduino)-> ......wireless(nrf24l01)......<-pc game pad

1 game pad start 

2 osh start register -> game pad key map 

3 game pad -> registered osh running

4 game pad 
     for {
        ask osh is live
     }
     
     
     
game pad design    

key map 0-49(can add more)

XAxis
YAxis
ZAxis  
