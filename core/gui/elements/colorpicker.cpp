#include "../../../includes.h"

// credit: https://yougame.biz/threads/331788/

void Colorpicker::update( ) {

	const int w{ COLORPICKER_SIZE };
	const int h{ COLORPICKER_SIZE };

	// allocate gradient.
	if ( m_gradient.empty( ) )
		m_gradient = std::vector< Color >( w * h );

	// allocate new texture in engine.
	if ( m_texture == 0 )
		m_texture = g_csgo.m_surface->CreateNewTextureID( true );

	// iterate width.
	for ( int i{ }; i < w; i++ ) {

		// iterate height.
		for ( int j{ }; j < h; j++ ) {

			float sat = j / float( h );
			float val = 1.0f - ( i / float( w ) );

			// write back to array.
			m_gradient[ i * w + j ] = Color::hsv_to_rgb( m_hue, sat, val, m_parent->m_alpha );
		}
	}

	// assign allocated memory containing the picker to said texture.
	g_csgo.m_surface->DrawSetTextureRGBA( m_texture, m_gradient.data( ), w, h );
}

void Colorpicker::draw( ) {

	Rect  area{ m_parent->GetElementsRect( ) };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	// label.
	render::menu_shade.string( p.x + LABEL_OFFSET, p.y - 2, { 205, 205, 205, m_parent->m_alpha }, m_label );

	// update preview opacity.
	Color preview	= m_color;
	preview.a( )	*= m_parent->m_opacity;

	// draw preview window.
	render::rect_filled( p.x + PREVIEW_OFFSET_X - PREVIEW_WIDTH + 1, p.y + 1, PREVIEW_WIDTH - 2, PREVIEW_HEIGHT - 2, preview );

	// fade / shine.
	render::rect_filled_fade( p.x + PREVIEW_OFFSET_X - PREVIEW_WIDTH + 1, p.y + 1, PREVIEW_WIDTH - 2, PREVIEW_HEIGHT - 2, { 50, 50, 35, m_parent->m_alpha }, 0, 150 );

	// outline.
	render::rect( p.x + PREVIEW_OFFSET_X - PREVIEW_WIDTH, p.y, PREVIEW_WIDTH, PREVIEW_HEIGHT, { 0, 0, 0, m_parent->m_alpha } );

	// draw picker.
	if ( m_open ) {

		// update texture.
		Colorpicker::update( );

		// background.
		render::rect_filled( p.x + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_OFFSET_Y, COLORPICKER_SIZE + HUE_BAR_WIDTH + (COLORPICKER_PADDING * 3), COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + (COLORPICKER_PADDING * 3), { 41, 41, 41, m_parent->m_alpha } );

		// backgound outline.
		render::rect( p.x + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_OFFSET_Y, COLORPICKER_SIZE + HUE_BAR_WIDTH + (COLORPICKER_PADDING * 3), COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + (COLORPICKER_PADDING * 3), { 0, 0, 0, m_parent->m_alpha } );

		// set panel color.
		g_csgo.m_surface->DrawSetColor( { 255, 255, 255, m_parent->m_alpha } );

		// set texture.
		g_csgo.m_surface->DrawSetTexture( m_texture );

		// draw picker.
		g_csgo.m_surface->DrawTexturedRect( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y, p.x + COLORPICKER_SIZE + COLORPICKER_PADDING + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_SIZE + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y );

		// picker outline.
		render::rect( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y, COLORPICKER_SIZE, COLORPICKER_SIZE, { 0, 0, 0, m_parent->m_alpha } );

		// draw hue bar.
		for ( int i{ }; i < COLORPICKER_SIZE; ++i ) {

			float hue = i / float( COLORPICKER_SIZE );
			render::rect_filled( p.x + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y + i, HUE_BAR_WIDTH, 1, Color::hsv_to_rgb( hue, 1.0f, 1.0f, m_parent->m_alpha ) );
		}

		// hue bar outline.
		render::rect( p.x + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_X - 1, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y, HUE_BAR_WIDTH + 1, COLORPICKER_SIZE, { 0, 0, 0, m_parent->m_alpha } );

		// hue bar slider.
		render::rect_filled( p.x + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_X - 1, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y + m_hue * (COLORPICKER_SIZE - 3), HUE_BAR_WIDTH + 1, 3, { 255, 255, 255, m_parent->m_alpha } );

		// hue bar slider outline
		render::rect( p.x + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_X - 1, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y + m_hue * (COLORPICKER_SIZE - 3), HUE_BAR_WIDTH + 1, 3, { 0, 0, 0, m_parent->m_alpha } );

		// update alpha opacity.
		Color alpha_color	= m_color;
		alpha_color.a( )	*= m_parent->m_opacity;

		// draw alpha bar.
		render::rect_filled( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_Y, COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + COLORPICKER_PADDING, ALPHA_BAR_HEIGHT, alpha_color );

		// alpha bar outline.
		render::rect( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_Y - 1, COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + COLORPICKER_PADDING, ALPHA_BAR_HEIGHT + 1, { 0, 0, 0, m_parent->m_alpha } );

		// alpha bar slider.
		render::rect_filled( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X + m_alpha * ((COLORPICKER_SIZE - 3) + ALPHA_BAR_HEIGHT + COLORPICKER_PADDING), p.y + PREVIEW_HEIGHT + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_Y, 3, ALPHA_BAR_HEIGHT - 1, { 255, 255, 255, m_parent->m_alpha } );

		// alpha bar slider outline.
		render::rect( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X + m_alpha * ((COLORPICKER_SIZE - 3) + ALPHA_BAR_HEIGHT + COLORPICKER_PADDING), p.y + PREVIEW_HEIGHT + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_Y - 1, 3, ALPHA_BAR_HEIGHT + 1, { 0, 0, 0, m_parent->m_alpha } );

		// color selector.
		render::rect_filled( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X + m_saturation * (COLORPICKER_SIZE - 4), p.y + PREVIEW_HEIGHT + COLORPICKER_OFFSET_Y + COLORPICKER_PADDING + (1 - m_value) * (COLORPICKER_SIZE - 4), 4, 4, { 255, 255, 255, m_parent->m_alpha } );

		// color selector outline.
		render::rect( p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X + m_saturation * (COLORPICKER_SIZE - 4), p.y + PREVIEW_HEIGHT + COLORPICKER_OFFSET_Y + COLORPICKER_PADDING + (1 - m_value) * (COLORPICKER_SIZE - 4), 4, 4, { 0, 0, 0, m_parent->m_alpha } );
	}
}

