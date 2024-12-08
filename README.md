Imagine a situation where you have an electronic board with a microcontroller and a button. You
are required to check if the button is pressed (state ON) or not (state OFF) and for how long the
button is kept pressed. If the button gets pressed, you also need to have a clear state definition
ranging from ON, PROTECTED and OFF; as per the state diagram below. The protected state is
defined as an intermediate stage between the transition from the ON state to the OFF state, so
that each time the button is released, the button state changes to PROTECTED for 10 seconds
before going to the OFF state. Implement this logic in a generic way using the C language and
share this code through a public repository on GitHub. Low-level methods can be abstracted. If
you prefer, use frameworks like Arduino, ESP32 or even other market platforms in this low-level
code abstraction.
![alt text](image.png)