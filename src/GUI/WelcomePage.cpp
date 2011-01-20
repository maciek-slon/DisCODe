#include "WelcomePage.h"

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

WelcomePage::~WelcomePage()
{

}

void WelcomePage::on_btnConnect_clicked() {
	emit do_connect_sig();
}

void WelcomePage::on_btnSpawn_clicked() {

}
