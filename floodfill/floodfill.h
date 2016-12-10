
#ifndef floodfill_H_INCLUDED
#define floodfill_H_INCLUDED

#include <iostream>

typedef unsigned char byte;



struct VALS {
   byte i;
   byte j;
};
typedef struct VALS IJ;


class QUEUE{
    private :
 IJ arrayList[16];

 byte last=0;
 byte first=0;

    public:
       void enqueue(byte , byte);
       IJ   dequeue(void);
       byte N=0;

};



class floodfill{

    private :
QUEUE que;
byte counter_ = 0;
int  input_array[4][4];
void updateNeighbours(byte , byte);
byte si;
byte sj;


int blackNodes(void);

    public  :
    void compute(byte , byte);
    void initialize(int  [4][4]);

};
#endif
