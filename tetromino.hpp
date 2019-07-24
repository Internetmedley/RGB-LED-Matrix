#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "pixel.hpp"
#include "P3-RGB-LED-matrix.hpp"


namespace tetris{
    
class Tetromino : public hwlib::drawable{
    private:
    hwlib::xy orig_anchor_start, orig_anchor_end;
    hwlib::xy orig_b1_start, orig_b1_end;
    hwlib::xy orig_b2_start, orig_b2_end;
    hwlib::xy orig_b3_start, orig_b3_end;
    hwlib::color ink;
    
    void update( hwlib::window & w ) {
        forget( w );
        start      = start + speed;
        b1_start   = b1_start + speed;
        b2_start   = b2_start + speed;
        b3_start   = b3_start + speed;
        anchor_end = anchor_end + speed;
        b1_end     = b1_end + speed;
        b2_end     = b2_end + speed;
        b3_end     = b3_end + speed;
        speed      = hwlib::xy(-2, 0);
        draw( w );
    }
    
    public:
    hwlib::xy anchor_end; //anchor_start is start from drawables
    hwlib::xy b1_start, b1_end;
    hwlib::xy b2_start, b2_end;
    hwlib::xy b3_start, b3_end;
    hwlib::xy speed;
    
    Tetromino( hwlib::xy anchor_start,
               hwlib::xy b1_start,
               hwlib::xy b2_start,
               hwlib::xy b3_start,
               uint8_t col ):
        hwlib::drawable( anchor_start ),
        orig_anchor_start( anchor_start ), orig_anchor_end( anchor_end ),
        orig_b1_start( b1_start ), orig_b1_end( b1_end ),
        orig_b2_start( b2_start ), orig_b2_end( b2_end ),
        orig_b3_start( b3_start ), orig_b3_end( b3_end ),
        ink( col ),
        anchor_end( anchor_start + hwlib::xy(1, 1) ),
        b1_start( b1_start ), b1_end( b1_start + hwlib::xy(1, 1) ),
        b2_start( b2_start ), b2_end( b2_start + hwlib::xy(1, 1) ),
        b3_start( b3_start ), b3_end( b3_start + hwlib::xy(1, 1) ),
        speed( hwlib::xy(-2, 0) )
    {}
    
    void reset() {
        start      = orig_anchor_start;
        b1_start   = orig_b1_start;
        b2_start   = orig_b2_start;
        b3_start   = orig_b3_start;
        anchor_end = orig_anchor_end;
        b1_end     = orig_b1_end;
        b2_end     = orig_b2_end;
        b3_end     = orig_b3_end;
    }
    
    //virtual void rotate();
    
    void draw( hwlib::window & w ) override {
        hwlib::rectangle( start, anchor_end, ink ).draw( w );
        hwlib::rectangle( b1_start, b1_end, ink ).draw( w );
        hwlib::rectangle( b2_start, b2_end, ink ).draw( w );
        hwlib::rectangle( b3_start, b3_end, ink ).draw( w );
    }
    
    void forget( hwlib::window & w ) {
        hwlib::rectangle( start, anchor_end, hwlib::color( BLACK ) ).draw( w );
        hwlib::rectangle( b1_start, b1_end,  hwlib::color( BLACK ) ).draw( w );
        hwlib::rectangle( b2_start, b2_end,  hwlib::color( BLACK ) ).draw( w );
        hwlib::rectangle( b3_start, b3_end,  hwlib::color( BLACK ) ).draw( w );
    }
    
    void move_down( hwlib::window & w ){
        speed = hwlib::xy(-2, 0);
        update( w );
    }
    
     void move_left( hwlib::window & w ){
        speed = hwlib::xy(0, -2);
        update( w );
    }
    
     void move_right( hwlib::window & w ){
        speed = hwlib::xy(0, 2);
        update( w );
    }
    
    bool can_move_down( matrix::P3_RGB_LED_matrix & b ) {
        bool updatable = true;
        std::array< hwlib::xy *, 4> coordinates = { &start, &b1_start, &b2_start, &b3_start };
        for( auto & i : coordinates ){
            if( (*i + speed != start) && (*i + speed != b1_start) && (*i + speed != b2_start) && (*i + speed != b3_start) ){
                updatable = updatable && (!b.is_occupied( *i + speed ) && !b.is_occupied( *i + hwlib::xy(1, 1) + speed ));
                //hwlib::cout << "XY: " << i->x << " " << i->y << '\t'; 
                //hwlib::cout << "Occupied?: " << (b.is_occupied( *i + speed ) || b.is_occupied( *i + hwlib::xy(1, 1) + speed ))  << '\t';
            }
        }
        //hwlib::cout << '\n';
        return updatable;
    }
    
    bool can_move_left( matrix::P3_RGB_LED_matrix & b ){
        speed = hwlib::xy(0, -2);
        bool updatable = can_move_down( b );
        speed = hwlib::xy(-2, 0);
        return updatable;
    }
    
    bool can_move_right( matrix::P3_RGB_LED_matrix & b ){
        speed = hwlib::xy(0, 2);
        bool updatable = can_move_down( b );
        speed = hwlib::xy(-2, 0);
        return updatable;
    }
    
    uint8_t get_color(){
        return ink.small;
    }
};


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


#endif  //TETROMINO_HPP