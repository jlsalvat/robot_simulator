#pragma once
#include <string>
#include <SDL2/SDL_ttf.h>
#include "Parameters.h"

class Font  
{
public:

	///@{
	/// \name Construction and destruction
	////////////////////////////////////////////////////////////
	/// \brief Construct default
	///
//	Font();
	////////////////////////////////////////////////////////////
	/// \brief Construct from existing TTF_Font structure
	///
	/// \param[in] font Existing TTF_Font to manage
	///
	explicit Font(TTF_Font* font);
	////////////////////////////////////////////////////////////
	/// \brief Loads font from .ttf or .fon file
	///
	/// \param[in] file Pointer File name to load font from
	/// \param[in] ptsize %Point size (based on 72DPI) to load font as. This basically translates to pixel height
	/// \param[in] index Choose a font face from a file containing multiple font faces. The first face is always index 0
	///
	/// \throws SDL2pp::Exception
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC14
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC16
	///
	////////////////////////////////////////////////////////////
	Font(const std::string& file="arial", int ptsize = 16, long index = 0);

	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC18
	///
	////////////////////////////////////////////////////////////
	virtual ~Font();
	///@}

	///@{
	/// \name Copy and move

	////////////////////////////////////////////////////////////
	/// \brief Move constructor
	///
	/// \param[in] other SDL2pp::Font object to move data from
	///
	////////////////////////////////////////////////////////////
	Font(Font&& other) noexcept;

	////////////////////////////////////////////////////////////
	/// \brief Move assignment
	///
	/// \param[in] other SDL2pp::Font object to move data from
	///
	/// \returns Reference to self
	///
	////////////////////////////////////////////////////////////
	Font& operator=(Font&& other) noexcept;

	////////////////////////////////////////////////////////////
	/// \brief Deleted copy constructor
	///
	/// This class is not copyable
	///
	////////////////////////////////////////////////////////////
	Font(const Font&) = delete;

	////////////////////////////////////////////////////////////
	/// \brief Deleted assignment operator
	///
	/// This class is not copyable
	///
	////////////////////////////////////////////////////////////
	Font& operator=(const Font&) = delete;

	///@}

	///@{
	/// \name Compatibility with legacy SDL code

	////////////////////////////////////////////////////////////
	/// \brief Get pointer to managed TTF_Font structure
	///
	/// \returns Pointer to managed TTF_Font structure
	///
	////////////////////////////////////////////////////////////
	TTF_Font* get() const;

	///@{
	/// \name Attributes: font style

	////////////////////////////////////////////////////////////
	/// \brief Get the rendering style of the loaded font
	///
	/// \returns The style as a bitmask composed of the following masks:
	///          TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE,
	///          TTF_STYLE_STRIKETHROUGH. If no style is set then
	///          TTF_STYLE_NORMAL is returned
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC21
	///
	////////////////////////////////////////////////////////////
	int getStyle() const;

	////////////////////////////////////////////////////////////
	/// \brief Set the rendering style of the loaded font
	///
	/// \param[in] style The style as a bitmask composed of the following masks:
	///                  TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE,
	///                  TTF_STYLE_STRIKETHROUGH. If no style is desired then use
	///                  TTF_STYLE_NORMAL, which is the default.
	///
	/// \note This will flush the internal cache of previously rendered
	/// glyphs, even if there is no change in style, so it may be best
	/// to check the current style by using GetStyle() first
	///
	/// \note TTF_STYLE_UNDERLINE may cause surfaces created by TTF_RenderGlyph_*
	/// functions to be extended vertically, downward only, to encompass the
	/// underline if the original glyph metrics didn't allow for the underline
	/// to be drawn below. This does not change the math used to place a glyph
	/// using glyph metrics.
	/// On the other hand TTF_STYLE_STRIKETHROUGH doesn't extend the glyph,
	/// since this would invalidate the metrics used to position the glyph
	/// when blitting, because they would likely be extended vertically upward.
	/// There is perhaps a workaround, but it would require programs to be
	/// smarter about glyph blitting math than they are currently designed for.
	/// Still, sometimes the underline or strikethrough may be outside of the
	/// generated surface, and thus not visible when blitted to the screen.
	/// In this case, you should probably turn off these styles and draw your
	/// own strikethroughs and underlines.
	///
	/// \returns Reference to self
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC22
	///
	////////////////////////////////////////////////////////////
	Font& setStyle(int style = TTF_STYLE_NORMAL);

