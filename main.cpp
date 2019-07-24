#include <hwlib.hpp>
#include "P3-RGB-LED-matrix.hpp"
//#include "pixel.hpp"
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
    
    auto b_mv_left = target::pin_in( target::pins::d44 );           //blue button
    auto b_mv_right = target::pin_in( target::pins::d47 );          //green button
    auto b_rotate_clkwise = target::pin_in( target::pins::d46 );       //red button
    auto b_rotate_ctr_clkwise = target::pin_in( target::pins::d45 );   //white button
    
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
    
    matrix::P3_RGB_LED_matrix buf( lat, oe, clk, rows, rgb1, rgb2 );
    
    hwlib::empty_rectangle rect1( hwlib::xy(2, 15), hwlib::xy(14, 20), CYAN );
    hwlib::rectangle rect2( hwlib::xy(13, 8), hwlib::xy(18, 17), PURPLE );
    hwlib::line line1( hwlib::xy(5, 2), hwlib::xy(60, 30), YELLOW );
    hwlib::circle circle1( hwlib::xy( 40, 16), 3, PURPLE );
    hwlib::empty_rectangle outline1( hwlib::xy(11, 5), hwlib::xy(52, 26), WHITE );
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
    /*rect1.draw( buf );
    rect2.draw( buf );
    line1.draw( buf );
    circle1.draw( buf );
    
    for( ;; ){
        buf.flush();
    }*/
    
    /*for( ;; ){
        hwlib::cout << "blue: " << b_mv_left.read() << '\t';
        hwlib::cout << "white: " << b_anti_clkwise.read() << '\t';
        hwlib::cout << "red: " << b_clkwise.read() << '\t';
        hwlib::cout << "green: " << b_mv_right.read() << '\n' << '\n';
    }*/
    
    /*for(;;){
        std::random_shuffle(objects.begin(), objects.end());
        for( auto & i : objects ){
            i->draw( buf );
            for( auto j = 0; j < 25; j++ ){
                    buf.flush();
                }
            i->forget( buf );
        }
    }*/
    
    for( ;; ){
        std::random_shuffle(objects.begin(), objects.end());
        //hwlib::cout << "before iteration" << '\n';
        for( auto & i : objects ){
            //hwlib::cout << "can move down?: " << i->can_move_down( buf ) << '\n';
            i->draw( buf );
            //hwlib::cout << "success " << '\n';
            while( i->can_move_down( buf ) ){
                for( auto j = 0; j < 5; j++ ){
                    buf.flush();
                }
                
                i->move_down( buf );
                //hwlib::cout << "in loop" << '\n';
                if( b_mv_left.read() == true && i->can_move_left( buf ) ){
                    i->move_left( buf );
                }
                if( b_mv_right.read() == true && i->can_move_right( buf ) ){
                    i->move_right( buf );
                }
                if( b_rotate_clkwise.read() == true ){
                    i->rotate_clkwise( buf );
                }

                
                for( auto j = 0; j < 5; j++ ){
                    buf.flush();
                }
            }
            //hwlib::cout << "can still move down?: " << i->can_move_down( buf ) << '\n';
           i->reset();
        }
    } 
    
    return 0;
}