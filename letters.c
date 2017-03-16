/***********************************************\
*                    letters.c                  *
*           George Koskeridis (C) 2016          *
\***********************************************/


#include "common.h"
#include "main.h"

#define PEN_WIDTH 7
#define RESET 0

static const int down_boundary = 250, upper_boundary = 150;
static const int timeout = 1;
static int counter = 0;

int NextLetterK_state(void)
{
    int retvalue = K;

    if (!letters_drawn) { //if the letter isn't drawn yet animate the drawing
        int delta = 10;
        int points_drawn = 0;
        POINT up_left = {45, upper_boundary}, down_left = {up_left.x, down_boundary};

        if (!katelyn.letter[K][0].x) {
            KatelynPenWidth[K] = PEN_WIDTH;
            katelyn.letter[K][0].x = katelyn.letter[K][1].x = down_left.x;
            katelyn.letter[K][0].y = katelyn.letter[K][1].y = down_left.y;

            katelyn.letter[K][2].x = katelyn.letter[K][3].x = down_left.x + 44;
            katelyn.letter[K][2].y = katelyn.letter[K][3].y = down_left.y;

            katelyn.letter[K][4].x = katelyn.letter[K][5].x = up_left.x;
            katelyn.letter[K][4].y = katelyn.letter[K][5].y = up_left.y + 50;
            
            return retvalue;
        }

        if (katelyn.letter[K][1].x < up_left.x)
            katelyn.letter[K][1].x += delta;
        else
            points_drawn++;

        if (katelyn.letter[K][1].y > up_left.y)
            katelyn.letter[K][1].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[K][3].x > up_left.x + 14)
            katelyn.letter[K][3].x -= delta;
        else
            points_drawn++;
        
        if (katelyn.letter[K][3].y > up_left.y + 40)
            katelyn.letter[K][3].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[K][4].x < up_left.x + 40)
            katelyn.letter[K][4].x += delta;
        else
            points_drawn++;
        
        if (katelyn.letter[K][4].y > up_left.y)
            katelyn.letter[K][4].y -= delta;
        else
            points_drawn++;

        retvalue+=((points_drawn == katelyn.numOfPoints[K]) ? 1 : 0);
    } else { //else change color until timeout is reached
        if (counter >= 0 && counter < timeout) {
            counter++;
            katelyn.lett_clr[K] = RGB_GREEN;
        } else {
            katelyn.lett_clr[K] = RGB_ORANGE;
            counter = RESET;
            retvalue++;
        }
    }
    return retvalue;
}

int NextLetterA_state(void)
{
    int retvalue = A;

    if (!letters_drawn) {
        int delta = 5, slow_delta = 1;
        int points_drawn = 0;
        POINT up_top = {130, upper_boundary}, 
              down_left = {up_top.x - 30, down_boundary},
              down_right = {up_top.x + 30, down_boundary};

        if (!katelyn.letter[A][0].x) {
            KatelynPenWidth[A] = PEN_WIDTH;
            katelyn.letter[A][0].x = katelyn.letter[A][1].x = up_top.x;
            katelyn.letter[A][0].y = katelyn.letter[A][1].y = up_top.y;

            katelyn.letter[A][2].x = katelyn.letter[A][3].x = up_top.x;
            katelyn.letter[A][2].y = katelyn.letter[A][3].y = up_top.y;

            katelyn.letter[A][4].x = katelyn.letter[A][5].x = up_top.x;
            katelyn.letter[A][4].y = katelyn.letter[A][5].y = 200;
            
            return retvalue;
        }

        if (katelyn.letter[A][1].x > down_left.x)
            katelyn.letter[A][1].x -= delta;
        else
            points_drawn++;

        if (katelyn.letter[A][1].y < down_left.y)
            katelyn.letter[A][1].y += delta;
        else
            points_drawn++;

        if (katelyn.letter[A][3].x < down_right.x)
            katelyn.letter[A][3].x += delta;
        else
            points_drawn++;
        
        if (katelyn.letter[A][3].y < down_right.y)
            katelyn.letter[A][3].y += delta;
        else
            points_drawn++;

        if (katelyn.letter[A][4].x > 120)
            katelyn.letter[A][4].x -= slow_delta;
        else
            points_drawn++;
        
        if (katelyn.letter[A][5].x < 140)
            katelyn.letter[A][5].x += slow_delta;
        else
            points_drawn++;

        retvalue+=((points_drawn == katelyn.numOfPoints[K]) ? 1 : 0);
    } else {
        if (counter >= 0 && counter < timeout) {
            counter++;
            katelyn.lett_clr[A] = RGB_GREEN;
        } else {
            katelyn.lett_clr[A] = RGB_ORANGE;
            counter = RESET;
            retvalue++;
        }
    }
    return retvalue;
}

