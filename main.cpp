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
    
    buf += hwlib::xy( 0, 0 );
    
    auto rows = hwlib::port_out_from( a, b, c, d );
    auto rgb1 = hwlib::port_out_from( b1, g1, r1 );
    auto rgb2 = hwlib::port_out_from( b2, g2, r2 );

    hwlib::wait_ms( 1000 );

    
    
    //uint8_t index = 16;
    /*for( ;; ){
        
        for( auto i = buf.dataport.rbegin(); i != buf.dataport.rend(); i++ ){
            
            oe.write( 0 );
            oe.flush();
            
            rows.write( index );
            rows.flush();
            
            for( auto j = i->rbegin(); j != i->rend(); j++ ){
                 
                lat.write( 0 );
                lat.flush();
                clk.write( 0 );
                clk.flush();
                
                rgb1.write( (*j >> 0x04) );
                rgb1.flush();
                //hwlib::cout << "RGB1: " << (*j >> 0x04) << '\n';
                rgb2.write( (*j & 0x07) );
                rgb2.flush();
                //hwlib::cout << "RGB2: " << (*j & 0x07) << '\n';
                
                clk.write( 1 );
                clk.flush();
                lat.write( 1 );
                lat.flush();
                hwlib::wait_ms( 100 );
                
            }
            oe.write( 1 );
                oe.flush();
            
            index--;
        }
        index = 0;
    }*/
    for( int i = 0; i < 16; i++ ){
        lat.write( 0 );
        lat.flush();
        oe.write( 0 );
        oe.flush();
        for( int j = 0; j < 64; j++ ){
            clk.write( 1 );
            clk.flush();
            clk.write( 0 );
            clk.flush();
        }
        lat.write( 1 );
        lat.flush();
        oe.write( 1 );
        oe.flush();
    }
    
    for( ;; ){
        for( size_t i = 0; i < buf.dataport.size(); i++ ){
            lat.write( 0 );
            lat.flush();
            oe.write( 0 );
            oe.flush();
            
            rows.write( i );
            rows.flush();
            
                for( size_t j = 0; j < buf.dataport[i].size(); j++ ){
                    
                    
                    
                    rgb1.write( int(buf.dataport[i][j] >> 0x04) );
                    rgb1.flush();
                    //hwlib::cout << "RGB1: " << (buf.dataport[i][j] >> 0x04) << '\n';
                    rgb2.write( int(buf.dataport[i][j] & 0x07) );
                    rgb2.flush();
                    //hwlib::cout << "RGB2: " << (buf.dataport[i][j] & 0x04) << '\n';
                    
                    clk.write( 1 );
                    clk.flush();
                    clk.write( 0 );
                    clk.flush();
                    
                   
                }
                lat.write( 1 );
                lat.flush();
                oe.write( 1 );
                oe.flush();
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