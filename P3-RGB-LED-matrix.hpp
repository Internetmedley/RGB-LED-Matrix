#ifndef P3_RGB_LED_matrix_HPP	
#define P3_RGB_LED_matrix_HPP	

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

/// @file

#include <hwlib.hpp>	
#include <array>	

using namespace hwlib::target;	

namespace matrix{	

/// \brief
/// P3_RGB_LED_matrix class
/// \details
/// This is a class that uses the hwlib library to provide the user with an interface to work with matrices like this one:
/// https://www.adafruit.com/product/2279
/// This class provides a buffer with write functions and a toolset of other functions.
/// The user must give three hwlib pins and three hwlib port_outs to the constructor for initialization.
class P3_RGB_LED_matrix : public hwlib::window{	
    private:	
    pin_out & lat;	
    pin_out & oe;	
    pin_out & clk;	
    hwlib::port_out & rows;	
    hwlib::port_out & rgb1;	
    hwlib::port_out & rgb2;	
    std::array<std::array<uint8_t, MAT_WIDTH>, ROW_MAX> dataport;
    
    void write_implementation( hwlib::xy pos, hwlib::color col ) override {
        auto & tmp_d = dataport[pos.y % ROW_MAX][pos.x];	
        tmp_d = ((pos.y < ROW_MAX) ? ((tmp_d & 0x0F) | (col.small << 0x04)) : (tmp_d & 0xF0) | col.small);
    }

    public:	
    /// \brief
    /// constructor from explicit values
    /// \details
    /// This constructor initializes the pins and port_outs and initializes its superclass, hwlib,
    /// with pre-defined values found at the top of the page.
    /// These values can be changed to change the resolution of the window.
    /// Lastly it intializes the dataport buffer(array) with zeroes.
    P3_RGB_LED_matrix( pin_out & latch, pin_out & out_enable, pin_out & clock, 	
            hwlib::port_out & rows_port, hwlib::port_out & rgb1_port,	
            hwlib::port_out & rgb2_port, 
            uint8_t foreground = WHITE, uint8_t background = BLACK ):
        hwlib::window( hwlib::xy(MAT_WIDTH, MAT_HEIGHT), foreground, background ),
        lat( latch ), oe( out_enable ), clk( clock ),
        rows( rows_port ), rgb1( rgb1_port ), rgb2( rgb2_port ),
        dataport( {{ 0 }} )
    {}

    /// \brief
    /// checks if given coordinate is valid
    /// \details
    /// This boolean member function checks to see if the given hwlib::xy pos is within the window resolution.
    bool is_within_bounds( const hwlib::xy & pos ){	
        return ((pos.x >= 0 && pos.x <= MAT_WIDTH) && (pos.y >= 0 && pos.y <= MAT_HEIGHT));	
    }	

    /// \brief
    /// checks if given coordinate is taken
    /// \details
    /// This boolean member function checks the buffer to see if the given index has a value greater than zero.
    bool is_occupied( hwlib::xy pos ){	
        if( is_within_bounds(pos) ){	
            auto & tmp_d = dataport[pos.y % ROW_MAX][pos.x];	
            return ((pos.y < ROW_MAX) ? ((tmp_d & 0xF0) > 0) : ((tmp_d & 0x0F) > 0));	
        }	
        else{	
            return true;	
        }	
    }	

    void clear(){	
        dataport = {{ 0 }};	
    }

    void flush() override {	
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

 #endif //P3_RGB_LED_matrix_HPP