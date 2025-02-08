// https://pubchem.ncbi.nlm.nih.gov/docs/pug-rest

#include "pubchemclient.h"

constexpr uint16_t PUBCHEM_SUBSCRIPT_BASE = 0x2080;

constexpr QLatin1String PUBCHEM_RECORD_URL("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/%1/property/MolecularFormula,MolecularWeight,IUPACName/JSON");
constexpr QLatin1String PUBCHEM_IMAGE_URL("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/%1/PNG?image_size=256x256");

PubChemClient::PubChemClient(QObject *parent)
	: parent(parent)
{
	manager = new QNetworkAccessManager(parent);
	loop = new QEventLoop(parent);
}

QNetworkReply* PubChemClient::sendRequest(QString url)
{
	QNetworkRequest request = QNetworkRequest(QUrl(url));
	QNetworkReply *reply = manager->get(request);

	parent->connect(reply, &QNetworkReply::finished, loop, &QEventLoop::quit);
	loop->exec();

	if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200)
	{
		return nullptr;
	}

	return reply;
}

QString PubChemClient::formatMolecularFormula(QString input)
{
	QString formatted;

	for (auto i = input.begin(), end = input.end(); i != end; ++i)
	{
		QChar current = *i;

		if (current.isDigit())
		{
			int subscript = PUBCHEM_SUBSCRIPT_BASE + current.unicode() - 48;
			current = QChar::fromUcs2(subscript);
		}

		formatted.append(current);
	}

	return formatted;
}

CompoundRecord PubChemClient::getRecordByName(QString name)
{
	CompoundRecord record;

	QPointer<QNetworkReply> recordReply(sendRequest(PUBCHEM_RECORD_URL.arg(name)));
	if (recordReply == nullptr) return record;

	QJsonDocument document = QJsonDocument::fromJson(recordReply->readAll());
	QJsonValue properties = document["PropertyTable"]["Properties"][0];

	record.molecularFormula = formatMolecularFormula(properties["MolecularFormula"].toString());
	record.molecularWeight = properties["MolecularWeight"].toString();
	record.iupacName = properties["IUPACName"].toString();

	QPointer<QNetworkReply> imageReply(sendRequest(PUBCHEM_IMAGE_URL.arg(name)));
	if (imageReply == nullptr) return record;

	QPixmap pixmap;
	pixmap.loadFromData(imageReply->readAll());

	record.image = pixmap;
	record.loaded = true;

	return record;
}
