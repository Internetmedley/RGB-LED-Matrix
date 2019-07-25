#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "rectangles.hpp"
#include "P3-RGB-LED-matrix.hpp"


namespace tetris{
    
class Tetromino : public hwlib::drawable{
    private:
    hwlib::xy orig_anchor_start, orig_anchor_end;
    hwlib::xy orig_b1_start, orig_b1_end;
    hwlib::xy orig_b2_start, orig_b2_end;
    hwlib::xy orig_b3_start, orig_b3_end;
    hwlib::color ink;
    
    
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
        orig_anchor_start( anchor_start ), orig_anchor_end( anchor_start + hwlib::xy(1, 1) ),
        orig_b1_start( b1_start ), orig_b1_end( b1_start + hwlib::xy(1, 1) ),
        orig_b2_start( b2_start ), orig_b2_end( b2_start + hwlib::xy(1, 1) ),
        orig_b3_start( b3_start ), orig_b3_end( b3_start + hwlib::xy(1, 1) ),
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
    
    void draw( hwlib::window & w ) override {
        hwlib::rectangle( start, anchor_end, ink ).draw( w );
        hwlib::rectangle( b1_start, b1_end, ink ).draw( w );
        hwlib::rectangle( b2_start, b2_end, ink ).draw( w );
        hwlib::rectangle( b3_start, b3_end, ink ).draw( w );
    }
    
    void forget( hwlib::window & w ) {
        //hwlib::cout << start << " - " << anchor_end << '\n';
        hwlib::rectangle( start, anchor_end, hwlib::color( BLACK ) ).draw( w );
        //hwlib::cout << b1_start << " - " << b1_end << '\n';
        hwlib::rectangle( b1_start, b1_end,  hwlib::color( BLACK ) ).draw( w );
        //hwlib::cout << b2_start << " - " << b2_end << '\n';
        hwlib::rectangle( b2_start, b2_end,  hwlib::color( BLACK ) ).draw( w );
        //hwlib::cout << b3_start << " - " << b3_end << '\n';
        hwlib::rectangle( b3_start, b3_end,  hwlib::color( BLACK ) ).draw( w );
    }
    
    bool can_line_clear( matrix::P3_RGB_LED_matrix & b ) {
        auto lowest_x = start.x;
        if( b1_start.x < lowest_x ) { lowest_x = b1_start.x; };
        if( b2_start.x < lowest_x ) { lowest_x = b2_start.x; };
        if( b2_start.x < lowest_x ) { lowest_x = b3_start.x; };
        
        bool full_row = true;
        for( auto y = 12; y <= 51; y++ ){
            full_row = full_row && (b.is_occupied( hwlib::xy(lowest_x, y) ) && b.is_occupied( hwlib::xy(lowest_x+1, y) ));
            hwlib::cout << hwlib::xy(lowest_x, y) << " occupied: " << b.is_occupied( hwlib::xy(lowest_x, y) ) << '\n';
        }
        
        return full_row;
    }
    
    virtual bool can_rotate_clkwise( matrix::P3_RGB_LED_matrix & b ) {
        int newX;
        int newY;
        hwlib::xy new_pos;
        bool updatable = true;
        std::array< hwlib::xy *, 4> start_coordinates = { &start, &b1_start, &b2_start, &b3_start };
        for( auto & i : start_coordinates ) {
            newX = ( 0 * (i->x - start.x)) + (1 * (i->y - start.y));
            newY = (-1 * (i->x - start.x)) + (0 * (i->y - start.y));
            new_pos = hwlib::xy(newX, newY);
            if( (*i + new_pos != start) && (*i + new_pos != b1_start) && (*i + new_pos != b2_start) && (*i + new_pos != b3_start) ) {
                updatable = updatable && !b.is_occupied( *i + new_pos ) && !b.is_occupied( *i + new_pos + hwlib::xy(1, 1) );
            }
        }
        return updatable;
    }
    
