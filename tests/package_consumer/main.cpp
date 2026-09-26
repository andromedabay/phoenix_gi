#include <wx/app.h>

int main()
{
    return wxApp::GetInstance() == nullptr ? 0 : 1;
}