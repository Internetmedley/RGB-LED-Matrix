#include <hwlib.hpp>
#include "buffer.hpp"
#include "pixel.hpp"
#include "tetromino.hpp"
#include <array>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm> 

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
    
    matrix::EmptyRectangle rect1( hwlib::xy(2, 15), hwlib::xy(14, 20), CYAN );
    matrix::Rectangle rect2( hwlib::xy(13, 8), hwlib::xy(18, 17), PURPLE );
    matrix::Line line1( hwlib::xy(5, 2), hwlib::xy(60, 30), YELLOW );
    matrix::Circle circle1( hwlib::xy( 40, 16), 3, PURPLE );
    matrix::EmptyRectangle outline1( hwlib::xy(11, 5), hwlib::xy(52, 26), WHITE );    
    matrix::EmptyRectangle outline2( hwlib::xy(10, 4), hwlib::xy(53, 27), WHITE ); 
    /*tetris::I_shape I( hwlib::xy(12, 6) );
    tetris::O_shape O( hwlib::xy(32, 20) );
    tetris::T_shape T( hwlib::xy(20, 14) );
    tetris::Z_shape Z( hwlib::xy(14, 6) );
    tetris::S_shape S( hwlib::xy(20, 10) );
    tetris::L_shape L( hwlib::xy(40, 6) );
    tetris::J_shape J( hwlib::xy(48, 20) );*/
    
    tetris::I_shape I;
    tetris::O_shape O;
    tetris::T_shape T;
    tetris::Z_shape Z;
    tetris::S_shape S;
    tetris::L_shape L;
    tetris::J_shape J;
    
    hwlib::wait_ms(1000);
    
    std::array<tetris::Tetromino *, 7> objects = { &I, &O, &T, &Z, &S, &L, &J };
    
    outline1.draw( buf );
    outline2.draw( buf );
    
    hwlib::cout << buf.is_occupied( hwlib::xy(11, 6)) << '\n';
    hwlib::cout << buf.is_occupied( hwlib::xy(12, 6)) << '\n';
    hwlib::cout << tetris::I_shape( hwlib::xy(16, 6) ).is_updatable( buf ) << '\n';
    
    for( ;; ){
    std::random_shuffle(objects.begin(), objects.end());
        for( auto & i : objects ){
            i->draw( buf );
            while( i->is_updatable( buf ) ){
                for( auto j = 0; j < 10; j++ ){
                    buf.sketch();
                }
                i->forget( buf );
                i->update();
                i->draw( buf );
                for( auto j = 0; j < 10; j++ ){
                    buf.sketch();
                }
            }
           //i->forget( buf ); 
           i->reset();
        }
    }  
    
    return 0;
}