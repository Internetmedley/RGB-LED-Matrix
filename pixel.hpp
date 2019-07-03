#ifndef PIXELS_HPP
#define PIXELS_HPP

namespace matrix{

class Drawable{
protected:
    hwlib::xy start;
    uint8_t col;
    
public:
    Drawable( const hwlib::xy & start, uint8_t color ): start( start ), col( color ) {}
    
    virtual void draw( Buffer & b ) = 0;
    virtual void update(){};
};


class Line : public Drawable{
private:
    hwlib::xy end;
    //uint8_t col;
    
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
        Drawable( start, color ),
        end( end )
        //col( color )
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
    //uint8_t col;
    
public:
    Rectangle( hwlib::xy start, hwlib::xy end, uint8_t color = WHITE ):
        Drawable( start, color ),
        end( end )
        //col( color )
    {}
    
    void draw( Buffer & b ) override { 
        for( auto y = start.y; y <= end.y; y++ ){
            for( auto x = start.x; x <= end.x; x++ ){
                b.write( hwlib::xy( x, y ), col );
            }
        }
        
        //for( auto y = start.y; y <= end.y; y++ ){
        //    Line( hwlib::xy(start.x, y), hwlib::xy(end.x+1, y), col ).draw( b );
        //}
    }
};

class EmptyRectangle : public Drawable{
private:
    hwlib::xy end;
    //uint8_t col;
    Line top;
    Line bottom;
    Line left;
    Line right;
    
public:
    EmptyRectangle( hwlib::xy start, hwlib::xy end, uint8_t color = WHITE ):
        Drawable( start, color ),
        end( end ),
        //col( color ),
        top( start, hwlib::xy(end.x, start.y), color ),
        bottom( hwlib::xy(start.x, end.y), end, color ),
        left( start, hwlib::xy(start.x, end.y), color ),
        right( hwlib::xy(end.x, start.y), hwlib::xy(end.x, end.y+1), color ) 
    {}
    
    void draw( Buffer & b ) override { 
        top.draw( b );
        bottom.draw( b );
        left.draw( b );
        right.draw( b );
    }
};

// ==========================================================================
//
// circle
//
// ==========================================================================

/// a circle object                   
class Circle : public Drawable {
private:   
   uint_fast16_t  radius;
   //uint8_t        col;
   
public:
   /// create a circle object of a specific color
   Circle( 
      hwlib::xy start, 
      uint_fast16_t radius, 
      uint8_t color = WHITE
   )
      : Drawable{ start, color }, radius{ radius }
   {}     
   
   void draw( Buffer & b ) override { 

      // don't draw anything when the size would be 0 
      if( radius < 1 ){
         return;       
      }
   
      // http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
      
      //color col = use_foreground ? w.foreground : ink;      
   
      int_fast16_t fx = 1 - radius;
      int_fast16_t ddFx = 1;
      int_fast16_t ddFy = -2 * radius;
      int_fast16_t x = 0;
      int_fast16_t y = radius;
    
      // top and bottom
      b.write( start + hwlib::xy( 0, + radius ), col );
      b.write( start + hwlib::xy( 0, - radius ), col );

      // left and right 
      b.write( start + hwlib::xy( + radius, 0 ), col );
      b.write( start + hwlib::xy( - radius, 0 ), col );
         
      // filled circle
      if(0) {
   
         // top and bottom
         b.write( start + hwlib::xy( 0, + radius ), col );
         b.write( start + hwlib::xy( 0, - radius ), col );

         // left and right
         Line(  
              start - hwlib::xy( radius, 0 ), 
              start + hwlib::xy( radius, 0 ), 
              col 
          ).draw( b );
      } 
    
      while( x < y ){
      
         // calculate next outer circle point
         if( fx >= 0 ){
           y--;
           ddFy += 2;
           fx += ddFy;
         }
         x++;
         ddFx += 2;
         fx += ddFx;   
                    
         b.write( start + hwlib::xy( + x, + y ), col );
         b.write( start + hwlib::xy( - x, + y ), col );
         b.write( start + hwlib::xy( + x, - y ), col );
         b.write( start + hwlib::xy( - x, - y ), col );
         b.write( start + hwlib::xy( + y, + x ), col );
         b.write( start + hwlib::xy( - y, + x ), col );
         b.write( start + hwlib::xy( + y, - x ), col );
         b.write( start + hwlib::xy( - y, - x ), col );
            
         // filled circle
         if(0) {
            Line( 
               start + hwlib::xy( -x,  y ), 
               start + hwlib::xy(  x,  y ), 
               col ).draw( b );
            Line( 
               start + hwlib::xy( -x, -y ), 
               start + hwlib::xy(  x, -y ), 
               col ).draw( b );
            Line( 
               start + hwlib::xy( -y,  x ), 
               start + hwlib::xy(  y,  x ), 
               col ).draw( b );
            Line( 
               start + hwlib::xy( -y, -x ), 
               start + hwlib::xy(  y, -x ), 
               col ).draw( b );
         }
      }
   }   
    
}; // class circle

}  //namespace matrix

#endif //PIXELS_HPP 