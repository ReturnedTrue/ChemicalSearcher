#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define DISCORD_APPLICATION_ID 1306068247975563355

using std::chrono::system_clock;

MainWindow::MainWindow(): QMainWindow(), ui(new Ui::MainWindow), chemClient(this), discordCore(), discordActivity()
{
    ui->setupUi(this);

	auto createResult = discord::Core::Create(DISCORD_APPLICATION_ID, DiscordCreateFlags_Default, &discordCore);

	discordActivity.SetDetails("Inputting chemical...");
	discordActivity.SetType(discord::ActivityType::Playing);

	auto& timestamps = discordActivity.GetTimestamps();
	timestamps.SetStart(system_clock::to_time_t(system_clock::now()));

	auto& activityAssets = discordActivity.GetAssets();
	activityAssets.SetLargeImage("atomic");

	discordCore->ActivityManager().UpdateActivity(discordActivity, [](auto result) {});

	auto timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MainWindow::update);
	timer->start(10);

	ui->errorLabel->setText("");
	ui->infoFrame->setVisible(false);

	ui->nameInput->installEventFilter(this);
	connect(ui->goButton, &QPushButton::clicked, this, &MainWindow::goButtonClicked);
}

void MainWindow::update() {
	discordCore->RunCallbacks();
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

		discordActivity.SetDetails("Inputting chemical...");
		discordCore->ActivityManager().UpdateActivity(discordActivity, [](auto result) {});

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

	discordActivity.SetDetails(("Viewing chemical: " + record->iupacName.toStdString()).c_str());
	discordCore->ActivityManager().UpdateActivity(discordActivity, [](auto result) {});

	delete record;
}

MainWindow::~MainWindow()
{
    delete ui;
	delete discordCore;
}
