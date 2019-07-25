#ifndef TETRIS_SHAPES_HPP
#define TETRIS_SHAPES_HPP

#include "tetromino.hpp"


namespace tetris{
    
class I_shape : public Tetromino{
    public:
    I_shape( hwlib::xy s = hwlib::xy(50, 12), uint8_t color = CYAN ): 
        Tetromino( hwlib::xy(s.x, s.y+4),
                   hwlib::xy(s),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x, s.y+6),
                   color ) 
    {}
};

class O_shape : public Tetromino{
public:
    O_shape( hwlib::xy s = hwlib::xy(48, 14), uint8_t color = YELLOW ): 
        Tetromino( hwlib::xy(s),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x+2, s.y),
                   hwlib::xy(s.x+2, s.y+2),
                   color ) 
    {}
    
    bool can_rotate_clkwise( matrix::P3_RGB_LED_matrix & b ) override { return false; }         //there is no use to rotating this piece
    bool can_rotate_ctr_clkwise( matrix::P3_RGB_LED_matrix & b ) override { return false; }
    void rotate_clkwise( hwlib::window & w ) override {}
    void rotate_ctr_clkwise( hwlib::window & w ) override {}
};

class T_shape : public Tetromino{
public:
    T_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = PURPLE ): 
        Tetromino( hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x+2, s.y+2),
                   hwlib::xy(s.x, s.y+4),
                   color )
    {}
};

class J_shape : public Tetromino{
public:
    J_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = BLUE ): 
        Tetromino( hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x+2, s.y),
                   hwlib::xy(s.x, s.y+4),
                   color )
    {}
};

class L_shape : public Tetromino{
public:
    L_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = WHITE ): 
        Tetromino( hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x, s.y+4),
                   hwlib::xy(s.x+2, s.y+4),
                   color )
    {}
};

class S_shape : public Tetromino{
public:
    S_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = GREEN ): 
        Tetromino( hwlib::xy(s.x+2, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x+2, s.y+4),
                   color )
    {}
};

class Z_shape : public Tetromino{
public:
    Z_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = RED ): 
        Tetromino( hwlib::xy(s.x+2, s.y+2),
                   hwlib::xy(s.x, s.y+4),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x+2, s.y),
                   color )
    {}
};
}

#endif //TETRIS_SHAPES_HPP