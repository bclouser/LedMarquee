
#include "characters.h"


/*
 [Number of options], [Number of columns]

 NOTE: In the columns, there are two options for each column. 1st space refers to PORTB (rows 1-8).
 the 2nd option refers to PORTD (row1).

 */
const RowNum characters[127][6] =
{    
    { 
    // spacebar 32
        0,  // column1
        0,  // column2
        0,  // column3
        0,  // column4
        0,  // column5
        0  // column6
    },    
  
    //SYMBOLS
    
    {
        //! 33  (25+26+12)
        0,
        0,
        row8+row7+row6+row5+row4+row2,
        0,
        0,
        0
    },
    {
        //" 34 (25+26+35)
        0,
        row8+row7,
        0,
        0,
        row8+row7,
        0
    },
    {
        //# 35  (25+26+14)
        row6+row3,
        row8+row7+row6+row5+row4+row3+row2+row1,
        row6+row3,
        row8+row7+row6+row5+row4+row3+row2+row1,
        row6+row3,
        0
    },
    {
        //$ 36  (25+26+15)
        row7+row6+row3,
        row8+row5+row2,
        row9+row8+row5+row2+row1,
        row8+row5+row2,
        row7+row4+row3,
        0
    },
    {
        //% 37
        row8+row2+row1,
        row9+row7+row3,
        row8+row4,
        row6+row5+row2,
        row7+row3+row1,
        row9+row8+row2
    },
    {
        //& 38
        row7+row3+row2,
        row8+row6+row4+row1,
        row9+row5+row1,
        row9+row6+row4+row2,
        row8+row7+row3,
        row4+row2+row1
    },
    {
        //' 39 
        0,
        row8+row7,
        0,
        0,
        0,
        0
    },
    {
        //( 40
        0,
        row7+row6+row5+row4+row3,
        row8+row2,
        row9,
        0,
        0
    },
    {
        //) 41
        0,
        row9+row1,
        row8+row2,
        row7+row6+row5+row4+row3,
        0,
        0
    },
    {
        //* 42
        row7+row5+row3,
        row6+row4,
        row5,
        row6+row4,
        row7+row5+row3,
        0
    },
    {
        //+ 43
        row5,
        row5,
        row7+row6+row5+row4+row3,
        row5,
        row5,
        0
    },
    {
        //' 44 
        0,
        row8+row7,
        0,
        0,
        0,
        0
    },
    {
        //- 45
        0,
        row5,
        row5,
        row5,
        row5,
        0
    },
    {
        //. 46 
        0, 
        0,
        0,
        0,
        row2,
        0
    },
    {
        /// 47
        0,
        row2+row1,
        row4+row3,
        row6+row5,
        row8+row7,
        0
    },
    

     
    //Numbers
    


    {
        //0 48
        row8+row7+row6+row5+row4+row3+row2,
        row9+row3+row1,
        row9+row5+row4+row1,
        row9+row7+row6+row1,
        row9+row8+row1,
        row8+row7+row6+row5+row4+row3+row2
    },
    {
        //1 49
        row1,
        row9+row1,
        row9+row1,
        allRows,
        row1,
        row1
    },
    {
        //2 50
        row8+row2+row1,
        row9+row3+row1,
        row9+row4+row1,
        row9+row5+row1,
        row9+row6+row1,
        row8+row7+row1
    },
    {
        //3 51
        row8+row2,
        row9+row1,
        row9+row6+row1,
        row9+row6+row1,
        row9+row6+row1,
        row8+row7+row6+row5+row4+row3+row2
    },
    {
        //4 52
        row6+row5,
        row7+row5,
        row8+row5, 
        row9+row5, 
        allRows,
        row5
    },
    {
        //5 53
        row9+row8+row7+row6+row5+row1,
        row9+row6+row1,
        row9+row6+row1,
        row9+row6+row1,
        row9+row6+row2,
        row9+row5+row4+row3
    },
    {
        //6 54
        row7+row6+row5+row4+row3,
        row8+row5+row2,
        row9+row6+row1,
        row9+row6+row1,
        row9+row5+row2,
        row9+row4+row3
    },
    {
        //7 55
        row9,
        row9,
        row9,
        row9+row3+row2+row1,
        row9+row7+row6+row5+row4,
        row9+row8+row6
    },
    {
        //8 56
        row7+row3,
        row8+row6+row4+row2,
        row9+row5+row1,
        row9+row5+row4+row1,
        row8+row6+row4+row2,
        row7+row3
    },
    {
        //9 57
        row7,
        row8+row6+row2,
        row9+row5+row1,
        row9+row5+row1,
        row8+row6+row2,
        row7+row6+row5+row4+row3
    },


    
    //Additonal Symbols
    
    {
        //: 58
        0,
        0,
        0,
        row6+row3,
        0,
        0
    },
    {
        //; 59
        0,
        0,
        row2,
        row6+row3,
        0, 
        0
    },
    {
        //< 60
        0,
        row5,
        row6+row4,
        row7+row3,
        row8+row2,
        0
    },
    {
        //= 61
        0,
        row6+row4,
        row6+row4,
        row6+row4,
        row6+row4,
        0
    },
    {
        //> 62
        0,
        row8+row2,
        row7+row3,
        row6+row4,
        row5,
        0
    },
    {
        //? 63
        0, 
        row7,
        row8,
        row9+row5+row4+row1,
        row8+row6,
        row7
    },
    {
        //@ 64
        row7+row6+row5+row4,
        row8+row3,
        row9+row6+row5+row2,
        row9+row7+row4+row2,
        row9+row6+row5+row2,
        row8+row7+row6+row5+row4
    },
    
    
    
    //UPPERCASE LETTERS
    
    
    {
        //A 65
        row7+row6+row5+row4+row3+row2+row1, 
        row8+row5,
        row9+row5,
        row9+row5,
        row8+row5,
        row7+row6+row5+row4+row3+row2+row1
    },
    { 
        //B 66
        allRows,
        row9+row5+row1,
        row9+row5+row1,
        row9+row5+row1,
        row9+row5+row1,
        row8+row7+row6+row4+row3+row2
    },  
    {   
        //C 67
        row8+row7+row6+row5+row4+row3+row2,
        row9+row1,
        row9+row1,
        row9+row1,
        row9+row1,
        row8+row2   
    },   
    {   
        //D 68
        allRows,
        row9+row1,
        row9+row1,
        row9+row1,
        row8+row2,
        row7+row6+row5+row4+row3
    },
    {
        //E 69
        allRows,
        row9+row5+row1,
        row9+row5+row1,
        row9+row5+row1,
        row9+row1,
        row9+row1
    },  
    { 
        //F 70
        allRows,
        row9+row5,
        row9+row5,
        row9+row5,
        row9,
        row9
    },
    {
        //G 71
        row8+row7+row6+row5+row4+row3+row2,
        row9+row1,
        row9+row1,
        row9+row5+row1,
        row9+row5+row1,
        row8+row5+row4+row3+row2
    },
    {
        //H 72
        allRows,
        row5,
        row5,
        row5,
        row5,
        allRows
    },
    { 
        //I 73
        row9+row1,
        row9+row1,
        allRows,
        row9+row1,
        row9+row1,
        0
    },
    { 
        //J 74
        row9+row2,
        row9+row1,
        row9+row1,
        allRows,
        row9,
        row9
    },
    {
        //K 75
        allRows,
        row5,
        row5,
        row6+row4,
        row7+row3,
        row9+row8+row2+row1
    },
    {
        //L 76
        allRows,
        row1,
        row1,
        row1,
        row1,
        row1
    },
    {  
        //M 77
        allRows,
        row8+row7,
        row6,
        row6,
        row8+row7,
        allRows  
    },
    {  
        //N 78
        allRows,
        row8+row7,
        row6+row5,
        row4+row3,
        row2+row1,
        allRows
    },
    {
        //0 79
        row7+row6+row5+row4+row3,
        row8+row2,
        row9+row1,
        row9+row1,
        row8+row2,
        row7+row6+row5+row4+row3
    },
    {  
        //P 80
        allRows,
        row9+row5,
        row9+row5,
        row9+row5,
        row9+row5,
        row8+row7+row6
    },
    {
        //Q 81
        row7+row6+row5+row4+row3,
        row8+row2,
        row9+row1,
        row9+row3+row1,
        row8+row2,
        row7+row6+row5+row4+row3+row1
    },
    {  
        //R 82
        allRows, 
        row9+row5, 
        row9+row5, 
        row9+row5+row4,
        row9+row5+row3,
        row6+row7+row8 +row2+row1  
    },  
    {
        //S 83
        row8+row7+row6+row2,
        row9+row5+row1,
        row9+row5+row1,
        row9+row5+row1,
        row9+row5+row1,
        row8+row4+row3+row2
    },
    {
        //T 84
        row9,
        row9, 
        allRows,
        row9,
        row9,
        0
    },
    {  
        //U 85
        allRows,
        row1,
        row1,
        row1,
        row1,
        allRows
    },
    {
        //V 86
        row9+row8+row7+row6+row5+row4,
        row3+row2,
        row1,
        row1,
        row3+row2,
        row9+row8+row7+row6+row5+row4
    },
    {
        //W 87
        allRows,
        row1,
        row2,
        row2,
        row1,
        allRows
    },
    {
        //X 88
        row9+row8+row2+row1,
        row7+row6+row4+row3,
        row5,
        row5,
        row7+row6+row4+row3,
        row9+row8+row2+row1
    },
    {
        //Y 89
        row9+row8,
        row7+row6, 
        row5+row4+row3+row2+row1,
        row7+row6, 
        row9+row8
    },
    {
        //Z 90 
        row9+row2+row1,
        row9+row3+row1,
        row9+row5+row4+row1,
        row9+row6+row5+row1,
        row9+row7+row1,
        row9+row8+row1
    },


    {
        //[ 91
        0, 
        row8+row7+row6+row5+row4+row3+row2, 
        row8+row2,
        row8+row2,
        0,
        0,
    },
    {
        //\ 92
        0,
        row8+row7,
        row6+row5,
        row4+row3,
        row2+row1,
        0
    },
    {
        //] 93
        0,
        0,
        row8+row2,
        row8+row2, 
        row8+row7+row6+row5+row4+row3+row2,
        0 
    },
    {
        //^ 94
        row6,
        row7,
        row8,
        row7,
        row6,
        0
    },
    {
        //_ 95
        row1,
        row1,
        row1,
        row1,
        row1,
        row1
    },
    {
        //` 96
        0, 
        0, 
        row8,
        row7,
        0,
        0,
    },

    
    
    //LOWERCASE LETTERS
    
    
    
    {
    //a 97
    row5+row4+row3+row2,
    row6+row1,
    row6+row1,
    row6+row2,
    row5+row4+row3+row2+row1,
    0
    },
    {
    //b 98
    allRows,
    row5+row2,
    row6+row1,
    row6+row1,
    row5+row2,
    row5+row4+row3+row2
    },

    {
    //c 99
    row5+row4+row3+row2,
    row6+row1,
    row6+row1,
    row6+row1, 
    row5+row2,
    0
    },
    {
    //d 100
    row5+row4+row3+row2,
    row6+row1,
    row6+row1,
    row6+row1,
    row5+row2,
    allRows
    },
    {
    //e 101
    row4+row3+row2,
    row5+row3+row1,
    row6+row3+row1,
    row6+row3+row1,
    row5+row3+row1,
    row4
    },
    {
    //f 102
    row4,
    row7+row6+row5+row4+row3+row2+row1,
    row8+row4,
    row8,
    row7,
    0
    },
    {
    //g 103
    row5,
    row4+row6,
    row1+row3+row7,
    row3+row7,
    row1+row4+row6,
    row2+row3+row4+row5
    },
    {
    //h 104
    allRows,
    row5,
    row6,
    row6,
    row6,
    row5+row4+row3+row2+row1
    },
    {
    //i 105
    0,
    0,
    row7+row4+row3+row2+row1,
    0,
    0,
    0
    },
    {
    //j 106
    row2,
    row1,
    row1,
    row8+row5+row4+row3+row2,
    0,
    0
    },
    {
    //k 107
    row7+row6+row5+row4+row3+row2+row1,
    row4,
    row4,
    row5+row3,
    row6+row2+row1,
    0
    },
    {
    //l 108
    0,
    0,
    row7+row6+row5+row4+row3+row2+row1,
    0,
    0,
    0
    },
    {
    //m  109
    row6+row5+row4+row3+row2+row1,
    row5+row4,
    row3,
    row3,
    row5+row4,
    row6+row5+row4+row3+row2+row1
    },
    {
    //n 110
    row6+row5+row4+row3+row2+row1, 
    row5,
    row6,
    row6,
    row5,
    row4+row3+row2+row1
    },
    {
    //o 111
    row4+row3,
    row5+row2,
    row6+row1,
    row6+row1,
    row5+row2,
    row4+row3
    },
    {
    //p 112
    row4+row3+row2+row1,
    row5+row3,
    row6+row2,
    row6+row2,
    row5+row3,
    row4
    },
    {
    //q 113
    row4,
    row5+row3,
    row6+row2,
    row6+row2,
    row5+row3,
    row4+row3+row2+row1
    },
    {
    //r 114
    row4+row3+row2+row1,
    row5,
    row6,
    row6,
    row5,
    0
    },
    {
    //s 115
    row5+row1,
    row6+row4+row1,
    row6+row4+row1,
    row6+row4+row1,
    row6+row3+row2,
    0
    },
    {
    //t 116
    0,
    row5,
    row7+row6+row5+row4+row3+row2+row1,
    row5,
    0,
    0
    },
    {
    //u 117
    row6+row5+row4+row3+row2,
    row1,
    row1,
    row1,
    row6+row5+row4+row3+row2,
    0
    },
    {
    //v 118
    row6+row5+row4+row3,
    row2,
    row1,
    row2,
    row6+row5+row4+row3,
    0
    },
    {
    //w 119
    row6+row5+row4+row3+row2,
    row1,
    row2,
    row2,
    row1,
    row6+row5+row4+row3+row2
    },
    {
    //x 120
    row6+row1,
    row5+row2,
    row4+row3,
    row5+row2,
    row6+row1,
    0
    },
    {
    //y 121
    row6+row5,
    row4+row1,
    row3+row2,
    row4,
    row6+row5,
    0
    },
    {
    //z 122
    0,
    row6+row2+row1,
    row6+row3+row1,
    row6+row4+row1,
    row5+row1,
    0
    },


   
    //More Symbols
    


    {
        //{ 123
        0,
        0,
        row5,
        row8+row7+row6+row4+row3+row2,
        row8+row2,
        0
    },
    {
        //| 124
        0, 
        0,
        0,
        row9+row8+row7+row6+row4+row3+row2+row1,
        0,
        0
    },
    {
        //} 125
        0,
        row8+row2,
        row8+row7+row6+row4+row3+row2,
        row5,
        0,
        0
    },
    {
        //~ 126
        row6,
        row7,
        row6,
        row5,
        row4,
        row5
    },
};