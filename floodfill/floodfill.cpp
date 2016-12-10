
#include "floodfill.h"
#include <Math.h>
using namespace std;




void floodfill::initialize (int input [4][4])
{
   for (int i=0; i < 4; i++)
        for(int j = 0; j < 4 ;j++)
        {
        (input_array)[i][j] = input[i][j];
        }
}

void floodfill::updateNeighbours(byte i,byte j) // return number of  editable neighbours
{
    int counter = 0;
    int p_i = 0;                         // plus i
    int p_j = 1;                         // plus j
    int pp_i;                          //prev plus i
    int ti;                          //temp_i
    int tj;
for (int z = 1; z < 5; z++)
            {
                ti = i + (p_i);               // (+0,+1)->(-1,+0)->(+0,+1)->(-1,0)
                tj = j + (p_j);
                pp_i =  p_i;
                p_i = ((int) pow(-1, z))*(p_j);
                p_j = ((int) pow(-1, z))*(pp_i);
               if ((ti >= 0) && (ti < (4)) && (tj >= 0) && (tj < (4)))
               {
                    if( (input_array[ti][tj] == 0) && !((ti == si) && (tj == sj))  )
                    {


                        que.enqueue((byte)ti,(byte)tj);

                           input_array[ti][tj] += input_array[i][j]+1;
                           counter_ ++;

                         for(int i = 0 ; i < 4 ; i ++)
                                for( int j = 0 ; j < 4 ;j++)
                                    {
                                        cout << input_array[i][j] ;
                                        if(j == 3)
                                        cout <<endl ;

                                    }
                        cout << "MIAFTW"<<" "<<(int)que.N<<" "<<(int)counter_<<endl;

                    }

                }
            }

}

int floodfill::blackNodes()
{
    int counter = 0 ;
    for(int i = 0; i < 4 ; i++)
        for(int j = 0; j < 4 ;j++)
    {
        if (input_array[i][j] == -1)
            counter ++;
    }
return counter;
}

void floodfill::compute(byte i, byte j)
{
    si = i;
    sj = j;
    IJ temp;
    temp.i = i ;
    temp.j = j;
    int bNodes = blackNodes();



    while( 1)
    {
        updateNeighbours(temp.i,temp.j);
        temp = que.dequeue();


    }


   }



void QUEUE::enqueue(byte i, byte j)
{
    IJ in;
    in.i=i;
    in.j=j;
   arrayList[last++] = in;
   if (last == 16) last = 0;
   N++;

}

IJ QUEUE::dequeue()
{
IJ out = arrayList[first];
    N--;
    first++;
    if(first == 16) first = 0;
    return out;
}
