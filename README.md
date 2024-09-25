# Arduino Front I/O Volume
I have a [custom PC](https://buu342.me/blog/projects/SGIIndySleeper.html), and I wanted to be able to click the buttons on the front of the case to increase/decrease volume. So, I bought a [Beetle board](https://www.dfrobot.com/product-1075.html) (an Arduino Leonardo compatible board) and wrote this code to simulate keyboard volume keys using the digital pins on the board.

GPIO `D9` is the volume up key, and `D10` is the volume down key. This can be changed by modifying the `BUTTON_UP` and `BUTTON_DOWN` defines. 

The code properly debounces the buttons and simulates key repetition (the delay that happens when you hold a key on your keyboard down). You can modify the delay time by changing the `DELAYTIME` define, and the repeat rate by modifying `RATE`. 
