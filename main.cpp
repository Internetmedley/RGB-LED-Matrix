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
    
    auto rows = hwlib::port_out_from( a, b, c, d );
    auto rgb1 = hwlib::port_out_from( b1, g1, r1 );
    auto rgb2 = hwlib::port_out_from( b2, g2, r2 );

    
    //uint8_t duty_cycle = 0x00;
    
    matrix::Buffer buf( lat, oe, clk, rows, rgb1, rgb2 );
    
    //buf.write_rand_col( hwlib::xy(15, 16) );
    matrix::EmptyRectangle rect1( hwlib::xy(2, 15), hwlib::xy(14, 20), CYAN );
    matrix::Rectangle rect2( hwlib::xy(13, 8), hwlib::xy(18, 17), PURPLE );
    matrix::Line line1( hwlib::xy(5, 2), hwlib::xy(60, 30), YELLOW );
    matrix::Circle circle1( hwlib::xy( 40, 16), 3, PURPLE );
    
    rect1.draw( buf );
    rect2.draw( buf );
    line1.draw( buf );
    circle1.draw( buf );
    
    /*buf.write( hwlib::xy(0, 0), PURPLE );
    buf.write( hwlib::xy(1, 0), PURPLE );
    buf.write( hwlib::xy(2, 0), RED );
    buf.write( hwlib::xy(3, 0), PURPLE );
    buf.write( hwlib::xy(4, 0), PURPLE );
    buf.write( hwlib::xy(5, 0), GREEN );
    buf.write( hwlib::xy(7, 0), PURPLE );
    buf.write( hwlib::xy(6, 0), PURPLE );
    buf.write( hwlib::xy(0, 1), PURPLE );
    buf.write( hwlib::xy(1, 1), PURPLE );
    buf.write( hwlib::xy(2, 1), BLUE );
    buf.write( hwlib::xy(3, 1), PURPLE );
    buf.write( hwlib::xy(4, 1), PURPLE );
    buf.write( hwlib::xy(5, 1), PURPLE );
    buf.write( hwlib::xy(6, 1), WHITE );
    buf.write( hwlib::xy(7, 1), PURPLE );*/
    
    
    //buf.dataport[15][0] = 0x77;
    //buf.dataport[14][0] = 0x77;
    //buf.dataport[13][0] = 0x77;
    //buf.dataport[12][0] = 0x77;
    //buf.dataport[11][0] = 0x77;
    //buf.dataport[0][0] = 0x77
    
    
    for( ;; ){
        buf.sketch();
    }
    
    return 0;
}