    virtual bool can_rotate_ctr_clkwise( matrix::P3_RGB_LED_matrix & b ) {
        int newX;
        int newY;
        hwlib::xy new_pos;
        bool updatable = true;
        std::array< hwlib::xy *, 4> start_coordinates = { &start, &b1_start, &b2_start, &b3_start };
        for( auto & i : start_coordinates ) {
            newX = (0 * (i->x - start.x)) + (-1 * (i->y - start.y));
            newY = (1 * (i->x - start.x)) + ( 0 * (i->y - start.y));
            new_pos = hwlib::xy(newX, newY);
            if( (*i + new_pos != start) && (*i + new_pos != b1_start) && (*i + new_pos != b2_start) && (*i + new_pos != b3_start) ){ 
                updatable = updatable && (!b.is_occupied( *i + new_pos ) && !b.is_occupied( *i + new_pos + hwlib::xy(1, 1) ));
            }
        }
        return updatable;
    }

    
    virtual void rotate_clkwise( hwlib::window & w ) {
        this->forget( w );
        int newX;
        int newY;
        auto anchor_pos = start;
        std::array< hwlib::xy *, 4> start_coordinates = { &start, &b1_start, &b2_start, &b3_start };
        for( auto & i : start_coordinates ){
            *i = *i - anchor_pos;
            newX = ( 0 * i->x) + (1 * i->y);
            newY = (-1 * i->x) + (0 * i->y);
            *i = hwlib::xy( newX, newY );
            *i = *i + anchor_pos;
        }
        anchor_end = start + hwlib::xy(1, 1);
        b1_end = b1_start + hwlib::xy(1, 1);
        b2_end = b2_start + hwlib::xy(1, 1);
        b3_end = b3_start + hwlib::xy(1, 1);
        b3_end = b3_start + hwlib::xy(1, 1);
        this->draw( w );
    }
    
    virtual void rotate_ctr_clkwise( hwlib::window & w ) {
        this->forget( w );
        int newX;
        int newY;
        auto anchor_pos = start;
        std::array< hwlib::xy *, 4> start_coordinates = { &start, &b1_start, &b2_start, &b3_start };
        for( auto & i : start_coordinates ){
            *i = *i - anchor_pos;
            newX = (0 * i->x) + (-1 * i->y);
            newY = (1 * i->x) + ( 0 * i->y);
            *i = hwlib::xy( newX, newY );
            *i = *i + anchor_pos;
        }
        anchor_end = start + hwlib::xy(1, 1);
        b1_end = b1_start + hwlib::xy(1, 1);
        b2_end = b2_start + hwlib::xy(1, 1);
        b3_end = b3_start + hwlib::xy(1, 1);
        b3_end = b3_start + hwlib::xy(1, 1);
        this->draw( w );
    }
        
    void update( hwlib::window & w ) {
        //hwlib::cout << "dit hier" << '\n';
        this->forget( w );
        //hwlib::cout << "update vergeten" << '\n';
        start      = start + speed;
        b1_start   = b1_start + speed;
        b2_start   = b2_start + speed;
        b3_start   = b3_start + speed;
        anchor_end = anchor_end + speed;
        b1_end     = b1_end + speed;
        b2_end     = b2_end + speed;
        b3_end     = b3_end + speed;
        speed      = hwlib::xy(-2, 0);
        this->draw( w );
        //hwlib::cout << "update getekend" << '\n';
    }
    
    void move_down( hwlib::window & w ){
        speed = hwlib::xy(-2, 0);
        this->update( w );
    }
    
     void move_left( hwlib::window & w ){
        speed = hwlib::xy(0, -2);
        this->update( w );
    }
    
     void move_right( hwlib::window & w ){
        speed = hwlib::xy(0, 2);
        this->update( w );
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
        bool updatable = this->can_move_down( b );
        speed = hwlib::xy(-2, 0);
        return updatable;
    }
    
    bool can_move_right( matrix::P3_RGB_LED_matrix & b ){
        speed = hwlib::xy(0, 2);
        bool updatable = this->can_move_down( b );
        speed = hwlib::xy(-2, 0);
        return updatable;
    }
    
    uint8_t get_color(){
        return ink.small;
    }
};
}

#endif  //TETROMINO_HPP