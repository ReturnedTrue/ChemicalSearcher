#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(): QMainWindow(), ui(new Ui::MainWindow), chemClient(this)
{
    ui->setupUi(this);

	ui->errorLabel->setText("");
	ui->infoFrame->setVisible(false);

	connect(ui->goButton, &QPushButton::clicked, this, &MainWindow::goButtonClicked);
	ui->nameInput->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
	if (object != ui->nameInput || event->type() != QEvent::KeyPress) return false;

	QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
	if (keyEvent->key() != Qt::Key_Return) return false;

	MainWindow::goButtonClicked(true);
	return true;
}

void MainWindow::goButtonClicked(bool checked) 
{
	ui->errorLabel->setText("");
	ui->infoFrame->setVisible(false);

	auto record = chemClient.getRecordByName(ui->nameInput->toPlainText());

	if (record == nullptr)
	{
		ui->errorLabel->setText("chemical not found");
		return;
	}

	ui->infoFrame->setVisible(true);

	ui->infoLabel->setText(
		"<b>IUPAC Name:</b> " + record->iupacName + "<br />" +
		"<b>Molecular Formula:</b> " + record->molecularFormula + "<br />"
		"<b>Molecular Weight:</b> " + record->molecularWeight
	);

	ui->infoLabel->adjustSize();

	ui->infoImage->setPixmap(record->image);
	ui->infoImage->adjustSize();

	delete record;
}

MainWindow::~MainWindow()
{
    delete ui;
}
