class Color {
private:
	// easy reinterpret.
	union {
		struct { 
			uint8_t m_r; 
			uint8_t m_g; 
			uint8_t m_b; 
			uint8_t m_a; 
		};

		uint32_t m_rgba;
	};

public:
	// ctors.
	__forceinline Color( ) : m_rgba{ 0 } { }
	__forceinline Color( int r, int g, int b, int a = 255 ) : m_r{ ( uint8_t )r }, m_g{ ( uint8_t )g }, m_b{ ( uint8_t )b }, m_a{ ( uint8_t )a } { }
	__forceinline Color( uint32_t rgba ) : m_rgba{ rgba } { }

    static void color_to_hsv( Color c, float& h, float& s, float& v ) {

        float r_norm = float( c.r( ) ) / 255.0f;
        float g_norm = float( c.g( ) ) / 255.0f;
        float b_norm = float( c.b( ) ) / 255.0f;

        float cmax = std::max( { r_norm, g_norm, b_norm } );
        float cmin = std::min( { r_norm, g_norm, b_norm } );
        float diff = cmax - cmin;

        v = cmax;

        if ( cmax == 0.0f )
            s = 0.0f;
        else
            s = diff / cmax;

        if ( diff == 0.0f )
            h = 0.0f;
        else if ( cmax == r_norm )
            h = std::fmod( ( 60 * ( ( g_norm - b_norm ) / diff ) + 360 ), 360 ) / 360.0f;
        else if ( cmax == g_norm )
            h = std::fmod( ( 60 * ( ( b_norm - r_norm ) / diff ) + 120 ), 360 ) / 360.0f;
        else
            h = std::fmod( ( 60 * ( ( r_norm - g_norm ) / diff ) + 240 ), 360 ) / 360.0f;
    }

    static Color hsv_to_rgb( float h, float s, float v, float a ) {

        float r, g, b;

        if (s == 0.0f) {
            r = g = b = v;
        }
        else {
            int i   = int( h * 6.0f );
            float f = ( h * 6.0f ) - i;
            float p = v * ( 1.0f - s );
            float q = v * ( 1.0f - s * f );
            float t = v * ( 1.0f - s * ( 1.0f - f ) );

            switch ( i % 6 ) {
            case 0: r = v, g = t, b = p; break;
            case 1: r = q, g = v, b = p; break;
            case 2: r = p, g = v, b = t; break;
            case 3: r = p, g = q, b = v; break;
            case 4: r = t, g = p, b = v; break;
            case 5: r = v, g = p, b = q; break;
            }
        }

        return Color( r * 255, g * 255, b * 255, a );
    }

    static Color hsv_to_rgb( float h, float s, float v ) {
        return hsv_to_rgb( h, s, v, 255.0f );
    }

	// member accessors.
	__forceinline uint8_t& r( ) { return m_r; }
	__forceinline uint8_t& g( ) { return m_g; }
	__forceinline uint8_t& b( ) { return m_b; }
	__forceinline uint8_t& a( ) { return m_a; }
	__forceinline uint32_t& rgba( ) { return m_rgba; }

    // operators.
    __forceinline operator uint32_t( ) { return m_rgba; }
};

namespace colors {
	static Color white{ 255, 255, 255, 255 };
	static Color black{ 0, 0, 0, 255 };
	static Color red{ 255, 0, 0, 255 };
	static Color burgundy{ 0xff2d00b3 };
	static Color light_blue{ 95, 174, 227, 255 };
	static Color orange{ 243, 156, 18, 255 };
	static Color transparent_green{ 0, 255, 0, 200 };
	static Color transparent_yellow{ 255, 255, 0, 200 };
	static Color transparent_red{ 255, 0, 0, 200 };
}
