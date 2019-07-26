#ifndef RECTANGLES_HPP
#define RECTANGLES_HPP

/// @file

#include <hwlib.hpp>

namespace hwlib{

/// \brief
/// hwlib rectangle class
/// \details
/// This class is a subclass of hwlib::drawable that can draw a filled rectangle to a hwlib::window.
class rectangle : public drawable{
private:
    xy      end;
    color   ink;
    bool    use_foreground;
    
public:
    /// \brief
    /// constructor from explicit values
    /// \details
    /// This constructor initializes its drawable superclass , xy coordinate end and the color ink.
    rectangle( xy start, xy end, color col = color( WHITE ) ):
        drawable( start ),
        end( end ),
        ink( col )
    {}
    
    /// \brief
    /// draws rectangle
    /// \details
    /// This member function writes all the necessary pixels to the window to represent a rectangle.
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

/// \brief
/// hwlib empty_rectangle class
/// \details
/// This class is a subclass of hwlib::drawable that can draw an empty rectangle to a hwlib::window.
/// It contains 4 lines from hwlib for the lines on the outside. 
class empty_rectangle : public drawable{
private:
    xy      end;
    color   ink;
    line    top;
    line    bottom;
    line    left;
    line    right;
    
public:
    /// \brief
    /// constructor from explicit values
    /// \details
    /// This constructor initializes its drawable superclass , xy coordinate end, the 4 hwlib lines and the color ink.
    empty_rectangle( xy start, xy end, color col = color( WHITE ) ):
        drawable( start ),
        end( end ),
        ink( col ),
        top( start, xy(end.x, start.y), col ),
        bottom( xy(start.x, end.y), end, col ),
        left( start, xy(start.x, end.y), col ),
        right( xy(end.x, start.y), xy(end.x, end.y+1), col ) 
    {}
    
    /// \brief
    /// draws rectangle
    /// \details
    /// This member function writes all the necessary pixels to the window to represent a rectangle.
    void draw( window & w ) override { 
        top.draw( w );
        bottom.draw( w );
        left.draw( w );
        right.draw( w );
    }
};
}

#endif //RECTANGLES_HPP