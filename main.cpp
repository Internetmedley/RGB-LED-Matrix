#include <hwlib.hpp>
#include "buffer.hpp"
#include "pixel.hpp"
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
    
    auto rows = hwlib::port_out_from( d, c, b, a );
    auto rgb1 = hwlib::port_out_from( b1, g1, r1 );
    auto rgb2 = hwlib::port_out_from( b2, g2, r2 );

    
    //uint8_t duty_cycle = 0x00;
    
    matrix::Buffer buf( lat, oe, clk, rows, rgb1, rgb2 );
    
    //buf.write_rand_col( hwlib::xy(15, 16) );
    matrix::EmptyRectangle rect1( hwlib::xy(15, 15), hwlib::xy(20, 20), RED );
    matrix::Rectangle rect2( hwlib::xy(13, 8), hwlib::xy(18, 17), PURPLE );
    matrix::Line line1( hwlib::xy(5, 2), hwlib::xy(12, 30), GREEN );
    
    //rect1.draw( buf );
    rect2.draw( buf );
    //line1.draw( buf );
    //buf.write( hwlib::xy(15, 15), WHITE );
    
    for( ;; ){
        buf.sketch();
    }
    
    return 0;
}