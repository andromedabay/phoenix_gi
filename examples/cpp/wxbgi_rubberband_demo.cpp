/**
 * @file wxbgi_rubberband_demo.cpp
 * @brief Shows immediate-only draws and transform previews alongside retained geometry.
 */

#include "wx_bgi.h"
#include "wx_bgi_dds.h"
#include "wx_bgi_ext.h"

#include <cstdio>
#include <string>

int main()
{
    wxbgi_wx_app_create();
    wxbgi_wx_frame_create(640, 420, "DDS Rubber-Band Preview");
    if (graphresult() != 0)
        return 1;

    wxbgi_dds_layer_create("Geometry");
    setcolor(bgi::LIGHTCYAN);
    line(50, 70, 260, 210, 0, "Geometry");
    const std::string sourceId(wxbgi_dds_get_id_at(wxbgi_dds_object_count() - 1));

    cleardevice();
    wxbgi_render_dds("default");

    setcolor(bgi::YELLOW);
    line(50, 90, 280, 250, 1, "DefaultLayer");
    wxbgi_dds_translate(sourceId.c_str(), 250.f, 0.f, 0.f, 1, "DefaultLayer");

    std::printf("Rubber-band previews were drawn without adding DDS objects (count: %d).\n",
                wxbgi_dds_object_count());
    wxbgi_wx_close_after_ms(6000);
    wxbgi_wx_app_main_loop();
    return 0;
}
