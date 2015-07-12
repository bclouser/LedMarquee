
#include "characters.h"


/*
 [Number of options], [Number of columns]

 NOTE: In the columns, there are two options for each column. 1st space refers to PORTB (rows 1-8).
 the 2nd option refers to PORTD (row9).

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
        row2+row3+row4+row5+row6+row8,
        0,
        0,
        0
    },
    {
        //" 34 (25+26+35)
        0,
        row2+row3,
        0,
        0,
        row2+row3,
        0
    },
    {
        //# 35  (25+26+14)
        row4+row7,
        row2+row3+row4+row5+row6+row7+row8+row9,
        row4+row7,
        row2+row3+row4+row5+row6+row7+row8+row9,
        row4+row7,
        0
    },
    {
        //$ 36  (25+26+15)
        row3+row4+row7,
        row2+row5+row8,
        row1+row2+row5+row8+row9,
        row2+row5+row8,
        row3+row6+row7,
        0
    },
    {
        //% 37
        row2+row8+row9,
        row1+row3+row7,
        row2+row6,
        row4+row5+row8,
        row3+row7+row9,
        row1+row2+row8
    },
    {
        //& 38
        row3+row7+row8,
        row2+row4+row6+row9,
        row1+row5+row9,
        row1+row4+row6+row8,
        row2+row3+row7,
        row6+row8+row9
    },
    {
        //' 39 
        0,
        row2+row3,
        0,
        0,
        0,
        0
    },
    {
        //( 40
        0,
        row3+row4+row5+row6+row7,
        row2+row8,
        row1,
        0,
        0
    },
    {
        //) 41
        0,
        row1+row9,
        row2+row8,
        row3+row4+row5+row6+row7,
        0,
        0
    },
    {
        //* 42
        row3+row5+row7,
        row4+row6,
        row5,
        row4+row6,
        row3+row5+row7,
        0
    },
    {
        //+ 43
        row5,
        row5,
        row3+row4+row5+row6+row7,
        row5,
        row5,
        0
    },
    {
        //' 44 
        0,
        row2+row3,
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
        row8,
        0
    },
    {
        /// 47
        0,
        row8+row9,
        row6+row7,
        row4+row5,
        row2+row3,
        0
    },
    

     
    //Numbers
    


    {
        //0 48
        row2+row3+row4+row5+row6+row7+row8,
        row1+row7+row9,
        row1+row5+row6+row9,
        row1+row3+row4+row9,
        row1+row2+row9,
        row2+row3+row4+row5+row6+row7+row8
    },
    {
        //1 49
        row9,
        row1+row9,
        row1+row9,
        allRows,
        row9,
        row9
    },
    {
        //2 50
        row2+row8+row9,
        row1+row7+row9,
        row1+row6+row9,
        row1+row5+row9,
        row1+row4+row9,
        row2+row3+row9
    },
    {
        //3 51
        row2+row8,
        row1+row9,
        row1+row4+row9,
        row1+row4+row9,
        row1+row4+row9,
        row2+row3+row4+row5+row6+row7+row8
    },
    {
        //4 52
        row4+row5,
        row3+row5,
        row2+row5, 
        row1+row5, 
        allRows,
        row5
    },
    {
        //5 53
        row2+row3+row4+row5,
        row1+row5+row9,
        row1+row3+row9,
        row1+row3+row9,
        row1+row4+row8,
        row1+row6+row7
    },
    {
        //6 54
        row3+row4+row5+row6+row7,
        row2+row5+row8,
        row1+row4+row9,
        row1+row4+row9,
        row1+row5+row8,
        row1+row6+row7
    },
    {
        //7 55
        row2,
        row1,
        row1,
        row1+row7+row8+row9,
        row1+row4+row5,
        row2+row3+row4
    },
    {
        //8 56
        row3+row7,
        row2+row4+row6+row8,
        row1+row4+row9,
        row1+row4+row9,
        row2+row4+row6+row8,
        row3+row7
    },
    {
        //9 57
        row3,
        row2+row4+row8,
        row1+row5+row9,
        row1+row5+row9,
        row2+row4+row8,
        row3+row4+row5+row6+row7
    },


    
    //Additonal Symbols
    
    {
        //: 58
        0,
        0,
        0,
        row4+row7,
        0,
        0
    },
    {
        //; 59
        0,
        0,
        row8,
        row4+row7,
        0, 
        0
    },
    {
        //< 60
        0,
        row5,
        row4+row6,
        row3+row7,
        row2+row8,
        0
    },
    {
        //= 61
        0,
        row4+row6,
        row4+row6,
        row4+row6,
        row4+row6,
        0
    },
    {
        //> 62
        0,
        row2+row8,
        row3+row7,
        row4+row6,
        row5,
        0
    },
    {
        //? 63
        0, 
        row3,
        row2,
        row1+row5+row6+row9,
        row2+row4,
        row3
    },
    {
        //@ 64
        row3+row4+row5+row6,
        row2+row7,
        row1+row4+row5+row8,
        row1+row3+row6+row8,
        row1+row4+row5+row8,
        row2+row3+row4+row5+row6
    },
    
    
    
    //UPPERCASE LETTERS
    
    
    {
        //A 65
        row3+row4+row5+row6+row7+row8+row9, 
        row1+row5,
        row2+row5,
        row1+row5,
        row2+row5,
        row3+row4+row5+row6+row7+row8+row9
    },
    { 
        //B 66
        allRows,
        row1+row5+row9,
        row1+row5+row9,
        row1+row5+row9,
        row1+row5+row9,
        row2+row3+row4+row6+row7+row8
    },  
    {   
        //C 67
        row2+row3+row4+row5+row6+row7+row8,
        row1+row9,
        row1+row9,
        row1+row9,
        row1+row9,
        row2+row8   
    },   
    {   
        //D 68
        allRows,
        row1+row9,
        row1+row9,
        row1+row9,
        row2+row8,
        row3+row4+row5+row6+row7
    },
    {
        //E 69
        allRows,
        row1+row5+row9,
        row1+row5+row9,
        row1+row5+row9,
        row1+row9,
        row1+row9
    },  
    { 
        //F 70
        allRows,
        row1+row5,
        row1+row5,
        row1+row5,
        row1,
        row1
    },
    {
        //G 71
        row2+row3+row4+row6+row7,
        row1+row9,
        row1+row9,
        row1+row5+row9,
        row1+row5+row9,
        row2+row5+row6+row7+row8
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
        row1+row9,
        row1+row9,
        allRows,
        row1+row9,
        row1+row9,
        0
    },
    { 
        //J 74
        row1+row8,
        row1+row9,
        row1+row9,
        allRows,
        row1,
        row1
    },
    {
        //K 75
        allRows,
        row5,
        row5,
        row4+row6,
        row3+row7,
        row1+row2+row8+row9
    },
    {
        //L 76
        allRows,
        row9,
        row9,
        row9,
        row9,
        row9
    },
    {  
        //M 77
        allRows,
        row2+row3,
        row4,
        row4,
        row2+row3,
        allRows  
    },
    {  
        //N 78
        allRows,
        row2+row3,
        row4+row5,
        row6+row7,
        row8+row9,
        allRows
    },
    {
        //0 79
        row3+row4+row5+row6+row7,
        row2+row8,
        row1+row9,
        row1+row9,
        row2+row8,
        row3+row4+row5+row6+row7
    },
    {  
        //P 80
        allRows,
        row1+row5,
        row1+row5,
        row1+row5,
        row1+row5,
        row2+row3+row4
    },
    {
        //Q 81
        row3+row4+row5+row6+row7,
        row2+row8,
        row1+row9,
        row1+row7+row9,
        row2+row8,
        row3+row4+row5+row6+row7+row9
    },
    {  
        //R 82
        allRows, 
        row1+row5, 
        row1+row5, 
        row1+row5+row6,
        row1+row5+row7,
        row4+row3+row2 +row8+row9  
    },  
    {
        //S 83
        row2+row3+row4+row8,
        row1+row5+row9,
        row1+row5+row9,
        row1+row5+row9,
        row1+row5+row9,
        row2+row6+row7+row8
    },
    {
        //T 84
        row1,
        row1, 
        allRows,
        row1,
        row1,
        0
    },
    {  
        //U 85
        allRows,
        row9,
        row9,
        row9,
        row9,
        allRows
    },
    {
        //V 86
        row1+row2+row3+row4+row5+row6,
        row7+row8,
        row9,
        row9,
        row7+row8,
        row1+row2+row3+row4+row5+row6
    },
    {
        //W 87
        allRows,
        row9,
        row8,
        row8,
        row9,
        allRows
    },
    {
        //X 88
        row1+row2+row8+row9,
        row3+row4+row6+row7,
        row5,
        row5,
        row3+row4+row6+row7,
        row1+row2+row8+row9
    },
    {
        //Y 89
        row1+row2,
        row3+row4, 
        row5+row6+row7+row8+row9,
        row3+row4, 
        row1+row2
    },
    {
        //Z 90 
        row1+row8+row9,
        row1+row7+row9,
        row1+row5+row6+row9,
        row1+row4+row5+row9,
        row1+row3+row9,
        row1+row2+row9
    },


    {
        //[ 91
        0, 
        row2+row3+row4+row5+row6+row7+row8, 
        row2+row8,
        row2+row8,
        0,
        0,
    },
    {
        //\ 92
        0,
        row2+row3,
        row4+row5,
        row6+row7,
        row8+row9,
        0
    },
    {
        //] 93
        0,
        0,
        row2+row8,
        row2+row8, 
        row2+row3+row4+row5+row6+row7+row8,
        0 
    },
    {
        //^ 94
        row4,
        row3,
        row2,
        row3,
        row4,
        0
    },
    {
        //_ 95
        row9,
        row9,
        row9,
        row9,
        row9,
        row9
    },
    {
        //` 96
        0, 
        0, 
        row2,
        row3,
        0,
        0,
    },

    
    
    //LOWERCASE LETTERS
    
    
    
    {
    //a 97
    row5+row6+row7+row8,
    row4+row9,
    row4+row9,
    row4+row8,
    row5+row6+row7+row8+row9,
    0
    },
    {
    //b 98
    allRows,
    row5+row8,
    row4,
    row4,
    row4,
    row5+row6+row7+row8
    },
    {
    //c 99
    row5+row6+row7+row8,
    row4+row9,
    row4+row9,
    row4+row9, 
    row5+row8,
    0
    },
    {
    //d 100
    row5+row6+row7+row8,
    row4+row9,
    row4+row9,
    row4+row9,
    row5+row8,
    allRows
    },
    {
    //e 101
    row6+row7+row8,
    row5+row7+row9,
    row4+row7+row9,
    row4+row7+row9,
    row5+row7+row9,
    row6
    },
    {
    //f 102
    row6,
    row3+row4+row5+row6+row7+row8+row9,
    row2+row6,
    row2,
    row3,
    0
    },
    {
    //g 103
    row6+row9,
    row5+row7,
    row4+row8,
    row4+row8,
    row5+row7,
    row6+row7+row8+row9
    },
    {
    //h 104
    allRows,
    row5,
    row4,
    row4,
    row4,
    row5+row6+row7+row8+row9
    },
    {
    //i 105
    0,
    0,
    row3+row6+row7+row8+row9,
    0,
    0,
    0
    },
    {
    //j 106
    row8,
    row9,
    row9,
    row2+row5+row6+row7+row8,
    0,
    0
    },
    {
    //k 107
    row3+row4+row5+row6+row7+row8+row9,
    row6,
    row6,
    row5+row7,
    row4+row8+row9,
    0
    },
    {
    //l 108
    0,
    0,
    row3+row4+row5+row6+row7+row8+row9,
    0,
    0,
    0
    },
    {
    //m  109
    row4+row5+row6+row7+row8+row9,
    row5+row6,
    row7,
    row7,
    row5+row6,
    row4+row5+row6+row7+row8+row9
    },
    {
    //n 110
    row4+row5+row6+row7+row8+row9, 
    row5,
    row4,
    row4,
    row5,
    row6+row7+row8+row9
    },
    {
    //o 111
    row6+row7,
    row5+row8,
    row4+row9,
    row4+row9,
    row5+row8,
    row6+row7
    },
    {
    //p 112
    row6+row7+row8+row9,
    row5+row7,
    row4+row8,
    row4+row8,
    row5+row7,
    row6
    },
    {
    //q 113
    row6,
    row5+row7,
    row4+row8,
    row4+row8,
    row5+row7,
    row6+row7+row8+row9
    },
    {
    //r 114
    row6+row7+row8+row9,
    row5,
    row4,
    row4,
    row5,
    0
    },
    {
    //s 115
    row5+row9,
    row4+row6+row9,
    row4+row6+row9,
    row4+row6+row9,
    row4+row7+row8,
    0
    },
    {
    //t 116
    0,
    row5,
    row3+row4+row5+row6+row7+row8+row9,
    row5,
    0,
    0
    },
    {
    //u 117
    row4+row5+row6+row7+row8,
    row9,
    row9,
    row9,
    row4+row5+row6+row7+row8,
    0
    },
    {
    //v 118
    row4+row5+row6+row7,
    row8,
    row9,
    row8,
    row4+row5+row6+row7,
    0
    },
    {
    //w 119
    row4+row5+row6+row7+row8,
    row9,
    row8,
    row8,
    row9,
    row4+row5+row6+row7+row8
    },
    {
    //x 120
    row4+row9,
    row5+row8,
    row6+row7,
    row5+row8,
    row4+row9,
    0
    },
    {
    //y 121
    row4+row5,
    row6+row9,
    row7+row8,
    row6,
    row4+row5,
    0
    },
    {
    //z 122
    0,
    row4+row8+row9,
    row4+row7+row9,
    row4+row6+row9,
    row5+row9,
    0
    },


   
    //More Symbols
    


    {
        //{ 123
        0,
        0,
        row5,
        row2+row3+row4+row6+row7+row8,
        row2+row8,
        0
    },
    {
        //| 124
        0, 
        0,
        0,
        row1+row2+row3+row4+row6+row7+row8+row9,
        0,
        0
    },
    {
        //} 125
        0,
        row2+row8,
        row2+row3+row4+row6+row7+row8,
        row5,
        0,
        0
    },
    {
        //~ 126
        row4,
        row3,
        row4,
        row5,
        row6,
        row5
    },
};