int NextLetterT_state(void)
{
    int retvalue = T;

    if (!letters_drawn) {
        int delta = 5, slow_delta = 2;
        int points_drawn = 0;
        LONG up_left_x = 170, up_right_x = 220, median = (up_left_x + up_right_x) / 2;

        if (!katelyn.letter[T][0].x) {
            KatelynPenWidth[T] = PEN_WIDTH;
            katelyn.letter[T][0].x = katelyn.letter[T][1].x = median;
            katelyn.letter[T][0].y =  katelyn.letter[T][1].y = upper_boundary;

            katelyn.letter[T][2].x = katelyn.letter[T][3].x = (up_left_x + up_right_x) / 2;
            katelyn.letter[T][2].y = katelyn.letter[T][3].y = (upper_boundary + down_boundary) / 2;

            return retvalue;
        }

        if (katelyn.letter[T][0].x > up_left_x)
            katelyn.letter[T][0].x -= slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[T][1].x < up_right_x)
            katelyn.letter[T][1].x += slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[T][2].y > upper_boundary)
            katelyn.letter[T][2].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[T][3].y < down_boundary)
            katelyn.letter[T][3].y += delta;
        else
            points_drawn++;

        retvalue+=((points_drawn == katelyn.numOfPoints[T]) ? 1 : 0);
    } else {
        if (counter >= 0 && counter < timeout) {
            counter++;
            katelyn.lett_clr[T] = RGB_GREEN;
        } else {
            katelyn.lett_clr[T] = RGB_ORANGE;
            counter = RESET;
            retvalue++;
        }
    }
    return retvalue;
}

int NextLetterE_state(void)
{
    int retvalue = E;

    if (!letters_drawn) {
        int delta = 5, slow_delta = 1;
        int points_drawn = 0;
        LONG left_bndry = 240, right_bndry = 290;

        if (!katelyn.letter[E][0].x) {
            KatelynPenWidth[E] = PEN_WIDTH;
            katelyn.letter[E][0].x = katelyn.letter[E][1].x = (left_bndry + right_bndry) / 2;
            katelyn.letter[E][0].y = katelyn.letter[E][1].y = upper_boundary;

            katelyn.letter[E][2].x = katelyn.letter[E][3].x = left_bndry;
            katelyn.letter[E][2].y = katelyn.letter[E][3].y = (upper_boundary + down_boundary) / 2;

            katelyn.letter[E][4].x = katelyn.letter[E][5].x = (left_bndry + right_bndry) / 2;
            katelyn.letter[E][4].y = katelyn.letter[E][5].y = down_boundary;

            katelyn.letter[E][6].x = katelyn.letter[E][7].x = (left_bndry + left_bndry + 35) / 2;
            katelyn.letter[E][6].y = katelyn.letter[E][7].y = upper_boundary + 40;

            return retvalue;
        }

        if (katelyn.letter[E][0].x > left_bndry)
            katelyn.letter[E][0].x -= slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[E][1].x < right_bndry)
            katelyn.letter[E][1].x += slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[E][2].y > upper_boundary)
            katelyn.letter[E][2].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[E][3].y < down_boundary)
            katelyn.letter[E][3].y += delta;
        else
            points_drawn++;

        if (katelyn.letter[E][4].x > left_bndry)
            katelyn.letter[E][4].x -= slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[E][5].x < right_bndry)
            katelyn.letter[E][5].x += slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[E][6].x > left_bndry)
            katelyn.letter[E][6].x -= slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[E][7].x < left_bndry + 35)
            katelyn.letter[E][7].x += slow_delta;
        else
            points_drawn++;

        retvalue+=((points_drawn == katelyn.numOfPoints[E]) ? 1 : 0);
    } else {
        if (counter >= 0 && counter < timeout) {
            counter++;
            katelyn.lett_clr[E] = RGB_GREEN;
        } else {
            katelyn.lett_clr[E] = RGB_ORANGE;
            counter = RESET;
            retvalue++;
        }
    }
    return retvalue;
}

