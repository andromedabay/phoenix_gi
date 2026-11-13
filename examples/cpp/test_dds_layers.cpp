/**
 * @file test_dds_layers.cpp
 * @brief Layer membership, visibility, lifecycle, persistence, and previews.
 */

#include "wx_bgi.h"
#include "wx_bgi_dds.h"
#include "wx_bgi_ext.h"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace
{

void require(bool condition, const char *message)
{
    if (!condition)
    {
        std::fprintf(stderr, "FAIL [test_dds_layers]: %s\n", message);
        std::exit(1);
    }
}

} // namespace

int main()
{
    wxbgi_wx_app_create();
    wxbgi_wx_frame_create(320, 240, "test_dds_layers");
    require(graphresult() == 0, "failed to create test window");

    require(wxbgi_dds_layer_count() == 1, "scene should start with one layer");
    require(std::string(wxbgi_dds_layer_get_name_at(0)) == "DefaultLayer",
            "DefaultLayer should be first in the layer list");
    require(wxbgi_dds_layer_create("Sketch") == 1, "failed to create Sketch");
    require(wxbgi_dds_layer_create("Sketch") == 0, "duplicate layer was accepted");
    require(wxbgi_dds_layer_delete("DefaultLayer") == 0,
            "DefaultLayer should not be deletable");

    setcolor(bgi::WHITE);
    line(10, 10, 100, 100, 0, "Sketch");
    const std::string sourceId(wxbgi_dds_get_id_at(wxbgi_dds_object_count() - 1));
    require(!sourceId.empty(), "retained line has no ID");
    require(std::string(wxbgi_dds_get_layer(sourceId.c_str())) == "Sketch",
            "draw call did not assign the requested layer");

    const int retainedCount = wxbgi_dds_object_count();
    line(15, 15, 110, 110, 1, "Sketch");
    require(wxbgi_dds_object_count() == retainedCount,
            "rubber-band line was added to the DDS");
    require(std::string(wxbgi_dds_translate(sourceId.c_str(), 20.f, 0.f, 0.f,
                                           1, "Sketch")).empty(),
            "rubber-band transform unexpectedly returned a retained ID");
    require(wxbgi_dds_object_count() == retainedCount,
            "rubber-band transform was added to the DDS");

    const std::string transformId(
        wxbgi_dds_translate(sourceId.c_str(), 0.f, 20.f, 0.f, 0, "Sketch"));
    require(!transformId.empty(), "retained transform was not created");
    require(std::string(wxbgi_dds_get_layer(transformId.c_str())) == "Sketch",
            "transform did not use the requested layer");

    require(wxbgi_dds_layer_set_visible("Sketch", 0) == 1,
            "failed to hide Sketch");
    require(wxbgi_dds_layer_get_visible("Sketch") == 0,
            "Sketch visibility state was not updated");
    wxbgi_render_dds("default");
    require(wxbgi_dds_layer_set_visible("Sketch", 1) == 1,
            "failed to show Sketch");

    require(wxbgi_dds_set_layer(sourceId.c_str(), "DefaultLayer") == 1,
            "failed to move an object to DefaultLayer");
    require(std::string(wxbgi_dds_get_layer(sourceId.c_str())) == "DefaultLayer",
            "object layer assignment did not change");

    const std::string json(wxbgi_dds_to_json());
    require(json.find("\"layer\": \"Sketch\"") != std::string::npos,
            "JSON omitted layer membership");
    require(wxbgi_dds_from_json(json.c_str()) == 0, "JSON round-trip failed");
    require(wxbgi_dds_layer_exists("Sketch") == 1,
            "JSON round-trip lost Sketch");
    require(std::string(wxbgi_dds_get_layer(transformId.c_str())) == "Sketch",
            "JSON round-trip lost transform layer membership");

    const std::string yaml(wxbgi_dds_to_yaml());
    require(yaml.find("layers:") != std::string::npos,
            "YAML omitted layer table");
    require(wxbgi_dds_from_yaml(yaml.c_str()) == 0, "YAML round-trip failed");
    require(wxbgi_dds_layer_exists("Sketch") == 1,
            "YAML round-trip lost Sketch");
    require(std::string(wxbgi_dds_get_layer(transformId.c_str())) == "Sketch",
            "YAML round-trip lost transform layer membership");

    const int beforeClear = wxbgi_dds_object_count();
    require(wxbgi_dds_layer_clear("Sketch") == 1, "failed to clear Sketch");
    require(wxbgi_dds_object_count() == beforeClear - 1,
            "clear did not remove the object assigned to Sketch");
    require(wxbgi_dds_layer_exists("Sketch") == 1,
            "clear unexpectedly deleted Sketch");

    require(wxbgi_dds_layer_create("Temporary") == 1,
            "failed to create Temporary");
    const int beforeDelete = wxbgi_dds_object_count();
    circle(180, 120, 25, 0, "Temporary");
    require(wxbgi_dds_object_count() == beforeDelete + 1,
            "layered circle was not retained");
    require(wxbgi_dds_layer_delete("Temporary") == 1,
            "failed to delete Temporary");
    require(wxbgi_dds_object_count() == beforeDelete,
            "deleting a layer did not delete its objects");
    require(wxbgi_dds_layer_exists("Temporary") == 0,
            "deleted layer still exists");

    wxbgi_wx_close_after_ms(250);
    wxbgi_wx_app_main_loop();
    std::printf("PASS [test_dds_layers]\n");
    return 0;
}
