/*
  HT1632 Test Program
*/
#include <ht1632c.h>
#include "ai_snake.h"

#define CS     4 /* Arduino Pin 12 */
#define CLK    3 /* Arduino Pin 11 */
#define WR     5 /* Arduino Pin 13 */
#define DATA   2 /* Arduino Pin 10 */

ht1632c ledMatrix = ht1632c(&PORTB, DATA, WR, CLK, CS, GEOM_32x16, 1);
game_struct_t gameStruct;
snake_struct_t snakeStruct;

int ht1632c_plot(void* cookie, uint8_t X, uint8_t Y, uint32_t color);
int ht1632c_clear(void* cookie); 
int ht1632c_push(void * cookie);

#if 0
void printStruct(snake_struct_t * ss){
    Serial.print("Apple "); Serial.print(GET_X(ss->apple_pos)); Serial.print(" "); Serial.println(GET_Y(ss->apple_pos));
    for(uint8_t i = 0; i < (ss->length + 1); i++){
        Serial.print(GET_X(ss->array[i]), HEX);
        Serial.println(GET_Y(ss->array[i]), HEX);
    }
}
#endif

void setup() {
    ledMatrix.clear();
    ledMatrix.pwm(15);

    Serial.begin(115200);

    /* Setup the Game Strcut */
    memset(&gameStruct, 0x00, sizeof(game_struct_t));

    memset(&snakeStruct, 0x00, sizeof(snake_struct_t));


    gameStruct.plot_function  = ht1632c_plot;
    gameStruct.clear_function = ht1632c_clear;
    gameStruct.push_function  = ht1632c_push;
    gameStruct.cookie         = NULL;
    gameStruct.board_x        = 31;
    gameStruct.board_y        = 15;
    gameStruct.color_depth    = 2;

    snakeInit(&gameStruct, &snakeStruct);

}

void loop() {
    runFrame();
    drawSnake();
    /*printStruct(&snakeStruct);*/
    ledMatrix.sendframe();
    Serial.println("Frame Ran");
    delay(20);
}

int ht1632c_plot(void* cookie, uint8_t x, uint8_t y, uint32_t color){
    ledMatrix.plot(x, y, color);
    return 1;
}

int ht1632c_clear(void* cookie){
    ledMatrix.clear();
    return 1;
} 
int ht1632c_push(void * cookie){
    ledMatrix.sendframe();
    return 1;
}