int NextLetterL_state(void)
{
    int retvalue = L;

    if (!letters_drawn) {
        int delta = 5, slow_delta = 1;
        int points_drawn = 0;
        LONG left_x = 310, right_x = 360;

        if (!katelyn.letter[L][0].x) {
            KatelynPenWidth[L] = PEN_WIDTH;
            katelyn.letter[L][0].x = katelyn.letter[L][1].x = left_x;
            katelyn.letter[L][0].y = katelyn.letter[L][1].y = (upper_boundary + down_boundary) / 2;

            katelyn.letter[L][2].x = katelyn.letter[L][3].x = (left_x + right_x) / 2;
            katelyn.letter[L][2].y = katelyn.letter[L][3].y = down_boundary;

            return retvalue;
        }

        if (katelyn.letter[L][0].y > upper_boundary)
            katelyn.letter[L][0].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[L][1].y < down_boundary)
            katelyn.letter[L][1].y += delta;
        else
            points_drawn++;

        if (katelyn.letter[L][2].x > left_x)
            katelyn.letter[L][2].x -= slow_delta;
        else
            points_drawn++;

        if (katelyn.letter[L][3].x < right_x)
            katelyn.letter[L][3].x += slow_delta;
        else
            points_drawn++;

        retvalue+=((points_drawn == katelyn.numOfPoints[L]) ? 1 : 0);
    } else {
        if (counter >= 0 && counter < timeout) {
            counter++;
            katelyn.lett_clr[L] = RGB_GREEN;
        } else {
            katelyn.lett_clr[L] = RGB_ORANGE;
            counter = RESET;
            retvalue++;
        }
    }
    return retvalue;
}

int NextLetterY_state(void)
{
    int retvalue = Y;

    if (!letters_drawn) {
        int delta = 5;
        int points_drawn = 0;
        LONG left_x = 370, right_x = 410;

        if (!katelyn.letter[Y][0].x) {
            KatelynPenWidth[Y] = PEN_WIDTH;
            katelyn.letter[Y][1].x = katelyn.letter[Y][0].x = left_x + 20;
            katelyn.letter[Y][1].y = katelyn.letter[Y][0].y = upper_boundary + 50;

            katelyn.letter[Y][3].x = katelyn.letter[Y][2].x = left_x + 10;
            katelyn.letter[Y][3].y = katelyn.letter[Y][2].y = down_boundary;

            return retvalue;
        }

        if (katelyn.letter[Y][0].x > left_x)
            katelyn.letter[Y][0].x -= delta;
        else
            points_drawn++;

        if (katelyn.letter[Y][0].y > upper_boundary)
            katelyn.letter[Y][0].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[Y][2].x < right_x)
            katelyn.letter[Y][2].x += delta;
        else
            points_drawn++;

        if (katelyn.letter[Y][2].y > upper_boundary)
            katelyn.letter[Y][2].y -= delta;
        else
            points_drawn++;

        retvalue+=((points_drawn == katelyn.numOfPoints[Y]) ? 1 : 0);
    } else {
        if (counter >= 0 && counter < timeout) {
            counter++;
            katelyn.lett_clr[Y] = RGB_GREEN;
        } else {
            katelyn.lett_clr[Y] = RGB_ORANGE;
            counter = RESET;
            retvalue++;
        }
    }
    return retvalue;
}

int NextLetterN_state(void)
{
    int retvalue = N;

    if (!letters_drawn) {
        int delta  = 5;
        int points_drawn = 0;
        LONG left_x = 430, right_x = 480;

        if (!katelyn.letter[N][0].x) {
            KatelynPenWidth[N] = PEN_WIDTH;
            katelyn.letter[N][0].x = katelyn.letter[N][1].x = left_x;
            katelyn.letter[N][0].y = katelyn.letter[N][1].y = (upper_boundary + down_boundary) / 2;

            katelyn.letter[N][2].x = katelyn.letter[N][3].x = left_x;
            katelyn.letter[N][2].y = katelyn.letter[N][3].y = upper_boundary;

            katelyn.letter[N][4].x = katelyn.letter[N][5].x = right_x;
            katelyn.letter[N][4].y = katelyn.letter[N][5].y = (upper_boundary + down_boundary) / 2;

            return retvalue;
        }

        if (katelyn.letter[N][0].y > upper_boundary)
            katelyn.letter[N][0].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[N][1].y < down_boundary)
            katelyn.letter[N][1].y += delta;
        else
            points_drawn++;

        if (katelyn.letter[N][3].x < right_x)
            katelyn.letter[N][3].x += delta;
        else
            points_drawn++;

        if (katelyn.letter[N][3].y < down_boundary)
            katelyn.letter[N][3].y += delta;
        else
            points_drawn++;

        if (katelyn.letter[N][5].y > upper_boundary)
            katelyn.letter[N][5].y -= delta;
        else
            points_drawn++;

        if (katelyn.letter[N][4].y < down_boundary)
            katelyn.letter[N][4].y += delta;
        else
            points_drawn++;

        retvalue+=((points_drawn == katelyn.numOfPoints[N]) ? 1 : 0);
    } else {
        if (counter >= 0 && counter < timeout) {
            counter++;
            katelyn.lett_clr[N] = RGB_GREEN;
        } else {
            katelyn.lett_clr[N] = RGB_ORANGE;
            counter = RESET;
            retvalue++;
        }
    }
    return retvalue;
}
