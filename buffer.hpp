#ifndef BUFFER_HPP
#define BUFFER_HPP

#define MAT_WIDTH   64
#define MAT_HEIGHT  32
#define ROW_MAX     16

#define WHITE       0x07
#define YELLOW      0x06
#define PURPLE      0x05
#define RED         0x04
#define CYAN        0x03
#define GREEN       0x02
#define BLUE        0x01
#define BLACK       0x00

#include <hwlib.hpp>
#include <array>
//#include <cstdlib>
//#include <ctime>


using namespace hwlib::target;


namespace matrix{
    
class Buffer{
    private:
        pin_out & lat;
        pin_out & oe;
        pin_out & clk;
        hwlib::port_out & rows;
        hwlib::port_out & rgb1;
        hwlib::port_out & rgb2;
        std::array<std::array<uint8_t, MAT_WIDTH>, ROW_MAX> dataport;
        
    public:
        Buffer( pin_out & latch, pin_out & out_enable, pin_out & clock, 
                hwlib::port_out & rows_port, hwlib::port_out & rgb1_port,
                hwlib::port_out & rgb2_port ):
        lat( latch ), oe( out_enable ), clk( clock ),
        rows( rows_port ), rgb1( rgb1_port ), rgb2( rgb2_port ),
        dataport( {{ 0 }} )
        {}
        
    
    void write( hwlib::xy pos, uint8_t color = WHITE ){
        uint8_t & tmp_d = dataport[pos.y % ROW_MAX][pos.x % MAT_WIDTH];
        if( pos.y < ROW_MAX ){ 
            tmp_d = ((tmp_d & 0x0F) | (color << 0x04)); 
        }
        else { 
            tmp_d = ((tmp_d & 0xF0) | color); 
        }
    }
    
    /*void write_rand_col( hwlib::xy pos ){
        uint8_t & tmp_d = dataport[pos.y % ROW_MAX][pos.x % MAT_WIDTH];
        srand(static_cast<unsigned int>(time(0)));
        uint8_t color = ( (rand()+1) & 0x77 );
        if( pos.y < ROW_MAX ) { tmp_d = (tmp_d & 0x0F) | (color << 0x04); }
        else { tmp_d = (tmp_d & 0xF0) | color; }
    }*/
        
    void sketch(){
        uint8_t tmp_i = 0;
        for( auto i = dataport.begin(); i !=dataport.end(); i++ ){
        oe.write( 1 );
        oe.flush(); 
        rows.write( tmp_i );
        rows.flush();
        tmp_i++;
   
        for( auto j = i->begin(); j != i->end(); j++  ){
            clk.write( 0 );
            clk.flush();
            
            rgb1.write( (*j >> 0x04) );
            rgb1.flush();
            rgb2.write( (*j & 0x07) );
            rgb2.flush();
            
            clk.write( 1 );
            clk.flush();
            }
            
        lat.write( 1 );
        lat.flush();
        oe.write( 0 );
        oe.flush();
        lat.write( 0 );
        lat.flush();
        hwlib::wait_us( 256 );
        }
    }
};
}

#endif //BUFFER_HPP