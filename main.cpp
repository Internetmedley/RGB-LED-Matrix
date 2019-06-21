#include <hwlib.hpp>
#include "buffer.hpp"
#include <array>

// ===========================================================================

namespace target = hwlib::target;

int main(){
    auto a = target::pin_out( 0, 16 );          //A0
    auto b = target::pin_out( 0, 24 );
    auto c = target::pin_out( 0, 23 );
    auto d = target::pin_out( 0, 22 );
    
    auto r1 = target::pin_out( target::pins::d24 );
    auto g1 = target::pin_out( target::pins::d25 );
    auto b1 = target::pin_out( target::pins::d26 );
    auto r2 = target::pin_out( target::pins::d27 );
    auto g2 = target::pin_out( target::pins::d28 );
    auto b2 = target::pin_out( target::pins::d29 );
    
    auto oe  = target::pin_out( target::pins::d9  );
    auto lat = target::pin_out( target::pins::d10 );
    auto clk = target::pin_out( target::pins::d11 );
    
    
    
    struct uint4_t{
        unsigned b : 4;
    };
    
    struct uint3_t{
        unsigned b : 3;
    };
    
    //uint8_t duty_cycle = 0x00;
    
    Buffer buf( lat, oe, clk );
    
    buf += hwlib::xy( 30, 15 );
    
    //uint4_t row_data = { 0x00 };
    auto rows = hwlib::port_out_from( a, b, c, d );
    
    //uint3_t rgb1_data = { 0x05 };
    auto rgb1 = hwlib::port_out_from( r1, g1, b1 );
    //rgb1.write( rgb1_data.b );
    //rgb1.flush();
    
    //uint3_t rgb2_data = { 0x05 };
    auto rgb2 = hwlib::port_out_from( r2, g2, b2 );
    //rgb2.write( rgb2_data.b );
    //rgb2.flush();
    
    lat.write( 0 );
    lat.flush();
    oe.write( 1 );
    oe.flush();
    
    hwlib::wait_ms( 1000 );
    
    for( auto i = buf.dataport.end(); i != buf.dataport.begin(); i-- ){
        for( auto j = i->end(); j != i->begin(); j-- ){
            *j = 0;
            //hwlib::cout << *j << '\t';
        }
        //hwlib::cout << '\n';
    }
            
            
    

    //rgb1.write( rgb1_data.b );
    //rgb1.flush();
    //rgb2.write( rgb2_data.b );
    //rgb2.flush();

    //rows.write( row_data.b );
    //rows.flush();
    int index = 0;
    for( ;; ){
    for( auto i = buf.dataport.end(); i != buf.dataport.begin(); i-- ){
        oe.write( 0 );
        oe.flush();
        index++;
        for( auto j = i->end(); j != i->begin(); j-- ){
            clk.write( 0 );
            clk.flush();
            
            rows.write( index );
            rows.flush();
            rgb1.write( *j >> 0x04 );
            rgb1.flush();
            rgb2.write( *j & 0x07 );
            rgb2.flush();
            
            clk.write( 1 );
            clk.flush();
            lat.write( 1 );
            lat.flush();
            lat.write( 0 );
            lat.flush();
        }
    }

    }
    /*for (;;){
        for ( unsigned int i = 0; i <= 5; i++ ){
            oe.write( 0 );
            oe.flush();
            
            clk.write( 0 );
            clk.flush();
            
            rows.write( row_data.b );
            rows.flush();
            row_data.b++;
            
            rgb1.write( rgb1_data.b );
            rgb1.flush();
            rgb1_data.b++;
            
            rgb2.write( rgb2_data.b );
            rgb2.flush();
            rgb2_data.b++;
            
            clk.write( 1 );
            clk.flush();
            hwlib::wait_ms( 10 );
            
            lat.write( 1 );
            hwlib::wait_ms( 20 );
            lat.write( 0 );
            
            hwlib::wait_ms( 100 );
            oe.write( 1 );
            oe.flush();
        }
    }*/

    return  0;
}