#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "pixel.hpp"
#include "buffer.hpp"


namespace tetris{
    
class Tetromino : public matrix::Drawable{
    protected:
    matrix::Rectangle anchor;   
    matrix::Rectangle block1;
    matrix::Rectangle block2;
    matrix::Rectangle block3;
    

public:
    Tetromino( hwlib::xy anchor_start, hwlib::xy anchor_end, 
               hwlib::xy b1_start, hwlib::xy b1_end,
               hwlib::xy b2_start, hwlib::xy b2_end, 
               hwlib::xy b3_start, hwlib::xy b3_end, 
               uint8_t color ):
        Drawable( anchor_start, color ),
        anchor( anchor_start, anchor_end, color ),
        block1( b1_start, b1_end, color ),
        block2( b2_start, b2_end, color ),
        block3( b3_start, b3_end, color )
    {}
    
    void draw( Buffer & b ) override {
        anchor.draw( b );
        block1.draw( b );
        block2.draw( b );
        block3.draw( b );
    }
};


class I_shape : public Tetromino{
private:
    

public:
    I_shape( hwlib::xy s = hwlib::xy(50, 12), uint8_t color = WHITE ): 
        Tetromino( hwlib::xy(s.x, s.y+4), hwlib::xy(s.x+1, s.y+5), 
                   hwlib::xy(s), hwlib::xy(s.x+1, s.y+1),
                   hwlib::xy(s.x, s.y+2), hwlib::xy(s.x+1, s.y+3),
                   hwlib::xy(s.x, s.y+2), hwlib::xy(s.x+1, s.y+3), color ) 
    {}

};
}


#endif  //TETROMINO_HPP