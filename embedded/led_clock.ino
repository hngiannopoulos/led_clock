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

int snake_print(void * cookie, const char *format, ...);
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
    gameStruct.print_function = snake_print;
    gameStruct.board_x        = 31;
    gameStruct.board_y        = 15;
    gameStruct.color_depth    = 2;

    /* 8 3 5 5 1 Efficient - gets in loop */
    /* 8 1 2 5 1 Efficient gets in loop */
    /* 7 7 8 4 1 Pretty efficcient no loop*/
    /* 5 6 1 2 3 Likes to loop on iteself gets into loop */

    gameStruct.manhattan_weight    = 3;
    gameStruct.turn_weight         = 8;
    gameStruct.no_turn_weight      = 9;
    gameStruct.look_ahead_distance = 9;
    gameStruct.look_ahead_weight   = 4;

    snakeInit(&gameStruct, &snakeStruct);
    
    snakeStruct.cookie         = NULL;
    snakeStruct.snake_color = 1;
    snakeStruct.apple_color = 2;
#ifdef OBSTACLE_ON
    snakeStruct.obstacle_color = 3;
#endif
}

void loop() {
    runFrame(&snakeStruct);
    drawSnake(&snakeStruct);
    if(snakeStruct.state == WON){
        Serial.println("WON");
        //Serial.println(snakeStruct->apple_count);
    }
    else if(snakeStruct.state == DEAD){
        snake_print(NULL, "LOST %u \n\r", snakeStruct.apple_count);
    }

    // Serial.print("HEAD - ");
    // Serial.print(GET_X(snakeStruct.array[snakeStruct.length -1]));
    // Serial.print(" - ");
    // Serial.println(GET_Y(snakeStruct.array[snakeStruct.length -1]));

    /*printStruct(&snakeStruct);*/
    ledMatrix.sendframe();
    //Serial.println(snakeStruct.state);

    //delay(20);
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

int snake_print(void * cookie, const char *format, ...){
    char buffer[SPRINTF_BUFF_LEN] ="";
    va_list argptr;
    va_start(argptr, format);

    sprintf(buffer, format, argptr);

    va_end(argptr);
    Serial.print(buffer);
    return 1;
}