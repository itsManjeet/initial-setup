#include <glibmm.h>

#include "Application.hxx"
#include "config.hxx"

int main(int argc, char** argv) {
    Glib::setenv("GSETTINGS_SCHEMA_DIR", GENERATED_DATA_DIR, false);
    auto app = Application::create();

    return app->run(argc, argv);
}