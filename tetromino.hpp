#ifndef TETROMINO_HPP
#define TETROMINO_HPP

/// @file

#include "rectangles.hpp"
#include "P3-RGB-LED-matrix.hpp"

namespace tetris{

/// \brief
/// tetromino class
/// \details
/// This class inherits from hwlib::drawable to make shapes of 4 blocks like the ones used in Tetris.
/// Each block of the shape is represented by 4 pixels.
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
    
    /// \brief
    /// constructor from explicit values
    /// \details
    /// This constructor initializes all coordinate variables using given start positions for each of the 4 blocks.
    /// An end coordinate is always a start value + xy(1, 1 ) to represent a block of 4 pixels.
    /// Initializes color with given color and speed with xy(-2, 0).
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

    /// \brief
    /// resets coordinate values
    /// \details
    /// This member function resets its member variables to the values it got when it was first initialized.
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
    
    /// \brief
    /// writes to the window
    /// \details
    /// This member function makes rectangles using its xy and color member variables and then immediately writes them to the window.
    void draw( hwlib::window & w ) override {
        hwlib::rectangle( start, anchor_end, ink ).draw( w );
        hwlib::rectangle( b1_start, b1_end, ink ).draw( w );
        hwlib::rectangle( b2_start, b2_end, ink ).draw( w );
        hwlib::rectangle( b3_start, b3_end, ink ).draw( w );
    }
    
    /// \brief
    /// removes tetromino's
    /// \details
    /// This member function makes rectangles using its xy member variables but it uses black as color when writing to the window.
    /// This causes the window to "forget" where the shape is and it removes it from the buffer.  
    void forget( hwlib::window & w ) {
        hwlib::rectangle( start, anchor_end, hwlib::color( BLACK ) ).draw( w );
        hwlib::rectangle( b1_start, b1_end,  hwlib::color( BLACK ) ).draw( w );        
        hwlib::rectangle( b2_start, b2_end,  hwlib::color( BLACK ) ).draw( w );
        hwlib::rectangle( b3_start, b3_end,  hwlib::color( BLACK ) ).draw( w );
    }
    
    /// \brief
    /// checks for filled rows
    /// \details
    /// This member function checks to see if there is a filled row in the buffer on the x line.
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
    
    /// \brief
    /// checks if tetromino can rotate clockwise
    /// \details
    /// This member function looks in the buffer to see if the tetromino is eligible for clockwise rotation.
    /// In order for a tetromino to be able to rotate all new positions of the tetromino must not be occupied by another drawable. 
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
    
    /// \brief
    /// checks if tetromino can rotate counter-clockwise
    /// \details
    /// This member function looks in the buffer to see if the tetromino is eligible for counter-clockwise rotation.
    /// In order for a tetromino to be able to rotate all new positions of the tetromino must not be occupied by another drawable.
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

    /// \brief
    /// rotates tetromino clockwise
    /// \details
    /// This member function rotates the tetromino clockwise using given window.
    /// Old position is forgotten from the window before new position is calculated.
    /// After calculating new positions, the tetromino is drawn into the window again.
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
    
    /// \brief
    /// rotates tetromino counter-clockwise
    /// \details
    /// This member function rotates the tetromino counter-clockwise using given window.
    /// Old position is forgotten from the window before new position is calculated.
    /// After calculating new positions, the tetromino is drawn into the window again.
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

    /// \brief
    /// updates the xy coordinates
    /// \details
    /// This member function updates its member xy variables by adding member variable speed to it.   
    /// First it forgets the tetromino from window, then it calculates new coordinates.
    /// After calculating it will redraw the tetromino into the given window.
    void update( hwlib::window & w ) {
        this->forget( w );
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
    }
    
    /// \brief
    /// moves tetromino down
    /// \details
    /// This member function moves the tetromino down by calling the update function after setting the speed to xy(-2, 0).
    void move_down( hwlib::window & w ){
        speed = hwlib::xy(-2, 0);
        this->update( w );
    }

    /// \brief
    /// moves tetromino to the left
    /// \details
    /// This member function moves the tetromino to the left by calling the update function after setting the speed to xy(0, -2).
    void move_left( hwlib::window & w ){
        speed = hwlib::xy(0, -2);
        this->update( w );
    }
    
    /// \brief
    /// moves tetromino to the right
    /// \details
    /// This member function moves the tetromino to the right by calling the update function after setting the speed to xy(0, 2).
    void move_right( hwlib::window & w ){
        speed = hwlib::xy(0, 2);
        this->update( w );
    }
    
    /// \brief
    /// checks if tetromino can move down
    /// \details
    /// This member function checks to see using given RGB-LED-matrix buffer if the tetromino is able to move down.
    /// It loops through an array of member variables and looks if the xy coordinate plus the speed does not equal any of its own
    /// xy member variables and if these new positions are not occupied. 
    bool can_move_down( matrix::P3_RGB_LED_matrix & b ) {
        bool updatable = true;
        std::array< hwlib::xy *, 4> coordinates = { &start, &b1_start, &b2_start, &b3_start };
        for( auto & i : coordinates ){
            if( (*i + speed != start) && (*i + speed != b1_start) && (*i + speed != b2_start) && (*i + speed != b3_start) ){
                updatable = updatable && (!b.is_occupied( *i + speed ) && !b.is_occupied( *i + hwlib::xy(1, 1) + speed ));
            }
        }
        return updatable;
    }
    
    /// \brief
    /// checks if tetromino can move to the left
    /// \details
    /// This member function changes the speed member variable and then stores the result for the can_move_down member function in bool updatable.
    /// After this speed will be put back to xy(-2, 0 ) and the stored result in updatable will be returned. 
    bool can_move_left( matrix::P3_RGB_LED_matrix & b ){
        speed = hwlib::xy(0, -2);
        bool updatable = this->can_move_down( b );
        speed = hwlib::xy(-2, 0);
        return updatable;
    }
    
    /// \brief
    /// checks if tetromino can move to the right
    /// \details
    /// This member function changes the speed member variable and then stores the result for the can_move_down member function in bool updatable.
    /// After this speed will be put back to xy(-2, 0 ) and the stored result in updatable will be returned. 
    bool can_move_right( matrix::P3_RGB_LED_matrix & b ){
        speed = hwlib::xy(0, 2);
        bool updatable = this->can_move_down( b );
        speed = hwlib::xy(-2, 0);
        return updatable;
    }
    
    /// \brief
    /// returns color value
    /// \details
    /// This member function returns the member variable color.
    uint8_t get_color(){
        return ink.small;
    }
};
}

#endif  //TETROMINO_HPP