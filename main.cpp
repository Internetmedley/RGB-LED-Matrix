#include <hwlib.hpp>
#include "P3-RGB-LED-matrix.hpp"
#include "rectangles.hpp"
#include "tetromino.hpp"
#include "tetris-shapes.hpp"
#include <array>

#include <algorithm>    //for std::random_shuffle

// ===========================================================================

namespace target = hwlib::target;

int main(){
    auto a = target::pin_out( 0, 16 );  //A0
    auto b = target::pin_out( 0, 24 );  //A1
    auto c = target::pin_out( 0, 23 );  //A2
    auto d = target::pin_out( 0, 22 );  //A3
    
    auto b_mv_left = target::pin_in( target::pins::d44 );               //blue button
    auto b_mv_right = target::pin_in( target::pins::d47 );              //green button
    auto b_rotate_clkwise = target::pin_in( target::pins::d46 );        //red button
    auto b_rotate_ctr_clkwise = target::pin_in( target::pins::d45 );    //white button
    
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

    hwlib::wait_ms(1000);

    matrix::P3_RGB_LED_matrix buf( lat, oe, clk, rows, rgb1, rgb2 );
    
    hwlib::empty_rectangle outline1( hwlib::xy(11, 5), hwlib::xy(52, 26), WHITE );
    tetris::I_shape I;
    tetris::O_shape O;
    tetris::T_shape T;
    tetris::Z_shape Z;
    tetris::S_shape S;
    tetris::L_shape L;
    tetris::J_shape J;
    
    std::array<tetris::Tetromino *, 7> objects = { &I, &O, &T, &Z, &S, &L, &J };
    outline1.draw( buf );
    for( ;; ){
        std::random_shuffle(objects.begin(), objects.end());

        for( auto & i : objects ){
            i->draw( buf );
            while( i->can_move_down( buf ) ){
                buf.flush();
                buf.flush();
                i->move_down( buf );
                if( b_mv_left.read() == true && i->can_move_left( buf ) ){
                    i->move_left( buf );
                }
                if( b_mv_right.read() == true && i->can_move_right( buf ) ){
                    i->move_right( buf );
                }
                buf.flush();
                buf.flush();
                buf.flush();
                buf.flush();
                if( b_rotate_clkwise.read() == true && i->can_rotate_clkwise( buf ) ){
                    i->rotate_clkwise( buf );
                }
                
                if( b_rotate_ctr_clkwise.read() == true && i->can_rotate_ctr_clkwise( buf ) ){
                    i->rotate_ctr_clkwise( buf );
                }
                buf.flush();
                buf.flush();
            }
           i->reset();
        }
    } 
    return 0;
}