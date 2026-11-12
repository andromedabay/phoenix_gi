#pragma once
// wx_bgi_canvas.h — WxBgiCanvas: BGI drawing surface embedded in a wxWidgets panel.
// Include this header in your application AFTER the wxWidgets headers.
//
// WxBgiCanvas is compiled into the wx_bgi_wx static library (not the DLL),
// so it uses your application''s copy of wxWidgets — no DLL-boundary issues.

#include <wx/glcanvas.h>
#include "wx_bgi.h"
#include "wx_bgi_ext.h"

namespace wxbgi {

/**
 * @brief A wxGLCanvas that hosts the wx_bgi BGI drawing surface.
 *
 * Drop this panel into any wxFrame or wxPanel.  After the first paint the
 * BGI state is initialised and you can call any BGI drawing function.
 *
 * Typical usage:
 * @code
 *   // In wxFrame constructor — no GL work here:
 *   m_canvas = new wxbgi::WxBgiCanvas(this, wxID_ANY,
 *                                     wxDefaultPosition, wxSize(800, 600));
 *   // After first paint fires (e.g. via wxEVT_PAINT or wxCallAfter):
 *   setbkcolor(BLACK);
 *   circle(200, 200, 80);
 *   m_canvas->SetAutoRefreshHz(60);
 * @endcode
 */
class WxBgiCanvas : public wxGLCanvas
{
public:
    explicit WxBgiCanvas(wxWindow* parent,
                         wxWindowID id    = wxID_ANY,
                         const wxPoint&  pos   = wxDefaultPosition,
                         const wxSize&   size  = wxDefaultSize,
                         long            style = 0,
                         const wxString& name  = wxASCII_STR("WxBgiCanvas"));

    ~WxBgiCanvas() override;

    /** Render the BGI buffer immediately. */
    void Render();

    /**
     * @brief Enable idle-driven auto-refresh at the requested frame rate.
     * @param hz  Target refresh rate.  Pass 0 to disable.
     */
    void SetAutoRefreshHz(int hz);

protected:
    /**
     * @brief Called inside Render() after the GL context is current and BGI
     *        is initialised, but *before* the BGI pixel buffer is blitted to
     *        this canvas's OpenGL surface.
     *
     * Override in subclasses to render a specific camera's scene into the
     * shared BGI scratch buffer.  Because wx paint events are sequential on
     * the main thread, the scratch buffer is safe to reuse across panels.
     *
     * @param w Logical pixel width of this canvas.
     * @param h Logical pixel height of this canvas.
     */
    virtual void PreBlit(int w, int h) { (void)w; (void)h; }

    /**
     * @brief Called AFTER @c wxbgi_wx_render_page_gl_vp() and all 3-D solid
     *        GL passes, but BEFORE @c SwapBuffers().
     *
     * Override in subclasses to composite overlays (concentric circles, grid,
     * UCS axes) on top of 3-D solid geometry.  Typically implemented by
     * calling @c wxbgi_wx_render_overlays_for_camera() for this panel's camera.
     *
     * @param pageW Logical BGI page width.
     * @param pageH Logical BGI page height.
     * @param vpW   Physical framebuffer width (may differ on HiDPI).
     * @param vpH   Physical framebuffer height (may differ on HiDPI).
     */
    virtual void PostBlit(int pageW, int pageH, int vpW, int vpH)
    {
        (void)pageW; (void)pageH; (void)vpW; (void)vpH;
    }

private:
    wxGLContext* m_glContext{nullptr};   ///< Points to the process-wide shared context.
    bool         m_ownsGLCtx{false};    ///< True only for the first canvas that created the context.
    bool         m_glewInited{false};
    bool         m_bgiInited{false};
    int          m_autoRefreshHz{0};
    wxLongLong_t m_lastRenderMs{0};

public:
    /**
     * @brief Handles paint events for the canvas.
     *
     * This method is called when the canvas needs to be repainted. It creates a
     * wxPaintDC to prepare for drawing and then calls the Render() method to
     * perform the actual rendering of the BGI buffer onto the canvas.
     *
     * @param evt The paint event.
     */
    void OnPaint(wxPaintEvent& evt);

