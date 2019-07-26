#ifndef TETRIS_SHAPES_HPP
#define TETRIS_SHAPES_HPP

/// @file

#include "tetromino.hpp"


namespace tetris{
    
/// \brief
/// I_shape class
/// \details
/// This class inherits from Tetromino and creates the so-called I-shape as can be found here:
/// https://en.wikipedia.org/wiki/Tetromino
class I_shape : public Tetromino{
    public:
    /// \brief
    /// constructor
    /// \details
    /// This constructor initializes its superclass with set values using a starting position.
    /// The starting position should be the left-most pixel of your shape.
    I_shape( hwlib::xy s = hwlib::xy(50, 12), uint8_t color = CYAN ): 
        Tetromino( hwlib::xy(s.x, s.y+4),
                   hwlib::xy(s),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x, s.y+6),
                   color ) 
    {}
};

/// \brief
/// O_shape class
/// \details
/// This class inherits from Tetromino and creates the so-called O-shape as can be found here:
/// https://en.wikipedia.org/wiki/Tetromino
class O_shape : public Tetromino{
public:
    /// \brief
    /// constructor
    /// \details
    /// This constructor initializes its superclass with set values using a starting position.
    /// The starting position should be the left-most pixel of your shape.
    O_shape( hwlib::xy s = hwlib::xy(48, 14), uint8_t color = YELLOW ): 
        Tetromino( hwlib::xy(s),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x+2, s.y),
                   hwlib::xy(s.x+2, s.y+2),
                   color ) 
    {}
    
    bool can_rotate_clkwise( matrix::P3_RGB_LED_matrix & b ) override { return false; }         //there is no use to rotating this piece
    bool can_rotate_ctr_clkwise( matrix::P3_RGB_LED_matrix & b ) override { return false; }
    void rotate_clkwise( hwlib::window & w ) override {}
    void rotate_ctr_clkwise( hwlib::window & w ) override {}
};

/// \brief
/// T_shape class
/// \details
/// This class inherits from Tetromino and creates the so-called T-shape as can be found here:
/// https://en.wikipedia.org/wiki/Tetromino
class T_shape : public Tetromino{
public:
    /// \brief
    /// constructor
    /// \details
    /// This constructor initializes its superclass with set values using a starting position.
    /// The starting position should be the left-most pixel of your shape.
    T_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = PURPLE ): 
        Tetromino( hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x+2, s.y+2),
                   hwlib::xy(s.x, s.y+4),
                   color )
    {}
};

/// \brief
/// J_shape class
/// \details
/// This class inherits from Tetromino and creates the so-called J-shape as can be found here:
/// https://en.wikipedia.org/wiki/Tetromino
class J_shape : public Tetromino{
public:
    /// \brief
    /// constructor
    /// \details
    /// This constructor initializes its superclass with set values using a starting position.
    /// The starting position should be the left-most pixel of your shape.
    J_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = BLUE ): 
        Tetromino( hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x+2, s.y),
                   hwlib::xy(s.x, s.y+4),
                   color )
    {}
};

/// \brief
/// L_shape class
/// \details
/// This class inherits from Tetromino and creates the so-called L-shape as can be found here:
/// https://en.wikipedia.org/wiki/Tetromino
class L_shape : public Tetromino{
public:
    /// \brief
    /// constructor
    /// \details
    /// This constructor initializes its superclass with set values using a starting position.
    /// The starting position should be the left-most pixel of your shape.
    L_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = WHITE ): 
        Tetromino( hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x, s.y+4),
                   hwlib::xy(s.x+2, s.y+4),
                   color )
    {}
};

/// \brief
/// S_shape class
/// \details
/// This class inherits from Tetromino and creates the so-called S-shape as can be found here:
/// https://en.wikipedia.org/wiki/Tetromino
class S_shape : public Tetromino{
public:
    /// \brief
    /// constructor
    /// \details
    /// This constructor initializes its superclass with set values using a starting position.
    /// The starting position should be the left-most pixel of your shape.
    S_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = GREEN ): 
        Tetromino( hwlib::xy(s.x+2, s.y+2),
                   hwlib::xy(s),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x+2, s.y+4),
                   color )
    {}
};

/// \brief
/// Z_shape class
/// \details
/// This class inherits from Tetromino and creates the so-called Z-shape as can be found here:
/// https://en.wikipedia.org/wiki/Tetromino
class Z_shape : public Tetromino{
public:
    /// \brief
    /// constructor
    /// \details
    /// This constructor initializes its superclass with set values using a starting position.
    /// The starting position should be the left-most pixel of your shape.
    Z_shape( hwlib::xy s = hwlib::xy(48, 12), uint8_t color = RED ): 
        Tetromino( hwlib::xy(s.x+2, s.y+2),
                   hwlib::xy(s.x, s.y+4),
                   hwlib::xy(s.x, s.y+2),
                   hwlib::xy(s.x+2, s.y),
                   color )
    {}
};
}

#endif //TETRIS_SHAPES_HPP