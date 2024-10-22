#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(): QMainWindow(), ui(new Ui::MainWindow), chemClient(this)
{
    ui->setupUi(this);

	ui->errorLabel->setText("");
	ui->infoFrame->setVisible(false);

	connect(ui->goButton, &QPushButton::clicked, this, &MainWindow::goButtonClicked);
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
		"<b>Molecular Formula:</b> " + record->molecularFormula
	);

	ui->infoLabel->adjustSize();

	ui->infoImage->setPixmap(record->image);
	ui->infoImage->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}
