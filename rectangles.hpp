#ifndef RECTANGLES_HPP
#define RECTANGLES_HPP

#include <hwlib.hpp>

namespace hwlib{

class rectangle : public drawable{
private:
    xy      end;
    color   ink;
    bool    use_foreground;
    
public:
    rectangle( xy start, xy end, color col = color( WHITE ) ):
        drawable( start ),
        end( end ),
        ink( col )
    {}
    
    void draw( window & w ) override { 
        /*for( auto y = start.y; y <= end.y; y++ ){
            for( auto x = start.x; x <= end.x; x++ ){
                w.write( xy( x, y ), ink );
            }
        }*/
        
        for( auto y = start.y; y <= end.y; y++ ){
            line( xy(start.x, y), xy(end.x+1, y), ink ).draw( w );
        }
    }
};

class empty_rectangle : public drawable{
private:
    xy      end;
    color   ink;
    line    top;
    line    bottom;
    line    left;
    line    right;
    
public:
    empty_rectangle( xy start, xy end, color col = color( WHITE ) ):
        drawable( start ),
        end( end ),
        ink( col ),
        top( start, xy(end.x, start.y), col ),
        bottom( xy(start.x, end.y), end, col ),
        left( start, xy(start.x, end.y), col ),
        right( xy(end.x, start.y), xy(end.x, end.y+1), col ) 
    {}
    
    void draw( window & w ) override { 
        top.draw( w );
        bottom.draw( w );
        left.draw( w );
        right.draw( w );
    }
};
}


#endif //RECTANGLES_HPP