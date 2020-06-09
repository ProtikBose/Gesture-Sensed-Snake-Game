# Gesture-Sensed-Snake-Game

It is a typical snake game but it is played on four 8 by 8 dot matrices . The snake moves with gesture and the gesture is recognized by an accelerometer sensor . There are levels and this levels can be set.

Four 8 by 8 rgb dot matrices are controlled by three types of decoders. Rows are controlled with two 3 by 8 decoder(IC 74138). As the output of the decoder is active low, so the outputs of these decoders are passed through NOT gates (IC 7404) . Columns are controlled with three 4 by 16 decoders (IC 74154). I have used accelerometer in mpu 6050 to detect acceleration in X and Y axes to move the snake. 

You can watch the full demonstration [here](https://www.youtube.com/watch?v=ftV2LjR0soU&t=1s)

[![Watch the video](https://img.youtube.com/vi/ftV2LjR0soU/maxresdefault.jpg)](https://youtu.be/ftV2LjR0soU)
