#include "pubchemclient.h"

PubChemClient::PubChemClient(QObject *parent): manager(), loop(), parent(parent) {}

QNetworkReply* PubChemClient::sendRequest(QString url)
{
	auto request = QNetworkRequest(QUrl(url));
	auto reply = manager.get(request);

	parent->connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();

	if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200)
	{
		return nullptr;
	}

	return reply;
}

CompoundRecord* PubChemClient::getRecordByName(QString name)
{
	auto recordReply = sendRequest("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + name + "/property/MolecularFormula,IUPACName/JSON");
	if (recordReply == nullptr) return nullptr;

	auto data = QJsonDocument::fromJson(recordReply->readAll());
	auto properties = data["PropertyTable"]["Properties"][0];

	auto record = new CompoundRecord();
	record->molecularFormula = properties["MolecularFormula"].toString();
	record->iupacName = properties["IUPACName"].toString();

	auto imageReply = sendRequest("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + name + "/PNG?image_size=256x256");
	if (imageReply == nullptr) return nullptr;

	auto pixmap = QPixmap();
	pixmap.loadFromData(imageReply->readAll());

	record->image = pixmap;
	
	return record;
}