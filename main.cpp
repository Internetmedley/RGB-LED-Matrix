#include <hwlib.hpp>
#include <array>
#include "wall.hpp"

// ===========================================================================

class drawable {
protected:

   hwlib::window & w;
   hwlib::xy location;
   hwlib::xy size;
   
public:

   drawable( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & size ):
      w( w ),
      location( location ),
      size( size )
   {}      
   
    hwlib::xy bounce{-1, -1};
   virtual void draw() = 0;
   virtual void update(){}
   bool overlaps( const drawable & other );   
   virtual void interact( drawable & other ){}
   
   hwlib::ostream & print( hwlib::ostream & out ) const {
      return out << location << " " << ( location + size );
   }      
};

hwlib::ostream & operator<<( hwlib::ostream & lhs, const drawable & rhs ){
   return rhs.print( lhs );
}

bool within( int x, int a, int b ){
   return ( x >= a ) && ( x <= b );
}

bool drawable::overlaps( const drawable & other ){
   
   bool x_overlap = within( 
      location.x, 
      other.location.x, 
      other.location.x + other.size.x
   ) || within( 
      other.location.x, 
      location.x, 
      location.x + size.x
   );
     
   bool y_overlap = within( 
      location.y, 
      other.location.y, 
      other.location.y + other.size.y
   ) || within( 
      other.location.y, 
      location.y, 
      location.y + size.y
   );
   
   return x_overlap && y_overlap;
}

// ===========================================================================

class line : public drawable {
private:

   hwlib::xy end;
   
public:

   line( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & end ):
      drawable( w, location, end - location ),
      end( end )
   {}
   
   void draw() override {
      hwlib::line x( location, end );
      x.draw( w );;
   }
};

// ===========================================================================

class circle : public drawable {
protected:

   int radius;
 
public:

   circle( hwlib::window & w, const hwlib::xy & midpoint, int radius ):
      drawable( w, 
         midpoint - hwlib::xy( radius, radius ), 
         hwlib::xy( radius, radius ) * 2 ),
      radius( radius )
   {}
   
   void draw() override {
      hwlib::circle c( location + hwlib::xy( radius, radius ), radius );
      c.draw( w );
   }
};

// ===========================================================================

class ball : public circle {
private:

   hwlib::xy speed;
   
public:

   ball( 
      hwlib::window & w, 
      const hwlib::xy & midpoint, 
      int radius, 
      const hwlib::xy & speed 
   ):
      circle( w, midpoint, radius ),
      speed( speed )  
   {}
   
   void update() override {
      location = location + speed; 
   }
   
   void interact( drawable & other ) override {
      if( this != & other){
         if( overlaps( other )){
            speed.x *= other.bounce.x;
            speed.y *= other.bounce.y;
            
            if(other.bounce.x < 0){
                location.x = location.x + 2* speed.x;
            }

            if(other.bounce.y < 0){
                location.y = location.y + 2* speed.y;
            }
            

         }
      }
   }   
};

// ===========================================================================

int main(){
   hwlib::target::window w( hwlib::xy( 128, 64 ), 2 );
   line top( w, hwlib::xy(   0,  5 ), hwlib::xy( 127,  5 ) );
   line right( w, hwlib::xy( 122,  0 ), hwlib::xy( 122, 63 ) );
   line bottom( w, hwlib::xy(   0, 58 ), hwlib::xy( 127, 58 ) );
   line left( w, hwlib::xy(   5,  0 ), hwlib::xy(   5, 63 )  );
   top.bounce.x = 1;
   bottom.bounce.x = 1;
   right.bounce.y = 1;
   left.bounce.y = 1;
   ball b( w, hwlib::xy( 50, 20 ), 9, hwlib::xy( 5, 1 ) );
   
   std::array< drawable *, 5 > objects = { &b, &top, &left, &right, &bottom };
    int update_interval = 0;
   for(;;){
       
      w.clear();
      for( auto & p : objects ){
          
          p->update();
      }
      for( auto & p : objects ){
         for( auto & other : objects ){
            p->interact( *other );
         } 
      }
    for( auto & p : objects ){
         p->draw();
      }
    if( update_interval == 1 ){
        wall( w, hwlib::xy(   0,  0 ), hwlib::xy( 127,  5 ), update_interval ).draw();
        wall( w, hwlib::xy( 122,  0 ), hwlib::xy( 127, 63 ), update_interval ).draw();
        wall( w, hwlib::xy(   0, 58 ), hwlib::xy( 127, 63 ), update_interval ).draw();
        wall( w, hwlib::xy(   0,  0 ), hwlib::xy(   5, 63 ), update_interval ).draw();
        update_interval = 0;
    }
    else{
        update_interval = 1;
    }
    w.flush();
    hwlib::wait_ms( 200 );

   }
}

