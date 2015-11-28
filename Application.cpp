#include "Application.h"

void setApplicationSettings(QApplication &app)
{
    app.setApplicationName(APPNAME);
    app.setApplicationVersion(APPVER);
    app.setOrganizationName(ORGNAME);
    app.setOrganizationDomain(ORGDOMAIN);
    app.setStyle("fusion");
}
