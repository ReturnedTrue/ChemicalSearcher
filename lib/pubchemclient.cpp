// https://pubchem.ncbi.nlm.nih.gov/docs/pug-rest

#include "pubchemclient.h"

const int SUBSCRIPT_BASE = 0x2080;

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

QString PubChemClient::formatMolecularFormula(QString input)
{
	auto formatted = QString();

	for (auto i = input.begin(), end = input.end(); i != end; ++i)
	{
		auto current = *i;
		
		if (current.isDigit())
		{
			auto subscript = SUBSCRIPT_BASE + current.unicode() - 48;
			formatted.append(QChar::fromUcs2(subscript));

			continue;
		}


		formatted.append(current);
	}

	return formatted;
}

CompoundRecord* PubChemClient::getRecordByName(QString name)
{
	auto recordReply = sendRequest("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + name + "/property/MolecularFormula,MolecularWeight,IUPACName/JSON");
	if (recordReply == nullptr) return nullptr;

	auto document = QJsonDocument::fromJson(recordReply->readAll());
	auto properties = document["PropertyTable"]["Properties"][0];

	auto record = new CompoundRecord();
	record->molecularFormula = formatMolecularFormula(properties["MolecularFormula"].toString());
	record->molecularWeight = properties["MolecularWeight"].toString();
	record->iupacName = properties["IUPACName"].toString();

	auto imageReply = sendRequest("https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + name + "/PNG?image_size=256x256");
	if (imageReply == nullptr) return nullptr;

	auto pixmap = QPixmap();
	pixmap.loadFromData(imageReply->readAll());

	record->image = pixmap;

	delete recordReply;
	delete imageReply;
	
	return record;
}