#ifndef BUFFER_HPP
#define BUFFER_HPP

#define MAT_WIDTH   64
#define MAT_HEIGHT  32
#define ROW_MAX     16

#define WHITE   0x07
#define YELLOW  0x06
#define PURPLE  0x05
#define RED     0x04
#define CYAN    0x03
#define GREEN   0x02
#define BLUE    0x01
#define BLACK   0x00

#include <hwlib.hpp>
#include <array>

using namespace hwlib::target;

namespace matrix{

    class Buffer{
        private:
            pin_out & lat;
            pin_out & oe;
            pin_out & clk;
            hwlib::port_out_from & rows;
            hwlib::port_out_from & rgb1;
            hwlib::port_out_from & rgb2;
            std::array< std::array< uint8_t, MAT_WIDTH >, ROW_MAX > dataport;
            
        public:
            Buffer( pin_out & latch, pin_out & out_enable, pin_out & clock ):
            lat( latch ),
            oe( out_enable ),
            clk( clock ),
            dataport( {{ 0 }} )
            {}
            
        
        void write( hwlib::xy pos, uint8_t color ){
            if( rhs.y < ROW_MAX ){
                dataport[rhs.y][rhs.x] = color;
            }
            else{
                dataport[rhs.y % ROW_MAX][rhs.x] = ( color << 0x04 );
            }
        }
            
        void sketch(){
            for( size_t i = 0; i < dataport.size(); i++ ){
            oe.write( 1 );
            oe.flush(); 
            rows.write( i );
            rows.flush();
       
            for( auto j = dataport[i].begin(); j != dataport[i].end(); j++  ){
                clk.write( 0 );
                clk.flush();
                
                rgb1.write( (buf.dataport[i][j] >> 0x04) );
                rgb1.flush();
                //hwlib::cout << "RGB1: " << ( *j >> 0x04) << '\n';
                rgb2.write( (buf.dataport[i][j] & 0x07) );
                rgb2.flush();
                //hwlib::cout << "RGB2: " << ( *j & 0x04) << '\n';
                //hwlib::cout << "waarde voor RGB1: " << ( buf.dataport[i][j] >> 0x04 ) << '\n';
                //hwlib::cout << "waarde voor RGB2: " << ( buf.dataport[i][j] & 0x07 ) << '\n';
                
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