    /**
     * @brief Handles resize events for the canvas.
     *
     * When the canvas is resized, this method checks if the BGI pixel buffer needs
     * to be resized. If the new size is different from the current size, it calls
     * wxbgi_wx_resize() to adjust the BGI buffer accordingly. Finally, it triggers
     * a refresh of the canvas to ensure the new size is rendered.
     *
     * @param evt The size event containing the new dimensions.
     */
    void OnSize(wxSizeEvent& evt);

    /**
     * @brief Handles idle events for auto-refresh.
     *
     * If auto-refresh is enabled, this method checks if enough time has passed
     * since the last render to meet the target refresh rate. If so, it triggers
     * a refresh of the canvas. This method is called during idle time when the
     * application is not processing other events.
     *
     * @param evt The idle event.
     */
    void OnIdle(wxIdleEvent& evt);

    /**
     * @brief Handles key press events.
     *
     * Forwards the key press to the BGI key handler, then allows wxWidgets to
     * continue processing the event. Override this method in subclasses to implement
     * custom key press behavior. Remember to call the base class implementation if
     * you want to preserve the default behavior also.
     *
     * @param evt Key event containing the key press information.
     */
    void OnKeyDown(wxKeyEvent& evt);

    /**
     * @brief Handles key release events.
     *
     * Forwards the key release to the BGI key handler, then allows wxWidgets to
     * continue processing the event. Override this method in subclasses to implement
     * custom key release behavior. Remember to call the base class implementation if
     * you want to preserve the default behavior also.
     *
     * @param evt Key event containing the key release information.
     */
    void OnKeyUp(wxKeyEvent& evt);

    /**
     * @brief Handles character input events.
     *
     * Forwards the Unicode code point of the character to the BGI character handler,
     * then allows wxWidgets to continue processing the event. Override this method
     * in subclasses to implement custom character input behavior. Remember to call
     * the base class implementation if you want to preserve the default behavior also.
     *
     * @param evt Key event containing the character input information.
     */
    void OnChar(wxKeyEvent& evt);

    /**
     * @brief Handles mouse movement over the canvas.
     *
     * Forwards the canvas-relative mouse coordinates to the BGI mouse handler,
     * then allows wxWidgets to continue processing the event. Override this method
     * in subclasses to implement custom mouse behavior. Remember to call the base class
     * implementation if you want to preserve the default behavior also.
     *
     * @param evt Mouse event containing the pointer position.
     */
    void OnMouseMove(wxMouseEvent& evt);

    /**
     * @brief Handles mouse button press events.
     *
     * Forwards the button press to the BGI mouse handler, then allows wxWidgets to
     * continue processing the event. Override this method in subclasses to implement
     * custom mouse button behavior. Remember to call the base class implementation if
     * you want to preserve the default behavior also.
     *
     * @param evt Mouse event containing the button press information.
     */
    void OnMouseDown(wxMouseEvent& evt);

    /**
     * @brief Handles mouse button release events.
     *
     * Forwards the button release to the BGI mouse handler, then allows wxWidgets to
     * continue processing the event. Override this method in subclasses to implement
     * custom mouse button behavior. Remember to call the base class implementation if
     * you want to preserve the default behavior also.
     *
     * @param evt Mouse event containing the button release information.
     */
    void OnMouseUp(wxMouseEvent& evt);

    /**
     * @brief Handles mouse wheel events.
     *
     * Forwards the scroll delta to the BGI scroll handler, then allows wxWidgets to continue
     *  processing the event. Override this method in subclasses to implement custom mouse
     *  wheel behavior. Remember to call the base class implementation if you want
     *  to preserve the default behavior also.
     *
     * @param evt Mouse event containing the wheel rotation information.
     */
    void OnMouseWheel(wxMouseEvent& evt);

private:
    static int WxKeyToGlfw(int wxKey);

    /**
     * @brief Routes key events to the BGI key handler.
     *
     * Converts the wxWidgets key code to a GLFW key code and forwards the event
     * to the BGI key handler. If the conversion fails, the event is skipped.
     *
     * @param evt The key event.
     * @param action The action (press or release).
     */
    void       RouteKeyEvent(wxKeyEvent& evt, int action);
};

} // namespace wxbgi
