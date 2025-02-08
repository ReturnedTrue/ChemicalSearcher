#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <chrono>

constexpr uint64_t DISCORD_APPLICATION_ID = 1306068247975563355;

inline time_t GetTimeNow()
{
	using std::chrono::system_clock;

	return system_clock::to_time_t(system_clock::now());
}

void IgnoreActivityResult(discord::Result result)
{
}

MainWindow::MainWindow()
	: QMainWindow()
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	chemClient = new PubChemClient(this);

	discord::Result createResult = discord::Core::Create(DISCORD_APPLICATION_ID, DiscordCreateFlags_NoRequireDiscord, &discordCore);

	if (createResult == discord::Result::Ok)
	{
		discordActivity = discord::Activity();
		discordActivity.SetDetails("Inputting Chemical...");
		discordActivity.SetType(discord::ActivityType::Playing);

		discord::ActivityTimestamps &timestamps = discordActivity.GetTimestamps();
		timestamps.SetStart(GetTimeNow());

		discord::ActivityAssets &activityAssets = discordActivity.GetAssets();
		activityAssets.SetLargeImage("atomic");

		discordCore->ActivityManager().UpdateActivity(discordActivity, IgnoreActivityResult);

		QTimer *timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &MainWindow::updateActivity);
		timer->start(10);
	}

	ui->errorLabel->setText("");
	ui->infoFrame->setVisible(false);

	ui->nameInput->installEventFilter(this);

	connect(ui->goButton, &QPushButton::clicked, this, &MainWindow::goButtonClicked);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete discordCore;
}

void MainWindow::updateActivity() 
{
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

	CompoundRecord record = chemClient->getRecordByName(ui->nameInput->toPlainText());

	if (!record.loaded)
	{
		ui->errorLabel->setText("chemical not found");

		if (discordCore)
		{
			discordActivity.SetDetails("Inputting Chemical...");
			discordCore->ActivityManager().UpdateActivity(discordActivity, IgnoreActivityResult);
		}

		return;
	}

	ui->infoFrame->setVisible(true);

	ui->infoLabel->setText(
		"<b>IUPAC Name:</b> " + record.iupacName + "<br />" +
		"<b>Molecular Formula:</b> " + record.molecularFormula + "<br />"
		"<b>Molecular Weight:</b> " + record.molecularWeight
	);

	ui->infoLabel->adjustSize();

	ui->infoImage->setPixmap(record.image);
	ui->infoImage->adjustSize();

	if (discordCore)
	{
		discordActivity.SetDetails(("Viewing Chemical: " + record.iupacName.toStdString()).c_str());
		discordCore->ActivityManager().UpdateActivity(discordActivity, IgnoreActivityResult);
	}
}
