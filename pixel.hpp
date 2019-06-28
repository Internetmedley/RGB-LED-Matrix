#ifndef PIXELS_HPP
#define PIXELS_HPP

#include "buffer.hpp"

namespace matrix{

class Drawable{
protected:
    hwlib::xy start;
    
public:
    Drawable( const hwlib::xy & start ): start( start ) {}
    
    virtual void draw( Buffer & b ) = 0;

};


class Line : public Drawable{
private:
    hwlib::xy end;
    uint8_t col;
    
    static void swap( int_fast16_t & a, int_fast16_t & b ){
        auto t = a;
        a = b;
        b = t;
    }
    
    static int_fast16_t abs( int_fast16_t x ){
      return x >= 0 ? x : -x;
   }
    
public:
    Line( hwlib::xy start, hwlib::xy end, uint8_t color = WHITE ):
    Drawable( start ),
    end( end ),
    col( color )
    {}
    
    void draw( Buffer & b ) override { 
       
      int_fast16_t x0 = start.x;
      int_fast16_t y0 = start.y;
      int_fast16_t x1 = end.x; 
      int_fast16_t y1 = end.y;
                   
      // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
      // http://homepages.enterprise.net/murphy/thickline/index.html
     
      int_fast16_t Dx = x1 - x0; 
      int_fast16_t Dy = y1 - y0;
   
      int_fast16_t steep = (abs(Dy) >= abs(Dx));
   
      if( steep ){
         swap( x0, y0 );
         swap( x1, y1 );
      
         // recompute Dx, Dy after swap
         Dx = x1 - x0;
         Dy = y1 - y0;
      }
   
      int_fast16_t xstep = 1;
      if( Dx < 0 ){
         xstep = -1;
         Dx = -Dx;
      }
   
      int_fast16_t ystep = 1;
      if( Dy < 0 ){
         ystep = -1;    
         Dy = -Dy; 
      }
      int_fast16_t TwoDy = 2*Dy; 
      int_fast16_t TwoDyTwoDx = TwoDy - 2*Dx; // 2*Dy - 2*Dx
      int_fast16_t E = TwoDy - Dx; //2*Dy - Dx
      int_fast16_t y = y0;
      int_fast16_t xDraw, yDraw, x;  
      for( x = x0; x != x1; x += xstep ){    
         if (steep) {     
            xDraw = y;
            yDraw = x;
         } else {     
            xDraw = x;
            yDraw = y;
         }
         
        b.write( hwlib::xy( xDraw, yDraw ), col );

         if( E > 0 ){
            E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
            y = y + ystep;
         } else {
            E += TwoDy; //E += 2*Dy;
         }
      }
    }
};

class Rectangle : public Drawable{
private:
    hwlib::xy end;
    uint8_t col;
    
public:
    Rectangle( hwlib::xy start, hwlib::xy end, uint8_t color = WHITE ):
    Drawable( start ),
    end( end ),
    col( color )
    {}
    
    void draw( Buffer & b ) override { 
        for( auto y = start.y; y <= end.y; y++ ){
            for( auto x = start.x; x <= end.x; x++ ){
                b.write( hwlib::xy( x, y ), col );
            }
        }
        
        //for( auto y = start.y; y <= end.y; y++ ){
        //    Line( hwlib::xy(start.x, y), hwlib::xy(end.x, y) ).draw( b );
        //}
    }
};

class EmptyRectangle : public Drawable{
private:
    hwlib::xy end;
    uint8_t col;
    Line top;
    Line bottom;
    Line left;
    Line right;
    
public:
    EmptyRectangle( hwlib::xy start, hwlib::xy end, uint8_t color = WHITE ):
    Drawable( start ),
    end( end ),
    col( color ),
    top( start, hwlib::xy(end.x, start.y) ),
    bottom( hwlib::xy(start.x, end.y), end ),
    left( start,  hwlib::xy(start.x, end.y) ),
    right( hwlib::xy(end.x, start.y), end ) 
    {}
    
    void draw( Buffer & b ) override { 
        top.draw( b );
        bottom.draw( b );
        left.draw( b );
        right.draw( b );
    }
};

}

#endif //PIXELS_HPP 