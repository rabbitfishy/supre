#pragma once

#define PREVIEW_WIDTH			20
#define PREVIEW_HEIGHT			8
#define PREVIEW_OFFSET_X		175

#define COLORPICKER_OFFSET_X	10
#define COLORPICKER_OFFSET_Y	5
#define COLORPICKER_PADDING		5
#define COLORPICKER_SIZE		150

#define HUE_BAR_WIDTH			10
#define ALPHA_BAR_HEIGHT		10

class Colorpicker : public Element {
public:
	__forceinline Colorpicker( ) : m_open{ false }, m_label{ }, m_color{ }, m_ptr{ nullptr } {
		m_flags		= ElementFlags::DRAW | ElementFlags::CLICK | ElementFlags::ACTIVE | ElementFlags::SAVE;
		m_type		= ElementTypes::COLORPICKER;
		m_h			= m_base_h = PREVIEW_HEIGHT;
		m_use_label = true;
		m_show		= true;
	}

	__forceinline void setup( const std::string &label, const std::string &file_id, Color color, Color* ptr = nullptr ) {
		m_label   = label;
		m_file_id = file_id;
		m_color   = color;
		m_alpha	  = m_color.a( ) / 255.0f;
		m_ptr     = ptr;

		if( m_ptr )
			*m_ptr = m_color;
	}

	__forceinline void set( Color color ) {
		bool changed = m_color.rgba( ) != color.rgba( );

		m_color = color;
		m_alpha = m_color.a( ) / 255.0f;

		if( m_ptr )
			*m_ptr = m_color;

		if( changed && m_callback )
			m_callback( );
	}

	__forceinline Color get( ) {
		return m_color;
	}

	__forceinline void reset_drag( ) {
		m_color_drag	= false;
		m_hue_drag		= false;
		m_alpha_drag	= false;
	}

	static void update( );

protected:
	bool					m_open;
	std::string				m_label;
	Color					m_color;
	Color*					m_ptr;

	float					m_hue;
	float					m_saturation;
	float					m_value;
	float					m_alpha;

	bool					m_alpha_drag;
	bool					m_hue_drag;
	bool					m_color_drag;

	int						m_texture;
	std::vector< Color >	m_gradient;

protected:
	void draw( ) override;
	void think( ) override;
	void click( ) override;
};