void Colorpicker::think( ) {

	Rect  area{ m_parent->GetElementsRect( ) };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	if( m_open ) {

		// color square bounds.
		Rect color_square{ p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y, COLORPICKER_SIZE, COLORPICKER_SIZE };

		// hue bounds.
		Rect hue_bar{ p.x + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_X - 1, p.y + PREVIEW_HEIGHT + COLORPICKER_PADDING + COLORPICKER_OFFSET_Y, HUE_BAR_WIDTH + 1, COLORPICKER_SIZE };

		// alpha bounds.
		Rect alpha_bar{ p.x + COLORPICKER_PADDING + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_SIZE + (COLORPICKER_PADDING * 2) + COLORPICKER_OFFSET_Y - 1, COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + COLORPICKER_PADDING, ALPHA_BAR_HEIGHT + 1 };

		// update colors.
		if ( g_input.GetKeyState( VK_LBUTTON ) ) {

			// within color square.
			if ( g_input.IsCursorInRect( color_square ) || m_color_drag ) {

				// update color saturation.
				m_saturation = std::clamp( ( g_input.m_mouse.x - color_square.x ) / float( COLORPICKER_SIZE ), 0.0f, 1.0f );

				// update color value.
				m_value = 1.0f - std::clamp( ( g_input.m_mouse.y - color_square.y ) / float( COLORPICKER_SIZE ), 0.0f, 1.0f );

				// dragging color.
				m_color_drag = true;
			}

			// within hue slider.
			else if ( g_input.IsCursorInRect( hue_bar ) || m_hue_drag ) {

				// update color hue.
				m_hue = std::clamp( ( g_input.m_mouse.y - hue_bar.y ) / float( COLORPICKER_SIZE ), 0.0f, 1.0f );

				// dragging hue.
				m_hue_drag = true;
			}

			// within alpha slider.
			else if ( g_input.IsCursorInRect( alpha_bar ) || m_alpha_drag ) {

				// update color alpha.
				m_alpha = std::clamp( ( g_input.m_mouse.x - alpha_bar.x ) / float( COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + COLORPICKER_PADDING ), 0.0f, 1.0f );

				// dragging alpha.
				m_alpha_drag = true;
			}
		}
		else {

			// reset drag states.
			Colorpicker::reset_drag( );
		}

		// update height.
		m_h = PREVIEW_HEIGHT + COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + ( COLORPICKER_PADDING * 3 ) + COLORPICKER_OFFSET_Y;

		// setup width addition.
		int preview_width		= p.x + PREVIEW_OFFSET_X + 1;
		int colorpicker_width	= p.x + COLORPICKER_OFFSET_X + COLORPICKER_SIZE + HUE_BAR_WIDTH + ( COLORPICKER_PADDING * 3 );
		int diff				= ( colorpicker_width - preview_width );

		// update width.
		m_w = PREVIEW_OFFSET_X + ( diff > 0 ? diff : 0 );

		// color picker window.
		Rect picker_window{ p.x + COLORPICKER_OFFSET_X, p.y + PREVIEW_HEIGHT + COLORPICKER_OFFSET_Y, COLORPICKER_SIZE + HUE_BAR_WIDTH + ( COLORPICKER_PADDING * 3 ), COLORPICKER_SIZE + ALPHA_BAR_HEIGHT + ( COLORPICKER_PADDING * 3 ) };

		// using color picker.
		if ( m_color_drag || m_hue_drag || m_alpha_drag ) {

			// set updated colors.
			m_color			= Color::hsv_to_rgb( m_hue, m_saturation, m_value );
			m_color.a( )	= ( m_alpha * 255.f );
		}

		// update active element.
		if ( g_input.IsCursorInRect( picker_window ) ) {

			m_parent->m_active_element = this;

			// don't drag menu.
			g_gui.m_drag_form = nullptr;
		}
		else {

			// close on element selection.
			if ( m_parent->m_active_element != this ) {

				m_open = false;

				// callback on close.
				if ( m_callback )
					m_callback( );
			}
		}
	}
	else {

		// restore bounds.
		m_h = PREVIEW_HEIGHT;
		m_w = PREVIEW_OFFSET_X;
	}

	if( m_ptr )
		*m_ptr = m_color;
}

void Colorpicker::click( ) {
	Rect  area{ m_parent->GetElementsRect( ) };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	// color preview.
	Rect preview{ p.x + PREVIEW_OFFSET_X - PREVIEW_WIDTH, p.y, PREVIEW_WIDTH, PREVIEW_HEIGHT };

	// cursor in preview.
	if ( g_input.IsCursorInRect( preview ) ) {

		// grab previous color.
		if ( !m_open )
			Color::color_to_hsv( m_color, m_hue, m_saturation, m_value );

		// set open state.
		m_open = !m_open;
	}
}