	////////////////////////////////////////////////////////////
	/// \brief Get the current outline size of the loaded font
	///
	/// \returns The size of the outline currently set on the font, in pixels
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC23
	///
	////////////////////////////////////////////////////////////
	int getOutline() const;

	////////////////////////////////////////////////////////////
	/// \brief Set the outline pixel width of the loaded font
	///
	/// \param[in] outline The size of outline desired, in pixels.
	///                    Use 0 (zero) to turn off outlining.
	///
	/// \note This will flush the internal cache of previously rendered
	/// glyphs, even if there is no change in outline size, so it may be best
	/// to check the current outline size by using GetOutline() first
	///
	/// \returns Reference to self
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC24
	///
	////////////////////////////////////////////////////////////
	Font& setOutline(int outline = 0);

	///@}

	///@{
	/// \name Attributes: font settings

	////////////////////////////////////////////////////////////
	/// \brief Get the current hinting setting of the loaded font
	///
	/// \returns The hinting type matching one of the following defined values:
	///          TTF_HINTING_NORMAL, TTF_HINTING_LIGHT, TTF_HINTING_MONO,
	///          TTF_HINTING_NONE. If no hinting is set then TTF_HINTING_NORMAL is returned
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC25
	///
	////////////////////////////////////////////////////////////
	int getHinting() const;

	////////////////////////////////////////////////////////////
	/// \brief Set the hinting of the loaded font
	///
	/// \param[in] hinting The hinting setting desired, which is one of:
	///                    TTF_HINTING_NORMAL, TTF_HINTING_LIGHT, TTF_HINTING_MONO,
	///                    TTF_HINTING_NONE. The default is TTF_HINTING_NORMAL
	///
	/// You should experiment with this setting if you know which font
	/// you are using beforehand, especially when using smaller sized
	/// fonts. If the user is selecting a font, you may wish to let them
	/// select the hinting mode for that font as well
	///
	/// \note This will flush the internal cache of previously rendered
	/// glyphs, even if there is no change in hinting, so it may be best
	/// to check the current hinting by using GetHinting() first
	///
	/// \returns Reference to self
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC26
	///
	////////////////////////////////////////////////////////////
	Font& setHinting(int hinting = TTF_HINTING_NORMAL);
	///@}

	///@{
	/// \name Attributes: Font metrics

	////////////////////////////////////////////////////////////
	/// \brief Calculate the resulting surface size of the LATIN1 encoded Font rendered using font
	///
	/// \param[in] Font String to size up
	///
	/// \returns Point representing dimensions of the rendered Font
	///
	/// \throws SDL2pp::Exception
	///
	/// No actual rendering is done, however correct kerning is done
	/// to get the actual width. The height returned in h is the same
	/// as you can get using GetHeight()
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC39
	///
	////////////////////////////////////////////////////////////
	SDL_Point  GetSizeText(const std::string& text) const;
	///@}
	///@{
	/// \name load Surface from text 

	////////////////////////////////////////////////////////////
	/// \brief load Surface from text using solid mode (with foreground color)
	///
	/// \param[in] text LATIN1 string to render
	/// \param[in] fg Color to render the text in
	///
	/// \returns Surface containing rendered text
	///
	/// \throws runtime_error
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC43
	///
	////////////////////////////////////////////////////////////

	SDL_Surface*  loadSurface(const std::string& text, SDL_Color fg);
	////////////////////////////////////////////////////////////
	/// \brief load Surface from text using solid mode (with foreground color)
	///
	/// \param[in] text LATIN1 string to render
	/// \param[in] fg Color to render the text in
	/// \param[in] bg Color to render the background box in
	///
	/// \returns Surface containing rendered text
	///
	/// \throws runtime_error
	///
	/// \see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC43
	///
	////////////////////////////////////////////////////////////

	SDL_Surface* loadSurface(const std::string& text, SDL_Color fg, SDL_Color bg);

private:
	TTF_Font* ttf_font; ///< Managed TTF_Font object
};




