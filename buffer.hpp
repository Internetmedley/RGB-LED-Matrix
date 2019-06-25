#define MAT_WIDTH   64
#define MAT_HEIGHT  32
#define ROW_MAX     16

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <hwlib.hpp>

using namespace hwlib::target;

class Buffer{
public:
    pin_out & lat; 
    pin_out & oe;
    pin_out & clk;
    std::array<std::array< uint8_t, MAT_WIDTH>, ROW_MAX> dataport;       // 64 and 16
    

    Buffer( pin_out & latch, pin_out & output_enable, pin_out & clock):
    lat( latch ),
    oe( output_enable ),
    clk( clock ),
    dataport( {{}} )
    { for( auto i = dataport.begin(); i != dataport.end(); i++ ){
        for( auto j = i->begin(); j != i->end(); j++ ){
            *j = 0x00;
        }
    }}
    
    void add_empty_rect();
    void add_filled_rect();
    
    Buffer & operator+=( const hwlib::xy rhs ){
        if( rhs.y < ROW_MAX ){
            dataport[rhs.y][rhs.x] = 0x70;                  //RGB1
        }
        else{
            dataport[(rhs.y % ROW_MAX)][rhs.x] = 0x07;        //RGB2
        }
        return *this;
    }

};

class Pixel{
protected:
    


public:
    
    

};

#endif //BUFFER_HPP