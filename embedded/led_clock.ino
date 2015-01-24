/*
  HT1632 Test Program
*/
#include <ht1632c.h>

#define CS     4 /* Arduino Pin 12 */
#define CLK    3 /* Arduino Pin 11 */
#define WR     5 /* Arduino Pin 13 */
#define DATA   2 /* Arduino Pin 10 */

ht1632c ledMatrix = ht1632c(&PORTB, DATA, WR, CLK, CS, GEOM_32x16, 1);

void setup() {
    ledMatrix.clear();
    ledMatrix.pwm(1);

}

void loop() {
    ledMatrix.line(0,10, 5,15, random(1,4));
    ledMatrix.sendframe();
    delay(10000);
}
