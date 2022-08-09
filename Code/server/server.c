/*  Project:        Audio Encryption Project
    Description:    Element 14 Pi-Fest challenge submission. System comprised of three Raspberry Pi Picos 
                    named Bob, Alice, and Server. System securely sends audio between Bob and Alice using 
                    a lightweight cryptographic algorithm. Keys are exchanged using X3DH. Bob receives audio 
                    as input, encrypts it, and sends it to the server. Alice receives the message, decrypts
                    it, and plays the audio.
    Documentation:  https://github.com/lin0m/lightweight-cryptography
    Date:           Aug 9th, 2022
    School:         Cal Poly Pomona 
    Group:          Dr. Aly's Research Team
    Members:        Melvin Relf
                    Russell Hua
                    Alexander Ea
                    Shahzman Saqib
                    Muhammed Kamil
                    Patricia Ankunda
                    Lino Mercado-Esquivias
*/

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

    /*---------------WAIT FOR MESSAGE---------------*/

    /*--------------CHECK MESSAGE TYPE--------------*/
    switch(msg_type){
    /*--------------------TYPE 0--------------------*/
        case 0:
        break;
    /*--------------------TYPE 1--------------------*/
        case 1:
        break;
    /*--------------------TYPE 2--------------------*/
        case 2:
        break;
    /*--------------------TYPE 3--------------------*/
        case 3;
        break;
        default:
        // error message
    }
    return 0;
}

void type0(message){

}

void type1(message){

}

void type2(message){

}

void type3(message){

}