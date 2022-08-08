#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include "espHelper.h"

void type0(message){};
void type1(message){};
void type2(message){};
void type3(message){};

int main()
{
    stdio_init_all();
    // variables
    int msg_type;
    int aliceID;
    int bobID;

    /*----------------------------------WAIT FOR MESSAGE-----------------------------------*/

    /*---------------------------------CHECK MESSAGE TYPE----------------------------------*/
    switch(msg_type){
        case 0:
        break;
        case 1:
        break;
        case 2:
        break;
        case 3;
        break;
        default:
        // error message
    }
    return 0;
}

/*---------------------------------------TYPE 0----------------------------------------*/
void type0(message){

}
/*---------------------------------------TYPE 1----------------------------------------*/
void type1(message){
        
}
/*---------------------------------------TYPE 2----------------------------------------*/
void type2(message){
        
}
/*---------------------------------------TYPE 3----------------------------------------*/
void type3(message){
        
}